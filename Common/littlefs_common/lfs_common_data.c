/*
 * lfs_common_data.c v1.0.0
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

#include "lfs_common_data.h"
#include "rm_littlefs_flash_config.h"

/* Instance structure to use this module. */

rm_littlefs_flash_instance_ctrl_t g_rm_littlefs0_ctrl;

#ifdef LFS_NO_MALLOC
static uint8_t g_rm_littlefs0_read[64];
static uint8_t g_rm_littlefs0_prog[64];
static uint8_t g_rm_littlefs0_lookahead[16];
#endif

struct lfs g_rm_littlefs0_lfs;

const struct lfs_config g_rm_littlefs0_lfs_cfg =
{
    .context = &g_rm_littlefs0_ctrl,
    .read = &rm_littlefs_flash_read,
    .prog = &rm_littlefs_flash_write,
    .erase = &rm_littlefs_flash_erase,
    .sync = &rm_littlefs_flash_sync,
    .read_size = LFS_FLASH_READ_SIZE,
    .prog_size = LFS_FLASH_PROGRAM_SIZE,
    .block_size = LFS_FLASH_BLOCK_SIZE,
    .block_count = LFS_FLASH_BLOCK_COUNT,
    .block_cycles = 1024,
    .cache_size = 64,
    .lookahead_size = 16,
#ifdef LFS_NO_MALLOC
    .read_buffer = (void *) g_rm_littlefs0_read,
    .prog_buffer = (void *) g_rm_littlefs0_prog,
    .lookahead_buffer = (void *) g_rm_littlefs0_lookahead,

#endif
        };


const rm_littlefs_cfg_t g_rm_littlefs0_cfg =
{ .p_lfs_cfg = &g_rm_littlefs0_lfs_cfg };

/* Instance structure to use this module. */
const rm_littlefs_instance_t g_rm_littlefs0 =
{ .p_ctrl = &g_rm_littlefs0_ctrl, .p_cfg = &g_rm_littlefs0_cfg, .p_api = &g_rm_littlefs_on_flash, };

/**********************************************************************************************************************
 * Function Name: littlFs_init
 * Description  : .
 * Return Value : .
 *********************************************************************************************************************/
int32_t littlFs_init(void)
{
    int32_t err;
    RM_LITTLEFS_FLASH_Open(g_rm_littlefs0.p_ctrl, g_rm_littlefs0.p_cfg);
    err = lfs_mount(&g_rm_littlefs0_lfs, &g_rm_littlefs0_lfs_cfg);

    if (LFS_ERR_OK != err)
    {
        err = lfs_format(&g_rm_littlefs0_lfs, &g_rm_littlefs0_lfs_cfg);
        if (LFS_ERR_OK == err)
        {
            err = lfs_mount(&g_rm_littlefs0_lfs, &g_rm_littlefs0_lfs_cfg);
       }
    }
    return err;

}
/*****************************************************************************************
End of function littlFs_init
****************************************************************************************/

/**********************************************************************************************************************
 * Function Name: littlFs_format
 * Description  : .
 * Return Value : .
 *********************************************************************************************************************/
int32_t littlFs_format(void)
{
    int32_t err;
    RM_LITTLEFS_FLASH_Open(g_rm_littlefs0.p_ctrl, g_rm_littlefs0.p_cfg);
    err = lfs_format(&g_rm_littlefs0_lfs, &g_rm_littlefs0_lfs_cfg);
    if (LFS_ERR_OK == err)
    {
        err = lfs_mount(&g_rm_littlefs0_lfs, &g_rm_littlefs0_lfs_cfg);
    }
    return err;
}
/*****************************************************************************************
End of function littlFs_format
****************************************************************************************/
