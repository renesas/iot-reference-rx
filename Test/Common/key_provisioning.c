/* Standard includes. */
#include <stdio.h>
#include <string.h>

/* FreeRTOS includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

/* PKCS#11 includes. */
#include "core_pkcs11.h"
#include "test_param_config.h"

/* Client credential includes. */
#include "aws_clientcredential.h"
#include "aws_clientcredential_keys.h"
#include "iot_default_root_certificates.h"

/* Key provisioning include. */
#include "dev_mode_key_provisioning.h"

/* Utilities include. */
#include "core_pki_utils.h"

/* mbedTLS includes. */
#include "mbedtls/pk.h"
#include "mbedtls/oid.h"

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
#ifndef DELAY_BEFORE_KEY_PAIR_GENERATION_SECS
    #define DELAY_BEFORE_KEY_PAIR_GENERATION_SECS    ( 10 )
#endif


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

/*-----------------------------------------------------------*/

static CK_RV prvExportPublicKey( CK_SESSION_HANDLE xSession,
                                 CK_OBJECT_HANDLE xPublicKeyHandle,
                                 uint8_t ** ppucDerPublicKey,
                                 uint32_t * pulDerPublicKeyLength )
{
    CK_RV xResult;
    CK_FUNCTION_LIST_PTR pxFunctionList;
    CK_KEY_TYPE xKeyType = 0;
    CK_ATTRIBUTE xTemplate = { 0 };
    uint8_t pucEcP256AsnAndOid[] =
    {
        0x30, 0x59, 0x30, 0x13, 0x06, 0x07, 0x2a, 0x86,
        0x48, 0xce, 0x3d, 0x02, 0x01, 0x06, 0x08, 0x2a,
        0x86, 0x48, 0xce, 0x3d, 0x03, 0x01, 0x07, 0x03,
        0x42, 0x00
    };
    uint8_t pucUnusedKeyTag[] = { 0x04, 0x41 };

    /* This variable is used only for its size. This gets rid of compiler warnings. */
    ( void ) pucUnusedKeyTag;

    xResult = C_GetFunctionList( &pxFunctionList );

    /* Query the key type. */
    if( CKR_OK == xResult )
    {
        xTemplate.type = CKA_KEY_TYPE;
        xTemplate.pValue = &xKeyType;
        xTemplate.ulValueLen = sizeof( xKeyType );
        xResult = pxFunctionList->C_GetAttributeValue( xSession,
                                                       xPublicKeyHandle,
                                                       &xTemplate,
                                                       1 );
    }

    /* Scope to ECDSA keys only, since there's currently no use case for
     * onboard keygen and certificate enrollment for RSA. */
    if( ( CKR_OK == xResult ) && ( CKK_ECDSA == xKeyType ) )
    {
        /* Query the size of the public key. */
        xTemplate.type = CKA_EC_POINT;
        xTemplate.pValue = NULL;
        xTemplate.ulValueLen = 0;
        xResult = pxFunctionList->C_GetAttributeValue( xSession,
                                                       xPublicKeyHandle,
                                                       &xTemplate,
                                                       1 );

        /* Allocate a buffer large enough for the full, encoded public key. */
        if( CKR_OK == xResult )
        {
            /* Add space for the full DER header. */
            xTemplate.ulValueLen += sizeof( pucEcP256AsnAndOid ) - sizeof( pucUnusedKeyTag );
            *pulDerPublicKeyLength = xTemplate.ulValueLen;

            /* Get a heap buffer. */
            *ppucDerPublicKey = pvPortMalloc( xTemplate.ulValueLen );

            /* Check for resource exhaustion. */
            if( NULL == *ppucDerPublicKey )
            {
                xResult = CKR_HOST_MEMORY;
            }
        }

        /* Export the public key. */
        if( CKR_OK == xResult )
        {
            xTemplate.pValue = *ppucDerPublicKey + sizeof( pucEcP256AsnAndOid ) - sizeof( pucUnusedKeyTag );
            xTemplate.ulValueLen -= ( sizeof( pucEcP256AsnAndOid ) - sizeof( pucUnusedKeyTag ) );
            xResult = pxFunctionList->C_GetAttributeValue( xSession,
                                                           xPublicKeyHandle,
                                                           &xTemplate,
                                                           1 );
        }

        /* Prepend the full DER header. */
        if( CKR_OK == xResult )
        {
            memcpy( *ppucDerPublicKey, pucEcP256AsnAndOid, sizeof( pucEcP256AsnAndOid ) );
        }
    }

    /* Free memory if there was an error after allocation. */
    if( ( NULL != *ppucDerPublicKey ) && ( CKR_OK != xResult ) )
    {
        vPortFree( *ppucDerPublicKey );
        *ppucDerPublicKey = NULL;
    }

    return xResult;
}

