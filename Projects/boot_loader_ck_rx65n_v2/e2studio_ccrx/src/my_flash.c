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
 * File Name    : my_flash.c
 * Description  : Functions for the Firmware update module.
 **********************************************************************************************************************
/**********************************************************************************************************************
 Includes   <System Includes> , "Project Includes"
 *********************************************************************************************************************/
#include "r_fwup_if.h"
#include "r_fwup_wrap_com.h"
#include "r_fwup_wrap_flash.h"

#include "r_flash_rx_if.h"

/**********************************************************************************************************************
 Macro definitions
 *********************************************************************************************************************/
#if (FWUP_CFG_UPDATE_MODE == FWUP_DUAL_BANK) && (FLASH_CFG_CODE_FLASH_BGO == 1) && (FLASH_CFG_DATA_FLASH_BGO == 1)
#define FLASH_BGO_MODE 1
#else
#define FLASH_BGO_MODE 0
#endif

/**********************************************************************************************************************
 Local Typedef definitions
 *********************************************************************************************************************/

/**********************************************************************************************************************
 Exported global variables
 *********************************************************************************************************************/

/**********************************************************************************************************************
 Private (static) variables and functions
 *********************************************************************************************************************/
#if (FLASH_BGO_MODE == 1)
static uint8_t s_flash_wait_flag = 0;
static e_fwup_err_t s_bgo_ret;
static void u_cb_function(void *event)
{
    flash_int_cb_args_t *ready_event = event;

    switch (ready_event->event)
    {
        case FLASH_INT_EVENT_ERASE_COMPLETE:
        case FLASH_INT_EVENT_WRITE_COMPLETE:
        case FLASH_INT_EVENT_TOGGLE_BANK:
            s_bgo_ret = FWUP_SUCCESS;
            break;
        default:
            s_bgo_ret = FWUP_ERR_FLASH;
            break;
    }
    s_flash_wait_flag = 1;
}
#endif /* (FLASH_BGO_MODE == 1) */

/*
 * Internal flash
 */
/**********************************************************************************************************************
 * Function Name: my_flash_open_function
 * Description  : wrapper function for initializing Flash module.
 * Arguments    : None
 * Return Value : FWUP_SUCCESS   : success
 *                FWUP_ERR_FLASH : flash open error
 *********************************************************************************************************************/
e_fwup_err_t my_flash_open_function(void)
{
    if (FLASH_SUCCESS != R_FLASH_Open())
    {
        return (FWUP_ERR_FLASH);
    }
#if (FLASH_BGO_MODE == 1)
    flash_interrupt_config_t cb_func_info;
    cb_func_info.pcallback = u_cb_function;
    cb_func_info.int_priority = 1;
    if (FLASH_SUCCESS != R_FLASH_Control(FLASH_CMD_SET_BGO_CALLBACK,(void *)&cb_func_info))
    {
        return (FWUP_ERR_FLASH);
    }
#endif /* (FLASH_BGO_MODE == 1) */
    return (FWUP_SUCCESS);
}
/**********************************************************************************************************************
 End of function my_flash_open_function
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: my_flash_close_function
 * Description  : wrapper function for closing Flash module.
 * Arguments    : None
 * Return Value : None
 *********************************************************************************************************************/
void my_flash_close_function(void)
{
    R_FLASH_Close();
}
/**********************************************************************************************************************
 End of function my_flash_close_function
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: my_flash_erase_function
 * Description  : wrapper function for erasing Flash.
 * Arguments    : addr           : erasure destination address
 *                num_blocks     : number of blocks to erase
 * Return Value : FWUP_SUCCESS   : success
 *                FWUP_ERR_FLASH : flash erase error
 *********************************************************************************************************************/
