/*
 * rm_littlefs_flash.h v1.0.0
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

/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
#include "rm_littlefs_api.h"
#include "platform.h"
#include "r_flash_rx_if.h"
#include "lfs.h"
#include "lfs_util_config.h"
#include "rm_littlefs_flash_config.h"
#if LFS_THREAD_SAFE
    #include "FreeRTOS.h"
    #include "semphr.h"

#endif

#ifndef RM_LITTLEFS_FLASH_H
#define RM_LITTLEFS_FLASH_H

/* Common macro for FSP header files. There is also a corresponding FSP_FOOTER macro at the end of this file. */
FSP_HEADER

/*******************************************************************************************************************//**
 * @addtogroup RM_LITTLEFS_FLASH
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/
#ifdef RM_LITTLEFS_FLASH_DATA_START
#else
#define rm_littlefs_flash_data_start    (FLASH_DF_BLOCK_32)
#endif

/***********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/
/** Instance control block.  This is private to the FSP and should not be used or modified by the application. */
typedef struct st_rm_littlefs_flash_instance_ctrl
{
    uint32_t                  open;
    rm_littlefs_cfg_t const * p_cfg;
#if LFS_THREAD_SAFE
    SemaphoreHandle_t         xSemaphore;
    StaticSemaphore_t         xMutexBuffer;
#endif
} rm_littlefs_flash_instance_ctrl_t;

/**********************************************************************************************************************
 * Exported global variables
 **********************************************************************************************************************/

/** @cond INC_HEADER_DEFS_SEC */
/** Filled in Interface API structure for this Instance. */
extern const rm_littlefs_api_t g_rm_littlefs_on_flash;

/** @endcond */

static const uint32_t rm_littlefs_flash_data_start = RM_LITTLEFS_FLASH_DATA_START;

/**********************************************************************************************************************
 * Function Prototypes
 **********************************************************************************************************************/
/**********************************************************************************************************************
 * Function Name: RM_LITTLEFS_FLASH_Open
 * Description  : Opens the driver and initializes lower layer driver.
 *                Implements rm_littlefs_api_t::open()
 * Arguments    : p_ctrl
 *              : p_cfg
 * Return Value : See RENESAS_ERROR_CODES or functions called by this function for other possible return codes.
 *                This function calls: flash_api_t::open
 *********************************************************************************************************************/
fsp_err_t RM_LITTLEFS_FLASH_Open (rm_littlefs_ctrl_t * const p_ctrl, rm_littlefs_cfg_t const * const p_cfg);

/**********************************************************************************************************************
 * Function Name: RM_LITTLEFS_FLASH_Close
 * Description  : Closes the lower level driver.
 *                Implements rm_littlefs_api_t::close().
 * Argument     : p_ctrl
 * Return Value : See @ref RENESAS_ERROR_CODES or functions called by this function for other possible return codes.
 *                This function calls: flash_api_t::close
 *********************************************************************************************************************/
fsp_err_t RM_LITTLEFS_FLASH_Close (rm_littlefs_ctrl_t * const p_ctrl);

/**********************************************************************************************************************
 * Function Name: rm_littlefs_flash_read
 * Description  : Read from the flash driver. Negative error codes are propogated to the user.
 * Arguments    : c           Pointer to the LittleFS config block.
 *              : block       The block number
 *              : off         Offset in bytes
 *              : buffer      The buffer to copy data into
 *              : size        The size in bytes
 * Return Value : LFS_ERR_OK  Read is complete. If there is a blank area in the read area, incorrect values may be read
 *                LFS_ERR_IO  Lower level driver is not open.
 *********************************************************************************************************************/
int rm_littlefs_flash_read (const struct lfs_config * c, lfs_block_t block, lfs_off_t off, void * buffer,
                            lfs_size_t size);

/**********************************************************************************************************************
 * Function Name: rm_littlefs_flash_write
 * Description  : Writes requested bytes to flash.
 * Arguments    : c           Pointer to the LittleFS config block.
 *              : block       The block number
 *              : off         Offset in bytes
 *              : buffer      The buffer containing data to be written.
 *              : size        The size in bytes
 * Return Value : LFS_ERR_OK  Success.
 *                LFS_ERR_IO  Lower layer is not open or failed to write the flash.
 *********************************************************************************************************************/
int rm_littlefs_flash_write (const struct lfs_config * c,
                            lfs_block_t               block,
                            lfs_off_t                 off,
                            const void              * buffer,
                            lfs_size_t                size);

/**********************************************************************************************************************
 * Function Name: rm_littlefs_flash_erase
 * Description  : Erase the logical block. The location and number of blocks to be erased will depend on block size.
 * Arguments    : c           Pointer to the LittleFS config block.
 *              : block       The logical block number
 * Return Value : LFS_ERR_OK  Success.
 *                LFS_ERR_IO  Lower layer is not open or failed to erase the flash.
 *********************************************************************************************************************/
int rm_littlefs_flash_erase (const struct lfs_config * c, lfs_block_t block);

/**********************************************************************************************************************
 * Function Name: rm_littlefs_flash_lock
 * Description  : Returns the version of this module.
 * Argument     : c
 * Return Value : LFS_ERR_OK  Success.
 *                LFS_ERR_IO  Lower layer is not open or failed to lock the flash.
 *********************************************************************************************************************/
int rm_littlefs_flash_lock (const struct lfs_config * c);

/**********************************************************************************************************************
 * Function Name: rm_littlefs_flash_unlock
 * Description  : Returns the version of this module.
 * Argument     : c
 * Return Value : LFS_ERR_OK  Success.
 *                LFS_ERR_IO  Lower layer is not open or failed to unlock the flash.
 *********************************************************************************************************************/
int rm_littlefs_flash_unlock (const struct lfs_config * c);

/**********************************************************************************************************************
 * Function Name: rm_littlefs_flash_sync
 * Description  : Stub function required by LittleFS. All calls immedialy write/erase the lower layer.
 * Argument     : c           Pointer to the LittleFS config block.
 * Return Value : LFS_ERR_OK  Success.
 *********************************************************************************************************************/
int rm_littlefs_flash_sync (const struct lfs_config * c);

/* Common macro for FSP header files. There is also a corresponding FSP_HEADER macro at the top of this file. */
FSP_FOOTER

#endif                                 /* RM_LITTLEFS_FLASH_H */