/* Determine which required client crypto objects are already present in
 * storage. */
static CK_RV prvGetProvisionedState( CK_SESSION_HANDLE xSession,
                                     ProvisionedState_t * pxProvisionedState )
{
    CK_RV xResult;
    CK_FUNCTION_LIST_PTR pxFunctionList;
    CK_SLOT_ID_PTR pxSlotId = NULL;
    CK_ULONG ulSlotCount = 0;
    CK_TOKEN_INFO xTokenInfo = { 0 };
    unsigned int i = 0;

    xResult = C_GetFunctionList( &pxFunctionList );

    /* Check for a private key. */
    if( CKR_OK == xResult )
    {
        xResult = xFindObjectWithLabelAndClass( xSession,
                                                pkcs11configLABEL_DEVICE_PRIVATE_KEY_FOR_TLS,
                                                sizeof( pkcs11configLABEL_DEVICE_PRIVATE_KEY_FOR_TLS ) - 1,
                                                CKO_PRIVATE_KEY,
                                                &pxProvisionedState->xPrivateKey );
    }

    if( ( CKR_OK == xResult ) && ( CK_INVALID_HANDLE != pxProvisionedState->xPrivateKey ) )
    {
        /* Check also for the corresponding public. */
        xResult = xFindObjectWithLabelAndClass( xSession,
                                                pkcs11configLABEL_DEVICE_PUBLIC_KEY_FOR_TLS,
                                                sizeof( pkcs11configLABEL_DEVICE_PUBLIC_KEY_FOR_TLS ) - 1,
                                                CKO_PUBLIC_KEY,
                                                &pxProvisionedState->xPublicKey );
    }

    if( ( CKR_OK == xResult ) && ( CK_INVALID_HANDLE != pxProvisionedState->xPublicKey ) )
    {
        /* Export the public key. */
        xResult = prvExportPublicKey( xSession,
                                      pxProvisionedState->xPublicKey,
                                      &pxProvisionedState->pucDerPublicKey,
                                      &pxProvisionedState->ulDerPublicKeyLength );
    }

    /* Check for the client certificate. */
    if( CKR_OK == xResult )
    {
        xResult = xFindObjectWithLabelAndClass( xSession,
                                                pkcs11configLABEL_DEVICE_CERTIFICATE_FOR_TLS,
                                                sizeof( pkcs11configLABEL_DEVICE_CERTIFICATE_FOR_TLS ) - 1,
                                                CKO_CERTIFICATE,
                                                &pxProvisionedState->xClientCertificate );
    }

    /* Check for a crypto element identifier. */
    if( CKR_OK == xResult )
    {
        xResult = xGetSlotList( &pxSlotId, &ulSlotCount );
    }

    if( CKR_OK == xResult )
    {
        xResult = pxFunctionList->C_GetTokenInfo( pxSlotId[ 0 ], &xTokenInfo );
        vPortFree( pxSlotId );
    }

    if( ( CKR_OK == xResult ) && ( '\0' != xTokenInfo.label[ 0 ] ) && ( ' ' != xTokenInfo.label[ 0 ] ) )
    {
        /* PKCS #11 requires that token info fields are padded out with space
         * characters. However, a NULL terminated copy will be more useful to the
         * caller. */
        for( i = 0; i < sizeof( xTokenInfo.label ); i++ )
        {
            if( xTokenInfo.label[ i ] == ' ' )
            {
                break;
            }
        }

        if( 0 != i )
        {
            pxProvisionedState->pcIdentifier = ( char * ) pvPortMalloc( 1 + i * sizeof( xTokenInfo.label[ 0 ] ) );

            if( NULL != pxProvisionedState->pcIdentifier )
            {
                memcpy( pxProvisionedState->pcIdentifier,
                        xTokenInfo.label,
                        i );
                pxProvisionedState->pcIdentifier[ i ] = '\0';
            }
            else
            {
                xResult = CKR_HOST_MEMORY;
            }
        }
    }

    return xResult;
}

