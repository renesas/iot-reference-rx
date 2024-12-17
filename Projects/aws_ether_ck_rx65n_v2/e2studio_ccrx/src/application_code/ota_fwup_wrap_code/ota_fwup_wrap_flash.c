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
 * Copyright (C) 2024 Renesas Electronics Corporation. All rights reserved.
 *********************************************************************************************************************/
/**********************************************************************************************************************
 * File Name    : ota_fwup_wrap_flash.c
 * Description  : User functions of FWUP module.
 **********************************************************************************************************************
 * History : DD.MM.YYYY Version Description
 *         : 08.04.2024 1.00    First Release
 *********************************************************************************************************************/

/**********************************************************************************************************************
 Includes   <System Includes> , "Project Includes"
 *********************************************************************************************************************/

#include "ota_fwup_wrap_flash.h"

extern volatile UPDATA_DATA_FLASH_CONTROL_BLOCK update_data_flash_control_block;

/**********************************************************************************************************************
 * Function Name: ota_flash_open_function
 * Description  : user-defined wrapper function for initializing Flash module.
 * Arguments    : None
 * Return Value : FWUP_SUCCESS   : success
 *                FWUP_ERR_FLASH : flash open error
 *********************************************************************************************************************/
e_fwup_err_t ota_flash_open_function(void)
{
	if(update_data_flash_control_block.status == DATA_FLASH_UPDATE_STATE_UNINITIALIZE )
	{
	    /* Call commonapi open function for Flash */
	    e_commonapi_err_t common_api_err = R_Demo_Common_API_Flash_Open();
	    if (COMMONAPI_SUCCESS != common_api_err)
	    {
	        return (FWUP_ERR_FLASH);
	    }

		LogDebug( ("ota_flash_open_function: Create and give semaphore!") );
	}

    return (FWUP_SUCCESS);
}
/**********************************************************************************************************************
 End of function ota_flash_open_function
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: ota_flash_close_function
 * Description  : user-defined wrapper function for closing Flash module.
 * Arguments    : None
 * Return Value : None
 *********************************************************************************************************************/
void ota_flash_close_function(void)
{
	update_data_flash_control_block.status = DATA_FLASH_UPDATE_STATE_UNINITIALIZE;
}
/**********************************************************************************************************************
 End of function ota_flash_close_function
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: ota_flash_erase_function
 * Description  : user-defined wrapper function for erasing Flash.
 * Arguments    : addr           : erasure destination address
 *                num_blocks     : number of blocks to erase
 * Return Value : FWUP_SUCCESS   : success
 *                FWUP_ERR_FLASH : flash erase error
 *********************************************************************************************************************/
e_fwup_err_t ota_flash_erase_function(uint32_t addr, uint32_t num_blocks)
{
    uint32_t blk_addr;
    flash_err_t flash_error_code = FLASH_ERR_BUSY;

#if (FLASH_TYPE == FLASH_TYPE_1)
    blk_addr = addr;
#else
    if((FLASH_DF_BLOCK_0 <= addr) && (addr < FLASH_DF_BLOCK_INVALID ))
    {
        blk_addr = addr;
    }
    else
    {
        blk_addr = addr + (FWUP_CFG_CF_BLK_SIZE * (num_blocks - 1));
    }
#endif /* (FLASH_TYPE == FLASH_TYPE_1) */

    /* Flash access protect */
    LogDebug( ("ota_flash_erase_function: Get semaphore for flash erase...") );

	xSemaphoreTake( xSemaphoreFlashAccess, portMAX_DELAY );
	update_data_flash_control_block.status = DATA_FLASH_UPDATE_STATE_ERASE_WAIT_COMPLETE;
	flash_error_code = R_FLASH_Erase((flash_block_address_t )blk_addr, num_blocks);

    if (FLASH_SUCCESS == flash_error_code)
    {
   		/* wait for the semaphore to be released by callback */
        xSemaphoreTake( xSemaphoreFlashAccess, portMAX_DELAY );
        xSemaphoreGive(xSemaphoreFlashAccess);
        return (FWUP_SUCCESS);
    }
    else
    {
    	LogError( ("Flash erase: NG, at address %x, ret = %d\r\n", blk_addr, flash_error_code ) );
    	xSemaphoreGive(xSemaphoreFlashAccess);
        return (FWUP_ERR_FLASH);
    }

#if (defined(MCU_RX130) && (MCU_ROM_SIZE_BYTES > 0x40000L))
    /* For parts with CF > 256K, erase and blankcheck cannot cross 256k boundary */
    if ((blk_addr > (uint32_t)FLASH_CF_BLOCK_INVALID) &&
        (blk_addr < (uint32_t)FLASH_CF_256KBOUNDARY) &&
        ((blk_addr + (num_blocks * FLASH_CF_BLOCK_SIZE) - 1) > (uint32_t)FLASH_CF_256KBOUNDARY))
    {
        uint32_t tmp_num_blocks = ((uint32_t)FLASH_CF_256KBOUNDARY - addr) / FLASH_CF_BLOCK_SIZE;
        r_fwup_wrap_disable_interrupt();
        ret = R_FLASH_Erase((flash_block_address_t )blk_addr, tmp_num_blocks);
        r_fwup_wrap_enable_interrupt();
        if (FLASH_SUCCESS != ret)
        {
            return (FWUP_ERR_FLASH);
        }
        blk_addr = (uint32_t)FLASH_CF_256KBOUNDARY;
        num_blocks -= tmp_num_blocks;
    }
#endif /* (defined(MCU_RX130) && (MCU_ROM_SIZE_BYTES > 0x40000L)) */

}
/**********************************************************************************************************************
 End of function ota_flash_erase_function
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: ota_flash_write_function
 * Description  : user-defined wrapper function for writing Flash.
 * Arguments    : src_addr       : write source address
 *                dest_addr      : write destination address
 *                num_bytes      : number of bytes to write
 * Return Value : FWUP_SUCCESS   : success
 *                FWUP_ERR_FLASH : flash write error
 *********************************************************************************************************************/
