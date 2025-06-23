/*
 * FreeRTOS V202012.00
 * Copyright (C) 2020 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 * Modifications Copyright (C) 2023-2025 Renesas Electronics Corporation or its affiliates.
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
 * http://aws.amazon.com/freertos
 * http://www.FreeRTOS.org
 */


/**
 * @file aws_dev_mode_key_provisioning.c
 * @brief Simple key provisioning example using PKCS #11
 *
 * A simple example to demonstrate key and certificate provisioning in
 * flash using PKCS #11 interface. This should be replaced
 * by production ready key provisioning mechanism.
 */

/* Standard includes. */
#include <stdio.h>
#include <string.h>

/* FreeRTOS includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

/* Client credential includes. */
#include "aws_clientcredential.h"
#include "aws_clientcredential_keys.h"
#include "iot_default_root_certificates.h"

/* Key provisioning include. */
#include "aws_dev_mode_key_provisioning.h"

/* Utilities include. */
#include "core_pki_utils.h"

/**
 *  @brief Declaring MBEDTLS_ALLOW_PRIVATE_ACCESS allows access to mbedtls "private" fields.
 */
#define MBEDTLS_ALLOW_PRIVATE_ACCESS

/* mbedTLS includes. */
#include "mbedtls/pk.h"
#include "mbedtls/oid.h"
#include "mbedtls/ctr_drbg.h"
#include "mbedtls/entropy.h"

#include "demo_config.h"
#include "store.h"
#include "serial.h"

/* Default FreeRTOS API for console logging. */
#define DEV_MODE_KEY_PROVISIONING_PRINT( X )    configPRINTF(X)

/* For writing log lines without a prefix. */
extern void vLoggingPrint( const char * pcFormat );

#define PEM_BEGIN              "-----BEGIN "
#define PEM_END                "-----END "

/* Developer convenience override, for lab testing purposes, for generating
 * a new default key pair, regardless of whether an existing key pair is present. */
#define keyprovisioningFORCE_GENERATE_NEW_KEY_PAIR    0

/* Internal structure for parsing RSA keys. */

/* Length parameters for importing RSA-2048 private keys. */
#define MODULUS_LENGTH        pkcs11RSA_2048_MODULUS_BITS / 8
#define E_LENGTH              3
#define D_LENGTH              pkcs11RSA_2048_MODULUS_BITS / 8
#define PRIME_1_LENGTH        128
#define PRIME_2_LENGTH        128
#define EXPONENT_1_LENGTH     128
#define EXPONENT_2_LENGTH     128
#define COEFFICIENT_LENGTH    128

/* Adding one to all of the lengths because ASN1 may pad a leading 0 byte
 * to numbers that could be interpreted as negative */
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

/* Internal structure for capturing the provisioned state of the host device. */
typedef struct ProvisionedState_t
{
    CK_OBJECT_HANDLE xPrivateKey;
    CK_OBJECT_HANDLE xClientCertificate;
    CK_OBJECT_HANDLE xPublicKey;
    uint8_t * pucDerPublicKey;
    uint32_t ulDerPublicKeyLength;
    char * pcIdentifier; /* The token label. On some devices, a unique device
                          * ID might be stored here which can be used as a field
                          * in the subject of the device certificate. */
} ProvisionedState_t;

/* This function can be found in libraries/3rdparty/mbedtls_utils/mbedtls_utils.c. */
extern int convert_pem_to_der( const unsigned char * pucInput,
                               size_t xLen,
                               unsigned char * pucOutput,
                               size_t * pxOlen );

/*-----------------------------------------------------------*/

typedef struct PreProvisioningParams_t
{
    uint8_t * pucClientCredential;      /**< Pointer to the device Credential in PEM format.
                                         *   See tools/certificate_configuration/PEMfileToCString.html
                                         *   for help with formatting.*/
    uint32_t ulClientCredentialLength;  /**< Length of the Credential data, in bytes. */
} PreProvisioningParams_t;

CK_RV vDevModeKeyPreProvisioning( KeyValueStore_t Keystore,KVStoreKey_t ID, int32_t xvaluelength );
CK_RV xDestroyDefaultPrivatekeyObjects( CK_SESSION_HANDLE xSession );
CK_RV xDestroyDefaultCertificateObjects( CK_SESSION_HANDLE xSession );
CK_RV xDestroyDefaultObjects( KVStoreKey_t ID, CK_SESSION_HANDLE xSession );
BaseType_t vAlternateKeyPreProvisioning( KVStoreKey_t ID, PreProvisioningParams_t * xParams );
CK_RV xPreProvisionDevice( CK_SESSION_HANDLE xSession, KVStoreKey_t ID, PreProvisioningParams_t * pxParams );