/*-----------------------------------------------------------*/

/* Write the ASN.1 encoded bytes of the device public key to the console.
 * This is for debugging purposes as well as to faciliate developer-driven
 * certificate enrollment for onboard crypto hardware (i.e. if available). */
static void prvWriteHexBytesToConsole( char * pcDescription,
                                       uint8_t * pucData,
                                       uint32_t ulDataLength )
{
#define BYTES_TO_DISPLAY_PER_ROW    16
    char pcByteRow[ 1 + ( BYTES_TO_DISPLAY_PER_ROW * 2 ) + ( BYTES_TO_DISPLAY_PER_ROW / 2 ) ];
    char * pcNextChar = pcByteRow;
    uint32_t ulIndex = 0;
    uint8_t ucByteValue = 0;

    /* Write help text to the console. */
    configPRINTF( ( "%s, %d bytes:\r\n", pcDescription, ulDataLength ) );

    /* Iterate over the bytes of the encoded public key. */
    for( ; ulIndex < ulDataLength; ulIndex++ )
    {
        /* Convert one byte to ASCII hex. */
        ucByteValue = *( pucData + ulIndex );
        snprintf( pcNextChar,
                  sizeof( pcByteRow ) - ( pcNextChar - pcByteRow ),
                  "%02x",
                  ucByteValue );
        pcNextChar += 2;

        /* Check for the end of a two-byte display word. */
        if( 0 == ( ( ulIndex + 1 ) % sizeof( uint16_t ) ) )
        {
            *pcNextChar = ' ';
            pcNextChar++;
        }

        /* Check for the end of a row. */
        if( 0 == ( ( ulIndex + 1 ) % BYTES_TO_DISPLAY_PER_ROW ) )
        {
            *pcNextChar = '\0';
            vLoggingPrint( pcByteRow );
            vLoggingPrint( "\r\n" );
            pcNextChar = pcByteRow;
        }
    }

    /* Check for a partial line to print. */
    if( pcNextChar > pcByteRow )
    {
        *pcNextChar = '\0';
        vLoggingPrint( pcByteRow );
        vLoggingPrint( "\r\n" );
    }
}

/*-----------------------------------------------------------*/
/* Attempt to provision the device with a client certificate, associated
 * private and public key pair, and optional Just-in-Time Registration certificate.
 * If either component of the key pair is unavailable in storage, generate a new
 * pair. */
