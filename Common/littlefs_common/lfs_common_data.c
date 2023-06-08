/***********************************************************************************************************************
 * Copyright 2023 Renesas Electronics Corporation and/or its affiliates.  All Rights Reserved.
 *
 * This software and documentation are supplied by Renesas Electronics America Inc. and may only be used with products
 * of Renesas Electronics Corp. and its affiliates ("Renesas").  No other uses are authorized.  Renesas products are
 * sold pursuant to Renesas terms and conditions of sale.  Purchasers are solely responsible for the selection and use
 * of Renesas products and Renesas assumes no liability.  No license, express or implied, to any intellectual property
 * right is granted by Renesas. This software is protected under all applicable laws, including copyright laws. Renesas
 * reserves the right to change or discontinue this software and/or this documentation. THE SOFTWARE AND DOCUMENTATION
 * IS DELIVERED TO YOU "AS IS," AND RENESAS MAKES NO REPRESENTATIONS OR WARRANTIES, AND TO THE FULLEST EXTENT
 * PERMISSIBLE UNDER APPLICABLE LAW, DISCLAIMS ALL WARRANTIES, WHETHER EXPLICITLY OR IMPLICITLY, INCLUDING WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, AND NONINFRINGEMENT, WITH RESPECT TO THE SOFTWARE OR
 * DOCUMENTATION.  RENESAS SHALL HAVE NO LIABILITY ARISING OUT OF ANY SECURITY VULNERABILITY OR BREACH.  TO THE MAXIMUM
 * EXTENT PERMITTED BY LAW, IN NO EVENT WILL RENESAS BE LIABLE TO YOU IN CONNECTION WITH THE SOFTWARE OR DOCUMENTATION
 * (OR ANY PERSON OR ENTITY CLAIMING RIGHTS DERIVED FROM YOU) FOR ANY LOSS, DAMAGES, OR CLAIMS WHATSOEVER, INCLUDING,
 * WITHOUT LIMITATION, ANY DIRECT, CONSEQUENTIAL, SPECIAL, INDIRECT, PUNITIVE, OR INCIDENTAL DAMAGES; ANY LOST PROFITS,
 * OTHER ECONOMIC DAMAGE, PROPERTY DAMAGE, OR PERSONAL INJURY; AND EVEN IF RENESAS HAS BEEN ADVISED OF THE POSSIBILITY
 * OF SUCH LOSS, DAMAGES, CLAIMS OR COSTS.
 **********************************************************************************************************************/

#include "common_data.h"
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

int32_t littlFs_init( void )
{
	int32_t err ;
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

int32_t littlFs_format( void )
{
	int32_t err ;
	RM_LITTLEFS_FLASH_Open(g_rm_littlefs0.p_ctrl, g_rm_littlefs0.p_cfg);
	err = lfs_format(&g_rm_littlefs0_lfs, &g_rm_littlefs0_lfs_cfg);
	if (LFS_ERR_OK == err)
	{
		err = lfs_mount(&g_rm_littlefs0_lfs, &g_rm_littlefs0_lfs_cfg);
	}
	return err;
}