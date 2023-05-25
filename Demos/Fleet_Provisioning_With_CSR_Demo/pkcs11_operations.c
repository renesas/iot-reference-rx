/*
 * FreeRTOS V202112.00
 * Copyright (C) 2020 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 * Modifications Copyright (C) 2023 Renesas Electronics Corporation. or its affiliates.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * https://www.FreeRTOS.org
 * https://github.com/FreeRTOS
 *
 */

/**
 * @file pkcs11_operations.c
 *
 * @brief This file provides wrapper functions for PKCS11 operations.
 */

/* Standard includes. */
#include <errno.h>
#include <assert.h>

/* Config include. */
#include "demo_config.h"

/* Interface include. */
#include "pkcs11_operations.h"

/* PKCS #11 include. */
#include "core_pkcs11_config.h"
#include "core_pki_utils.h"
#include "mbedtls_utils.h"
#include "mbedtls_pk_pkcs11.h"

/* MbedTLS include. */
#include "mbedtls/error.h"
#include "mbedtls/oid.h"
#include "mbedtls/pk.h"
#include "mbedtls/sha256.h"
#include "mbedtls/x509_crt.h"
#include "mbedtls/x509_csr.h"
#include "mbedtls/ctr_drbg.h"
#include "mbedtls/entropy.h"

/* Length parameters for importing RSA-2048 private keys. */
#define MODULUS_LENGTH        pkcs11RSA_2048_MODULUS_BITS / 8
#define E_LENGTH              3
#define D_LENGTH              pkcs11RSA_2048_MODULUS_BITS / 8
#define PRIME_1_LENGTH        128
#define PRIME_2_LENGTH        128
#define EXPONENT_1_LENGTH     128
#define EXPONENT_2_LENGTH     128
#define COEFFICIENT_LENGTH    128

#define EC_PARAMS_LENGTH      10
#define EC_D_LENGTH           32

/**
 * @brief Struct for holding parsed RSA-2048 private keys.
 */
typedef struct RsaParams_t
{
    CK_BYTE modulus[ MODULUS_LENGTH + 1 ];
    CK_BYTE e[ E_LENGTH + 1 ];
    CK_BYTE d[ D_LENGTH + 1 ];
    CK_BYTE prime1[ PRIME_1_LENGTH + 1 ];
    CK_BYTE prime2[ PRIME_2_LENGTH + 1 ];
    CK_BYTE exponent1[ EXPONENT_1_LENGTH + 1 ];
    CK_BYTE exponent2[ EXPONENT_2_LENGTH + 1 ];
    CK_BYTE coefficient[ COEFFICIENT_LENGTH + 1 ];
} RsaParams_t;

/* CC-RX Compiler v3.04.00 and below do not support the strnlen function, so use the strlen function instead. */
#if !defined(strnlen)
	#define strnlen( _s1, _s2)	(strlen( _s1))
#endif

/*-----------------------------------------------------------*/

/**
 * @brief Delete the specified crypto object from storage.
 *
 * @param[in] xSession The PKCS #11 session.
 * @param[in] pxPkcsLabelsPtr The list of labels to remove.
 * @param[in] pxClass The list of corresponding classes.
 * @param[in] xCount The length of #pxPkcsLabelsPtr and #pxClass.
 */
static CK_RV prvDestroyProvidedObjects( CK_SESSION_HANDLE xSession,
                                        CK_BYTE_PTR * pxPkcsLabelsPtr,
                                        CK_OBJECT_CLASS * pxClass,
                                        CK_ULONG xCount );

/**
 * @brief Generate a new ECDSA key pair using PKCS #11.
 *
 * @param[in] xSession The PKCS #11 session.
 * @param[in] pcPrivateKeyLabel The label to store the private key.
 * @param[in] pcPublicKeyLabel The label to store the public key.
 * @param[out] xPrivateKeyHandlePtr The handle of the private key.
 * @param[out] xPublicKeyHandlePtr The handle of the public key.
 */
static CK_RV prvGenerateKeyPairEC( CK_SESSION_HANDLE xSession,
                                   const char * pcPrivateKeyLabel,
                                   const char * pcPublicKeyLabel,
                                   CK_OBJECT_HANDLE_PTR xPrivateKeyHandlePtr,
                                   CK_OBJECT_HANDLE_PTR xPublicKeyHandlePtr );

/**
 * @brief Import the specified ECDSA private key into storage.
 *
 * @param[in] session The PKCS #11 session.
 * @param[in] label The label to store the key.
 * @param[in] mbedPkContext The private key to store.
 */
static CK_RV provisionPrivateECKey( CK_SESSION_HANDLE session,
                                    const char * label,
                                    mbedtls_pk_context * mbedPkContext );