/* Import the specified ECDSA private key into storage. */
static CK_RV prvProvisionPrivateECKey( CK_SESSION_HANDLE xSession,
                                       uint8_t * pucLabel,
                                       CK_OBJECT_HANDLE_PTR pxObjectHandle,
                                       mbedtls_pk_context * pxMbedPkContext )
{
    CK_RV xResult = CKR_OK;
    CK_FUNCTION_LIST_PTR pxFunctionList = NULL;
    CK_BYTE * pxD;               /* Private value D. */
    CK_BYTE * pxEcParams = NULL; /* DER-encoding of an ANSI X9.62 Parameters value */
    int lMbedResult = 0;
    CK_BBOOL xTrue = CK_TRUE;
    CK_KEY_TYPE xPrivateKeyType = CKK_EC;
    CK_OBJECT_CLASS xPrivateKeyClass = CKO_PRIVATE_KEY;
    mbedtls_ecp_keypair * pxKeyPair = ( mbedtls_ecp_keypair * ) pxMbedPkContext->pk_ctx;

    xResult = C_GetFunctionList( &pxFunctionList );

#define EC_PARAMS_LENGTH    10
#define EC_D_LENGTH         32

    pxD = pvPortMalloc( EC_D_LENGTH );

    if( ( pxD == NULL ) )
    {
        xResult = CKR_HOST_MEMORY;
    }

    if( xResult == CKR_OK )
    {
        lMbedResult = mbedtls_mpi_write_binary( &( pxKeyPair->d ), pxD, EC_D_LENGTH );

        if( lMbedResult != 0 )
        {
            DEV_MODE_KEY_PROVISIONING_PRINT( ( "Failed to parse EC private key components. \r\n" ) );
            xResult = CKR_ATTRIBUTE_VALUE_INVALID;
        }
    }

    if( xResult == CKR_OK )
    {
        if( pxKeyPair->grp.id == MBEDTLS_ECP_DP_SECP256R1 )
        {
            pxEcParams = ( CK_BYTE * ) ( "\x06\x08" MBEDTLS_OID_EC_GRP_SECP256R1 );
        }
        else
        {
            xResult = CKR_CURVE_NOT_SUPPORTED;
        }
    }

    if( xResult == CKR_OK )
    {
        CK_ATTRIBUTE xPrivateKeyTemplate[] =
        {
            { CKA_CLASS,     NULL /* &xPrivateKeyClass*/, sizeof( CK_OBJECT_CLASS )                        },
            { CKA_KEY_TYPE,  NULL /* &xPrivateKeyType*/,  sizeof( CK_KEY_TYPE )                            },
            { CKA_LABEL,     pucLabel,                    ( CK_ULONG ) strlen( ( const char * ) pucLabel ) },
            { CKA_TOKEN,     NULL /* &xTrue*/,            sizeof( CK_BBOOL )                               },
            { CKA_SIGN,      NULL /* &xTrue*/,            sizeof( CK_BBOOL )                               },
            { CKA_EC_PARAMS, NULL /* pxEcParams*/,        EC_PARAMS_LENGTH                                 },
            { CKA_VALUE,     NULL /* pxD*/,               EC_D_LENGTH                                      }
        };

        /* Aggregate initializers must not use the address of an automatic variable. */
        /* See MSVC Compiler Warning C4221 */
        xPrivateKeyTemplate[ 0 ].pValue = &xPrivateKeyClass;
        xPrivateKeyTemplate[ 1 ].pValue = &xPrivateKeyType;
        xPrivateKeyTemplate[ 3 ].pValue = &xTrue;
        xPrivateKeyTemplate[ 4 ].pValue = &xTrue;
        xPrivateKeyTemplate[ 5 ].pValue = pxEcParams;
        xPrivateKeyTemplate[ 6 ].pValue = pxD;

        xResult = pxFunctionList->C_CreateObject( xSession,
                                                  ( CK_ATTRIBUTE_PTR ) &xPrivateKeyTemplate,
                                                  sizeof( xPrivateKeyTemplate ) / sizeof( CK_ATTRIBUTE ),
                                                  pxObjectHandle );
    }

    if( pxD != NULL )
    {
        vPortFree( pxD );
    }

    return xResult;
}

/*-----------------------------------------------------------*/

/* Import the specified RSA private key into storage. */
static CK_RV prvProvisionPrivateRSAKey( CK_SESSION_HANDLE xSession,
                                        uint8_t * pucLabel,
                                        CK_OBJECT_HANDLE_PTR pxObjectHandle,
                                        mbedtls_pk_context * pxMbedPkContext )
{
    CK_RV xResult = CKR_OK;
    CK_FUNCTION_LIST_PTR pxFunctionList = NULL;
    int lMbedResult = 0;
    CK_KEY_TYPE xPrivateKeyType = CKK_RSA;
    mbedtls_rsa_context * xRsaContext = pxMbedPkContext->pk_ctx;
    CK_OBJECT_CLASS xPrivateKeyClass = CKO_PRIVATE_KEY;
    RsaParams_t * pxRsaParams = NULL;
    CK_BBOOL xTrue = CK_TRUE;

    xResult = C_GetFunctionList( &pxFunctionList );

    pxRsaParams = pvPortMalloc( sizeof( RsaParams_t ) );

    if( pxRsaParams == NULL )
    {
        xResult = CKR_HOST_MEMORY;
    }

    if( xResult == CKR_OK )
    {
        memset( pxRsaParams, 0, sizeof( RsaParams_t ) );

        lMbedResult = mbedtls_rsa_export_raw( xRsaContext,
                                              pxRsaParams->modulus, MODULUS_LENGTH + 1,
                                              pxRsaParams->prime1, PRIME_1_LENGTH + 1,
                                              pxRsaParams->prime2, PRIME_2_LENGTH + 1,
                                              pxRsaParams->d, D_LENGTH + 1,
                                              pxRsaParams->e, E_LENGTH + 1 );

        if( lMbedResult != 0 )
        {
            DEV_MODE_KEY_PROVISIONING_PRINT( ( "Failed to parse RSA private key components. \r\n" ) );
            xResult = CKR_ATTRIBUTE_VALUE_INVALID;
        }

        /* Export Exponent 1, Exponent 2, Coefficient. */
        lMbedResult |= mbedtls_mpi_write_binary( ( mbedtls_mpi const * ) &xRsaContext->DP, pxRsaParams->exponent1, EXPONENT_1_LENGTH + 1 );
        lMbedResult |= mbedtls_mpi_write_binary( ( mbedtls_mpi const * ) &xRsaContext->DQ, pxRsaParams->exponent2, EXPONENT_2_LENGTH + 1 );
        lMbedResult |= mbedtls_mpi_write_binary( ( mbedtls_mpi const * ) &xRsaContext->QP, pxRsaParams->coefficient, COEFFICIENT_LENGTH + 1 );

        if( lMbedResult != 0 )
        {
            DEV_MODE_KEY_PROVISIONING_PRINT( ( "Failed to parse RSA private key Chinese Remainder Theorem variables. \r\n" ) );
            xResult = CKR_ATTRIBUTE_VALUE_INVALID;
        }
    }

    if( xResult == CKR_OK )
    {
        /* When importing the fields, the pointer is incremented by 1
         * to remove the leading 0 padding (if it existed) and the original field length is used */


        CK_ATTRIBUTE xPrivateKeyTemplate[] =
        {
            { CKA_CLASS,            NULL /* &xPrivateKeyClass */, sizeof( CK_OBJECT_CLASS )                        },
            { CKA_KEY_TYPE,         NULL /* &xPrivateKeyType */,  sizeof( CK_KEY_TYPE )                            },
            { CKA_LABEL,            pucLabel,                     ( CK_ULONG ) strlen( ( const char * ) pucLabel ) },
            { CKA_TOKEN,            NULL /* &xTrue */,            sizeof( CK_BBOOL )                               },
            { CKA_SIGN,             NULL /* &xTrue */,            sizeof( CK_BBOOL )                               },
            { CKA_MODULUS,          pxRsaParams->modulus + 1,     MODULUS_LENGTH                                   },
            { CKA_PRIVATE_EXPONENT, pxRsaParams->d + 1,           D_LENGTH                                         },
            { CKA_PUBLIC_EXPONENT,  pxRsaParams->e + 1,           E_LENGTH                                         },
            { CKA_PRIME_1,          pxRsaParams->prime1 + 1,      PRIME_1_LENGTH                                   },
            { CKA_PRIME_2,          pxRsaParams->prime2 + 1,      PRIME_2_LENGTH                                   },
            { CKA_EXPONENT_1,       pxRsaParams->exponent1 + 1,   EXPONENT_1_LENGTH                                },
            { CKA_EXPONENT_2,       pxRsaParams->exponent2 + 1,   EXPONENT_2_LENGTH                                },
            { CKA_COEFFICIENT,      pxRsaParams->coefficient + 1, COEFFICIENT_LENGTH                               }
        };

        /* Aggregate initializers must not use the address of an automatic variable. */
        /* See MSVC Compiler Warning C4221 */
        xPrivateKeyTemplate[ 0 ].pValue = &xPrivateKeyClass;
        xPrivateKeyTemplate[ 1 ].pValue = &xPrivateKeyType;
        xPrivateKeyTemplate[ 3 ].pValue = &xTrue;
        xPrivateKeyTemplate[ 4 ].pValue = &xTrue;

        xResult = pxFunctionList->C_CreateObject( xSession,
                                                  ( CK_ATTRIBUTE_PTR ) &xPrivateKeyTemplate,
                                                  sizeof( xPrivateKeyTemplate ) / sizeof( CK_ATTRIBUTE ),
                                                  pxObjectHandle );
    }

    if( NULL != pxRsaParams )
    {
        vPortFree( pxRsaParams );
    }

    return xResult;
}

