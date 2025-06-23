/*
* Copyright (c) 2023-2025 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/
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

#define BL_UART_RTS                 (PORTC.PODR.BIT.B0)
#define BL_USER_SW_PORT             (PORTD.PIDR.BIT.B1)
#define BL_USER_SW_ON               (0)
#define BL_FLASH_BUF_SIZE           (FWUP_CFG_CF_W_UNIT_SIZE)

#define BL_SCI_RATE                 (115200)
#define BL_SCI_INT_PRIORITY         (15)
#define BL_SCI_CH                   (SCI_CH5)
#define BL_SCI_PIN_FUNC             (R_SCI_PinSet_SCI5)
#define BL_SCI_TX_BUFSIZ            (SCI_CFG_CH5_TX_BUFSIZ)

/* Initial image program mode? */
#define BL_UPDATE_MODE                      (0)  /* 0:Disable 1:Enable */

/* Does an image exist on the main side? */
#define BL_INITIAL_IMAGE_INSTALL            (0)  /* 0:Disable 1:Enable */

/* Verify main area */
#define BL_ERASE_BUFFER_AREA_AFTER_VERIFIED (0)  /* 0:Disable 1:Enable */

/* Update data flash area */
#define BL_UPDATE_DATA_FLASH                (0)  /* 0:Disable 1:Enable */

/* Enable rollback to old image if signature verification fails */
#define BL_ENABLE_ROLLBACK                  (1)  /* 0:Disable 1:Enable */

/**********************************************************************************************************************
Typedef definitions
**********************************************************************************************************************/
/* Flash Buffer */
typedef struct st_bl_buf
{
    uint8_t  buf[BL_FLASH_BUF_SIZE+8];
    uint32_t cnt;
    uint32_t total;
} st_flash_buf_t;

/**********************************************************************************************************************
Public Functions
**********************************************************************************************************************/

#endif /* SAMPLE_BOOT_LOADER_MAIN_H_ */
