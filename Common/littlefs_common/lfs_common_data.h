/*
* Copyright (c) 2023-2025 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/

/***********************************************************************************************************************
 * File Name    : lfs_common_data.h
 * Description  : This header file declares common API functions and data structures for managing LittleFS
 *                and Flash memory operations.
 **********************************************************************************************************************/

/**********************************************************************************************************************
 Includes   <System Includes> , "Project Includes"
 *********************************************************************************************************************/
#include <stdint.h>

#ifdef __TEST__
#include "dev_mode_key_provisioning.h"
#else
#include "aws_dev_mode_key_provisioning.h"
#endif
#include "mbedtls/platform.h"
#include "psa/crypto.h"
#include "psa/crypto_extra.h"
#include "r_flash_rx_if.h"
#include "rm_littlefs_api.h"
#include "rm_littlefs_flash.h"
#include "lfs_util.h"
#include "mbedtls_pk_pkcs11.h"
#include "core_mqtt.h"

#ifndef COMMON_DATA_H_
#define COMMON_DATA_H_

FSP_HEADER
/* Flash on Flash HP Instance */


/** Access the Flash HP instance using these structures when calling API functions directly (::p_api is not used). */


#ifndef NULL
void NULL(flash_callback_args_t *p_args);
#endif
/** LittleFS on Flash Instance. */
extern const rm_littlefs_instance_t g_rm_littlefs0;
extern rm_littlefs_flash_instance_ctrl_t g_rm_littlefs0_ctrl;
extern const rm_littlefs_cfg_t g_rm_littlefs0_cfg;

extern struct lfs g_rm_littlefs0_lfs;
extern const struct lfs_config g_rm_littlefs0_lfs_cfg;
extern lfs_t g_lfs;

/**********************************************************************************************************************
 * Function Name: littlFs_init
 * Description  : .
 * Return Value : .
 *********************************************************************************************************************/
int32_t littlFs_init (void);

/**********************************************************************************************************************
 * Function Name: littlFs_format
 * Description  : .
 * Return Value : .
 *********************************************************************************************************************/
int32_t littlFs_format (void);

/**********************************************************************************************************************
 * Function Name: g_common_init
 * Description  : .
 * Return Value : .
 *********************************************************************************************************************/
void g_common_init (void);
FSP_FOOTER
#endif /* COMMON_DATA_H_ */