/*-----------------------------------------------------------*/

/* Import the specified private key into storage. */
CK_RV xProvisionPrivateKey( CK_SESSION_HANDLE xSession,
                            uint8_t * pucPrivateKey,
                            size_t xPrivateKeyLength,
                            uint8_t * pucLabel,
                            CK_OBJECT_HANDLE_PTR pxObjectHandle )
{
    CK_RV xResult = CKR_OK;
    mbedtls_pk_type_t xMbedKeyType = MBEDTLS_PK_NONE;
    CK_OBJECT_CLASS xPrivateKeyClass = CKO_PRIVATE_KEY;
    int lMbedResult = 0;
    mbedtls_pk_context xMbedPkContext = { 0 };
    #if MBEDTLS_VERSION_NUMBER >= 0x03000000
        mbedtls_entropy_context xEntropyContext;
        mbedtls_ctr_drbg_context xDrbgContext;
    #endif

    mbedtls_pk_init( &xMbedPkContext );
    #if MBEDTLS_VERSION_NUMBER < 0x03000000
        lMbedResult = mbedtls_pk_parse_key( &xMbedPkContext, pucPrivateKey, xPrivateKeyLength, NULL, 0 );
    #else
        mbedtls_entropy_init( &xEntropyContext );
        mbedtls_ctr_drbg_init( &xDrbgContext );
        lMbedResult = mbedtls_ctr_drbg_seed( &xDrbgContext, mbedtls_entropy_func, &xEntropyContext, NULL, 0 );
        if( lMbedResult == 0 )
        {
            lMbedResult = mbedtls_pk_parse_key( &xMbedPkContext, pucPrivateKey, xPrivateKeyLength, NULL, 0,
                mbedtls_ctr_drbg_random, &xDrbgContext );
        }
        mbedtls_ctr_drbg_free( &xDrbgContext );
        mbedtls_entropy_free( &xEntropyContext );
    #endif /* MBEDTLS_VERSION_NUMBER < 0x03000000 */
    if( lMbedResult != 0 )
    {
        DEV_MODE_KEY_PROVISIONING_PRINT( ( "Unable to parse private key.\r\n" ) );
        xResult = CKR_ARGUMENTS_BAD;
    }

    /* Determine whether the key to be imported is RSA or EC. */
    if( xResult == CKR_OK )
    {
    	{
            xMbedKeyType = mbedtls_pk_get_type( &xMbedPkContext );

            if( xMbedKeyType == MBEDTLS_PK_RSA )
            {
                xResult = prvProvisionPrivateRSAKey( xSession,
                                                     pucLabel,
                                                     pxObjectHandle,
                                                     &xMbedPkContext );
            }
            else if( ( xMbedKeyType == MBEDTLS_PK_ECDSA ) || ( xMbedKeyType == MBEDTLS_PK_ECKEY ) || ( xMbedKeyType == MBEDTLS_PK_ECKEY_DH ) )
            {
                xResult = prvProvisionPrivateECKey( xSession,
                                                    pucLabel,
                                                    pxObjectHandle,
                                                    &xMbedPkContext );
            }
            else
            {
                DEV_MODE_KEY_PROVISIONING_PRINT( ( "Invalid private key type provided.  RSA-2048 and EC P-256 keys are supported.\r\n" ) );
                xResult = CKR_ARGUMENTS_BAD;
            }
    	}

    }

    mbedtls_pk_free( &xMbedPkContext );

    return xResult;
}

/*-----------------------------------------------------------*/

