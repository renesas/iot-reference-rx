/**********************************************************************************************************************
 * DISCLAIMER
 * This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products. No
 * other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all
 * applicable laws, including copyright laws.
 * THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
 * THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED. TO THE MAXIMUM
 * EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES
 * SHALL BE LIABLE FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO
 * THIS SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
 * Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability of
 * this software. By using this software, you agree to the additional terms and conditions found by accessing the
 * following link:
 * http://www.renesas.com/disclaimer
 *
 * Copyright (C) 2024 Renesas Electronics Corporation. All rights reserved.
 *********************************************************************************************************************/
/**********************************************************************************************************************
 * File Name    : ota_fwup_wrap_verify.c
 * Description  : User functions for the FWUP module.
 **********************************************************************************************************************
 * History : DD.MM.YYYY Version Description
 *         : 08.04.2023 1.00    First Release
 *********************************************************************************************************************/

/**********************************************************************************************************************
 Includes   <System Includes> , "Project Includes"
 *********************************************************************************************************************/

#include "ota_fwup_wrap_verify.h"

static uint8_t * get_cert( uint32_t * ulSignerCertSize );
void* s_ctx_iot;
S_C_CH_FAR VERIFICATION_SCHEME_ECDSA[] = "sig-sha256-ecdsa";
S_C_CH_FAR VERIFICATION_SCHEME_SHA[]  = "hash-sha256";

/**********************************************************************************************************************
 * Function Name: ota_get_crypt_context_function
 * Description  : user-defined wrapper function to get the crypt library's context.
 * Arguments    : none
 * Return Value : library's static pointer
 *********************************************************************************************************************/
void * ota_get_crypt_context_function(void)
{
    return ((void *)s_ctx_iot);
}
/**********************************************************************************************************************
 End of function ota_get_crypt_context_function
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: ota_sha256_init_function_init
 * Description  : user-defined wrapper function to initialize SHA256.
 * Arguments    : vp_ctx : context
 * Return Value : library processing result
 *********************************************************************************************************************/
int32_t ota_sha256_init_function(void * vp_ctx)
{
    (void) vp_ctx;

    if (pdFALSE == CRYPTO_SignatureVerificationStart( &s_ctx_iot,
                                                cryptoASYMMETRIC_ALGORITHM_ECDSA,
                                                cryptoHASH_ALGORITHM_SHA256 ) )
    {
        return -1;
    }
    return 0;
}
/**********************************************************************************************************************
 End of function ota_sha256_init_function
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: ota_sha256_update_function
 * Description  : user-defined wrapper function for sha256.
 * Arguments    : vp_ctx   : context
 *                p_data   : message data
 *                data_len : data len
 * Return Value : library processing result
 *********************************************************************************************************************/
int32_t ota_sha256_update_function(void * vp_ctx, C_U8_FAR *p_data, uint32_t datalen)
{

    (void) vp_ctx;
    CRYPTO_SignatureVerificationUpdate(s_ctx_iot, ( const uint8_t * ) p_data, datalen);
    return 0;
}
/**********************************************************************************************************************
 End of function ota_sha256_update_function
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: ota_sha256_final_function
 * Description  : user-defined wrapper function for sha256.
 * Arguments    : p_hash : hash value storage destination pointer
 *                vp_ctx : context
 * Return Value : library processing result
 *********************************************************************************************************************/
int32_t ota_sha256_final_function(uint8_t *p_hash, void * vp_ctx)
{
    (void) p_hash;
    (void) vp_ctx;
    return 0;
}
/**********************************************************************************************************************
 End of function ota_sha256_final_function
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: ota_verify_ecdsa_function
 * Description  : user-defined wrapper function for ECDSA verification.
 * Arguments    : p_hash     : hash data
 *                p_sig_type : signature type
 *                p_sig      : signature
 *                sig_size   : signature size
 * Return Value : 0          : verify OK
 *                other      : verify NG
 *********************************************************************************************************************/
int32_t ota_verify_ecdsa_function(uint8_t *p_hash, uint8_t *p_sig_type, uint8_t *p_sig, uint32_t sig_size)
{
    int32_t x_result = -1;
#if (FWUP_CFG_SIGNATURE_VERIFICATION == 0)

#endif /* (FWUP_CFG_SIGNATURE_VERIFICATION == 0) */

    /* SHA256 */
    if (0 == STRCMP((const char FWUP_FAR *)p_sig_type, VERIFICATION_SCHEME_SHA))
    {
        return MEMCMP(p_sig, p_hash, sig_size);
    }

#if (FWUP_CFG_SIGNATURE_VERIFICATION == 0)
    /* ECDSA */
/**** Start user code ****/

    uint32_t ulSignerCertSize;
    uint8_t * pucSignerCert = NULL;

    extern OtaFileContext_t* pOTAFileContext;

    LogInfo( ( "Started %s signature verification", VERIFICATION_SCHEME_ECDSA ) );
    pucSignerCert = get_cert( &ulSignerCertSize );

    if( pucSignerCert == NULL )
    {
        return -1;
    }
    else
    {
        if ( CRYPTO_SignatureVerificationFinal( s_ctx_iot, ( char * ) pucSignerCert, ulSignerCertSize,
                pOTAFileContext->pSignature->data, pOTAFileContext->pSignature->size ) == pdFALSE )
        {
             LogError( ( "Finished %s signature verification, but signature verification failed",
                         VERIFICATION_SCHEME_ECDSA ) );
             return -1;
        }
        else
        {
            LogInfo( ( "Finished %s signature verification, signature verification passed",
                        VERIFICATION_SCHEME_ECDSA ) );
        }
    }
    return 0;
    /**** End user code   ****/
#endif /* (FWUP_CFG_SIGNATURE_VERIFICATION == 0) */

    return (x_result);
}
/**********************************************************************************************************************
 End of function ota_verify_ecdsa_function
 *********************************************************************************************************************/


/**********************************************************************************************************************
 * Function Name: get_cert
 * Description  : wrapper module for extracting code-signer certificate.
 * Arguments    : * ulSignerCertSize : Code-signer certificate length
 * Return Value : pucSignerCert      : Code-signer certificate
 *********************************************************************************************************************/
static uint8_t * get_cert( uint32_t * ulSignerCertSize )
{
    uint8_t * pucCertData = NULL;
    uint32_t ulCertSize;
    uint8_t * pucSignerCert = NULL;
	
    size_t valueLength = prvGetCacheEntryLength(KVS_CODE_SIGN_CERT_ID);

    pucCertData = (uint8_t *)GetStringValue(KVS_CODE_SIGN_CERT_ID, valueLength);

    if (pucCertData != NULL)
    {
        LogInfo( ( "Using certificate stored in DF...") );
        ulCertSize = valueLength;
    }
    else {
        /* Allocate memory for the signer certificate plus a terminating zero so we can copy it and return to the caller. */
        LogError( ( "No certificate stored in DF! Please commit using CLI mode.") );
        return pucSignerCert;
    }

    pucSignerCert = pvPortMalloc( ulCertSize + 1 );

    if ( pucSignerCert != NULL )
    {
        memcpy( pucSignerCert, pucCertData, ulCertSize );
        /* The crypto code requires the terminating zero to be part of the length so add 1 to the size. */
        pucSignerCert[ ulCertSize ] = 0U;
        *ulSignerCertSize = ulCertSize + 1U;
    }
    else
    {
        LogError( ( "Failed to store the certificate in dynamic memory: pvPortMalloc failed to allocate a buffer of size %d", ulCertSize ) );
    }

    return pucSignerCert;

}
/**********************************************************************************************************************
 End of function get_cert
 *********************************************************************************************************************/