/**
 * @brief Import the specified RSA private key into storage.
 *
 * @param[in] session The PKCS #11 session.
 * @param[in] label The label to store the key.
 * @param[in] mbedPkContext The private key to store.
 */
static CK_RV provisionPrivateRSAKey( CK_SESSION_HANDLE session,
                                     const char * label,
                                     mbedtls_pk_context * mbedPkContext );

/*-----------------------------------------------------------*/

static CK_RV prvDestroyProvidedObjects( CK_SESSION_HANDLE xSession,
                                        CK_BYTE_PTR * pxPkcsLabelsPtr,
                                        CK_OBJECT_CLASS * pxClass,
                                        CK_ULONG xCount )
{
    CK_RV xResult;
    CK_FUNCTION_LIST_PTR xFunctionList;
    CK_OBJECT_HANDLE xObjectHandle;
    CK_BYTE * pxLabelPtr;
    CK_ULONG xIndex = 0;

    xResult = C_GetFunctionList( &xFunctionList );

    if( xResult != CKR_OK )
    {
        LogError( ( "Could not get a PKCS #11 function pointer." ) );
    }
    else
    {
        for( xIndex = 0; xIndex < xCount; xIndex++ )
        {
            pxLabelPtr = pxPkcsLabelsPtr[ xIndex ];

            xResult = xFindObjectWithLabelAndClass( xSession, ( char * ) pxLabelPtr,
                                                    strnlen( ( char * ) pxLabelPtr, pkcs11configMAX_LABEL_LENGTH ),
                                                    pxClass[ xIndex ], &xObjectHandle );

            while( ( xResult == CKR_OK ) && ( xObjectHandle != CK_INVALID_HANDLE ) )
            {
                xResult = xFunctionList->C_DestroyObject( xSession, xObjectHandle );

                /* PKCS #11 allows a module to maintain multiple objects with the same
                 * label and type. The intent of this loop is to try to delete all of
                 * them. However, to avoid getting stuck, we won't try to find another
                 * object of the same label/type if the previous delete failed. */
                if( xResult == CKR_OK )
                {
                    xResult = xFindObjectWithLabelAndClass( xSession, ( char * ) pxLabelPtr,
                                                            strnlen( ( char * ) pxLabelPtr, pkcs11configMAX_LABEL_LENGTH ),
                                                            pxClass[ xIndex ], &xObjectHandle );
                }
                else
                {
                    break;
                }
            }
        }
    }

    return xResult;
}

/*-----------------------------------------------------------*/

static CK_RV prvGenerateKeyPairEC( CK_SESSION_HANDLE xSession,
                                   const char * pcPrivateKeyLabel,
                                   const char * pcPublicKeyLabel,
                                   CK_OBJECT_HANDLE_PTR xPrivateKeyHandlePtr,
                                   CK_OBJECT_HANDLE_PTR xPublicKeyHandlePtr )
{
    CK_RV xResult;
    CK_MECHANISM xMechanism = { CKM_EC_KEY_PAIR_GEN, NULL_PTR, 0 };
    CK_FUNCTION_LIST_PTR xFunctionList;
    CK_BYTE pxEcParams[] = pkcs11DER_ENCODED_OID_P256; /* prime256v1 */
    CK_KEY_TYPE xKeyType = CKK_EC;

    CK_BBOOL xTrueObject = CK_TRUE;
    CK_ATTRIBUTE pxPublicKeyTemplate[] =
    {
        { CKA_KEY_TYPE,  NULL /* &keyType */,         sizeof( xKeyType )             },
        { CKA_VERIFY,    NULL /* &trueObject */,      sizeof( xTrueObject )          },
        { CKA_EC_PARAMS, NULL /* ecParams */,         sizeof( pxEcParams )           },
        { CKA_LABEL,     ( void * ) pcPublicKeyLabel, strnlen( pcPublicKeyLabel, pkcs11configMAX_LABEL_LENGTH )}
    };

    /* Aggregate initializers must not use the address of an automatic variable. */
    pxPublicKeyTemplate[ 0 ].pValue = &xKeyType;
    pxPublicKeyTemplate[ 1 ].pValue = &xTrueObject;
    pxPublicKeyTemplate[ 2 ].pValue = &pxEcParams;

    CK_ATTRIBUTE privateKeyTemplate[] =
    {
        { CKA_KEY_TYPE, NULL /* &keyType */,          sizeof( xKeyType )             },
        { CKA_TOKEN,    NULL /* &trueObject */,       sizeof( xTrueObject )          },
        { CKA_PRIVATE,  NULL /* &trueObject */,       sizeof( xTrueObject )          },
        { CKA_SIGN,     NULL /* &trueObject */,       sizeof( xTrueObject )          },
        { CKA_LABEL,    ( void * ) pcPrivateKeyLabel, strnlen( pcPrivateKeyLabel, pkcs11configMAX_LABEL_LENGTH )}
    };

    /* Aggregate initializers must not use the address of an automatic variable. */
    privateKeyTemplate[ 0 ].pValue = &xKeyType;
    privateKeyTemplate[ 1 ].pValue = &xTrueObject;
    privateKeyTemplate[ 2 ].pValue = &xTrueObject;
    privateKeyTemplate[ 3 ].pValue = &xTrueObject;

    xResult = C_GetFunctionList( &xFunctionList );

    if( xResult != CKR_OK )
    {
        LogError( ( "Could not get a PKCS #11 function pointer." ) );
    }
    else
    {
        xResult = xFunctionList->C_GenerateKeyPair( xSession,
                                                    &xMechanism,
                                                    pxPublicKeyTemplate,
                                                    sizeof( pxPublicKeyTemplate ) / sizeof( CK_ATTRIBUTE ),
                                                    privateKeyTemplate, sizeof( privateKeyTemplate ) / sizeof( CK_ATTRIBUTE ),
                                                    xPublicKeyHandlePtr,
                                                    xPrivateKeyHandlePtr );
    }

    return xResult;
}

