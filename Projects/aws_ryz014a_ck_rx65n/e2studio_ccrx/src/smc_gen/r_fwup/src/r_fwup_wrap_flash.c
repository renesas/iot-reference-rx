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
 * Copyright (C) 2023 Renesas Electronics Corporation. All rights reserved.
 *********************************************************************************************************************/
/**********************************************************************************************************************
 * File Name    : r_fwup_wrap_flash.c
 * Version      : 2.0
 * Description  : Functions for the Firmware update module.
 *********************************************************************************************************************/

/**********************************************************************************************************************
 Includes   <System Includes> , "Project Includes"
 *********************************************************************************************************************/
#include "r_fwup_if.h"
#include "r_fwup_wrap_com.h"
#include "r_fwup_wrap_flash.h"

/**** Start user code ****/
#include "r_flash_rx_if.h"
#include "FreeRTOS.h"
/**** End user code   ****/

/**********************************************************************************************************************
 Macro definitions
 *********************************************************************************************************************/
/**** Start user code ****/
/**** End user code   ****/
#define DATA_FLASH_UPDATE_STATE_INITIALIZE 0
#define DATA_FLASH_UPDATE_STATE_ERASE 1
#define DATA_FLASH_UPDATE_STATE_ERASE_WAIT_COMPLETE 2
#define DATA_FLASH_UPDATE_STATE_WRITE 3
#define DATA_FLASH_UPDATE_STATE_WRITE_WAIT_COMPLETE 4
#define DATA_FLASH_UPDATE_STATE_FINALIZE 5
#define DATA_FLASH_UPDATE_STATE_FINALIZE_COMPLETED 6
#define DATA_FLASH_UPDATE_STATE_ERROR 103
#define DATA_FLASH_UPDATE_STATE_UNINITIALIZE 0xFF

/**********************************************************************************************************************
 Local Typedef definitions
 *********************************************************************************************************************/
/**** Start user code ****/
/**** End user code   ****/

/**********************************************************************************************************************
 Exported global variables
 *********************************************************************************************************************/
/**** Start user code ****/
/**** End user code   ****/

/**********************************************************************************************************************
 Private (static) variables and functions
 *********************************************************************************************************************/
/**** Start user code ****/
/**** End user code   ****/
static void flashing_callback( void * event );
static xSemaphoreHandle xFWUPSemaphoreFlashAccess;
static uint32_t flash_status = DATA_FLASH_UPDATE_STATE_UNINITIALIZE;
/*
 * Internal flash
 */
/**********************************************************************************************************************
 * Function Name: r_fwup_wrap_flash_open
 * Description  : wrapper function for initializing Flash module.
 * Arguments    : None
 * Return Value : FWUP_SUCCESS
 *                FWUP_ERR_FLASH
 *********************************************************************************************************************/
e_fwup_err_t r_fwup_wrap_flash_open(void)
{
    /**** Start user code ****/
	if( flash_status == DATA_FLASH_UPDATE_STATE_UNINITIALIZE )
	{
		R_FLASH_Close();
		if (FLASH_SUCCESS != R_FLASH_Open())
		{
			return (FWUP_ERR_FLASH);
		}

		LogDebug( ("r_fwup_wrap_flash_open: Create and give semaphore!") );

		xFWUPSemaphoreFlashAccess = xSemaphoreCreateBinary();
		xSemaphoreGive( xFWUPSemaphoreFlashAccess );

		flash_interrupt_config_t cb_func_info;

		cb_func_info.pcallback = flashing_callback;
		cb_func_info.int_priority = 3;
		R_FLASH_Control(FLASH_CMD_SET_BGO_CALLBACK, (void *)&cb_func_info);

		flash_status = DATA_FLASH_UPDATE_STATE_INITIALIZE;
	}

    return (FWUP_SUCCESS);
    /**** End user code   ****/
}
/**********************************************************************************************************************
 End of function r_fwup_wrap_flash_open
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: r_fwup_wrap_flash_close
 * Description  : wrapper function for closing Flash module.
 * Arguments    : None
 * Return Value : None
 *********************************************************************************************************************/
