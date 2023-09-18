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

/* FSP includes. */
#include "string.h"
#include "rm_littlefs_flash.h"
#include "rm_littlefs_flash_config.h"
#include "demo_config.h"

flash_res_t g_blank_check_result;

/* Get the data flash block size defined in bsp_feature.h for this MCU. */
#define RM_LITTLEFS_FLASH_DATA_BLOCK_SIZE      FLASH_DF_BLOCK_SIZE


#define RM_LITTLEFS_FLASH_MINIMUM_BLOCK_SIZE    (104)

#ifndef RM_LITTLEFS_FLASH_SEMAPHORE_TIMEOUT
 #define RM_LITTLEFS_FLASH_SEMAPHORE_TIMEOUT    UINT32_MAX
#endif


/** "RLFS" in ASCII, used to determine if channel is open. */
#define RM_LITTLEFS_FLASH_OPEN           (0x524C4653ULL)

#define DATA_FLASH_UPDATE_STATE_INITIALIZE 0
#define DATA_FLASH_UPDATE_STATE_ERASE 1
#define DATA_FLASH_UPDATE_STATE_ERASE_WAIT_COMPLETE 2
#define DATA_FLASH_UPDATE_STATE_WRITE 3
#define DATA_FLASH_UPDATE_STATE_WRITE_WAIT_COMPLETE 4
#define DATA_FLASH_UPDATE_STATE_FINALIZE 5
#define DATA_FLASH_UPDATE_STATE_FINALIZE_COMPLETED 6
#define DATA_FLASH_UPDATE_STATE_ERROR 103

typedef struct _update_data_flash_control_block {
	uint32_t status;
}UPDATA_DATA_FLASH_CONTROL_BLOCK;

static void flashing_callback( void * event );

static void write_callback( void * event );
static void erase_flashing_callback( void * event );

static UPDATA_DATA_FLASH_CONTROL_BLOCK update_data_flash_control_block;
static void update_dataflash_data(const struct lfs_config * c,
        lfs_block_t               block,
        lfs_off_t                 off,
        const void              * buffer,
        lfs_size_t                size,
		uint32_t				  update_state);


void data_flash_update_status_initialize(void);
/** LittleFS API mapping for LittleFS Port interface */
const rm_littlefs_api_t g_rm_littlefs_on_flash =
{
    .open  = RM_LITTLEFS_FLASH_Open,
    .close = RM_LITTLEFS_FLASH_Close,
};

/*******************************************************************************************************************//**
 * @addtogroup RM_LITTLEFS_FLASH
 * @{
 **********************************************************************************************************************/

/*******************************************************************************************************************//**
 * Opens the driver and initializes lower layer driver.
 *
 * Implements @ref rm_littlefs_api_t::open().
 *
 * @retval     FSP_SUCCESS                Success.
 * @retval     FSP_ERR_ASSERTION          An input parameter was invalid.
 * @retval     FSP_ERR_ALREADY_OPEN       Module is already open.
 * @retval     FSP_ERR_INVALID_SIZE       The provided block size is invalid.
 * @retval     FSP_ERR_INVALID_ARGUMENT   Flash BGO mode must be disabled.
 * @retval     FSP_ERR_INTERNAL           Failed to create the semaphore.
 *
 * @return     See @ref RENESAS_ERROR_CODES or functions called by this function for other possible return codes. This
 *             function calls:
 *             * @ref flash_api_t::open
 **********************************************************************************************************************/
