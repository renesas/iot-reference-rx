/***********************************************************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products. No
* other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all
* applicable laws, including copyright laws.
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
* THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED. TO THE MAXIMUM
* EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES
* SHALL BE LIABLE FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO THIS
* SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability of
* this software. By using this software, you agree to the additional terms and conditions found by accessing the
* following link:
* http://www.renesas.com/disclaimer
*
* Copyright (C) 2023 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/
/***********************************************************************************************************************
* File Name     : boot_loader.c
* Description   : main function.
***********************************************************************************************************************/
/**********************************************************************************************************************
 Includes   <System Includes> , "Project Includes"
 *********************************************************************************************************************/
#include "boot_loader.h"

/**********************************************************************************************************************
 Macro definitions
 *********************************************************************************************************************/
#define BL_MSG_SEND_VIA_UART "send image(*.rsu) via UART.\r\n"
#define BL_MSG_ACTIVATE_NEW_APP "activating new image ...\r\n"
#define BL_MSG_SW_RESET "software reset...\r\n"
#define BL_MSG_ERROR "error occurred. please reset your board.\r\n"
#define BL_MSG_JUMP_TO_USER_PROG "execute new image ...\r\n"
#define BL_MSG_START_BOOTLOADER "\r\n==== RX65N : BootLoader [%s] ====\r\n"
#define BL_MSG_OK "OK\r\n"
#define BL_MSG_NG "NG\r\n"

#define BL_LOG_ENABLE          (1)
#if (BL_LOG_ENABLE == 0)
#define BL_LOG(...)
#else
#define BL_LOG                 printf
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
S_C_CH_FAR BL_MSG_UPDATE_MODE_STR[][32] = {"dual bank", "with buffer", "without buffer", "with ext-buffer"};

#if (FWUP_CFG_UPDATE_MODE != FWUP_DUAL_BANK) && (FWUP_CFG_UPDATE_MODE != FWUP_SINGLE_BANK_WO_BUFFER)
S_C_CH_FAR MSG_TEMPCOPY_OK[] = "copy to main area ... OK\r\n";
S_C_CH_FAR MSG_TEMPCOPY_NG[] = "copy to main area ... NG\r\n";
#endif /* (FWUP_CFG_UPDATE_MODE == 0) */

static void sci_callback (void *pArgs);
static void sample_buffering (uint8_t rx_data);
static e_fwup_err_t open_boot_loader  (void);
static void close_boot_loader (void);
static void sample_buf_init   (void);
static e_fwup_err_t sample_write_image (e_fwup_area_t bank);
static e_fwup_err_t sample_verify_img (e_fwup_area_t area);
static void sample_exec_img (void);
static void sample_sw_reset (void);
static e_fwup_err_t sample_activate_img (void);

static st_flash_buf_t s_flash_buf;
static sci_hdl_t s_hdl;
static uint8_t s_err_flg = 0;

/**********************************************************************************************************************
* Function Name: sci_callback
**********************************************************************************************************************/
static void sci_callback(void *pArgs)
{
    sci_cb_args_t * p_args;
    p_args = (sci_cb_args_t*) pArgs;
    uint8_t  ch;

    switch (p_args->event)
    {
    case SCI_EVT_RX_CHAR:
        R_SCI_Receive(p_args->hdl, &ch, 1);
        sample_buffering(ch);
        break;
    default:
        R_BSP_NOP(); /* Do nothing */
        break;
    }
}
/**********************************************************************************************************************
 End of function sci_callback
 *********************************************************************************************************************/