void r_fwup_wrap_flash_close(void)
{
    /**** Start user code ****/
    if( NULL != xFWUPSemaphoreFlashAccess )
    {
        vSemaphoreDelete( xFWUPSemaphoreFlashAccess );
        xFWUPSemaphoreFlashAccess = NULL;
    }

    flash_status = DATA_FLASH_UPDATE_STATE_UNINITIALIZE;

    R_FLASH_Close();
    /**** End user code   ****/
}
/**********************************************************************************************************************
 End of function r_fwup_wrap_flash_close
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: r_fwup_wrap_flash_erase
 * Description  : wrapper function for erasing Flash.
 * Arguments    : addr
 *                num_blocks
 * Return Value : FWUP_SUCCESS
 *                FWUP_ERR_FLASH
 *********************************************************************************************************************/
e_fwup_err_t r_fwup_wrap_flash_erase(uint32_t addr, uint32_t num_blocks)
{
    /**** Start user code ****/
    uint32_t blk_addr;
    flash_err_t flash_error_code = FLASH_ERR_BUSY;

#if (FLASH_TYPE == FLASH_TYPE_1)
    blk_addr = addr;
#else
    blk_addr = addr + (FWUP_CFG_CF_BLK_SIZE * (num_blocks - 1));
#endif

    // Flash access protect
    LogDebug( ("r_fwup_wrap_flash_erase: Get semaphore for flash erase...") );

	xSemaphoreTake( xFWUPSemaphoreFlashAccess, portMAX_DELAY );
	flash_status = DATA_FLASH_UPDATE_STATE_ERASE_WAIT_COMPLETE;
	r_fwup_wrap_disable_interrupt();
	flash_error_code = R_FLASH_Erase((flash_block_address_t )blk_addr, num_blocks);
    r_fwup_wrap_enable_interrupt();

    //wait for the semaphore to be released by callback
    xSemaphoreTake( xFWUPSemaphoreFlashAccess, portMAX_DELAY );

    if (FLASH_SUCCESS != flash_error_code)
    {
    	LogError( ("Flash erase: NG, at address %x, ret = %d\r\n", blk_addr, flash_error_code ) );
    	xSemaphoreGive(xFWUPSemaphoreFlashAccess);
        return (FWUP_ERR_FLASH);
    }

    xSemaphoreGive(xFWUPSemaphoreFlashAccess);

    return (FWUP_SUCCESS);
    /**** End user code   ****/
}
/**********************************************************************************************************************
 End of function r_fwup_wrap_flash_erase
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: r_fwup_wrap_flash_write
 * Description  : wrapper function for writing Flash.
 * Arguments    : src_addr
 *                dest_addr
 *                num_bytes
 * Return Value : FWUP_SUCCESS
 *                FWUP_ERR_FLASH
 *********************************************************************************************************************/
e_fwup_err_t r_fwup_wrap_flash_write(uint32_t src_addr, uint32_t dest_addr, uint32_t num_bytes)
{
    /**** Start user code ****/
	flash_err_t flash_error_code = FLASH_ERR_BUSY;

	// Flash access protect
    //LogDebug( ("r_fwup_wrap_flash_write: Get semaphore for flash write...") );
	xSemaphoreTake( xFWUPSemaphoreFlashAccess, portMAX_DELAY );
	//LogDebug( ("r_fwup_wrap_flash_write: Semaphore got, updating status...") );
	flash_status = DATA_FLASH_UPDATE_STATE_WRITE_WAIT_COMPLETE;

    r_fwup_wrap_disable_interrupt();
    flash_error_code = R_FLASH_Write(src_addr, dest_addr, num_bytes);
    r_fwup_wrap_enable_interrupt();

    //wait for the semaphore to be released by callback
    xSemaphoreTake( xFWUPSemaphoreFlashAccess, portMAX_DELAY );

    if (FLASH_SUCCESS != flash_error_code)
    {
    	LogError( ("r_fwup_wrap_flash_write: NG, R_FLASH_Write returns %d at %X", flash_error_code, dest_addr) );
    	xSemaphoreGive(xFWUPSemaphoreFlashAccess);
        return (FWUP_ERR_FLASH);
    }
    xSemaphoreGive(xFWUPSemaphoreFlashAccess);
    return (FWUP_SUCCESS);
    /**** End user code   ****/
}
/**********************************************************************************************************************
 End of function r_fwup_wrap_flash_write
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: r_fwup_wrap_flash_read
 * Description  : wrapper function for reading Flash.
 * Arguments    : buf_addr
 *                src_addr
 *                size
 * Return Value : FWUP_SUCCESS
 *********************************************************************************************************************/