/* Import the specified public key into storage. */
CK_RV xProvisionPublicKey( CK_SESSION_HANDLE xSession,
                           uint8_t * pucKey,
                           size_t xKeyLength,
                           CK_KEY_TYPE xPublicKeyType,
                           uint8_t * pucPublicKeyLabel,
                           CK_OBJECT_HANDLE_PTR pxPublicKeyHandle )
{
    CK_RV xResult;
    CK_BBOOL xTrue = CK_TRUE;
    CK_FUNCTION_LIST_PTR pxFunctionList;
    CK_OBJECT_CLASS xClass = CKO_PUBLIC_KEY;
    int lMbedResult = 0;
    mbedtls_pk_context xMbedPkContext = { 0 };
    #if MBEDTLS_VERSION_NUMBER >= 0x03000000
        mbedtls_entropy_context xEntropyContext;
        mbedtls_ctr_drbg_context xDrbgContext;
    #endif

    xResult = C_GetFunctionList( &pxFunctionList );

    mbedtls_pk_init( &xMbedPkContext );

    /* Try parsing the private key using mbedtls_pk_parse_key. */
    #if MBEDTLS_VERSION_NUMBER < 0x03000000
        lMbedResult = mbedtls_pk_parse_key( &xMbedPkContext, pucKey, xKeyLength, NULL, 0 );
    #else
        mbedtls_entropy_init( &xEntropyContext );
        mbedtls_ctr_drbg_init( &xDrbgContext );
        lMbedResult = mbedtls_ctr_drbg_seed( &xDrbgContext, mbedtls_entropy_func, &xEntropyContext, NULL, 0 );
        if( lMbedResult == 0 )
        {
            lMbedResult = mbedtls_pk_parse_key( &xMbedPkContext, pucKey, xKeyLength, NULL, 0,
                mbedtls_ctr_drbg_random, &xDrbgContext );
        }
        mbedtls_ctr_drbg_free( &xDrbgContext );
        mbedtls_entropy_free( &xEntropyContext );
    #endif /* MBEDTLS_VERSION_NUMBER < 0x03000000 */

    /* If mbedtls_pk_parse_key didn't work, maybe the private key is not included in the input passed in.
     * Try to parse just the public key. */
    if( lMbedResult != 0 )
    {
        lMbedResult = mbedtls_pk_parse_public_key( &xMbedPkContext, pucKey, xKeyLength );
    }

    if( lMbedResult != 0 )
    {
        DEV_MODE_KEY_PROVISIONING_PRINT( ( "Failed to parse the public key. \r\n" ) );
        xResult = CKR_ARGUMENTS_BAD;
    }

    if( ( xResult == CKR_OK ) && ( xPublicKeyType == CKK_RSA ) )
    {
        CK_BYTE xPublicExponent[] = { 0x01, 0x00, 0x01 };
        CK_BYTE xModulus[ MODULUS_LENGTH + 1 ] = { 0 };

        lMbedResult = mbedtls_rsa_export_raw( ( mbedtls_rsa_context * ) xMbedPkContext.pk_ctx,
                                              ( unsigned char * ) &xModulus, MODULUS_LENGTH + 1,
                                              NULL, 0,
                                              NULL, 0,
                                              NULL, 0,
                                              NULL, 0 );
        CK_ATTRIBUTE xPublicKeyTemplate[] =
        {
            { CKA_CLASS,           NULL /* &xClass */,         sizeof( CK_OBJECT_CLASS )                    },
            { CKA_KEY_TYPE,        NULL /* &xPublicKeyType */, sizeof( CK_KEY_TYPE )                        },
            { CKA_TOKEN,           NULL /* &xTrue */,          sizeof( xTrue )                              },
            { CKA_MODULUS,         NULL /* &xModulus + 1 */,   MODULUS_LENGTH                               },       /* Extra byte allocated at beginning for 0 padding. */
            { CKA_VERIFY,          NULL /* &xTrue */,          sizeof( xTrue )                              },
            { CKA_PUBLIC_EXPONENT, NULL /* xPublicExponent */, sizeof( xPublicExponent )                    },
            { CKA_LABEL,           pucPublicKeyLabel,          strlen( ( const char * ) pucPublicKeyLabel ) }
        };

        /* Aggregate initializers must not use the address of an automatic variable. */
        /* See MSVC Compiler Warning C4221 */
        xPublicKeyTemplate[ 0 ].pValue = &xClass;
        xPublicKeyTemplate[ 1 ].pValue = &xPublicKeyType;
        xPublicKeyTemplate[ 2 ].pValue = &xTrue;
        xPublicKeyTemplate[ 3 ].pValue = &xModulus + 1;
        xPublicKeyTemplate[ 4 ].pValue = &xTrue;
        xPublicKeyTemplate[ 5 ].pValue = xPublicExponent;

        xResult = pxFunctionList->C_CreateObject( xSession,
                                                  ( CK_ATTRIBUTE_PTR ) xPublicKeyTemplate,
                                                  sizeof( xPublicKeyTemplate ) / sizeof( CK_ATTRIBUTE ),
                                                  pxPublicKeyHandle );
    }
    else if( ( xResult == CKR_OK ) && ( xPublicKeyType == CKK_EC ) )
    {
        CK_BYTE xEcParams[] = pkcs11DER_ENCODED_OID_P256;
        size_t xLength;
        CK_BYTE xEcPoint[ 256 ] = { 0 };

        mbedtls_ecdsa_context * pxEcdsaContext = ( mbedtls_ecdsa_context * ) xMbedPkContext.pk_ctx;

        /* DER encoded EC point. Leave 2 bytes for the tag and length. */
        lMbedResult = mbedtls_ecp_point_write_binary( &pxEcdsaContext->grp,
                                                      &pxEcdsaContext->Q,
                                                      MBEDTLS_ECP_PF_UNCOMPRESSED,
                                                      &xLength,
                                                      xEcPoint + 2,
                                                      sizeof( xEcPoint ) - 2 );
        xEcPoint[ 0 ] = 0x04; /* Octet string. */
        xEcPoint[ 1 ] = ( CK_BYTE ) xLength;

        CK_ATTRIBUTE xPublicKeyTemplate[] =
        {
            { CKA_CLASS,     NULL /* &xClass */,         sizeof( xClass )                             },
            { CKA_KEY_TYPE,  NULL /* &xPublicKeyType */, sizeof( xPublicKeyType )                     },
            { CKA_TOKEN,     NULL /* &xTrue */,          sizeof( xTrue )                              },
            { CKA_VERIFY,    NULL /* &xTrue */,          sizeof( xTrue )                              },
            { CKA_EC_PARAMS, NULL /* xEcParams */,       sizeof( xEcParams )                          },
            { CKA_EC_POINT,  NULL /* xEcPoint */,        xLength + 2                                  },
            { CKA_LABEL,     pucPublicKeyLabel,          strlen( ( const char * ) pucPublicKeyLabel ) }
        };

        /* Aggregate initializers must not use the address of an automatic variable. */
        /* See MSVC Compiler Warning C4221 */
        xPublicKeyTemplate[ 0 ].pValue = &xClass;
        xPublicKeyTemplate[ 1 ].pValue = &xPublicKeyType;
        xPublicKeyTemplate[ 2 ].pValue = &xTrue;
        xPublicKeyTemplate[ 3 ].pValue = &xTrue;
        xPublicKeyTemplate[ 4 ].pValue = xEcParams;
        xPublicKeyTemplate[ 5 ].pValue = xEcPoint;

        xResult = pxFunctionList->C_CreateObject( xSession,
                                                  ( CK_ATTRIBUTE_PTR ) xPublicKeyTemplate,
                                                  sizeof( xPublicKeyTemplate ) / sizeof( CK_ATTRIBUTE ),
                                                  pxPublicKeyHandle );
    }
    else
    {
        xResult = CKR_ATTRIBUTE_VALUE_INVALID;
        DEV_MODE_KEY_PROVISIONING_PRINT( ( "Invalid key type. Supported options are CKK_RSA and CKK_EC" ) );
    }

    mbedtls_pk_free( &xMbedPkContext );

    return xResult;
}

/*-----------------------------------------------------------*/

/* Generate a new 2048-bit RSA key pair. Please note that C_GenerateKeyPair for
 * RSA keys is not supported by the FreeRTOS mbedTLS PKCS #11 port. */