/**********************************************************************************************************************
* Function Name: sample_sci_open
**********************************************************************************************************************/
static void sample_sci_open(void)
{
    sci_cfg_t cfg;

    /* Set up the configuration data structure for asynchronous (UART) operation. */
    cfg.async.baud_rate    = BL_SCI_RATE;
    cfg.async.clk_src      = SCI_CLK_INT;
    cfg.async.data_size    = SCI_DATA_8BIT;
    cfg.async.parity_en    = SCI_PARITY_OFF;
    cfg.async.parity_type  = SCI_EVEN_PARITY;
    cfg.async.stop_bits    = SCI_STOPBITS_1;
    cfg.async.int_priority = BL_SCI_INT_PRIORITY;

    R_BSP_InterruptsEnable();
    BL_SCI_PIN_FUNC();
    R_SCI_Open(BL_SCI_CH, SCI_MODE_ASYNC, &cfg, sci_callback, &s_hdl);
}
/**********************************************************************************************************************
 End of function sample_sci_open
 *********************************************************************************************************************/

/**********************************************************************************************************************
* Function Name: sample_sci_close
**********************************************************************************************************************/
static void sample_sci_close(void)
{
    R_SCI_Close(s_hdl);
}
/**********************************************************************************************************************
 End of function sample_sci_close
 *********************************************************************************************************************/

/**********************************************************************************************************************
* Function Name: sample_buf_init
**********************************************************************************************************************/
static void sample_buf_init(void)
{
    memset(&s_flash_buf, 0, sizeof(st_flash_buf_t));
}
/**********************************************************************************************************************
 End of function sample_buf_init
 *********************************************************************************************************************/

/**********************************************************************************************************************
* Function Name: sample_buffering
* Description  : This is a template for an SCI Async Mode callback function.
* Arguments    : rx_data
* Return Value : none
**********************************************************************************************************************/
static void sample_buffering(uint8_t rx_data)
{
    static uint32_t  file_size;

    /* Error occurred? */
    if (0 != s_err_flg)
    {
        return;
    }

    /* Buffering firmware */
    s_flash_buf.buf[s_flash_buf.cnt++] = rx_data;
    s_flash_buf.total++;

    /* Buffer full? */
    if (BL_FLASH_BUF_SIZE == s_flash_buf.cnt)
    {
        file_size = R_FWUP_GetImageSize();
        BL_UART_RTS = 1;
    }
    else
    {
        /* end of file? */
        if ((0 < file_size) && (file_size <= s_flash_buf.total))
        {
            BL_UART_RTS = 1;
        }
    }
}
/**********************************************************************************************************************
 End of function sample_buffering
 *********************************************************************************************************************/

/**********************************************************************************************************************
* Function Name: sample_write_image
* Description  :
* Arguments    : bank
* Return Value : FWUP_SUCCESS    : updated user program
*                FWUP_ERR_FLASH  : flash write error
*                FWUP_ERR_VERIFY : verify error
**********************************************************************************************************************/
static e_fwup_err_t sample_write_image(e_fwup_area_t area)
{
    e_fwup_err_t ret_val = FWUP_ERR_FAILURE;
    uint32_t write_size;

    while(1)
    {
        if (1 == BL_UART_RTS)
        {
            /* Wait for RTS=ON */
            R_FWUP_SoftwareDelay(500, FWUP_DELAY_MICROSECS);

            /* update firmware */
            write_size = (BL_FLASH_BUF_SIZE < s_flash_buf.cnt) ? BL_FLASH_BUF_SIZE : s_flash_buf.cnt;
            ret_val = R_FWUP_WriteImage(area, &s_flash_buf.buf[0], write_size);

            /* there are received data during RTS=ON */
            if (BL_FLASH_BUF_SIZE < s_flash_buf.cnt)
            {
                s_flash_buf.cnt -= BL_FLASH_BUF_SIZE;
                MEMCPY(&s_flash_buf.buf[0], &s_flash_buf.buf[BL_FLASH_BUF_SIZE], s_flash_buf.cnt);
            }
            else
            {
                s_flash_buf.cnt = 0;
            }

            /* updated or error? */
            if (FWUP_PROGRESS != ret_val)
            {
                break;
            }
            BL_UART_RTS = 0;
        }
    }

    sample_buf_init();

    /* Verify */
    if (FWUP_SUCCESS == ret_val)
    {
        ret_val = sample_verify_img(area);
    }
    return (ret_val);
}
/**********************************************************************************************************************
 End of function sample_write_image
 *********************************************************************************************************************/

