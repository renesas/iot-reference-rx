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
* File Name    : boot_loader.h
* Description  : sample boot loader header
***********************************************************************************************************************/

/**********************************************************************************************************************
Includes   <System Includes> , "Project Includes"
**********************************************************************************************************************/
#ifndef SAMPLE_BOOT_LOADER_MAIN_H_
#define SAMPLE_BOOT_LOADER_MAIN_H_

#include <stdio.h>
#include "platform.h"
#include "r_smc_entry.h"
#include "r_fwup_if.h"
#include "r_fwup_config.h"
#include "r_sci_rx_if.h"
#include "r_sci_rx_pinset.h"

/**********************************************************************************************************************
Macro definitions
**********************************************************************************************************************/
#define BL_MCU_NAME                 "RX65N"

#define BL_UART_RTS                 (PORT9.PODR.BIT.B3)
#define BL_USER_SW_PORT             (PORTD.PIDR.BIT.B0)
#define BL_USER_SW_ON               (0)
#define BL_FLASH_BUF_SIZE           (128*5)

#define BL_SCI_RATE                 (115200)
#define BL_SCI_INT_PRIORITY         (15)
#define BL_SCI_CH                   (SCI_CH5)
#define BL_SCI_PIN_FUNC             (R_SCI_PinSet_SCI5)
#define BL_SCI_TX_BUFSIZ            (SCI_CFG_CH5_TX_BUFSIZ)

/**********************************************************************************************************************
Typedef definitions
**********************************************************************************************************************/
/* Flash Buffer */
typedef struct st_bl_buf
{
    uint8_t  buf[BL_FLASH_BUF_SIZE + 8];
    uint32_t cnt;
    uint32_t total;
} st_flash_buf_t;

/**********************************************************************************************************************
Public Functions
**********************************************************************************************************************/

#endif /* SAMPLE_BOOT_LOADER_MAIN_H_ */