CK_RV xProvisionGenerateKeyPairRSA( CK_SESSION_HANDLE xSession,
                                    uint8_t * pucPrivateKeyLabel,
                                    uint8_t * pucPublicKeyLabel,
                                    CK_OBJECT_HANDLE_PTR pxPrivateKeyHandle,
                                    CK_OBJECT_HANDLE_PTR pxPublicKeyHandle )
{
    CK_RV xResult;
    CK_MECHANISM xMechanism =
    {
        CKM_RSA_PKCS_KEY_PAIR_GEN, NULL_PTR, 0
    };
    CK_FUNCTION_LIST_PTR pxFunctionList;
    CK_ULONG xModulusBits = pkcs11RSA_2048_MODULUS_BITS;
    CK_BYTE xPublicExponent[] = pkcs11RSA_PUBLIC_EXPONENT;

    CK_BBOOL xTrue = CK_TRUE;
    CK_ATTRIBUTE xPublicKeyTemplate[] =
    {
        { CKA_ENCRYPT,         NULL /* &xTrue */,          sizeof( xTrue )                              },
        { CKA_VERIFY,          NULL /* &xTrue */,          sizeof( xTrue )                              },
        { CKA_MODULUS_BITS,    NULL /* &xModulusBits */,   sizeof( xModulusBits )                       },
        { CKA_PUBLIC_EXPONENT, NULL /* xPublicExponent */, sizeof( xPublicExponent )                    },
        { CKA_LABEL,           pucPublicKeyLabel,          strlen( ( const char * ) pucPublicKeyLabel ) }
    };

    /* Aggregate initializers must not use the address of an automatic variable. */
    /* See MSVC Compiler Warning C4221 */
    xPublicKeyTemplate[ 0 ].pValue = &xTrue;
    xPublicKeyTemplate[ 1 ].pValue = &xTrue;
    xPublicKeyTemplate[ 2 ].pValue = &xModulusBits;
    xPublicKeyTemplate[ 3 ].pValue = &xPublicExponent;

    CK_ATTRIBUTE xPrivateKeyTemplate[] =
    {
        { CKA_TOKEN,   NULL /* &xTrue */,  sizeof( xTrue )                               },
        { CKA_PRIVATE, NULL /* &xTrue */,  sizeof( xTrue )                               },
        { CKA_DECRYPT, NULL /* &xTrue */,  sizeof( xTrue )                               },
        { CKA_SIGN,    NULL /* &xTrue */,  sizeof( xTrue )                               },
        { CKA_LABEL,   pucPrivateKeyLabel, strlen( ( const char * ) pucPrivateKeyLabel ) }
    };

    /* Aggregate initializers must not use the address of an automatic variable. */
    /* See MSVC Compiler Warning C4221 */
    xPrivateKeyTemplate[ 0 ].pValue = &xTrue;
    xPrivateKeyTemplate[ 1 ].pValue = &xTrue;
    xPrivateKeyTemplate[ 2 ].pValue = &xTrue;
    xPrivateKeyTemplate[ 3 ].pValue = &xTrue;

    xResult = C_GetFunctionList( &pxFunctionList );

    xResult = pxFunctionList->C_GenerateKeyPair( xSession,
                                                 &xMechanism,
                                                 xPublicKeyTemplate,
                                                 sizeof( xPublicKeyTemplate ) / sizeof( CK_ATTRIBUTE ),
                                                 xPrivateKeyTemplate, sizeof( xPrivateKeyTemplate ) / sizeof( CK_ATTRIBUTE ),
                                                 pxPublicKeyHandle,
                                                 pxPrivateKeyHandle );

    return xResult;
}

/*-----------------------------------------------------------*/

/* Generate a new ECDSA key pair using curve P256. */
CK_RV xProvisionGenerateKeyPairEC( CK_SESSION_HANDLE xSession,
                                   uint8_t * pucPrivateKeyLabel,
                                   uint8_t * pucPublicKeyLabel,
                                   CK_OBJECT_HANDLE_PTR pxPrivateKeyHandle,
                                   CK_OBJECT_HANDLE_PTR pxPublicKeyHandle )
{
    CK_RV xResult;
    CK_MECHANISM xMechanism =
    {
        CKM_EC_KEY_PAIR_GEN, NULL_PTR, 0
    };
    CK_FUNCTION_LIST_PTR pxFunctionList;
    CK_BYTE xEcParams[] = pkcs11DER_ENCODED_OID_P256; /* prime256v1 */
    CK_KEY_TYPE xKeyType = CKK_EC;

    CK_BBOOL xTrue = CK_TRUE;
    CK_ATTRIBUTE xPublicKeyTemplate[] =
    {
        { CKA_KEY_TYPE,  NULL /* &xKeyType */, sizeof( xKeyType )                           },
        { CKA_VERIFY,    NULL /* &xTrue */,    sizeof( xTrue )                              },
        { CKA_EC_PARAMS, NULL /* xEcParams */, sizeof( xEcParams )                          },
        { CKA_LABEL,     pucPublicKeyLabel,    strlen( ( const char * ) pucPublicKeyLabel ) }
    };

    /* Aggregate initializers must not use the address of an automatic variable. */
    /* See MSVC Compiler Warning C4221 */
    xPublicKeyTemplate[ 0 ].pValue = &xKeyType;
    xPublicKeyTemplate[ 1 ].pValue = &xTrue;
    xPublicKeyTemplate[ 2 ].pValue = &xEcParams;

    CK_ATTRIBUTE xPrivateKeyTemplate[] =
    {
        { CKA_KEY_TYPE, NULL /* &xKeyType */, sizeof( xKeyType )                            },
        { CKA_TOKEN,    NULL /* &xTrue */,    sizeof( xTrue )                               },
        { CKA_PRIVATE,  NULL /* &xTrue */,    sizeof( xTrue )                               },
        { CKA_SIGN,     NULL /* &xTrue */,    sizeof( xTrue )                               },
        { CKA_LABEL,    pucPrivateKeyLabel,   strlen( ( const char * ) pucPrivateKeyLabel ) }
    };

    /* Aggregate initializers must not use the address of an automatic variable. */
    /* See MSVC Compiler Warning C4221 */
    xPrivateKeyTemplate[ 0 ].pValue = &xKeyType;
    xPrivateKeyTemplate[ 1 ].pValue = &xTrue;
    xPrivateKeyTemplate[ 2 ].pValue = &xTrue;
    xPrivateKeyTemplate[ 3 ].pValue = &xTrue;

    xResult = C_GetFunctionList( &pxFunctionList );

    xResult = pxFunctionList->C_GenerateKeyPair( xSession,
                                                 &xMechanism,
                                                 xPublicKeyTemplate,
                                                 sizeof( xPublicKeyTemplate ) / sizeof( CK_ATTRIBUTE ),
                                                 xPrivateKeyTemplate, sizeof( xPrivateKeyTemplate ) / sizeof( CK_ATTRIBUTE ),
                                                 pxPublicKeyHandle,
                                                 pxPrivateKeyHandle );

    return xResult;
}

