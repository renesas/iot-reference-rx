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
* Copyright (C) 2013-2023 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/
/***********************************************************************************************************************
* File Name     : boot_loader.c
* Description   : main function.
***********************************************************************************************************************/
/**********************************************************************************************************************
 Includes   <System Includes> , "Project Includes"
 *********************************************************************************************************************/
#include <stdio.h>
#include "platform.h"
#include "r_smc_entry.h"
#include "r_fwup_if.h"
#include "r_fwup_config.h"
#include "r_sci_rx_if.h"
#include "r_sci_rx_pinset.h"

/**********************************************************************************************************************
 Macro definitions
 *********************************************************************************************************************/
#define UART_RTS                    (PORT9.PODR.BIT.B3)
#define FLASH_BUF_SIZE              (128*5)

#if !defined(BSP_CFG_SCI_UART_TERMINAL_ENABLE)
#error "Error! Need to define MY_BSP_CFG_SERIAL_TERM_SCI in r_bsp_config.h"
#elif BSP_CFG_SCI_UART_TERMINAL_CHANNEL == (0)
#define U_SCI_UART_CLI_PINSET()  R_SCI_PinSet_SCI0()
#define U_SCI_UART_CLI_SCI_CH          SCI_CH0
#elif BSP_CFG_SCI_UART_TERMINAL_CHANNEL == (1)
#define U_SCI_UART_CLI_PINSET()  R_SCI_PinSet_SCI1()
#define U_SCI_UART_CLI_SCI_CH          SCI_CH1
#elif BSP_CFG_SCI_UART_TERMINAL_CHANNEL == (2)
#define U_SCI_UART_CLI_PINSET()  R_SCI_PinSet_SCI2()
#define U_SCI_UART_CLI_SCI_CH          SCI_CH2
#elif BSP_CFG_SCI_UART_TERMINAL_CHANNEL == (3)
#define U_SCI_UART_CLI_PINSET()  R_SCI_PinSet_SCI3()
#define U_SCI_UART_CLI_SCI_CH          SCI_CH3
#elif BSP_CFG_SCI_UART_TERMINAL_CHANNEL == (4)
#define U_SCI_UART_CLI_PINSET()  R_SCI_PinSet_SCI4()
#define U_SCI_UART_CLI_SCI_CH          SCI_CH4
#elif BSP_CFG_SCI_UART_TERMINAL_CHANNEL == (5)
#define U_SCI_UART_CLI_PINSET()  R_SCI_PinSet_SCI5()
#define U_SCI_UART_CLI_SCI_CH          SCI_CH5
#define SCI_CFG_TX_BUFSIZ          SCI_CFG_CH5_TX_BUFSIZ
#elif BSP_CFG_SCI_UART_TERMINAL_CHANNEL == (6)
#define U_SCI_UART_CLI_PINSET()  R_SCI_PinSet_SCI6()
#define U_SCI_UART_CLI_SCI_CH          SCI_CH6
#elif BSP_CFG_SCI_UART_TERMINAL_CHANNEL == (7)
#define U_SCI_UART_CLI_PINSET()  R_SCI_PinSet_SCI7()
#define U_SCI_UART_CLI_SCI_CH          SCI_CH7
#elif BSP_CFG_SCI_UART_TERMINAL_CHANNEL == (8)
#define U_SCI_UART_CLI_PINSET()  R_SCI_PinSet_SCI8()
#define U_SCI_UART_CLI_SCI_CH          SCI_CH8
#define SCI_CFG_TX_BUFSIZ          SCI_CFG_CH8_TX_BUFSIZ
#elif BSP_CFG_SCI_UART_TERMINAL_CHANNEL == (9)
#define U_SCI_UART_CLI_PINSET()  R_SCI_PinSet_SCI9()
#define U_SCI_UART_CLI_SCI_CH          SCI_CH9
#elif BSP_CFG_SCI_UART_TERMINAL_CHANNEL == (10)
#define U_SCI_UART_CLI_PINSET()  R_SCI_PinSet_SCI10()
#define U_SCI_UART_CLI_SCI_CH          SCI_CH10
#elif BSP_CFG_SCI_UART_TERMINAL_CHANNEL == (11)
#define U_SCI_UART_CLI_PINSET()  R_SCI_PinSet_SCI11()
#define U_SCI_UART_CLI_SCI_CH          SCI_CH11
#elif BSP_CFG_SCI_UART_TERMINAL_CHANNEL == (12)
#define U_SCI_UART_CLI_PINSET()  R_SCI_PinSet_SCI12()
#define U_SCI_UART_CLI_SCI_CH          SCI_CH12
#else
#error "Error! Invalid setting for MY_BSP_CFG_SERIAL_TERM_SCI in r_bsp_config.h"
#endif
/**********************************************************************************************************************
 Local Typedef definitions
 *********************************************************************************************************************/