/*-----------------------------------------------------------*/

bool xGenerateKeyAndCsr( CK_SESSION_HANDLE xP11Session,
                         const char * pcPrivKeyLabel,
                         const char * pcPubKeyLabel,
                         char * pcCsrBuffer,
                         size_t xCsrBufferLength,
                         size_t * pxOutCsrLength,
                         const char * pcCsrsubjectname )
{
    CK_OBJECT_HANDLE xPrivKeyHandle;
    CK_OBJECT_HANDLE xPubKeyHandle;
    CK_RV xPkcs11Ret = CKR_OK;
    mbedtls_pk_context xPrivKey;
    mbedtls_ecdsa_context xEcdsaContext;
    mbedtls_x509write_csr xReq;
    int32_t ulMbedtlsRet = -1;
    const mbedtls_pk_info_t * pxHeader = mbedtls_pk_info_from_type( MBEDTLS_PK_ECKEY );

    configASSERT( pcPrivKeyLabel != NULL );
    configASSERT( pcPubKeyLabel != NULL );
    configASSERT( pcCsrBuffer != NULL );
    configASSERT( pxOutCsrLength != NULL );

    xPkcs11Ret = prvGenerateKeyPairEC( xP11Session,
                                       pcPrivKeyLabel,
                                       pcPubKeyLabel,
                                       &xPrivKeyHandle,
                                       &xPubKeyHandle );

    if( xPkcs11Ret == CKR_OK )
    {
        xPkcs11Ret = xPKCS11_initMbedtlsPkContext( &xPrivKey, xP11Session, xPrivKeyHandle );
    }

    if( xPkcs11Ret == CKR_OK )
    {
        mbedtls_x509write_csr_init( &xReq );
        mbedtls_x509write_csr_set_md_alg( &xReq, MBEDTLS_MD_SHA256 );

        ulMbedtlsRet = mbedtls_x509write_csr_set_key_usage( &xReq, MBEDTLS_X509_KU_DIGITAL_SIGNATURE );

        if( ulMbedtlsRet == 0 )
        {
            ulMbedtlsRet = mbedtls_x509write_csr_set_ns_cert_type( &xReq, MBEDTLS_X509_NS_CERT_TYPE_SSL_CLIENT );
        }

        if( ulMbedtlsRet == 0 )
        {
            ulMbedtlsRet = mbedtls_x509write_csr_set_subject_name( &xReq, pcCsrsubjectname );
        }

        if( ulMbedtlsRet == 0 )
        {
            mbedtls_x509write_csr_set_key( &xReq, &xPrivKey );

            ulMbedtlsRet = mbedtls_x509write_csr_pem( &xReq, ( unsigned char * ) pcCsrBuffer,
                                                      xCsrBufferLength, &lPKCS11RandomCallback,
                                                      &xP11Session );
        }

        mbedtls_x509write_csr_free( &xReq );

        mbedtls_pk_free( &xPrivKey );
    }

    *pxOutCsrLength = strlen( pcCsrBuffer );

    return( ulMbedtlsRet == 0 );
}

/*-----------------------------------------------------------*/