e_fwup_err_t ota_flash_write_function(uint32_t src_addr, uint32_t dest_addr, uint32_t num_bytes)
{
	flash_err_t flash_error_code = FLASH_ERR_BUSY;

	/* Flash access protect */
	xSemaphoreTake( xSemaphoreFlashAccess, portMAX_DELAY );
	update_data_flash_control_block.status = DATA_FLASH_UPDATE_STATE_WRITE_WAIT_COMPLETE;

	flash_error_code = R_FLASH_Write(src_addr, dest_addr, num_bytes);

	if (FLASH_SUCCESS == flash_error_code)
	{
		/* wait for the semaphore to be released by callback */
		xSemaphoreTake( xSemaphoreFlashAccess, portMAX_DELAY );
		xSemaphoreGive( xSemaphoreFlashAccess );
		return (FWUP_SUCCESS);
	}
	else
	{
		LogError( ("ota_flash_write_function: NG, R_FLASH_Write returns %d at %X", flash_error_code, dest_addr) );
		xSemaphoreGive(xSemaphoreFlashAccess);
		return (FWUP_ERR_FLASH);
	}
}
/**********************************************************************************************************************
 End of function ota_flash_write_function
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: ota_flash_read_function
 * Description  : user-defined wrapper function for reading Flash.
 * Arguments    : buf_addr     : storage destination address
 *                src_addr     : read source address
 *                size         : number of bytes to read
 * Return Value : FWUP_SUCCESS : success
 *********************************************************************************************************************/
e_fwup_err_t ota_flash_read_function(uint32_t buf_addr, uint32_t src_addr, uint32_t size)
{
	xSemaphoreTake( xSemaphoreFlashAccess, portMAX_DELAY );
	MEMCPY((void FWUP_FAR *)buf_addr, (void FWUP_FAR *)src_addr, size);
	xSemaphoreGive(xSemaphoreFlashAccess);
	return (FWUP_SUCCESS);
}
/**********************************************************************************************************************
 End of function ota_flash_read_function
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: ota_bank_swap_function
 * Description  : wrapper function for bank swap.
 * Arguments    : none
 * Return Value : FWUP_SUCCESS   : success
 *                FWUP_ERR_FLASH : flash control error
 *********************************************************************************************************************/
e_fwup_err_t ota_bank_swap_function(void)
{

	flash_err_t err;
	flash_bank_t bank_info;
	uint8_t bank_no;

	LogInfo( ("ota_bank_swap_function: Change startup bank...") );

	R_BSP_SoftwareDelay(5000, BSP_DELAY_MILLISECS);

	R_BSP_SET_PSW(0);

	r_fwup_wrap_disable_interrupt();

	err = R_FLASH_Control(FLASH_CMD_BANK_TOGGLE, NULL);
	r_fwup_wrap_enable_interrupt();

	if (FLASH_SUCCESS != err)
	{
		LogInfo( ("ota_bank_swap_function: NG, returns %d", err) );
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
	LogInfo( ("ota_bank_swap_function: The startup bank = %d", bank_no) );

	R_BSP_SoftwareDelay(500, BSP_DELAY_MILLISECS);

	R_BSP_SoftwareReset();

	return (FWUP_SUCCESS);
}
/**********************************************************************************************************************
 End of function ota_bank_swap_function
 *********************************************************************************************************************/