/**********************************************************************************************************************
* Function Name: open_boot_loader
* Description  :
* Arguments    :
* Return Value :
**********************************************************************************************************************/
static e_fwup_err_t open_boot_loader(void)
{
    R_BSP_NOP();
    R_BSP_InterruptsEnable();
    sample_sci_open();
    R_FWUP_SoftwareDelay(50, FWUP_DELAY_MILLISECS);

    BL_UART_RTS = 0;
    s_err_flg = 0;
    sample_buf_init();

    if (FWUP_SUCCESS != R_FWUP_Open())
    {
        return (FWUP_ERR_FAILURE);
    }

    return (FWUP_SUCCESS);
}
/**********************************************************************************************************************
 End of function open_boot_loader
 *********************************************************************************************************************/

/**********************************************************************************************************************
* Function Name: close_boot_loader
* Description  :
* Arguments    :
* Return Value :
**********************************************************************************************************************/
static void close_boot_loader(void)
{
    sample_sci_close();
    R_FWUP_Close();
    R_FWUP_SoftwareDelay(50, FWUP_DELAY_MILLISECS);
}
/**********************************************************************************************************************
 End of function close_boot_loader
 *********************************************************************************************************************/

/**********************************************************************************************************************
* Function Name: sample_verify_img
* Description  :
* Arguments    :
* Return Value :
**********************************************************************************************************************/
static e_fwup_err_t sample_verify_img(e_fwup_area_t area)
{
    if (FWUP_SUCCESS != R_FWUP_VerifyImage(area))
    {
        /* Erase the main side */
        R_FWUP_EraseArea(area);
        return (FWUP_ERR_FAILURE);
    }
    return (FWUP_SUCCESS);
}
/**********************************************************************************************************************
 End of function sample_verify_img
 *********************************************************************************************************************/

/**********************************************************************************************************************
* Function Name: sample_exec_img
* Description  :
* Arguments    :
* Return Value :
**********************************************************************************************************************/
static void sample_exec_img(void)
{
    BL_LOG(BL_MSG_JUMP_TO_USER_PROG);
    close_boot_loader();
    R_FWUP_ExecImage();
}
/**********************************************************************************************************************
 End of function sample_exec_img
 *********************************************************************************************************************/

/**********************************************************************************************************************
* Function Name: sample_sw_reset
* Description  :
* Arguments    :
* Return Value :
**********************************************************************************************************************/
static void sample_sw_reset(void)
{
    BL_LOG(BL_MSG_SW_RESET);
    close_boot_loader();
    R_FWUP_SoftwareReset();
}
/**********************************************************************************************************************
 End of function sample_sw_reset
 *********************************************************************************************************************/

/**********************************************************************************************************************
* Function Name: sample_activate_img
* Description  :
* Arguments    :
* Return Value :
**********************************************************************************************************************/
static e_fwup_err_t sample_activate_img(void)
{
    /* Copy the updated image from buffer side to main side */
    BL_LOG(BL_MSG_ACTIVATE_NEW_APP);
    if (FWUP_SUCCESS == R_FWUP_ActivateImage())
    {
        /* Software reset */
        BL_LOG(BL_MSG_OK);
        sample_sw_reset();
    }

    /* Exit with error */
    BL_LOG(BL_MSG_NG);
    return (FWUP_ERR_FAILURE);
}
/**********************************************************************************************************************
 End of function sample_activate_img
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: main
 *********************************************************************************************************************/