bool xLoadCertificate( CK_SESSION_HANDLE xP11Session,
                       const char * pcCertificate,
                       const char * pcLabel,
                       size_t xCertificateLength )
{
    PKCS11_CertificateTemplate_t xCertificateTemplate;
    CK_OBJECT_CLASS xCertificateClass = CKO_CERTIFICATE;
    CK_CERTIFICATE_TYPE xCertificateType = CKC_X_509;
    CK_FUNCTION_LIST_PTR xFunctionList = NULL;
    CK_RV xResult = CKR_OK;
    uint8_t * pucDerObject = NULL;
    int32_t ulConversion = 0;
    size_t xDerLen = 0;
    CK_BBOOL xTokenStorage = CK_TRUE;
    CK_BYTE pxSubject[] = "TestSubject";
    CK_OBJECT_HANDLE xObjectHandle = CK_INVALID_HANDLE;

    configASSERT( pcLabel != NULL );

    if( pcCertificate == NULL )
    {
        LogError( ( "Certificate cannot be null." ) );
        xResult = CKR_ATTRIBUTE_VALUE_INVALID;
    }

    if( xResult == CKR_OK )
    {
        /* Convert the certificate to DER format from PEM. The DER key should
         * be about 3/4 the size of the PEM key, so mallocing the PEM key size
         * is sufficient. */
        pucDerObject = ( uint8_t * ) malloc( xCertificateLength + 1 );
        xDerLen = xCertificateLength + 1;

        if( pucDerObject != NULL )
        {
            ulConversion = convert_pem_to_der( ( unsigned char * ) pcCertificate,
                                               xCertificateLength + 1,
                                               pucDerObject, &xDerLen );

            if( 0 != ulConversion )
            {
                LogError( ( "Failed to convert provided certificate." ) );
                xResult = CKR_ARGUMENTS_BAD;
            }
        }
        else
        {
            LogError( ( "Failed to allocate buffer for converting certificate to DER." ) );
            xResult = CKR_HOST_MEMORY;
        }
    }

    if( xResult == CKR_OK )
    {
        xResult = C_GetFunctionList( &xFunctionList );

        if( xResult != CKR_OK )
        {
            LogError( ( "Could not get a PKCS #11 function pointer." ) );
        }
    }

    if( xResult == CKR_OK )
    {
        /* Initialize the client certificate template. */
        xCertificateTemplate.xObjectClass.type = CKA_CLASS;
        xCertificateTemplate.xObjectClass.pValue = &xCertificateClass;
        xCertificateTemplate.xObjectClass.ulValueLen = sizeof( xCertificateClass );
        xCertificateTemplate.xSubject.type = CKA_SUBJECT;
        xCertificateTemplate.xSubject.pValue = pxSubject;
        xCertificateTemplate.xSubject.ulValueLen = strlen( ( const char * ) pxSubject );
        xCertificateTemplate.xValue.type = CKA_VALUE;
        xCertificateTemplate.xValue.pValue = pucDerObject;
        xCertificateTemplate.xValue.ulValueLen = xDerLen;
        xCertificateTemplate.xLabel.type = CKA_LABEL;
        xCertificateTemplate.xLabel.pValue = ( CK_VOID_PTR ) pcLabel;
        xCertificateTemplate.xLabel.ulValueLen = strnlen( pcLabel, pkcs11configMAX_LABEL_LENGTH );
        xCertificateTemplate.xCertificateType.type = CKA_CERTIFICATE_TYPE;
        xCertificateTemplate.xCertificateType.pValue = &xCertificateType;
        xCertificateTemplate.xCertificateType.ulValueLen = sizeof( CK_CERTIFICATE_TYPE );
        xCertificateTemplate.xTokenObject.type = CKA_TOKEN;
        xCertificateTemplate.xTokenObject.pValue = &xTokenStorage;
        xCertificateTemplate.xTokenObject.ulValueLen = sizeof( xTokenStorage );

        /* Best effort clean-up of the existing object, if it exists. */
        prvDestroyProvidedObjects( xP11Session, ( CK_BYTE_PTR * ) &pcLabel, &xCertificateClass, 1 );

        /* Create an object using the encoded client certificate. */
        LogInfo( ( "Writing certificate into label \"%s\".", pcLabel ) );

        xResult = xFunctionList->C_CreateObject( xP11Session,
                                                 ( CK_ATTRIBUTE_PTR ) &xCertificateTemplate,
                                                 sizeof( xCertificateTemplate ) / sizeof( CK_ATTRIBUTE ),
                                                 &xObjectHandle );
    }

    if( pucDerObject != NULL )
    {
        free( pucDerObject );
    }

    return( xResult == CKR_OK );
}

/*-----------------------------------------------------------*/

bool xPkcs11CloseSession( CK_SESSION_HANDLE xP11Session )
{
    CK_RV xResult = CKR_OK;
    CK_FUNCTION_LIST_PTR xFunctionList = NULL;

    xResult = C_GetFunctionList( &xFunctionList );

    if( xResult == CKR_OK )
    {
        xResult = xFunctionList->C_CloseSession( xP11Session );
    }

    if( xResult == CKR_OK )
    {
        xResult = xFunctionList->C_Finalize( NULL );
    }

    return( xResult == CKR_OK );
}

/*-----------------------------------------------------------*/

