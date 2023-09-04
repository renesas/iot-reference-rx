/*******************************************************************************
* Copyright (C) 2023 Renesas Electronics Corporation. All rights reserved.
*
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only
* intended for use with Renesas products. No other uses are authorized. This
* software is owned by Renesas Electronics Corporation and is protected under
* all applicable laws, including copyright laws.
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
* THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT
* LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE
* AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.
* TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS
* ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE
* FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR
* ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE
* BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software
* and to discontinue the availability of this software. By using this software,
* you agree to the additional terms and conditions found by accessing the
* following link:
* http://www.renesas.com/disclaimer
*******************************************************************************/

#ifndef RM_LITTLEFS_FLASH_H
#define RM_LITTLEFS_FLASH_H

/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
#include "rm_littlefs_api.h"
#include "platform.h"
#include "r_flash_rx_if.h"
#include "lfs.h"
#include "lfs_util_config.h"
#if LFS_THREAD_SAFE
 #include "FreeRTOS.h"
 #include "semphr.h"

#endif

/* Common macro for FSP header files. There is also a corresponding FSP_FOOTER macro at the end of this file. */
FSP_HEADER

/*******************************************************************************************************************//**
 * @addtogroup RM_LITTLEFS_FLASH
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/
#ifdef RM_LITTLEFS_FLASH_DATA_START
static const uint32_t rm_littlefs_flash_data_start = RM_LITTLEFS_FLASH_DATA_START;
#else
#define rm_littlefs_flash_data_start    FLASH_DF_BLOCK_32
#endif

/***********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/
/** Instance control block.  This is private to the FSP and should not be used or modified by the application. */
typedef struct st_rm_littlefs_flash_instance_ctrl
{
    uint32_t open;
    rm_littlefs_cfg_t const * p_cfg;
#if LFS_THREAD_SAFE
    SemaphoreHandle_t xSemaphore;
    StaticSemaphore_t xMutexBuffer;
#endif
} rm_littlefs_flash_instance_ctrl_t;

/**********************************************************************************************************************
 * Exported global variables
 **********************************************************************************************************************/

/** @cond INC_HEADER_DEFS_SEC */
/** Filled in Interface API structure for this Instance. */
extern const rm_littlefs_api_t g_rm_littlefs_on_flash;

extern xSemaphoreHandle xSemaphoreFlashAccess;

/** @endcond */

/**********************************************************************************************************************
 * Function Prototypes
 **********************************************************************************************************************/
fsp_err_t RM_LITTLEFS_FLASH_Open(rm_littlefs_ctrl_t * const p_ctrl, rm_littlefs_cfg_t const * const p_cfg);

fsp_err_t RM_LITTLEFS_FLASH_Close(rm_littlefs_ctrl_t * const p_ctrl);

int rm_littlefs_flash_read(const struct lfs_config * c, lfs_block_t block, lfs_off_t off, void * buffer,
                           lfs_size_t size);

int rm_littlefs_flash_write(const struct lfs_config * c,
                            lfs_block_t               block,
                            lfs_off_t                 off,
                            const void              * buffer,
                            lfs_size_t                size);

int rm_littlefs_flash_erase(const struct lfs_config * c, lfs_block_t block);

int rm_littlefs_flash_lock(const struct lfs_config * c);

int rm_littlefs_flash_unlock(const struct lfs_config * c);

int rm_littlefs_flash_sync(const struct lfs_config * c);

/* Common macro for FSP header files. There is also a corresponding FSP_HEADER macro at the top of this file. */
FSP_FOOTER

#endif                                 // RM_LITTLEFS_FLASH_H

/*******************************************************************************************************************//**
 * @} (end addtogroup RM_LITTLEFS)
 **********************************************************************************************************************/