e_fwup_err_t my_flash_erase_function(uint32_t addr, uint32_t num_blocks)
{
    uint32_t blk_addr;
    if((FLASH_DF_BLOCK_0 <= addr) && (addr < FLASH_DF_BLOCK_INVALID ))
    {
        blk_addr = addr;
    }
    else
    {
        blk_addr = addr + (FWUP_CFG_CF_BLK_SIZE * (num_blocks - 1));
    }

#if (FLASH_BGO_MODE == 1)
    s_flash_wait_flag = 0;
    flash_err_t ret = R_FLASH_Erase((flash_block_address_t )blk_addr, num_blocks);
    if (FLASH_SUCCESS != ret)
    {
        return (FWUP_ERR_FLASH);
    }
    /* WAIT_LOOP */
    while(s_flash_wait_flag == 0)
    {
        /**** Start user code ****/
        R_BSP_NOP();
        /**** End user code   ****/
    }
    return s_bgo_ret;
#else
    R_BSP_InterruptsDisable();
    flash_err_t ret = R_FLASH_Erase((flash_block_address_t )blk_addr, num_blocks);
    R_BSP_InterruptsEnable();
    if (FLASH_SUCCESS != ret)
    {
        return (FWUP_ERR_FLASH);
    }
    return (FWUP_SUCCESS);
#endif /* (FLASH_BGO_MODE == 1) */
}
/**********************************************************************************************************************
 End of function my_flash_erase_function
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: my_flash_write_function
 * Description  : wrapper function for writing Flash.
 * Arguments    : src_addr       : write source address
 *                dest_addr      : write destination address
 *                num_bytes      : number of bytes to write
 * Return Value : FWUP_SUCCESS   : success
 *                FWUP_ERR_FLASH : flash write error
 *********************************************************************************************************************/
e_fwup_err_t my_flash_write_function(uint32_t src_addr, uint32_t dest_addr, uint32_t num_bytes)
{
#if (FLASH_BGO_MODE == 1)
    s_flash_wait_flag = 0;
    flash_err_t ret = R_FLASH_Write(src_addr, dest_addr, num_bytes);
    if (FLASH_SUCCESS != ret)
    {
        return (FWUP_ERR_FLASH);
    }
    /* WAIT_LOOP */
    while(s_flash_wait_flag == 0)
    {
        /**** Start user code ****/
        R_BSP_NOP();
        /**** End user code   ****/
    }
    return s_bgo_ret;
#else
    R_BSP_InterruptsDisable();
    flash_err_t ret = R_FLASH_Write(src_addr, dest_addr, num_bytes);
    R_BSP_InterruptsEnable();
    if (FLASH_SUCCESS != ret)
    {
        return (FWUP_ERR_FLASH);
    }
    return (FWUP_SUCCESS);
#endif /* (FLASH_BGO_MODE == 1) */
}
/**********************************************************************************************************************
 End of function my_flash_write_function
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: my_flash_read_function
 * Description  : wrapper function for reading Flash.
 * Arguments    : buf_addr     : storage destination address
 *                src_addr     : read source address
 *                size         : number of bytes to read
 * Return Value : FWUP_SUCCESS : success
 *********************************************************************************************************************/
e_fwup_err_t my_flash_read_function(uint32_t buf_addr, uint32_t src_addr, uint32_t size)
{
    MEMCPY((void FWUP_FAR *)buf_addr, (void FWUP_FAR *)src_addr, size);
    return (FWUP_SUCCESS);
}
/**********************************************************************************************************************
 End of function my_flash_read_function
 *********************************************************************************************************************/

#if (FWUP_CFG_UPDATE_MODE == FWUP_DUAL_BANK)
/**********************************************************************************************************************
 * Function Name: my_bank_swap_function
 * Description  : wrapper function for bank swap.
 * Arguments    : none
 * Return Value : FWUP_SUCCESS   : success
 *                FWUP_ERR_FLASH : flash control error
 *********************************************************************************************************************/
e_fwup_err_t my_bank_swap_function(void)
{
    flash_err_t err;

#if (FLASH_BGO_MODE == 1)
    s_flash_wait_flag = 0;
    err = R_FLASH_Control(FLASH_CMD_BANK_TOGGLE, NULL);
    if (FLASH_SUCCESS != err)
    {
        return (FWUP_ERR_FLASH);
    }
    /* WAIT_LOOP */
    while(s_flash_wait_flag == 0)
    {
        /**** Start user code ****/
        R_BSP_NOP();
        /**** End user code   ****/
    }
    return s_bgo_ret;
#else
    R_BSP_InterruptsDisable();
    err = R_FLASH_Control(FLASH_CMD_BANK_TOGGLE, NULL);
    R_BSP_InterruptsEnable();

    if (FLASH_SUCCESS != err)
    {
        return (FWUP_ERR_FLASH);
    }
    return (FWUP_SUCCESS);
#endif /* (FLASH_BGO_MODE == 1) */
}
/**********************************************************************************************************************
 End of function my_bank_swap_function
 *********************************************************************************************************************/
#endif /* (FWUP_CFG_UPDATE_MODE == FWUP_DUAL_BANK) */