/*-----------------------------------------------------------*/

/* Import the specified X.509 client certificate into storage. */
CK_RV xProvisionCertificate( CK_SESSION_HANDLE xSession,
                             uint8_t * pucCertificate,
                             size_t xCertificateLength,
                             uint8_t * pucLabel,
                             CK_OBJECT_HANDLE_PTR pxObjectHandle )
{
    PKCS11_CertificateTemplate_t xCertificateTemplate;
    CK_OBJECT_CLASS xCertificateClass = CKO_CERTIFICATE;
    CK_CERTIFICATE_TYPE xCertificateType = CKC_X_509;
    CK_FUNCTION_LIST_PTR pxFunctionList;
    CK_RV xResult;
    uint8_t * pucDerObject = NULL;
    int32_t lConversionReturn = 0;
    size_t xDerLen = 0;
    CK_BBOOL xTokenStorage = CK_TRUE;

    /* TODO: Subject is a required attribute.
     * Currently, this field is not used by FreeRTOS ports,
     * this should be updated so that subject matches proper
     * format for future ports. */
    CK_BYTE xSubject[] = "TestSubject";

    /* Initialize the client certificate template. */
    xCertificateTemplate.xObjectClass.type = CKA_CLASS;
    xCertificateTemplate.xObjectClass.pValue = &xCertificateClass;
    xCertificateTemplate.xObjectClass.ulValueLen = sizeof( xCertificateClass );
    xCertificateTemplate.xSubject.type = CKA_SUBJECT;
    xCertificateTemplate.xSubject.pValue = xSubject;
    xCertificateTemplate.xSubject.ulValueLen = strlen( ( const char * ) xSubject );
    xCertificateTemplate.xValue.type = CKA_VALUE;
    xCertificateTemplate.xValue.pValue = ( CK_VOID_PTR ) pucCertificate;
    xCertificateTemplate.xValue.ulValueLen = ( CK_ULONG ) xCertificateLength;
    xCertificateTemplate.xLabel.type = CKA_LABEL;
    xCertificateTemplate.xLabel.pValue = ( CK_VOID_PTR ) pucLabel;
    xCertificateTemplate.xLabel.ulValueLen = strlen( ( const char * ) pucLabel );
    xCertificateTemplate.xCertificateType.type = CKA_CERTIFICATE_TYPE;
    xCertificateTemplate.xCertificateType.pValue = &xCertificateType;
    xCertificateTemplate.xCertificateType.ulValueLen = sizeof( CK_CERTIFICATE_TYPE );
    xCertificateTemplate.xTokenObject.type = CKA_TOKEN;
    xCertificateTemplate.xTokenObject.pValue = &xTokenStorage;
    xCertificateTemplate.xTokenObject.ulValueLen = sizeof( xTokenStorage );

    xResult = C_GetFunctionList( &pxFunctionList );

    /* Test for a valid certificate: 0x2d is '-', as in ----- BEGIN CERTIFICATE. */
    if( ( pucCertificate == NULL ) || ( pucCertificate[ 0 ] != 0x2d ) )
    {
        xResult = CKR_ATTRIBUTE_VALUE_INVALID;
    }

    if( xResult == CKR_OK )
    {
        /* Convert the certificate to DER format if it was in PEM. The DER key
         * should be about 3/4 the size of the PEM key, so mallocing the PEM key
         * size is sufficient. */
        pucDerObject = pvPortMalloc( xCertificateTemplate.xValue.ulValueLen );
        xDerLen = xCertificateTemplate.xValue.ulValueLen;

        if( pucDerObject != NULL )
        {
            lConversionReturn = convert_pem_to_der( xCertificateTemplate.xValue.pValue,
                                                    xCertificateTemplate.xValue.ulValueLen,
                                                    pucDerObject,
                                                    &xDerLen );

            if( 0 != lConversionReturn )
            {
                xResult = CKR_ARGUMENTS_BAD;
            }
        }
        else
        {
            xResult = CKR_HOST_MEMORY;
        }
    }

    if( xResult == CKR_OK )
    {
        /* Set the template pointers to refer to the DER converted objects. */
        xCertificateTemplate.xValue.pValue = pucDerObject;
        xCertificateTemplate.xValue.ulValueLen = xDerLen;
    }

    /* Best effort clean-up of the existing object, if it exists. */
    if( xResult == CKR_OK )
    {
        xDestroyProvidedObjects( xSession,
                                 &pucLabel,
                                 &xCertificateClass,
                                 1 );
    }

    /* Create an object using the encoded client certificate. */
    if( xResult == CKR_OK )
    {
    	DEV_MODE_KEY_PROVISIONING_PRINT( ( "Write certificate...\r\n" ) );

        xResult = pxFunctionList->C_CreateObject( xSession,
                                                  ( CK_ATTRIBUTE_PTR ) &xCertificateTemplate,
                                                  sizeof( xCertificateTemplate ) / sizeof( CK_ATTRIBUTE ),
                                                  pxObjectHandle );
    }

    if( pucDerObject != NULL )
    {
        vPortFree( pucDerObject );
    }

    return xResult;
}

/*-----------------------------------------------------------*/

/* Delete the specified crypto object from storage. */
CK_RV xDestroyProvidedObjects( CK_SESSION_HANDLE xSession,
                               CK_BYTE_PTR * ppxPkcsLabels,
                               CK_OBJECT_CLASS * xClass,
                               CK_ULONG ulCount )
{
    CK_RV xResult;
    CK_FUNCTION_LIST_PTR pxFunctionList;
    CK_OBJECT_HANDLE xObjectHandle;
    CK_BYTE * pxLabel;
    CK_ULONG uiIndex = 0;

    xResult = C_GetFunctionList( &pxFunctionList );

    for( uiIndex = 0; uiIndex < ulCount; uiIndex++ )
    {
        pxLabel = ppxPkcsLabels[ uiIndex ];

        xResult = xFindObjectWithLabelAndClass( xSession,
                                                ( char * ) pxLabel,
                                                strlen( ( char * ) pxLabel ),
                                                xClass[ uiIndex ],
                                                &xObjectHandle );

        while( ( xResult == CKR_OK ) && ( xObjectHandle != CK_INVALID_HANDLE ) )
        {
            xResult = pxFunctionList->C_DestroyObject( xSession, xObjectHandle );

            /* PKCS #11 allows a module to maintain multiple objects with the same
             * label and type. The intent of this loop is to try to delete all of them.
             * However, to avoid getting stuck, we won't try to find another object
             * of the same label/type if the previous delete failed. */
            if( xResult == CKR_OK )
            {
                xResult = xFindObjectWithLabelAndClass( xSession,
                                                        ( char * ) pxLabel,
                                                        strlen( ( char * ) pxLabel ),
                                                        xClass[ uiIndex ],
                                                        &xObjectHandle );
            }
            else
            {
                break;
            }
        }

        if( xResult == CKR_FUNCTION_NOT_SUPPORTED )
        {
            break;
        }
    }

    return xResult;
}

