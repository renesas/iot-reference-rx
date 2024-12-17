/*
 * rm_littlefs_flash.c v1.0.0
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

/* FSP includes. */
#include <string.h>
#include "rm_littlefs_flash.h"
#include "rm_littlefs_flash_config.h"
#include "r_common_api_flash.h"

/* Get the data flash block size defined in bsp_feature.h for this MCU. */
#define RM_LITTLEFS_FLASH_DATA_BLOCK_SIZE      (FLASH_DF_BLOCK_SIZE)


#define RM_LITTLEFS_FLASH_MINIMUM_BLOCK_SIZE    (104)

#ifndef RM_LITTLEFS_FLASH_SEMAPHORE_TIMEOUT
    #define RM_LITTLEFS_FLASH_SEMAPHORE_TIMEOUT    (UINT32_MAX)
#endif


/** "RLFS" in ASCII, used to determine if channel is open. */
#define RM_LITTLEFS_FLASH_OPEN           (0x524C4653ULL)

extern volatile UPDATA_DATA_FLASH_CONTROL_BLOCK update_data_flash_control_block;
extern volatile flash_res_t g_blank_check_result;

/** LittleFS API mapping for LittleFS Port interface */
const rm_littlefs_api_t g_rm_littlefs_on_flash =
{
    .open  = RM_LITTLEFS_FLASH_Open,
    .close = RM_LITTLEFS_FLASH_Close,
};

static void write_callback (void * event);
static void erase_flashing_callback (void * event);

static void update_dataflash_data (const struct lfs_config * c,
        lfs_block_t               block,
        lfs_off_t                 off,
        const void              * buffer,
        lfs_size_t                size,
        uint32_t                  update_state);

/**********************************************************************************************************************
 * Function Name: RM_LITTLEFS_FLASH_Open
 * Description  : Opens the driver and initializes lower layer driver.
 *                Implements rm_littlefs_api_t::open()
 * Arguments    : p_ctrl
 *              : p_cfg
 * Return Value : See RENESAS_ERROR_CODES or functions called by this function for other possible return codes.
 *                This function calls: flash_api_t::open
 *********************************************************************************************************************/
fsp_err_t RM_LITTLEFS_FLASH_Open(rm_littlefs_ctrl_t * const p_ctrl, rm_littlefs_cfg_t const * const p_cfg)
{
    rm_littlefs_flash_instance_ctrl_t * p_instance_ctrl = (rm_littlefs_flash_instance_ctrl_t *) p_ctrl;
    p_instance_ctrl->p_cfg = p_cfg;

    /* Call commonapi open function for Flash */
    e_commonapi_err_t common_api_err = R_Demo_Common_API_Flash_Open();
    if (COMMONAPI_SUCCESS != common_api_err)
    {
        while (1); /* infinite loop due to error */
    }

#if LFS_THREAD_SAFE
    p_instance_ctrl->xSemaphore = xSemaphoreCreateMutexStatic(&p_instance_ctrl->xMutexBuffer);

    if (NULL == p_instance_ctrl->xSemaphore)
    {
        return FSP_ERR_INTERNAL;
    }
#endif

    /* This module is now open. */
    p_instance_ctrl->open = RM_LITTLEFS_FLASH_OPEN;

    return FSP_SUCCESS;
}
/*****************************************************************************************
End of function RM_LITTLEFS_FLASH_Open
****************************************************************************************/

/**********************************************************************************************************************
 * Function Name: RM_LITTLEFS_FLASH_Close
 * Description  : Closes the lower level driver.
 *                Implements rm_littlefs_api_t::close().
 * Argument     : p_ctrl
 * Return Value : See @ref RENESAS_ERROR_CODES or functions called by this function for other possible return codes.
 *                This function calls: flash_api_t::close
 *********************************************************************************************************************/
fsp_err_t RM_LITTLEFS_FLASH_Close(rm_littlefs_ctrl_t * const p_ctrl)
{
    rm_littlefs_flash_instance_ctrl_t * p_instance_ctrl = (rm_littlefs_flash_instance_ctrl_t *) p_ctrl;

    p_instance_ctrl->open = 0;

#if LFS_THREAD_SAFE
    vSemaphoreDelete(p_instance_ctrl->xSemaphore);
#endif

    return FSP_SUCCESS;
}
/*****************************************************************************************
End of function RM_LITTLEFS_FLASH_Close
****************************************************************************************/

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
int rm_littlefs_flash_read(const struct lfs_config * c,
                            lfs_block_t               block,
                            lfs_off_t                 off,
                            void                    * buffer,
                            lfs_size_t                size)
{
    rm_littlefs_flash_instance_ctrl_t * p_instance_ctrl = (rm_littlefs_flash_instance_ctrl_t *) c->context;

    /* No flash access while reading */
    xSemaphoreTake(xSemaphoreFlashAccess, portMAX_DELAY);

    /* Read directly from the flash. */
    memcpy( buffer,
            (uint8_t *) (rm_littlefs_flash_data_start + (p_instance_ctrl->p_cfg->p_lfs_cfg->block_size * block) + off),
            size);
    xSemaphoreGive(xSemaphoreFlashAccess);

    return LFS_ERR_OK;
}
/*****************************************************************************************
End of function rm_littlefs_flash_read
****************************************************************************************/

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
int rm_littlefs_flash_write(const struct lfs_config * c,
                            lfs_block_t               block,
                            lfs_off_t                 off,
                            const void              * buffer,
                            lfs_size_t                size)
{
    /* if semaphore cannot be obtained then return error */
    flash_err_t flash_error_code = FLASH_ERR_BUSY;
    rm_littlefs_flash_instance_ctrl_t * p_instance_ctrl = (rm_littlefs_flash_instance_ctrl_t *) c->context;

    /* Flash access protect */
    xSemaphoreTake(xSemaphoreFlashAccess, portMAX_DELAY);

    update_data_flash_control_block.status = DATA_FLASH_UPDATE_STATE_WRITE_WAIT_COMPLETE;

    flash_error_code = R_FLASH_Write( (uint32_t)buffer,
            (rm_littlefs_flash_data_start +
            (p_instance_ctrl->p_cfg->p_lfs_cfg->block_size * block) + off), size );

    if ((FLASH_SUCCESS != flash_error_code))
    {
        xSemaphoreGive(xSemaphoreFlashAccess);
        return LFS_ERR_IO;
    }

    /* wait for the semaphore to be released by callback */
    xSemaphoreTake(xSemaphoreFlashAccess, portMAX_DELAY);
    xSemaphoreGive(xSemaphoreFlashAccess);
    return LFS_ERR_OK;
}
/*****************************************************************************************
End of function rm_littlefs_flash_write
****************************************************************************************/