static CK_RV provisionPrivateECKey( CK_SESSION_HANDLE session,
                                    const char * label,
                                    mbedtls_pk_context * mbedPkContext )
{
    CK_RV result = CKR_OK;
    CK_FUNCTION_LIST_PTR functionList = NULL;
    CK_BYTE * DPtr = NULL;        /* Private value D. */
    CK_BYTE * ecParamsPtr = NULL; /* DER-encoding of an ANSI X9.62 Parameters value */
    int mbedResult = 0;
    CK_BBOOL trueObject = CK_TRUE;
    CK_KEY_TYPE privateKeyType = CKK_EC;
    CK_OBJECT_CLASS privateKeyClass = CKO_PRIVATE_KEY;
    CK_OBJECT_HANDLE objectHandle = CK_INVALID_HANDLE;
    mbedtls_ecp_keypair * keyPair = ( mbedtls_ecp_keypair * ) mbedPkContext->pk_ctx;

    result = C_GetFunctionList( &functionList );

    if( result != CKR_OK )
    {
        LogError( ( "Could not get a PKCS #11 function pointer." ) );
    }
    else
    {
        DPtr = ( CK_BYTE * ) pvPortMalloc( EC_D_LENGTH );

        if( DPtr == NULL )
        {
            result = CKR_HOST_MEMORY;
        }
    }

    if( result == CKR_OK )
    {
        mbedResult = mbedtls_mpi_write_binary( &( keyPair->d ), DPtr, EC_D_LENGTH );

        if( mbedResult != 0 )
        {
            LogError( ( "Failed to parse EC private key components." ) );
            result = CKR_ATTRIBUTE_VALUE_INVALID;
        }
    }

    if( result == CKR_OK )
    {
        if( keyPair->grp.id == MBEDTLS_ECP_DP_SECP256R1 )
        {
            ecParamsPtr = ( CK_BYTE * ) ( "\x06\x08" MBEDTLS_OID_EC_GRP_SECP256R1 );
        }
        else
        {
            result = CKR_CURVE_NOT_SUPPORTED;
        }
    }

    if( result == CKR_OK )
    {
        CK_ATTRIBUTE privateKeyTemplate[] =
        {
            { CKA_CLASS,     NULL /* &privateKeyClass*/, sizeof( CK_OBJECT_CLASS )    },
            { CKA_KEY_TYPE,  NULL /* &privateKeyType*/,  sizeof( CK_KEY_TYPE )        },
            { CKA_LABEL,     ( void * ) label,           ( CK_ULONG ) strlen( label ) },
            { CKA_TOKEN,     NULL /* &trueObject*/,      sizeof( CK_BBOOL )           },
            { CKA_SIGN,      NULL /* &trueObject*/,      sizeof( CK_BBOOL )           },
            { CKA_EC_PARAMS, NULL /* ecParamsPtr*/,      EC_PARAMS_LENGTH             },
            { CKA_VALUE,     NULL /* DPtr*/,             EC_D_LENGTH                  }
        };

        /* Aggregate initializers must not use the address of an automatic variable. */
        privateKeyTemplate[ 0 ].pValue = &privateKeyClass;
        privateKeyTemplate[ 1 ].pValue = &privateKeyType;
        privateKeyTemplate[ 3 ].pValue = &trueObject;
        privateKeyTemplate[ 4 ].pValue = &trueObject;
        privateKeyTemplate[ 5 ].pValue = ecParamsPtr;
        privateKeyTemplate[ 6 ].pValue = DPtr;

        result = functionList->C_CreateObject( session,
                                               ( CK_ATTRIBUTE_PTR ) &privateKeyTemplate,
                                               sizeof( privateKeyTemplate ) / sizeof( CK_ATTRIBUTE ),
                                               &objectHandle );
    }

    if( DPtr != NULL )
    {
        vPortFree( DPtr );
    }

    return result;
}

/*-----------------------------------------------------------*/