/*-----------------------------------------------------------*/

/* Delete well-known crypto objects from storage. */
CK_RV xDestroyDefaultCryptoObjects( CK_SESSION_HANDLE xSession )
{
    CK_RV xResult;
    CK_BYTE * pxPkcsLabels[] =
    {
        ( CK_BYTE * ) pkcs11configLABEL_DEVICE_CERTIFICATE_FOR_TLS,
        ( CK_BYTE * ) pkcs11configLABEL_CODE_VERIFICATION_KEY,
        ( CK_BYTE * ) pkcs11configLABEL_DEVICE_PRIVATE_KEY_FOR_TLS,
        ( CK_BYTE * ) pkcs11configLABEL_DEVICE_PUBLIC_KEY_FOR_TLS
    };
    CK_OBJECT_CLASS xClass[] =
    {
        CKO_CERTIFICATE,
        CKO_PUBLIC_KEY,
        CKO_PRIVATE_KEY,
        CKO_PUBLIC_KEY
    };

    xResult = xDestroyProvidedObjects( xSession,
                                       pxPkcsLabels,
                                       xClass,
                                       sizeof( xClass ) / sizeof( CK_OBJECT_CLASS ) );

    return xResult;
}

/* Delete well-known crypto objects from storage. */
CK_RV xDestroyDefaultCertificateObjects( CK_SESSION_HANDLE xSession )
{
    CK_RV xResult;
    CK_BYTE * pxPkcsLabels[] =
    {
        ( CK_BYTE * ) pkcs11configLABEL_DEVICE_CERTIFICATE_FOR_TLS,
    };
    CK_OBJECT_CLASS xClass[] =
    {
        CKO_CERTIFICATE
    };

    xResult = xDestroyProvidedObjects( xSession,
                                       pxPkcsLabels,
                                       xClass,
                                       sizeof( xClass ) / sizeof( CK_OBJECT_CLASS ) );

    return xResult;
}

CK_RV xDestroyDefaultPrivatekeyObjects( CK_SESSION_HANDLE xSession )
{
    CK_RV xResult;
    CK_BYTE * pxPkcsLabels[] =
    {
        ( CK_BYTE * ) pkcs11configLABEL_DEVICE_PRIVATE_KEY_FOR_TLS,
    };
    CK_OBJECT_CLASS xClass[] =
    {
		CKO_PRIVATE_KEY
    };

    xResult = xDestroyProvidedObjects( xSession,
                                       pxPkcsLabels,
                                       xClass,
                                       sizeof( xClass ) / sizeof( CK_OBJECT_CLASS ) );

    return xResult;
}

CK_RV xDestroyDefaultObjects( KVStoreKey_t ID, CK_SESSION_HANDLE xSession )
{
	CK_RV xResult = CKR_ARGUMENTS_BAD;
	if (ID == KVS_DEVICE_CERT_ID)
	{
		CK_BYTE * pxPkcsLabels[] =
		{
			( CK_BYTE * ) pkcs11configLABEL_DEVICE_CERTIFICATE_FOR_TLS,
		};
		CK_OBJECT_CLASS xClass[] =
		{
			CKO_PRIVATE_KEY
		};
		xResult = xDestroyProvidedObjects( xSession,
											   pxPkcsLabels,
											   xClass,
											   sizeof( xClass ) / sizeof( CK_OBJECT_CLASS ) );
	}

	if(ID == KVS_DEVICE_PRIVKEY_ID)
	{
		CK_BYTE * pxPkcsLabels[] =
		{
			( CK_BYTE * ) pkcs11configLABEL_DEVICE_PRIVATE_KEY_FOR_TLS,
		};
		CK_OBJECT_CLASS xClass[] =
		{
			CKO_PRIVATE_KEY
		};
		xResult = xDestroyProvidedObjects( xSession,
											   pxPkcsLabels,
											   xClass,
											   sizeof( xClass ) / sizeof( CK_OBJECT_CLASS ) );
	}

	if(ID == KVS_DEVICE_PUBKEY_ID)
		{
			CK_BYTE * pxPkcsLabels[] =
			{
				( CK_BYTE * ) pkcs11configLABEL_DEVICE_PUBLIC_KEY_FOR_TLS,
			};
			CK_OBJECT_CLASS xClass[] =
			{
				CKO_PUBLIC_KEY
			};
			xResult = xDestroyProvidedObjects( xSession,
												   pxPkcsLabels,
												   xClass,
												   sizeof( xClass ) / sizeof( CK_OBJECT_CLASS ) );
		}



	return xResult;
}
/*-----------------------------------------------------------*/

/* Perform device provisioning using the default TLS client credentials. */
CK_RV vDevModeKeyPreProvisioning( KeyValueStore_t Keystore, KVStoreKey_t ID, int32_t xvaluelength )
{
	size_t valueLength = 0;
	char * pcBuffer = NULL;
	char * temp = NULL;
	PreProvisioningParams_t xParams ;


	pcBuffer = pvPortMalloc( xvaluelength + 1 );
	xReadEntry(ID, pcBuffer, xvaluelength);
	pcBuffer[xvaluelength] =  '\0';
	xParams.pucClientCredential  =  (uint8_t *)pcBuffer;

	/* The hard-coded client certificate and private key can be useful for
	* first-time lab testing. They are optional after the first run, though, and
	* not recommended at all for going into production. */
	if( ( NULL != xParams.pucClientCredential ) &&
	( 0 != strcmp( "", ( const char * ) xParams.pucClientCredential ) ) )
	{
		/* We want the NULL terminator to be written to storage, so include it
		* in the length calculation. */
		xParams.ulClientCredentialLength = sizeof( char ) + Keystore.table[ ID ].valueLength;
	}
	else
	{
		xParams.pucClientCredential = NULL;
	}


	return vAlternateKeyPreProvisioning(ID, &xParams );
}
BaseType_t vAlternateKeyPreProvisioning(KVStoreKey_t ID, PreProvisioningParams_t * xParams )
{
	CK_RV xResult = CKR_OK;
	CK_FUNCTION_LIST_PTR pxFunctionList = NULL;
	CK_SESSION_HANDLE xSession = 0;

	xResult = C_GetFunctionList( &pxFunctionList );

	/* Initialize the PKCS Module */
	if( xResult == CKR_OK )
	{
		xResult = xInitializePkcs11Token();
	}

	if( xResult == CKR_OK )
	{
		xResult = xInitializePkcs11Session( &xSession );
	}
	if( xResult == CKR_OK )
	{
		xResult = xPreProvisionDevice( xSession,ID, xParams );

		pxFunctionList->C_CloseSession( xSession );
	}

	return ( xResult == CKR_OK );
}