/**********************************************************************************************************************
 * Function Name: rm_littlefs_flash_erase
 * Description  : Erase the logical block. The location and number of blocks to be erased will depend on block size.
 * Arguments    : c           Pointer to the LittleFS config block.
 *              : block       The logical block number
 * Return Value : LFS_ERR_OK  Success.
 *                LFS_ERR_IO  Lower layer is not open or failed to erase the flash.
 *********************************************************************************************************************/
int rm_littlefs_flash_erase(const struct lfs_config * c, lfs_block_t block)
{
    /* if semaphore cannot be obtained then return error */
    flash_err_t flash_error_code = FLASH_ERR_BUSY;
    rm_littlefs_flash_instance_ctrl_t * p_instance_ctrl = (rm_littlefs_flash_instance_ctrl_t *) c->context;

    /* Flash access protect */
    xSemaphoreTake(xSemaphoreFlashAccess, portMAX_DELAY);

    update_data_flash_control_block.status = DATA_FLASH_UPDATE_STATE_ERASE_WAIT_COMPLETE;

    flash_error_code = R_FLASH_Erase((flash_block_address_t)(rm_littlefs_flash_data_start + (p_instance_ctrl->p_cfg->p_lfs_cfg->block_size * block)),
                                     p_instance_ctrl->p_cfg->p_lfs_cfg->block_size / RM_LITTLEFS_FLASH_DATA_BLOCK_SIZE);

    if ((FLASH_SUCCESS != flash_error_code))
    {
        xSemaphoreGive(xSemaphoreFlashAccess);
        return LFS_ERR_IO;
    }

    /* wait for the semaphore to be released by callback */
    xSemaphoreTake(xSemaphoreFlashAccess, portMAX_DELAY);
    xSemaphoreGive(xSemaphoreFlashAccess);
    return LFS_ERR_OK;
}
/*****************************************************************************************
End of function rm_littlefs_flash_erase
****************************************************************************************/

/**********************************************************************************************************************
 * Function Name: rm_littlefs_flash_lock
 * Description  : Returns the version of this module.
 * Argument     : c
 * Return Value : LFS_ERR_OK  Success.
 *                LFS_ERR_IO  Lower layer is not open or failed to lock the flash.
 *********************************************************************************************************************/
int rm_littlefs_flash_lock(const struct lfs_config * c)
{
#if LFS_THREAD_SAFE
    rm_littlefs_flash_instance_ctrl_t * p_instance_ctrl = (rm_littlefs_flash_instance_ctrl_t *) c->context;

    BaseType_t err = xSemaphoreTake(p_instance_ctrl->xSemaphore, RM_LITTLEFS_FLASH_SEMAPHORE_TIMEOUT);

    FSP_ERROR_RETURN(true == err, LFS_ERR_IO);

    return LFS_ERR_OK;
#else
    FSP_PARAMETER_NOT_USED(c);

    return LFS_ERR_IO;
#endif
}
/*****************************************************************************************
End of function rm_littlefs_flash_lock
****************************************************************************************/

/**********************************************************************************************************************
 * Function Name: rm_littlefs_flash_unlock
 * Description  : Returns the version of this module.
 * Argument     : c
 * Return Value : LFS_ERR_OK  Success.
 *                LFS_ERR_IO  Lower layer is not open or failed to unlock the flash.
 *********************************************************************************************************************/
int rm_littlefs_flash_unlock(const struct lfs_config * c)
{
#if LFS_THREAD_SAFE
    rm_littlefs_flash_instance_ctrl_t * p_instance_ctrl = (rm_littlefs_flash_instance_ctrl_t *) c->context;

    BaseType_t err = xSemaphoreGive(p_instance_ctrl->xSemaphore);

    FSP_ERROR_RETURN(true == err, LFS_ERR_IO);

    return LFS_ERR_OK;
#else
    FSP_PARAMETER_NOT_USED(c);

    return LFS_ERR_IO;
#endif
}
/*****************************************************************************************
End of function rm_littlefs_flash_unlock
****************************************************************************************/

/**********************************************************************************************************************
 * Function Name: rm_littlefs_flash_sync
 * Description  : Stub function required by LittleFS. All calls immedialy write/erase the lower layer.
 * Argument     : c           Pointer to the LittleFS config block.
 * Return Value : LFS_ERR_OK  Success.
 *********************************************************************************************************************/
int rm_littlefs_flash_sync(const struct lfs_config * c)
{
    FSP_PARAMETER_NOT_USED(c);

    return LFS_ERR_OK;
}
/*****************************************************************************************
End of function rm_littlefs_flash_sync
****************************************************************************************/

