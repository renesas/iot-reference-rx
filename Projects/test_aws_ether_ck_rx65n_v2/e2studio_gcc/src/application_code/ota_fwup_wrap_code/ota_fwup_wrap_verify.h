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
 * File Name    : ota_fwup_wrap_verify.h
 * Description  : User-defined functions for the Firmware update module.
 **********************************************************************************************************************
 * History : DD.MM.YYYY Version Description
 *         : 09.04.2023 2.00    First Release
 *********************************************************************************************************************/

/**********************************************************************************************************************
 Includes   <System Includes> , "Project Includes"
 *********************************************************************************************************************/

#ifndef APPLICATION_CODE_OTA_FWUP_WRAP_CODE_OTA_FWUP_WRAP_VERIFY_H_
#define APPLICATION_CODE_OTA_FWUP_WRAP_CODE_OTA_FWUP_WRAP_VERIFY_H_

#include "r_fwup_if.h"
#include "store.h"
#include "ota_private.h"

#if (FWUP_CFG_SIGNATURE_VERIFICATION == 0)
#include "iot_crypto.h"

/* OTA PAL test config file include. */
#include "demo_config.h"
#endif /* (FWUP_CFG_SIGNATURE_VERIFICATION == 0) */

/* User-defined wrapper functions */
void * ota_get_crypt_context_function(void);
int32_t ota_sha256_init_function(void * vp_ctx);
int32_t ota_sha256_update_function(void * vp_ctx, C_U8_FAR *p_data, uint32_t datalen);
int32_t ota_sha256_final_function(uint8_t *p_hash, void * vp_ctx);
int32_t ota_verify_ecdsa_function(uint8_t *p_hash, uint8_t *p_sig_type, uint8_t *p_sig, uint32_t sig_size);

#endif /* APPLICATION_CODE_OTA_FWUP_WRAP_CODE_OTA_FWUP_WRAP_VERIFY_H_ */