CK_RV xPreProvisionDevice( CK_SESSION_HANDLE xSession, KVStoreKey_t ID, PreProvisioningParams_t * pxParams )
{
	CK_RV xResult;
	CK_FUNCTION_LIST_PTR pxFunctionList;
	ProvisionedState_t xProvisionedState = { 0 };
	CK_OBJECT_HANDLE xObject = 0;
	BaseType_t status;
	extern BaseType_t xPending;
	char *pcBuffer = NULL;
	size_t bufferSize;


	xResult = C_GetFunctionList( &pxFunctionList );
	if (xResult == CKR_OK)
	{
		if( ( NULL != pxParams->pucClientCredential ))
		{
			bufferSize = prvGetCacheEntryLength(ID);
            pcBuffer = pvPortMalloc( bufferSize );
			xReadEntry(ID, pcBuffer, bufferSize);
			status = xPending;
			if(status == pdTRUE)
			{
				if ( strncmp( PEM_BEGIN,  (const char *)pxParams->pucClientCredential, strlen( PEM_BEGIN ) ) == 0 )
				{
					bool contain_end = strstr((const char *)pxParams->pucClientCredential,PEM_END);
					if( !contain_end )
					{
						DEV_MODE_KEY_PROVISIONING_PRINT(( "Error: PEM header does not contain the expected ending: '" PEM_END "'.\r\n" ));
						xResult = CKR_ARGUMENTS_BAD;
					}
					else
					{
						xResult = xDestroyDefaultObjects( ID,xSession );

						if (ID ==KVS_DEVICE_CERT_ID)
						{
							if( xResult != CKR_OK )
							{
								DEV_MODE_KEY_PROVISIONING_PRINT( ( "Warning: could not clean-up old crypto Certificate!\r\n" ) );
							}
							else
							{
								DEV_MODE_KEY_PROVISIONING_PRINT(( "Destroyed Certificate.\r\n" ));
								xResult = xProvisionCertificate( xSession,
																		 (uint8_t *)pxParams->pucClientCredential,
																		 pxParams->ulClientCredentialLength,
																		 ( uint8_t * ) pkcs11configLABEL_DEVICE_CERTIFICATE_FOR_TLS,
																		 &xObject );

								if( ( xResult != CKR_OK ) || ( xObject == CK_INVALID_HANDLE ) )
								{
									DEV_MODE_KEY_PROVISIONING_PRINT( ( "ERROR: Failed to provision device certificate. %d \r\n", xResult ) );
								}
							}
						}
						if (ID == KVS_DEVICE_PRIVKEY_ID)
						{
							if( xResult != CKR_OK )
							{
								DEV_MODE_KEY_PROVISIONING_PRINT( ( "Warning: could not clean-up old crypto Private key!\r\n" ) );
							}
							else
							{
								DEV_MODE_KEY_PROVISIONING_PRINT(( "Destroyed Private key.\r\n" ));
								xResult = xProvisionPrivateKey( xSession,  (uint8_t *)pxParams->pucClientCredential,
																		 pxParams->ulClientCredentialLength,
																		 ( uint8_t * ) pkcs11configLABEL_DEVICE_PRIVATE_KEY_FOR_TLS,
																		 &xObject );

								if( ( xResult != CKR_OK ) || ( xObject == CK_INVALID_HANDLE ) )
								{
									DEV_MODE_KEY_PROVISIONING_PRINT( ( "ERROR: Failed to provision device Private key. %d \r\n", xResult ) );
								}
								else
								{
									DEV_MODE_KEY_PROVISIONING_PRINT(( "Write Private key...\r\n" ));
								}
							}
						}
						if (ID == KVS_DEVICE_PUBKEY_ID)
						{
							if( xResult != CKR_OK )
							{
								DEV_MODE_KEY_PROVISIONING_PRINT( ( "Warning: could not clean-up old crypto Public key!\r\n" ) );
							}
							else
							{
								DEV_MODE_KEY_PROVISIONING_PRINT(( "Destroyed PUB key.\r\n" ));
								xResult = xProvisionPublicKey( xSession,  (uint8_t *)pxParams->pucClientCredential,
																		 pxParams->ulClientCredentialLength,
																		 CKK_EC,
																		 ( uint8_t * ) pkcs11configLABEL_DEVICE_PUBLIC_KEY_FOR_TLS,
																		 &xObject );

								if( ( xResult != CKR_OK ) || ( xObject == CK_INVALID_HANDLE ) )
								{
									DEV_MODE_KEY_PROVISIONING_PRINT( ( "ERROR: Failed to provision device Public key. %d \r\n", xResult ) );
								}
								else
								{
									DEV_MODE_KEY_PROVISIONING_PRINT(( "Write Public key...\r\n" ));
								}
							}
						}

					}
				}
				else
				{
					DEV_MODE_KEY_PROVISIONING_PRINT(( "Error: PEM header does not contain the expected text: '" PEM_BEGIN "'.\r\n" ));
					xResult = CKR_ARGUMENTS_BAD;
				}
			}
			else
			{
				DEV_MODE_KEY_PROVISIONING_PRINT( ( "Committed last time!\r\n" ) );
			}

		}
		else
		{
			DEV_MODE_KEY_PROVISIONING_PRINT( ( "No credentials!\r\n" ) );
			xResult = CKR_ARGUMENTS_BAD;
		}
	}
	/* Free memory. */
	if( NULL != xProvisionedState.pucDerPublicKey )
	{
		vPortFree( xProvisionedState.pucDerPublicKey );
	}

	if( NULL != xProvisionedState.pcIdentifier )
	{
		vPortFree( xProvisionedState.pcIdentifier );
	}

	vPortFree(pxParams->pucClientCredential);

	return xResult;
}