static CK_RV provisionPrivateRSAKey( CK_SESSION_HANDLE session,
                                     const char * label,
                                     mbedtls_pk_context * mbedPkContext )
{
    CK_RV result = CKR_OK;
    CK_FUNCTION_LIST_PTR functionList = NULL;
    int mbedResult = 0;
    CK_KEY_TYPE privateKeyType = CKK_RSA;
    mbedtls_rsa_context * rsaContext = ( mbedtls_rsa_context * ) mbedPkContext->pk_ctx;
    CK_OBJECT_CLASS privateKeyClass = CKO_PRIVATE_KEY;
    RsaParams_t * rsaParams = NULL;
    CK_BBOOL trueObject = CK_TRUE;
    CK_OBJECT_HANDLE objectHandle = CK_INVALID_HANDLE;

    result = C_GetFunctionList( &functionList );

    if( result != CKR_OK )
    {
        LogError( ( "Could not get a PKCS #11 function pointer." ) );
    }
    else
    {
        rsaParams = ( RsaParams_t * ) pvPortMalloc( sizeof( RsaParams_t ) );

        if( rsaParams == NULL )
        {
            result = CKR_HOST_MEMORY;
        }
    }

    if( result == CKR_OK )
    {
        memset( rsaParams, 0, sizeof( RsaParams_t ) );

        mbedResult = mbedtls_rsa_export_raw( rsaContext,
                                             rsaParams->modulus, MODULUS_LENGTH + 1,
                                             rsaParams->prime1, PRIME_1_LENGTH + 1,
                                             rsaParams->prime2, PRIME_2_LENGTH + 1,
                                             rsaParams->d, D_LENGTH + 1,
                                             rsaParams->e, E_LENGTH + 1 );

        if( mbedResult != 0 )
        {
            LogError( ( "Failed to parse RSA private key components." ) );
            result = CKR_ATTRIBUTE_VALUE_INVALID;
        }

        /* Export Exponent 1, Exponent 2, Coefficient. */
        mbedResult |= mbedtls_mpi_write_binary( ( mbedtls_mpi const * ) &rsaContext->DP, rsaParams->exponent1, EXPONENT_1_LENGTH + 1 );
        mbedResult |= mbedtls_mpi_write_binary( ( mbedtls_mpi const * ) &rsaContext->DQ, rsaParams->exponent2, EXPONENT_2_LENGTH + 1 );
        mbedResult |= mbedtls_mpi_write_binary( ( mbedtls_mpi const * ) &rsaContext->QP, rsaParams->coefficient, COEFFICIENT_LENGTH + 1 );

        if( mbedResult != 0 )
        {
            LogError( ( "Failed to parse RSA private key Chinese Remainder Theorem variables." ) );
            result = CKR_ATTRIBUTE_VALUE_INVALID;
        }
    }

    if( result == CKR_OK )
    {
        /* When importing the fields, the pointer is incremented by 1
         * to remove the leading 0 padding (if it existed) and the original field
         * length is used */

        CK_ATTRIBUTE privateKeyTemplate[] =
        {
            { CKA_CLASS,            NULL /* &privateKeyClass */, sizeof( CK_OBJECT_CLASS )    },
            { CKA_KEY_TYPE,         NULL /* &privateKeyType */,  sizeof( CK_KEY_TYPE )        },
            { CKA_LABEL,            ( void * ) label,            ( CK_ULONG ) strlen( label ) },
            { CKA_TOKEN,            NULL /* &trueObject */,      sizeof( CK_BBOOL )           },
            { CKA_SIGN,             NULL /* &trueObject */,      sizeof( CK_BBOOL )           },
            { CKA_MODULUS,          rsaParams->modulus + 1,      MODULUS_LENGTH               },
            { CKA_PRIVATE_EXPONENT, rsaParams->d + 1,            D_LENGTH                     },
            { CKA_PUBLIC_EXPONENT,  rsaParams->e + 1,            E_LENGTH                     },
            { CKA_PRIME_1,          rsaParams->prime1 + 1,       PRIME_1_LENGTH               },
            { CKA_PRIME_2,          rsaParams->prime2 + 1,       PRIME_2_LENGTH               },
            { CKA_EXPONENT_1,       rsaParams->exponent1 + 1,    EXPONENT_1_LENGTH            },
            { CKA_EXPONENT_2,       rsaParams->exponent2 + 1,    EXPONENT_2_LENGTH            },
            { CKA_COEFFICIENT,      rsaParams->coefficient + 1,  COEFFICIENT_LENGTH           }
        };

        /* Aggregate initializers must not use the address of an automatic variable. */
        privateKeyTemplate[ 0 ].pValue = &privateKeyClass;
        privateKeyTemplate[ 1 ].pValue = &privateKeyType;
        privateKeyTemplate[ 3 ].pValue = &trueObject;
        privateKeyTemplate[ 4 ].pValue = &trueObject;

        result = functionList->C_CreateObject( session,
                                               ( CK_ATTRIBUTE_PTR ) &privateKeyTemplate,
                                               sizeof( privateKeyTemplate ) / sizeof( CK_ATTRIBUTE ),
                                               &objectHandle );
    }

    if( NULL != rsaParams )
    {
    	vPortFree( rsaParams );
    }

    return result;
}

/*-----------------------------------------------------------*/

