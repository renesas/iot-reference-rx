/*
* Copyright (c) 2024-2025 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/
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
#include "job_parser.h"

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