CK_RV xProvisionDevice( CK_SESSION_HANDLE xSession,
                        ProvisioningParams_t * pxParams )
{
    CK_RV xResult;
    CK_FUNCTION_LIST_PTR pxFunctionList;
    ProvisionedState_t xProvisionedState = { 0 };
    CK_OBJECT_HANDLE xObject = 0;
    CK_BBOOL xImportedPrivateKey = CK_FALSE;
    CK_BBOOL xKeyPairGenerationMode = CK_FALSE;

    xResult = C_GetFunctionList( &pxFunctionList );

    #if ( PKCS11_TEST_IMPORT_PRIVATE_KEY_SUPPORT == 1 )

        /* Attempt to clean-up old crypto objects, but only if private key import is
         * supported by this application, and only if the caller has provided new
         * objects to use instead. */
        if( ( CKR_OK == xResult ) &&
            ( NULL != pxParams->pucClientCertificate ) &&
            ( NULL != pxParams->pucClientPrivateKey ) )
        {
            xResult = xDestroyDefaultCryptoObjects( xSession );

            if( xResult != CKR_OK )
            {
                configPRINTF( ( "Warning: could not clean-up old crypto objects. %d \r\n", xResult ) );
            }
        }
    #endif /* if ( PKCS11_TEST_IMPORT_PRIVATE_KEY_SUPPORT == 1 ) */

    /* If a client certificate has been provided by the caller, attempt to
     * import it. */
    if( ( xResult == CKR_OK ) && ( NULL != pxParams->pucClientCertificate ) )
    {
        xResult = xProvisionCertificate( xSession,
                                         pxParams->pucClientCertificate,
                                         pxParams->ulClientCertificateLength,
                                         ( uint8_t * ) pkcs11configLABEL_DEVICE_CERTIFICATE_FOR_TLS,
                                         &xObject );

        if( ( xResult != CKR_OK ) || ( xObject == CK_INVALID_HANDLE ) )
        {
            configPRINTF( ( "ERROR: Failed to provision device certificate. %d \r\n", xResult ) );
        }
    }

    #if ( PKCS11_TEST_IMPORT_PRIVATE_KEY_SUPPORT == 1 )

        /* If this application supports importing private keys, and if a private
         * key has been provided by the caller, attempt to import it. */
        if( ( xResult == CKR_OK ) && ( NULL != pxParams->pucClientPrivateKey ) )
        {
            xResult = xProvisionPrivateKey( xSession,
                                            pxParams->pucClientPrivateKey,
                                            pxParams->ulClientPrivateKeyLength,
                                            ( uint8_t * ) pkcs11configLABEL_DEVICE_PRIVATE_KEY_FOR_TLS,
                                            &xObject );

            if( ( xResult != CKR_OK ) || ( xObject == CK_INVALID_HANDLE ) )
            {
                configPRINTF( ( "ERROR: Failed to provision device private key with status %d.\r\n", xResult ) );
            }
            else
            {
                xImportedPrivateKey = CK_TRUE;
            }
        }
    #endif /* if ( PKCS11_TEST_IMPORT_PRIVATE_KEY_SUPPORT == 1 ) */

    /* If a Just-in-Time Provisioning certificate has been provided by the
     * caller, attempt to import it. Not all crypto tokens
     * and PKCS #11 module implementations provide storage for this particular
     * object. In that case, the statically defined object, if any, will be used
     * during TLS session negotiation with AWS IoT. */
    if( ( xResult == CKR_OK ) && ( NULL != pxParams->pucJITPCertificate ) )
    {
        xResult = xProvisionCertificate( xSession,
                                         pxParams->pucJITPCertificate,
                                         pxParams->ulJITPCertificateLength,
                                         ( uint8_t * ) pkcs11configLABEL_JITP_CERTIFICATE,
                                         &xObject );

        if( xResult == CKR_DEVICE_MEMORY )
        {
            xResult = CKR_OK;
            configPRINTF( ( "Warning: no persistent storage is available for the JITP certificate. The certificate in aws_clientcredential_keys.h will be used instead.\r\n" ) );
        }
    }

    /* Check whether a key pair is now present. In order to support X.509
     * certificate enrollment, the public and private key objects must both be
     * available. */
    if( ( xResult == CKR_OK ) && ( CK_FALSE == xImportedPrivateKey ) )
    {
        xResult = prvGetProvisionedState( xSession,
                                          &xProvisionedState );

        if( ( CK_INVALID_HANDLE == xProvisionedState.xPrivateKey ) ||
            ( CK_INVALID_HANDLE == xProvisionedState.xPublicKey ) ||
            ( NULL == xProvisionedState.pucDerPublicKey ) )
        {
            xKeyPairGenerationMode = CK_TRUE;
        }

        /* Ignore errors, since the board may have been previously used with a
         * different crypto middleware or app. If any of the above objects
         * couldn't be read, try to generate new ones below. */
        xResult = CKR_OK;
    }

    #if ( 1 == FORCE_GENERATE_NEW_KEY_PAIR )
        xKeyPairGenerationMode = CK_TRUE;
    #endif

    if( ( xResult == CKR_OK ) && ( CK_TRUE == xKeyPairGenerationMode ) )
    {
        /* Add a delay before calling logic for generating new key-pair on device (if boards supports on-board key-pair
         * generation) to avoid possible scenario of unexpectedly generating new keys on board during the flashing process
         * of a new image on the board.
         * If the flashing workflow of a device (for example, ESP32 boards) involves resetting the board before
         * flashing a new image, then a race condition can occur between the execution of an already
         * existing image on device (that is triggered by the device reset) and the flashing of the new image on the
         * device. When the existing image present on the device is configured to generate new key-pair (through the
         * FORCE_GENERATE_NEW_KEY_PAIR config), then a possible scenario of unexpected key-pair
         * generation on device can occur during flashing process, in which case, the certificate provisioned by
         * user becomes stale and device cannot perform TLS connection with servers as the provisioned device certificate
         * does not match the unexpectedly generated new key-pair.
         * Thus, by adding a delay, the possibility of hitting the race condition of the device executing an old
         * image that generates new key-pair is avoided because the logic of generating new key-pair is not executed
         * before the flashing process starts loading the new image onto the board.
         * Note: The delay of 150 seconds is used based on testing with an ESP32+ECC608A board. */
        configPRINTF( ( "Waiting for %d seconds before generating key-pair", DELAY_BEFORE_KEY_PAIR_GENERATION_SECS ) );
        vTaskDelay( pdMS_TO_TICKS( DELAY_BEFORE_KEY_PAIR_GENERATION_SECS * 1000 ) );

        /* Generate a new default key pair. */
        xResult = xProvisionGenerateKeyPairEC( xSession,
                                               ( uint8_t * ) pkcs11configLABEL_DEVICE_PRIVATE_KEY_FOR_TLS,
                                               ( uint8_t * ) pkcs11configLABEL_DEVICE_PUBLIC_KEY_FOR_TLS,
                                               &xProvisionedState.xPrivateKey,
                                               &xProvisionedState.xPublicKey );

        if( CKR_OK == xResult )
        {
            /* Clean-up the previous buffer, if any. */
            if( NULL != xProvisionedState.pucDerPublicKey )
            {
                vPortFree( xProvisionedState.pucDerPublicKey );
                xProvisionedState.pucDerPublicKey = NULL;
            }

            /* Get the bytes of the new public key. */
            prvExportPublicKey( xSession,
                                xProvisionedState.xPublicKey,
                                &xProvisionedState.pucDerPublicKey,
                                &xProvisionedState.ulDerPublicKeyLength );
        }

        /* Ensure that an error condition is set if either object is still
         * missing. */
        if( ( CKR_OK == xResult ) &&
            ( ( CK_INVALID_HANDLE == xProvisionedState.xPrivateKey ) ||
              ( CK_INVALID_HANDLE == xProvisionedState.xPublicKey ) ) )
        {
            xResult = CKR_KEY_HANDLE_INVALID;
        }
    }

    /* Log the device public key if one exists for developer convenience.
     * This can be useful for verifying that the provisioned certificate for the device
     * matches the public key on the device. */
    if( CK_INVALID_HANDLE != xProvisionedState.xPublicKey )
    {
        configPRINTF( ( "Printing device public key.\nMake sure that provisioned device certificate matches public key on device." ) );
        prvWriteHexBytesToConsole( "Device public key",
                                   xProvisionedState.pucDerPublicKey,
                                   xProvisionedState.ulDerPublicKeyLength );
    }

    /* Log the device public key for developer enrollment purposes, but only if
    * there's not already a certificate, or if a new key was just generated. */
    if( ( CKR_OK == xResult ) &&
        ( ( CK_INVALID_HANDLE == xProvisionedState.xClientCertificate ) ||
          ( CK_TRUE == xKeyPairGenerationMode ) ) &&
        ( CK_FALSE == xImportedPrivateKey ) )
    {
        configPRINTF( ( "Warning: the client certificate should be updated. Please see https://aws.amazon.com/freertos/getting-started/.\r\n" ) );

        if( NULL != xProvisionedState.pcIdentifier )
        {
            configPRINTF( ( "Recommended certificate subject name: CN=%s\r\n", xProvisionedState.pcIdentifier ) );
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

    return xResult;
}

/*-----------------------------------------------------------*/

/* Perform device provisioning using the specified TLS client credentials. */
CK_RV vAlternateKeyProvisioning( ProvisioningParams_t * xParams )
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
        xResult = xProvisionDevice( xSession, xParams );

        pxFunctionList->C_CloseSession( xSession );
    }

    return xResult;
}