e_fwup_err_t r_fwup_wrap_flash_read(uint32_t buf_addr, uint32_t src_addr, uint32_t size)
{
    /**** Start user code ****/
	//LogDebug( ("r_fwup_wrap_flash_read: Get semaphore for flash reading at %X with size = %d", src_addr, size) );
	xSemaphoreTake( xFWUPSemaphoreFlashAccess, portMAX_DELAY );
    MEMCPY((void FWUP_FAR *)buf_addr, (void FWUP_FAR *)src_addr, size);
    xSemaphoreGive(xFWUPSemaphoreFlashAccess);
    //LogDebug( ("r_fwup_wrap_flash_read: success!") );
    return (FWUP_SUCCESS);

    /**** End user code   ****/
}
/**********************************************************************************************************************
 End of function r_fwup_wrap_flash_read
 *********************************************************************************************************************/

#if (FWUP_CFG_UPDATE_MODE == FWUP_DUAL_BANK)
/**********************************************************************************************************************
 * Function Name: r_fwup_wrap_bank_swap
 * Description  : wrapper function for bank swap.
 * Arguments    : none
 * Return Value : FWUP_SUCCESS
 *                FWUP_ERR_FLASH
 *********************************************************************************************************************/
e_fwup_err_t r_fwup_wrap_bank_swap(void)
{
    /**** Start user code ****/

    flash_err_t err;
    flash_bank_t bank_info;
    uint8_t bank_no;

    LogInfo( ("r_fwup_wrap_bank_swap: Change startup bank...") );

    R_BSP_SoftwareDelay(5000, BSP_DELAY_MILLISECS);

    R_BSP_SET_PSW(0);

    r_fwup_wrap_disable_interrupt();
    err = R_FLASH_Close();
    err = R_FLASH_Open();
    err = R_FLASH_Control(FLASH_CMD_BANK_TOGGLE, NULL);
    r_fwup_wrap_enable_interrupt();

    if (FLASH_SUCCESS != err)
    {
    	LogInfo( ("r_fwup_wrap_bank_swap: NG, returns %d", err) );
        return (FWUP_ERR_FLASH);
    }

    err = R_FLASH_Control(FLASH_CMD_BANK_GET, (void *)&bank_info);
    if ( bank_info == FLASH_BANK1 )
    {
    	bank_no = 1;
    }
    else if ( bank_info == FLASH_BANK0 )
    {
    	bank_no = 0;
    }
    LogInfo( ("r_fwup_wrap_bank_swap: The startup bank = %d", bank_no) );

    R_BSP_SoftwareDelay(500, BSP_DELAY_MILLISECS);

    R_BSP_SoftwareReset();

    return (FWUP_SUCCESS);

    /**** End user code   ****/
}
/**********************************************************************************************************************
 End of function r_fwup_wrap_bank_swap
 *********************************************************************************************************************/
#endif /* (FWUP_CFG_UPDATE_MODE == FWUP_DUAL_BANK) */

#if (FWUP_CFG_UPDATE_MODE == FWUP_SINGLE_BANK_W_BUFFER_EXT)
/*
 * External flash
 */
/**********************************************************************************************************************
 * Function Name: r_fwup_wrap_ext_flash_open
 * Description  : wrapper function for opening external Flash.
 * Arguments    : None
 * Return Value : FWUP_SUCCESS
 *                FWUP_ERR_FLASH
 *********************************************************************************************************************/
e_fwup_err_t r_fwup_wrap_ext_flash_open(void)
{
    /**** Start user code ****/
    return (FWUP_SUCCESS);
    /**** End user code   ****/
}
/**********************************************************************************************************************
 End of function r_fwup_wrap_ext_flash_open
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: r_fwup_wrap_ext_flash_close
 * Description  : wrapper function for closing external Flash.
 * Arguments    : None
 * Return Value : None
 *********************************************************************************************************************/
