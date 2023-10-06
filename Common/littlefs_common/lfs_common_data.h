/*
 * lfs_common_data.h v1.0.0
 * Copyright (C) Renesas Electronics Corporation and/or its affiliates.
 * All Rights Reserved.
 *
 * SPDX-License-Identifier: MIT
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
 */

#ifndef COMMON_DATA_H_
#define COMMON_DATA_H_
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
#include "lfs_util.h"
#include "mbedtls_pk_pkcs11.h"
#include "core_mqtt.h"
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

int32_t littlFs_init( void );
int32_t littlFs_format( void );

void g_common_init(void);
FSP_FOOTER
#endif /* COMMON_DATA_H_ */