CK_RV provisionPrivateKey( CK_SESSION_HANDLE session,
                                  const char * privateKey,
                                  size_t privateKeyLength,
                                  const char * label )
{
    CK_RV result = CKR_OK;
    mbedtls_pk_type_t mbedKeyType = MBEDTLS_PK_NONE;
    int mbedResult = 0;
    mbedtls_pk_context mbedPkContext = { 0 };
#if MBEDTLS_VERSION_NUMBER >= 0x03000000
    mbedtls_entropy_context xEntropyContext;
    mbedtls_ctr_drbg_context xDrbgContext;
#endif

    mbedtls_pk_init( &mbedPkContext );

    /* Try parsing the private key using mbedtls_pk_parse_key. */
    #if MBEDTLS_VERSION_NUMBER < 0x03000000
        mbedResult = mbedtls_pk_parse_key( &mbedPkContext, ( const uint8_t * ) privateKey, privateKeyLength, NULL, 0  );
    #else
        mbedtls_entropy_init( &xEntropyContext );
        mbedtls_ctr_drbg_init( &xDrbgContext );
        mbedResult = mbedtls_ctr_drbg_seed( &xDrbgContext, mbedtls_entropy_func, &xEntropyContext, NULL, 0 );
        if( mbedResult == 0 )
        {
        	mbedResult = mbedtls_pk_parse_key( &mbedPkContext, ( const uint8_t * ) privateKey, privateKeyLength, NULL, 0,
                mbedtls_ctr_drbg_random, &xDrbgContext );
        }
        mbedtls_ctr_drbg_free( &xDrbgContext );
        mbedtls_entropy_free( &xEntropyContext );
    #endif /* MBEDTLS_VERSION_NUMBER < 0x03000000 */

    if( mbedResult != 0 )
    {
        LogError( ( "Unable to parse private key." ) );
        result = CKR_ARGUMENTS_BAD;
    }

    /* Determine whether the key to be imported is RSA or EC. */
    if( result == CKR_OK )
    {
        mbedKeyType = mbedtls_pk_get_type( &mbedPkContext );

        if( mbedKeyType == MBEDTLS_PK_RSA )
        {
            result = provisionPrivateRSAKey( session, label, &mbedPkContext );
        }
        else if( ( mbedKeyType == MBEDTLS_PK_ECDSA ) ||
                 ( mbedKeyType == MBEDTLS_PK_ECKEY ) ||
                 ( mbedKeyType == MBEDTLS_PK_ECKEY_DH ) )
        {
            result = provisionPrivateECKey( session, label, &mbedPkContext );
        }
        else
        {
            LogError( ( "Invalid private key type provided. Only RSA-2048 and "
                        "EC P-256 keys are supported." ) );
            result = CKR_ARGUMENTS_BAD;
        }
    }

    mbedtls_pk_free( &mbedPkContext );

    return result;
}

/*-----------------------------------------------------------*/

CK_RV provisionCertificate( CK_SESSION_HANDLE session,
                                   const char * certificate,
                                   size_t certificateLength,
                                   const char * label )
{
    PKCS11_CertificateTemplate_t certificateTemplate;
    CK_OBJECT_CLASS certificateClass = CKO_CERTIFICATE;
    CK_CERTIFICATE_TYPE certificateType = CKC_X_509;
    CK_FUNCTION_LIST_PTR functionList = NULL;
    CK_RV result = CKR_OK;
    uint8_t * derObject = NULL;
    int32_t conversion = 0;
    size_t derLen = 0;
    CK_BBOOL tokenStorage = CK_TRUE;
    CK_BYTE subject[] = "TestSubject";
    CK_OBJECT_HANDLE objectHandle = CK_INVALID_HANDLE;

    /* Initialize the client certificate template. */
    certificateTemplate.xObjectClass.type = CKA_CLASS;
    certificateTemplate.xObjectClass.pValue = &certificateClass;
    certificateTemplate.xObjectClass.ulValueLen = sizeof( certificateClass );
    certificateTemplate.xSubject.type = CKA_SUBJECT;
    certificateTemplate.xSubject.pValue = subject;
    certificateTemplate.xSubject.ulValueLen = strlen( ( const char * ) subject );
    certificateTemplate.xValue.type = CKA_VALUE;
    certificateTemplate.xValue.pValue = ( CK_VOID_PTR ) certificate;
    certificateTemplate.xValue.ulValueLen = ( CK_ULONG ) certificateLength;
    certificateTemplate.xLabel.type = CKA_LABEL;
    certificateTemplate.xLabel.pValue = ( CK_VOID_PTR ) label;
    certificateTemplate.xLabel.ulValueLen = strlen( label );
    certificateTemplate.xCertificateType.type = CKA_CERTIFICATE_TYPE;
    certificateTemplate.xCertificateType.pValue = &certificateType;
    certificateTemplate.xCertificateType.ulValueLen = sizeof( CK_CERTIFICATE_TYPE );
    certificateTemplate.xTokenObject.type = CKA_TOKEN;
    certificateTemplate.xTokenObject.pValue = &tokenStorage;
    certificateTemplate.xTokenObject.ulValueLen = sizeof( tokenStorage );

    if( certificate == NULL )
    {
        LogError( ( "Certificate cannot be null." ) );
        result = CKR_ATTRIBUTE_VALUE_INVALID;
    }

    if( result == CKR_OK )
    {
        result = C_GetFunctionList( &functionList );

        if( result != CKR_OK )
        {
            LogError( ( "Could not get a PKCS #11 function pointer." ) );
        }
    }

    if( result == CKR_OK )
    {
        /* Convert the certificate to DER format from PEM. The DER key should
         * be about 3/4 the size of the PEM key, so mallocing the PEM key size
         * is sufficient. */
        derObject = ( uint8_t * ) pvPortMalloc( certificateTemplate.xValue.ulValueLen );
        derLen = certificateTemplate.xValue.ulValueLen;

        if( derObject != NULL )
        {
            conversion = convert_pem_to_der( ( unsigned char * ) certificateTemplate.xValue.pValue,
                                             certificateTemplate.xValue.ulValueLen,
                                             derObject, &derLen );

            if( 0 != conversion )
            {
                LogError( ( "Failed to convert provided certificate." ) );
                result = CKR_ARGUMENTS_BAD;
            }
        }
        else
        {
            LogError( ( "Failed to allocate buffer for converting certificate to DER." ) );
            result = CKR_HOST_MEMORY;
        }
    }

    if( result == CKR_OK )
    {
        /* Set the template pointers to refer to the DER converted objects. */
        certificateTemplate.xValue.pValue = derObject;
        certificateTemplate.xValue.ulValueLen = derLen;

        /* Best effort clean-up of the existing object, if it exists. */
        prvDestroyProvidedObjects( session, ( CK_BYTE_PTR * ) &label, &certificateClass, 1 );

        /* Create an object using the encoded client certificate. */
        //LogInfo( ( "Writing certificate into label \"%s\".", label ) );

        result = functionList->C_CreateObject( session,
                                               ( CK_ATTRIBUTE_PTR ) &certificateTemplate,
                                               sizeof( certificateTemplate ) / sizeof( CK_ATTRIBUTE ),
                                               &objectHandle );
    }

    if( derObject != NULL )
    {
        vPortFree( derObject );
    }

    return result;
}

