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
 * Version      : 1.0
 * Description  : .
 *********************************************************************************************************************/

/**********************************************************************************************************************
 Includes   <System Includes> , "Project Includes"
 *********************************************************************************************************************/
#include "r_fwup_if.h"
#include "r_fwup_wrap_com.h"
#include "r_fwup_wrap_flash.h"

/**** Start user code ****/
#if defined(__RX)
#include "r_flash_rx_if.h"
#else
#include "r_rfd_common_api.h"
#endif /* defined(__RX) */
/**** End user code   ****/

/**********************************************************************************************************************
 Macro definitions
 *********************************************************************************************************************/
/**** Start user code ****/
/**** End user code   ****/

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
#if defined(__RX)
    if (FLASH_SUCCESS != R_FLASH_Open())
    {
        return FWUP_ERR_FLASH;
    }
    return FWUP_SUCCESS;
#else
    if (0 != rfd_wrap_flash_open())
    {
        return FWUP_ERR_FLASH;
    }

    /* Data flash access enable */
    if (0 < FWUP_CFG_DF_NUM_BLKS)
    {
        rfd_wrap_df_access_enable();
    }
    return FWUP_SUCCESS;
#endif /* defined(__RX) */
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
#if defined(__RX)
    R_FLASH_Close();
#else
    /* Data flash access enable */
    if (0 < FWUP_CFG_DF_NUM_BLKS)
    {
        rfd_wrap_df_access_disable();
    }
#endif /* defined(__RX) */
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
#if defined(__RX)
    uint32_t blk_addr;

    blk_addr = (addr + (FWUP_CFG_CF_BLK_SIZE * num_blocks) - 1) & 0xffffc000;
    r_fwup_wrap_disable_interrupt();
    flash_err_t ret = R_FLASH_Erase((flash_block_address_t )blk_addr, num_blocks);
    r_fwup_wrap_enable_interrupt();
    if (FLASH_SUCCESS != ret)
    {
        return FWUP_ERR_FLASH;
    }
    return FWUP_SUCCESS;
#else
    uint32_t ret;
    uint32_t start_block;
    uint8_t (FWUP_FAR_FUNC *pfunc)(uint16_t, uint16_t) = rfd_wrap_cf_erase;

    /* Convert Flash address to Flash block number */
    if (FWUP_CFG_DF_ADDR_L > addr)
    {
        /* Code flash */
        start_block = addr / FWUP_CFG_CF_BLK_SIZE;
    }
    else
    {
        /* Data flash */
        start_block = (addr - FWUP_CFG_DF_ADDR_L) / FWUP_CFG_DF_BLK_SIZE;
        pfunc = rfd_wrap_df_erase;
    }

    /* Erase flash memory. */
    r_fwup_wrap_disable_interrupt();
    ret = pfunc((uint16_t)start_block, (uint16_t)num_blocks);
    r_fwup_wrap_enable_interrupt();

    if (0 != ret)
    {
        return FWUP_ERR_FLASH;
    }
    return FWUP_SUCCESS;
#endif /* defined(__RX) */
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
#if defined(__RX)
    r_fwup_wrap_disable_interrupt();
    flash_err_t ret = R_FLASH_Write(src_addr, dest_addr, num_bytes);
    r_fwup_wrap_enable_interrupt();
    if (FLASH_SUCCESS != ret)
    {
        return FWUP_ERR_FLASH;
    }
    return FWUP_SUCCESS;
#else
    uint32_t ret;
    uint8_t (FWUP_FAR_FUNC *pfunc)(uint32_t, uint16_t, uint8_t __near *) = rfd_wrap_cf_write;

    if (FWUP_CFG_DF_ADDR_L <= dest_addr)
    {
        pfunc = rfd_wrap_df_write;
    }

    /* Write flash */
    r_fwup_wrap_disable_interrupt();
    ret = pfunc(dest_addr, (uint16_t)num_bytes, (uint8_t __near *)(uint16_t)src_addr);
    r_fwup_wrap_enable_interrupt();

    if (0 != ret)
    {
        return FWUP_ERR_FLASH;
    }
    return FWUP_SUCCESS;
#endif /* defined(__RX) */
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

    MEMCPY((void FWUP_FAR *)buf_addr, (void FWUP_FAR *)src_addr, size);
    return FWUP_SUCCESS;

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

    r_fwup_wrap_disable_interrupt();
    err = R_FLASH_Control(FLASH_CMD_BANK_TOGGLE, NULL);
    r_fwup_wrap_enable_interrupt();

    if (FLASH_SUCCESS != err)
    {
        return FWUP_ERR_FLASH;
    }
    return FWUP_SUCCESS;

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
#if defined(__RX)
#else
    R_QSPI_FLASH_Init_Driver();
    if (0 != R_QSPI_FLASH_Set_4byte_Address_Mode(FLASH_DEV0))
    {
        return FWUP_ERR_FLASH;
    }
    return FWUP_SUCCESS;
#endif /* defined(__RX) */
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
#if defined(__RX)
#else
#endif /* defined(__RX) */
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
#if defined(__RX)
#else
    int32_t  ret;
    uint32_t sector_addr;
    uint32_t sector_count;

    for (sector_count = 0; sector_count < num_sectors; sector_count++ )
    {
        sector_addr = addr + (FWUP_CFG_EXT_BUF_AREA_BLK_SIZE * sector_count);

        BSP_DISABLE_INTERRUPT();
        ret = R_QSPI_FLASH_Erase(FLASH_DEV0, sector_addr, FLASH_MODE_S_ERASE);
        BSP_ENABLE_INTERRUPT();

        if (0 != ret)
        {
            return FWUP_ERR_FLASH;
        }
    }
    return FWUP_SUCCESS;
#endif /* defined(__RX) */
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
#if defined(__RX)
#else
    r_qspi_flash_info_t flash_info_w;
    int32_t ret;

    flash_info_w.pData  = (uint8_t FAR *)src_addr;
    flash_info_w.Cnt    = num_bytes;
    flash_info_w.Addr   = dest_addr;

    BSP_DISABLE_INTERRUPT();
    ret = R_QSPI_FLASH_Write_Data(FLASH_DEV0, &flash_info_w);
    BSP_ENABLE_INTERRUPT();

    if (0 != ret)
    {
        return FWUP_ERR_FLASH;
    }
    return FWUP_SUCCESS;
#endif /* defined(__RX) */
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
#if defined(__RX)
#else
    r_qspi_flash_info_t flash_info_r;

    flash_info_r.pData  = (uint8_t FAR *)buf_addr;
    flash_info_r.Cnt    = size;
    flash_info_r.Addr   = src_addr;

    if (0 != R_QSPI_FLASH_Read_Data(FLASH_DEV0, &flash_info_r))
    {
        return FWUP_ERR_FLASH;
    }
    return FWUP_SUCCESS;
#endif /* defined(__RX) */
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

