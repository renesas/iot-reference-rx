/*
 * boot_loader.h
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

/* The RL78 that performs USB-serial conversion is used to write firmware and output log information.
 * However, the RL78 firmware does not support RTS flow control,
 * so the bootloader's flow control processing will not work properly. */
#define BL_UART_RTS                 (PORTC.PODR.BIT.B0)
#define BL_USER_SW_PORT             (PORTD.PIDR.BIT.B1)
#define BL_USER_SW_ON               (0)
#define BL_FLASH_BUF_SIZE           (128*5)

#define BL_SCI_RATE                 (115200)
#define BL_SCI_INT_PRIORITY         (15)
#define BL_SCI_CH                   (SCI_CH5)
#define BL_SCI_PIN_FUNC             (R_SCI_PinSet_SCI5)
#define BL_SCI_TX_BUFSIZ            (SCI_CFG_CH5_TX_BUFSIZ)

/* Initial image program mode? */
#define BL_UPDATE_MODE 						(0)	/* 0:Disable 1:Enable */

/* Does an image exist on the main side? */
#define BL_INITIAL_IMAGE_INSTALL 			(0)	/* 0:Disable 1:Enable */

/* Verify main area */
#define BL_ERASE_BUFFER_AREA_AFTER_VERIFIED (1)	/* 0:Disable 1:Enable */

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