/*-----------------------------------------------------------*/

/* Perform device provisioning using the default TLS client credentials. */
CK_RV vDevModeKeyProvisioning( void )
{
    ProvisioningParams_t xParams;

    xParams.pucJITPCertificate = ( uint8_t * ) keyJITR_DEVICE_CERTIFICATE_AUTHORITY_PEM;
    xParams.pucClientPrivateKey = ( uint8_t * ) keyCLIENT_PRIVATE_KEY_PEM;
    xParams.pucClientCertificate = ( uint8_t * ) keyCLIENT_CERTIFICATE_PEM;

    /* If using a JITR flow, a JITR certificate must be supplied. If using credentials generated by
     * AWS, this certificate is not needed. */
    if( ( NULL != xParams.pucJITPCertificate ) &&
        ( 0 != strcmp( "", ( const char * ) xParams.pucJITPCertificate ) ) )
    {
        /* We want the NULL terminator to be written to storage, so include it
         * in the length calculation. */
        xParams.ulJITPCertificateLength = sizeof( char ) + strlen( ( const char * ) xParams.pucJITPCertificate );
    }
    else
    {
        xParams.pucJITPCertificate = NULL;
    }

    /* The hard-coded client certificate and private key can be useful for
     * first-time lab testing. They are optional after the first run, though, and
     * not recommended at all for going into production. */
    if( ( NULL != xParams.pucClientPrivateKey ) &&
        ( 0 != strcmp( "", ( const char * ) xParams.pucClientPrivateKey ) ) )
    {
        /* We want the NULL terminator to be written to storage, so include it
         * in the length calculation. */
        xParams.ulClientPrivateKeyLength = sizeof( char ) + strlen( ( const char * ) xParams.pucClientPrivateKey );
    }
    else
    {
        xParams.pucClientPrivateKey = NULL;
    }

    if( ( NULL != xParams.pucClientCertificate ) &&
        ( 0 != strcmp( "", ( const char * ) xParams.pucClientCertificate ) ) )
    {
        /* We want the NULL terminator to be written to storage, so include it
         * in the length calculation. */
        xParams.ulClientCertificateLength = sizeof( char ) + strlen( ( const char * ) xParams.pucClientCertificate );
    }
    else
    {
        xParams.pucClientCertificate = NULL;
    }

    return vAlternateKeyProvisioning( &xParams );
}

/*-----------------------------------------------------------*/
