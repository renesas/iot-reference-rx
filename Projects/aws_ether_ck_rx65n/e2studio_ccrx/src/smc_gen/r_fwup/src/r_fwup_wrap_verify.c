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
 * Copyright (C) 2023 Renesas Electronics Corporation. All rights reserved.
 *********************************************************************************************************************/
/**********************************************************************************************************************
 * File Name    : r_fwup_wrap_verify.c
 * Version      : 2.0
 * Description  : Functions for the Firmware update module.
 *********************************************************************************************************************/

/**********************************************************************************************************************
 Includes   <System Includes> , "Project Includes"
 *********************************************************************************************************************/
#include "r_fwup_if.h"
#include "r_fwup_wrap_verify.h"

/**** Start user code ****/
/**** End user code   ****/

/**********************************************************************************************************************
 Macro definitions
 *********************************************************************************************************************/
/**** Start user code ****/
#define FWUP_HASH_BYTES        (TC_SHA256_DIGEST_SIZE)
#define FWUP_HASH_BITS         (FWUP_HASH_BYTES * 8)
#define FWUP_CERT_PEM           g_code_signer_public_key
/**** End user code   ****/

/**********************************************************************************************************************
 Local Typedef definitions
 *********************************************************************************************************************/
/**** Start user code ****/
/**** End user code   ****/

/**********************************************************************************************************************
 Exported global variables
 *********************************************************************************************************************/
/**** Start user code ****/
S_C_CH_FAR VERIFICATION_SCHEME_ECDSA[]  = "sig-sha256-ecdsa";
S_C_CH_FAR VERIFICATION_SCHEME_SHA[]  = "hash-sha256";
/**** End user code   ****/

/**********************************************************************************************************************
 Private (static) variables and functions
 *********************************************************************************************************************/
/**** Start user code ****/
#if (FWUP_CFG_SIGNATURE_VERIFICATION == 0)
static uint8_t * get_cert( const char * pucCertName, uint32_t * const ulSignerCertSize );

void* s_ctx_iot;

#endif /* (FWUP_CFG_SIGNATURE_VERIFICATION == 0) */
/**** End user code   ****/

/**********************************************************************************************************************
 * Function Name: r_fwup_wrap_get_crypt_context
 * Description  : wrapper function for get to the crypt library's context.
 * Arguments    : none
 * Return Value : library's static pointer
 **********************************************************************************************************************/
void * r_fwup_wrap_get_crypt_context(void)
{
    /* library's context. that need to be a static value. */
    /**** Start user code ****/
    return ((void *)s_ctx_iot);
    /**** End user code   ****/
}
/**********************************************************************************************************************
 End of function r_fwup_wrap_get_crypt_context
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: r_fwup_wrap_sha256_init
 * Description  : wrapper function for sha256.
 * Arguments    : vp_ctx
 * Return Value : result
 **********************************************************************************************************************/
int32_t r_fwup_wrap_sha256_init(void * vp_ctx)
{
    /**** Start user code ****/
	if (pdFALSE == CRYPTO_SignatureVerificationStart( &s_ctx_iot,
												cryptoASYMMETRIC_ALGORITHM_ECDSA,
												cryptoHASH_ALGORITHM_SHA256 ) )
	{
		return -1;
	}
	return 0;
    /**** End user code   ****/
}
/**********************************************************************************************************************
 End of function r_fwup_wrap_sha256_init
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: r_fwup_wrap_sha256_update
 * Description  : wrapper function for sha256.
 * Arguments    : vp_ctx
 *                p_data
 *                data_len
 * Return Value : result
 **********************************************************************************************************************/
int32_t r_fwup_wrap_sha256_update(void * vp_ctx, C_U8_FAR *p_data, uint32_t datalen)
{
    /**** Start user code ****/

	CRYPTO_SignatureVerificationUpdate(s_ctx_iot, ( const uint8_t * ) p_data, datalen);

	return 0;
    /**** End user code   ****/
}
/**********************************************************************************************************************
 End of function r_fwup_wrap_sha256_update
 *********************************************************************************************************************/

/**** Start user code ****/

#if (FWUP_CFG_SIGNATURE_VERIFICATION == 0)

int32_t r_fwup_wrap_verify_iot_cryto(OtaFileContext_t * const pFileContext, void * pvSigVerifyContext)
{
	uint32_t start_addr;
	uint32_t ulSignerCertSize;
    uint8_t * pucSignerCert = NULL;


	LogInfo( ( "Started %s signature verification, file: %s",
			VERIFICATION_SCHEME_ECDSA, ( const char * ) pFileContext->pCertFilepath ) );
	pucSignerCert = get_cert( ( const char * ) pFileContext->pCertFilepath,
								&ulSignerCertSize );

	if( pucSignerCert == NULL )
	{
		return -1;
	}
	else
	{
		if ( CRYPTO_SignatureVerificationFinal( s_ctx_iot, ( char * ) pucSignerCert, ulSignerCertSize,
											pFileContext->pSignature->data, pFileContext->pSignature->size ) == pdFALSE )
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
}

static uint8_t * get_cert( const char * pucCertName,
                                    uint32_t * const ulSignerCertSize )
{
    uint8_t * pucCertData;
    uint32_t ulCertSize;
    uint8_t * pucSignerCert = NULL;

    (void) pucCertName;

    LogInfo( ( "Using certificate defined by the otapalconfigCODE_SIGNING_CERTIFICATE macro...") );

    /* Allocate memory for the signer certificate plus a terminating zero so we can copy it and return to the caller. */
    ulCertSize = sizeof( otapalconfigCODE_SIGNING_CERTIFICATE );
    pucSignerCert = pvPortMalloc( ulCertSize + 1 );
    pucCertData = ( uint8_t * ) otapalconfigCODE_SIGNING_CERTIFICATE;

    if( pucSignerCert != NULL )
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

#endif  /* FWUP_CFG_SIGNATURE_VERIFICATION == 0 */

/**** End user code   ****/