void main(void)
{
    /* Open boot loader */
    if (FWUP_SUCCESS != open_boot_loader())
    {
        goto ERROR_END;
    }

#if (FWUP_CFG_UPDATE_MODE == FWUP_DUAL_BANK)
    /* use Main and Buffer */
    BL_LOG(BL_MSG_START_BOOTLOADER, BL_MSG_UPDATE_MODE_STR[FWUP_CFG_UPDATE_MODE]);
    if (true != R_FWUP_IsExistImage(FWUP_AREA_MAIN))
    {
        /* Install initial firmware */
        BL_LOG(BL_MSG_SEND_VIA_UART);
        if (FWUP_SUCCESS != sample_write_image(FWUP_AREA_BUFFER))
        {
            goto ERROR_END;
        }

        /* Activate image and Software reset */
        if (FWUP_SUCCESS != sample_activate_img())
        {
            goto ERROR_END;
        }
    }

#if defined(BSP_MCU_RX65N)
    /* Reset Generation Source is Deep standby? */
    if (1 == SYSTEM.RSTSR0.BIT.DPSRSTF)
    {
        /* This source means main side image is running so skip image verifying. */
        sample_exec_img();
    }
#endif

    /* Verify main area */
    if (FWUP_SUCCESS == sample_verify_img(FWUP_AREA_MAIN))
    {
        sample_exec_img();
    }

ERROR_END:
    R_FWUP_EraseArea(FWUP_AREA_BUFFER);
    BL_LOG(BL_MSG_ERROR);
    s_err_flg = 1;
    BL_UART_RTS = 0;
    while (1);
#else
    /* use Main and Buffer */
    BL_LOG(BL_MSG_START_BOOTLOADER, BL_MSG_UPDATE_MODE_STR[FWUP_CFG_UPDATE_MODE]);
    if (true != R_FWUP_IsExistImage(FWUP_AREA_MAIN))
    {
        /* Install initial firmware */
        BL_LOG(BL_MSG_SEND_VIA_UART);
        if (FWUP_SUCCESS != sample_write_image(FWUP_AREA_MAIN))
        {
            goto END;
        }
        goto SW_RESET;
    }

    if (true != R_FWUP_IsExistImage(FWUP_AREA_BUFFER))
    {
        /* Verify Main area */
        if (FWUP_SUCCESS != R_FWUP_VerifyImage(FWUP_AREA_MAIN))
        {
            R_FWUP_EraseArea(FWUP_AREA_MAIN);
            goto END;
        }

        /* Jump to user program */
        BL_LOG(BL_MSG_JUMP_TO_USER_PROG);
        close_boot_loader();
        R_FWUP_ExecImage();
    }

    /* Verify Buffer area */
    if (FWUP_SUCCESS != R_FWUP_VerifyImage(FWUP_AREA_BUFFER))
    {
        R_FWUP_EraseArea(FWUP_AREA_BUFFER);
    }
    else
    {
    	BL_LOG(BL_MSG_ACTIVATE_NEW_APP);
        if (FWUP_SUCCESS != R_FWUP_ActivateImage())
        {
            BL_LOG(MSG_TEMPCOPY_NG);
        }
        else
        {
            BL_LOG(MSG_TEMPCOPY_OK);
        }
    }

SW_RESET:
    BL_LOG(BL_MSG_SW_RESET);
    close_boot_loader();
    R_FWUP_SoftwareReset();
END:
    BL_LOG(BL_MSG_ERROR);
    s_err_flg = 1;
    BL_UART_RTS = 0;
    while (1);
#endif
}/* End of function main */


/***********************************************************************************************************************
 * Function Name: my_sw_charput_function
 * Description  : char data output API
 * Arguments    : data -
 *                    Send data with SCI
 * Return Value : none
 **********************************************************************************************************************/
void my_sw_charput_function(uint8_t data)
{
    uint32_t arg = 0;

    /* do not call printf()->charput in interrupt context */
    do
    {
        /* Casting void pointer is used for address. */
        R_SCI_Control(s_hdl, SCI_CMD_TX_Q_BYTES_FREE, (void*) &arg);
    } while (BL_SCI_TX_BUFSIZ != arg);

    /* Casting uint8_t pointer is used for address. */
    R_SCI_Send(s_hdl, (uint8_t*) &data, 1);

    return;
}
/**********************************************************************************************************************
 End of function my_sw_charput_function
 *********************************************************************************************************************/