fsp_err_t RM_LITTLEFS_FLASH_Open (rm_littlefs_ctrl_t * const p_ctrl, rm_littlefs_cfg_t const * const p_cfg)
{
    rm_littlefs_flash_instance_ctrl_t * p_instance_ctrl = (rm_littlefs_flash_instance_ctrl_t *) p_ctrl;
    p_instance_ctrl->p_cfg = p_cfg;
    flash_err_t err = R_FLASH_Open();

#if LFS_THREAD_SAFE
    p_instance_ctrl->xSemaphore = xSemaphoreCreateMutexStatic(&p_instance_ctrl->xMutexBuffer);

    if (NULL == p_instance_ctrl->xSemaphore)
    {
    	R_FLASH_Close();

        return FSP_ERR_INTERNAL;
    }
#endif

    /* This module is now open. */
    p_instance_ctrl->open = RM_LITTLEFS_FLASH_OPEN;

    xSemaphoreFlashAccess = xSemaphoreCreateBinary();
    xSemaphoreGive( xSemaphoreFlashAccess );

    data_flash_update_status_initialize();

    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * Closes the lower level driver.
 *
 * Implements @ref rm_littlefs_api_t::close().
 *
 * @retval FSP_SUCCESS           Media device closed.
 * @retval FSP_ERR_ASSERTION     An input parameter was invalid.
 * @retval FSP_ERR_NOT_OPEN      Module not open.
 *
 * @return See @ref RENESAS_ERROR_CODES or functions called by this function for other possible return codes.
 *         This function calls:
 *             * @ref flash_api_t::close
 **********************************************************************************************************************/
fsp_err_t RM_LITTLEFS_FLASH_Close (rm_littlefs_ctrl_t * const p_ctrl)
{
    rm_littlefs_flash_instance_ctrl_t * p_instance_ctrl = (rm_littlefs_flash_instance_ctrl_t *) p_ctrl;

    //p_instance_ctrl->open = 0; // comment out this line to keep littleFs work after closing

    if( NULL != xSemaphoreFlashAccess )
    {
        vSemaphoreDelete( xSemaphoreFlashAccess );
        xSemaphoreFlashAccess = NULL;
    }

    R_FLASH_Close();

#if LFS_THREAD_SAFE
    vSemaphoreDelete(p_instance_ctrl->xSemaphore);
#endif

    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * @} (end addtogroup RM_LITTLEFS_FLASH)
 **********************************************************************************************************************/

/*******************************************************************************************************************//**
 * Read from the flash driver. Negative error codes are propogated to the user.
 *
 * @param[in]  c           Pointer to the LittleFS config block.
 * @param[in]  block       The block number
 * @param[in]  off         Offset in bytes
 * @param[out] buffer      The buffer to copy data into
 * @param[in]  size        The size in bytes
 *
 * @retval     LFS_ERR_OK  Read is complete.
 * @retval     LFS_ERR_IO  Lower level driver is not open.
 **********************************************************************************************************************/
int rm_littlefs_flash_read (const struct lfs_config * c,
                            lfs_block_t               block,
                            lfs_off_t                 off,
                            void                    * buffer,
                            lfs_size_t                size)
{
    rm_littlefs_flash_instance_ctrl_t *p_instance_ctrl = (rm_littlefs_flash_instance_ctrl_t*) c->context;

    flash_err_t err;
    lfs_size_t round_size = size;
    uint32_t address = rm_littlefs_flash_data_start + (p_instance_ctrl->p_cfg->p_lfs_cfg->block_size * block) + off;
    uint32_t round_address = address;

    //Round down address to a multiple of FLASH_DF_MIN_PGM_SIZE
    lfs_size_t remainder = address % FLASH_DF_MIN_PGM_SIZE;
    if (remainder != 0)
        round_address = round_address - remainder;

    //Round up size to a multiple of FLASH_DF_MIN_PGM_SIZE
    remainder = size % FLASH_DF_MIN_PGM_SIZE;
    if (remainder != 0)
        round_size = size + FLASH_DF_MIN_PGM_SIZE - remainder;

    /* Run the blank check on the target area in data flash memory. */
    //LogInfo (("blank check area %.8X: %d", address, size));
    xSemaphoreTake(xSemaphoreFlashAccess, portMAX_DELAY);
    err = R_FLASH_BlankCheck (round_address, round_size, 0);
    if (FLASH_SUCCESS != err)
    {
        LogError(("Blank check failed on address %.8X size %d error %d", round_address, round_size, err));
        xSemaphoreGive(xSemaphoreFlashAccess);
        g_blank_check_result == FLASH_RES_NOT_BLANK;
    }

    xSemaphoreTake(xSemaphoreFlashAccess, portMAX_DELAY);
    if (g_blank_check_result == FLASH_RES_BLANK)
    {
        // return 0xFF if it is blank
        memset(buffer, 0xFF, size);
    }
    else
    {
        /* Read directly from the flash. */
        memcpy(buffer, (uint8_t* ) address, size);
    }
    xSemaphoreGive(xSemaphoreFlashAccess);

    return LFS_ERR_OK;
}

/*******************************************************************************************************************//**
 * Writes requested bytes to flash.
 *
 * @param[in]  c           Pointer to the LittleFS config block.
 * @param[in]  block       The block number
 * @param[in]  off         Offset in bytes
 * @param[in]  buffer      The buffer containing data to be written.
 * @param[in]  size        The size in bytes
 *
 * @retval     LFS_ERR_OK  Success.
 * @retval     LFS_ERR_IO  Lower layer is not open or failed to write the flash.
 **********************************************************************************************************************/
int rm_littlefs_flash_write (const struct lfs_config * c,
                             lfs_block_t               block,
                             lfs_off_t                 off,
                             const void              * buffer,
                             lfs_size_t                size)
{
	// if semaphore cannot be obtained then return error
    flash_err_t flash_error_code = FLASH_ERR_BUSY;
    rm_littlefs_flash_instance_ctrl_t * p_instance_ctrl = (rm_littlefs_flash_instance_ctrl_t *) c->context;

    // Flash access protect
    xSemaphoreTake( xSemaphoreFlashAccess, portMAX_DELAY );

    update_data_flash_control_block.status = DATA_FLASH_UPDATE_STATE_WRITE_WAIT_COMPLETE;

    R_BSP_InterruptsDisable();
    flash_error_code = R_FLASH_Write( (uint32_t)buffer,
            (rm_littlefs_flash_data_start +
             (p_instance_ctrl->p_cfg->p_lfs_cfg->block_size * block) + off), size );
    R_BSP_InterruptsEnable();

    //wait for the semaphore to be released by callback
    xSemaphoreTake( xSemaphoreFlashAccess, portMAX_DELAY );

    if( (FLASH_SUCCESS != flash_error_code) ) {
    	xSemaphoreGive(xSemaphoreFlashAccess);
    	return LFS_ERR_IO;
    }

	xSemaphoreGive(xSemaphoreFlashAccess);
    return LFS_ERR_OK;
}

/*******************************************************************************************************************//**
 * Erase the logical block. The location and number of blocks to be erased will depend on block size.
 *
 * @param[in]  c           Pointer to the LittleFS config block.
 * @param[in]  block       The logical block number
 *
 * @retval     LFS_ERR_OK  Success.
 * @retval     LFS_ERR_IO  Lower layer is not open or failed to erase the flash.
 **********************************************************************************************************************/
int rm_littlefs_flash_erase (const struct lfs_config * c, lfs_block_t block)
{
	// if semaphore cannot be obtained then return error
    flash_err_t flash_error_code = FLASH_ERR_BUSY;
    rm_littlefs_flash_instance_ctrl_t * p_instance_ctrl = (rm_littlefs_flash_instance_ctrl_t *) c->context;

    // Flash access protect
    xSemaphoreTake( xSemaphoreFlashAccess, portMAX_DELAY );

	update_data_flash_control_block.status = DATA_FLASH_UPDATE_STATE_ERASE_WAIT_COMPLETE;

    R_BSP_InterruptsDisable();
    flash_error_code = R_FLASH_Erase((flash_block_address_t)(rm_littlefs_flash_data_start + (p_instance_ctrl->p_cfg->p_lfs_cfg->block_size * block)),
			p_instance_ctrl->p_cfg->p_lfs_cfg->block_size / RM_LITTLEFS_FLASH_DATA_BLOCK_SIZE);
    R_BSP_InterruptsEnable();

    //wait for the semaphore to be released by callback
    xSemaphoreTake( xSemaphoreFlashAccess, portMAX_DELAY );

    if( (FLASH_SUCCESS != flash_error_code) ) {
    	xSemaphoreGive(xSemaphoreFlashAccess);
    	return LFS_ERR_IO;
    }

	xSemaphoreGive(xSemaphoreFlashAccess);
    return LFS_ERR_OK;
}

/*******************************************************************************************************************//**
 * Returns the version of this module.
 *
 * @retval     LFS_ERR_OK  Success.
 * @retval     LFS_ERR_IO  Lower layer is not open or failed to lock the flash.
 **********************************************************************************************************************/
int rm_littlefs_flash_lock (const struct lfs_config * c)
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

/*******************************************************************************************************************//**
 * Returns the version of this module.
 *
 * @retval     LFS_ERR_OK  Success.
 * @retval     LFS_ERR_IO  Lower layer is not open or failed to unlock the flash.
 **********************************************************************************************************************/
int rm_littlefs_flash_unlock (const struct lfs_config * c)
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

/*******************************************************************************************************************//**
 * Stub function required by LittleFS. All calls immedialy write/erase the lower layer.
 * @param[in]  c           Pointer to the LittleFS config block.
 * @retval     LFS_ERR_OK  Success.
 **********************************************************************************************************************/
int rm_littlefs_flash_sync (const struct lfs_config * c)
{
    FSP_PARAMETER_NOT_USED(c);

    return LFS_ERR_OK;
}

static void flashing_callback( void * event )
{
	uint32_t event_code;
	event_code = *((uint32_t*)event);

	static portBASE_TYPE xHigherPriorityTaskWoken;

	switch(event_code)
	{
		case FLASH_INT_EVENT_ERASE_COMPLETE:
			if(DATA_FLASH_UPDATE_STATE_ERASE_WAIT_COMPLETE == update_data_flash_control_block.status)
			{
				update_data_flash_control_block.status = DATA_FLASH_UPDATE_STATE_FINALIZE;
				xSemaphoreGiveFromISR( xSemaphoreFlashAccess, &xHigherPriorityTaskWoken );
			    portYIELD_FROM_ISR( xHigherPriorityTaskWoken );
			}
			else
			{
				update_data_flash_control_block.status = DATA_FLASH_UPDATE_STATE_ERROR;
				xSemaphoreGiveFromISR( xSemaphoreFlashAccess, &xHigherPriorityTaskWoken );
			}
			break;
		case FLASH_INT_EVENT_WRITE_COMPLETE:
			if(DATA_FLASH_UPDATE_STATE_WRITE_WAIT_COMPLETE == update_data_flash_control_block.status)
			{
				update_data_flash_control_block.status = DATA_FLASH_UPDATE_STATE_FINALIZE;
				xSemaphoreGiveFromISR( xSemaphoreFlashAccess, &xHigherPriorityTaskWoken );
			    portYIELD_FROM_ISR( xHigherPriorityTaskWoken );
			}
			else
			{
				update_data_flash_control_block.status = DATA_FLASH_UPDATE_STATE_ERROR;
				xSemaphoreGiveFromISR( xSemaphoreFlashAccess, &xHigherPriorityTaskWoken );
			}
			break;
        case FLASH_INT_EVENT_BLANK:
            g_blank_check_result = FLASH_RES_BLANK;
            xSemaphoreGiveFromISR( xSemaphoreFlashAccess, &xHigherPriorityTaskWoken );
            break;
        case FLASH_INT_EVENT_NOT_BLANK:
            g_blank_check_result = FLASH_RES_NOT_BLANK;
            xSemaphoreGiveFromISR( xSemaphoreFlashAccess, &xHigherPriorityTaskWoken );
            break;
		default:
			update_data_flash_control_block.status = DATA_FLASH_UPDATE_STATE_ERROR;
			xSemaphoreGiveFromISR( xSemaphoreFlashAccess, &xHigherPriorityTaskWoken );
			break;
	}
}

void data_flash_update_status_initialize(void)
{
	flash_interrupt_config_t cb_func_info;

	cb_func_info.pcallback = flashing_callback;
	cb_func_info.int_priority = 3;
	R_FLASH_Control(FLASH_CMD_SET_BGO_CALLBACK, (void *)&cb_func_info);

	update_data_flash_control_block.status = DATA_FLASH_UPDATE_STATE_INITIALIZE;
}