/*-----------------------------------------------------------*/

bool xLoadClaimCredentials( CK_SESSION_HANDLE xP11Session,
                           const char * pClaimCert,
                           size_t       ClaimCertLength,
                           const char * pClaimPrivKey,
                           size_t       ClaimPrivKeyLength )
{
    bool status;
    CK_RV ret;

    assert( pClaimCert != NULL );
    assert( ClaimCertLength != NULL );
    assert( pClaimPrivKey != NULL );
    assert( ClaimPrivKeyLength != NULL );

	status = true;

    if( status == true )
    {
        ret = provisionPrivateKey( xP11Session, pClaimPrivKey,
                                   ClaimPrivKeyLength, /* MbedTLS includes null character in length for PEM objects. */
								   pkcs11configLABEL_CLAIM_PRIVATE_KEY );
        status = ( ret == CKR_OK );
    }

    if( status == true )
    {
        ret = provisionCertificate( xP11Session, pClaimCert,
                                    ClaimCertLength, /* MbedTLS includes null character in length for PEM objects. */
									pkcs11configLABEL_CLAIM_CERTIFICATE );
        status = ( ret == CKR_OK );
    }

    return status;
}

/*-----------------------------------------------------------*/

CK_RV xGetCertificateAndKeyState( CK_SESSION_HANDLE xP11Session,
                                  CK_OBJECT_HANDLE_PTR pxClientCertificate,
                                  CK_OBJECT_HANDLE_PTR pxPrivateKey )
{
    CK_RV xResult;
    CK_FUNCTION_LIST_PTR pxFunctionList;

    xResult = C_GetFunctionList( &pxFunctionList );

    /* Check for a private key. */
    if( CKR_OK == xResult )
    {
        xResult = xFindObjectWithLabelAndClass( xP11Session,
                                                pkcs11configLABEL_DEVICE_PRIVATE_KEY_FOR_TLS,
                                                sizeof( pkcs11configLABEL_DEVICE_PRIVATE_KEY_FOR_TLS ) - 1,
                                                CKO_PRIVATE_KEY,
                                                pxPrivateKey );
    }

    /* Check for the client certificate. */
    if( CKR_OK == xResult )
    {
        xResult = xFindObjectWithLabelAndClass( xP11Session,
                                                pkcs11configLABEL_DEVICE_CERTIFICATE_FOR_TLS,
                                                sizeof( pkcs11configLABEL_DEVICE_CERTIFICATE_FOR_TLS ) - 1,
                                                CKO_CERTIFICATE,
                                                pxClientCertificate );
    }

    return xResult;
}
/*-----------------------------------------------------------*/