/* Flash Buffer */
typedef struct st_bl_buf
{
    uint8_t  buf[FLASH_BUF_SIZE + 8];
    uint32_t cnt;
    uint32_t total;
} st_flash_buf_t;

/**********************************************************************************************************************
 Exported global variables
 *********************************************************************************************************************/

/**********************************************************************************************************************
 Private (static) variables and functions
 *********************************************************************************************************************/
S_C_CH_FAR MSG_SEND_VIA_UART[] = "send image(*.rsu) via UART.\r\n";
S_C_CH_FAR MSG_ACTIVATE_NEW_APP[] = "activating new image ...\r\n";
S_C_CH_FAR MSG_SW_RESET[] = "software reset...\r\n";
S_C_CH_FAR MSG_ERROR[] = "error occurred. please reset your board.\r\n";
S_C_CH_FAR MSG_JUMP_TO_USER_PROG[] = "execute new image ...\r\n";
S_C_CH_FAR MSG_START_BOOTLOADER[] = "\r\n==== RX65N : BootLoader [%s] ====\r\n";
S_C_CH_FAR MSG_UPDATE_MODE_STR[][32] = {"dual bank", "with buffer", "without buffer", "with ext-buffer"};

#if (FWUP_CFG_UPDATE_MODE != FWUP_DUAL_BANK) && (FWUP_CFG_UPDATE_MODE != FWUP_SINGLE_BANK_WO_BUFFER)
S_C_CH_FAR MSG_TEMPCOPY_OK[] = "copy to main area ... OK\r\n";
S_C_CH_FAR MSG_TEMPCOPY_NG[] = "copy to main area ... NG\r\n";
#endif /* (FWUP_CFG_UPDATE_MODE == 0) */