void r_fwup_wrap_ext_flash_close(void)
{
    /**** Start user code ****/
    /**** End user code   ****/
}
/**********************************************************************************************************************
 End of function r_fwup_wrap_ext_flash_close
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: r_fwup_wrap_ext_flash_erase
 * Description  : wrapper function for erasing external Flash.
 * Arguments    : addr
 *              : num_sectors
 * Return Value : FWUP_SUCCESS
 *                FWUP_ERR_FLASH
 *********************************************************************************************************************/
e_fwup_err_t r_fwup_wrap_ext_flash_erase(uint32_t addr, uint32_t num_sectors)
{
    /**** Start user code ****/
    return (FWUP_SUCCESS);
    /**** End user code   ****/
}
/**********************************************************************************************************************
 End of function r_fwup_wrap_ext_flash_erase
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: r_fwup_wrap_ext_flash_write
 * Description  : wrapper function for writing external Flash.
 * Arguments    : src_addr
 *              : dest_addr
 *              : num_bytes
 * Return Value : FWUP_SUCCESS
 *                FWUP_ERR_FLASH
 *********************************************************************************************************************/
e_fwup_err_t r_fwup_wrap_ext_flash_write(uint32_t src_addr, uint32_t dest_addr, uint32_t num_bytes)
{
    /**** Start user code ****/
    return (FWUP_SUCCESS);
    /**** End user code   ****/
}
/**********************************************************************************************************************
 End of function r_fwup_wrap_ext_flash_write
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: r_fwup_wrap_ext_flash_read
 * Description  : wrapper function for reading external Flash.
 * Arguments    : buf_addr
 *              : src_addr
 *              : size
 * Return Value : FWUP_SUCCESS
 *                FWUP_ERR_FLASH
 *********************************************************************************************************************/
e_fwup_err_t r_fwup_wrap_ext_flash_read(uint32_t buf_addr, uint32_t src_addr, uint32_t size)
{
    /**** Start user code ****/
    return (FWUP_SUCCESS);
    /**** End user code   ****/
}
/**********************************************************************************************************************
 End of function r_fwup_wrap_ext_flash_read
 *********************************************************************************************************************/
#endif /* (FWUP_CFG_UPDATE_MODE == FWUP_SINGLE_BANK_W_BUFFER_EXT) */

/*
 * static functions
 */
/**** Start user code ****/
/**** End user code   ****/

static void flashing_callback( void * event )
{
	uint32_t event_code;
	event_code = *((uint32_t*)event);
	static portBASE_TYPE xHigherPriorityTaskWoken;

	switch(event_code)
	{
		case FLASH_INT_EVENT_ERASE_COMPLETE:
			if(DATA_FLASH_UPDATE_STATE_ERASE_WAIT_COMPLETE == flash_status)
			{
				flash_status = DATA_FLASH_UPDATE_STATE_FINALIZE;
				xSemaphoreGiveFromISR( xFWUPSemaphoreFlashAccess, &xHigherPriorityTaskWoken );
			    portYIELD_FROM_ISR( xHigherPriorityTaskWoken );
			    LogInfo( ("fwup_wrap_flash: flash erase completed!") );
			}
			else
			{
				flash_status = DATA_FLASH_UPDATE_STATE_ERROR;
				xSemaphoreGiveFromISR( xFWUPSemaphoreFlashAccess, &xHigherPriorityTaskWoken );
			}
			break;
		case FLASH_INT_EVENT_WRITE_COMPLETE:
			if(DATA_FLASH_UPDATE_STATE_WRITE_WAIT_COMPLETE == flash_status)
			{
				flash_status = DATA_FLASH_UPDATE_STATE_FINALIZE;
				xSemaphoreGiveFromISR( xFWUPSemaphoreFlashAccess, &xHigherPriorityTaskWoken );
			    portYIELD_FROM_ISR( xHigherPriorityTaskWoken );
			    LogInfo( ("fwup_wrap_flash: flash write completed!") );
			}
			else
			{
				flash_status = DATA_FLASH_UPDATE_STATE_ERROR;
				xSemaphoreGiveFromISR( xFWUPSemaphoreFlashAccess, &xHigherPriorityTaskWoken );
			}
			break;
		default:
			xSemaphoreGiveFromISR( xFWUPSemaphoreFlashAccess, &xHigherPriorityTaskWoken );
			break;
	}
}