static void sci_callback (void *pArgs);
static void sample_buffering (uint8_t rx_data);
static void open_boot_loader  (void);
static void close_boot_loader (void);
static void sample_buf_init   (void);
static e_fwup_err_t sample_write_image (e_fwup_area_t bank);

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
    cfg.async.baud_rate    = BSP_CFG_SCI_UART_TERMINAL_BITRATE;
    cfg.async.clk_src      = SCI_CLK_INT;
    cfg.async.data_size    = SCI_DATA_8BIT;
    cfg.async.parity_en    = SCI_PARITY_OFF;
    cfg.async.parity_type  = SCI_EVEN_PARITY;
    cfg.async.stop_bits    = SCI_STOPBITS_1;
    cfg.async.int_priority = 15;

    R_BSP_InterruptsEnable();
    U_SCI_UART_CLI_PINSET();
    R_SCI_Open(U_SCI_UART_CLI_SCI_CH, SCI_MODE_ASYNC, &cfg, sci_callback, &s_hdl);
}
/**********************************************************************************************************************
 End of function sample_sci_open
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
    if (FLASH_BUF_SIZE == s_flash_buf.cnt)
    {
        file_size = R_FWUP_GetImageSize();
        UART_RTS = 1;
    }
    else
    {
        /* end of file? */
        if ((0 < file_size) && (file_size <= s_flash_buf.total))
        {
            UART_RTS = 1;
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
        if (1 == UART_RTS)
        {
            /* Wait for RTS=ON */
            R_FWUP_SoftwareDelay(500, FWUP_DELAY_MICROSECS);

            /* update firmware */
            write_size = (FLASH_BUF_SIZE < s_flash_buf.cnt) ? FLASH_BUF_SIZE : s_flash_buf.cnt;
            ret_val = R_FWUP_WriteImage(area, &s_flash_buf.buf[0], write_size);

            /* there are received data during RTS=ON */
            if (FLASH_BUF_SIZE < s_flash_buf.cnt)
            {
                s_flash_buf.cnt -= FLASH_BUF_SIZE;
                MEMCPY(&s_flash_buf.buf[0], &s_flash_buf.buf[FLASH_BUF_SIZE], s_flash_buf.cnt);
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
            UART_RTS = 0;
        }
    }

    sample_buf_init();

    /* Verify */
    if (FWUP_SUCCESS == ret_val)
    {
        if (0 != R_FWUP_VerifyImage(area))
        {
            ret_val = FWUP_ERR_VERIFY;
        }
        else
        {
            ret_val = FWUP_SUCCESS;
        }
    }
    return ret_val;
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
static void open_boot_loader(void)
{
    R_BSP_NOP();

    R_BSP_InterruptsEnable();
    sample_sci_open();
    R_FWUP_SoftwareDelay(50, FWUP_DELAY_MILLISECS);

    UART_RTS = 0;
    s_err_flg = 0;
    sample_buf_init();
    R_FWUP_Open();
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
    R_FWUP_Close();
    R_FWUP_SoftwareDelay(50, FWUP_DELAY_MILLISECS);
}
/**********************************************************************************************************************
 End of function close_boot_loader
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: main
 *********************************************************************************************************************/
void main(void)
{
    open_boot_loader();

#if (FWUP_CFG_UPDATE_MODE == FWUP_DUAL_BANK)
    /* use Main and Buffer */
    printf(MSG_START_BOOTLOADER, MSG_UPDATE_MODE_STR[FWUP_CFG_UPDATE_MODE]);
    if (true != R_FWUP_IsExistImage(FWUP_AREA_MAIN))
    {
        /* Install initial firmware */
        printf(MSG_SEND_VIA_UART);
        if (FWUP_SUCCESS != sample_write_image(FWUP_AREA_BUFFER))
        {
            goto END;
        }
        goto SW_RESET;
    }

    /* Verify main area */
    if (0 != R_FWUP_VerifyImage(FWUP_AREA_MAIN))
    {
    	printf("Verifying image failed...\r\n");
        goto END;
    }

    /* Jump to user program */
    printf(MSG_JUMP_TO_USER_PROG);
    close_boot_loader();
    R_FWUP_ExecImage();

SW_RESET:
    printf(MSG_ACTIVATE_NEW_APP);
    R_FWUP_ActivateImage();
    printf(MSG_SW_RESET);
    close_boot_loader();
    R_FWUP_SoftwareReset();
END:
    R_FWUP_EraseArea(FWUP_AREA_BUFFER);
    printf(MSG_ERROR);
    s_err_flg = 1;
    UART_RTS = 0;
    while (1);
#else
    /* use Main and Buffer */
    printf(MSG_START_BOOTLOADER, MSG_UPDATE_MODE_STR[FWUP_CFG_UPDATE_MODE]);
    if (true != R_FWUP_IsExistImage(FWUP_AREA_MAIN))
    {
        /* Install initial firmware */
        printf(MSG_SEND_VIA_UART);
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
        printf(MSG_JUMP_TO_USER_PROG);
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
    	printf(MSG_ACTIVATE_NEW_APP);
        if (FWUP_SUCCESS != R_FWUP_ActivateImage())
        {
            printf(MSG_TEMPCOPY_NG);
        }
        else
        {
            printf(MSG_TEMPCOPY_OK);
        }
    }

SW_RESET:
    printf(MSG_SW_RESET);
    close_boot_loader();
    R_FWUP_SoftwareReset();
END:
    printf(MSG_ERROR);
    s_err_flg = 1;
    UART_RTS = 0;
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
    } while (SCI_CFG_TX_BUFSIZ != arg);

    /* Casting uint8_t pointer is used for address. */
    R_SCI_Send(s_hdl, (uint8_t*) &data, 1);

    return;
}
/**********************************************************************************************************************
 End of function my_sw_charput_function
 *********************************************************************************************************************/
