/*
* Copyright (C) 2016-2025 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/
/**********************************************************************************************************************
* File Name    : r_flash_type4_if.h
* Description  : This module implements equates/declarations specific to Flash Type 4 MCUs
**********************************************************************************************************************/
/**********************************************************************************************************************
* History : DD.MM.YYYY Version Description           
*         : 11.02.2016 1.00    Support for Flash Type 4 MCUs
*         : 12.08.2016 2.00    Modified for BSPless operation.
*         : 17.11.2016 2.10    Added FLASH_FREQ_xx and FLASH_FCU_INT_xxx #defines
*         : 05.10.2016 3.00    Merged functions common to other flash types into r_flash_fcu.c and r_flash_group.c.
*         : 03.02.2017 3.10    Added BANKSEL_ADDR and BANKSWP_MASK for RX65N-2M support.
*         : 26.06.2020 4.60    Added FLASH_ACCESS_WINDOW_END_NEXT_REG_VALUE #define.
*         : 20.03.2025 5.22    Changed the disclaimer in program sources
**********************************************************************************************************************/

#ifndef FLASH_TYPE4_HEADER_FILE
#define FLASH_TYPE4_HEADER_FILE

#include "r_flash_rx_if.h"
#if (FLASH_TYPE == 4)

/***********************************************************************************************************************
Includes   <System Includes> , "Project Includes"
***********************************************************************************************************************/
#include "r_flash_rx.h"

/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/
#define BANKSEL_ADDR            (0xFE7F5D20)
#define BANKSWP_MASK            (0x00000007)

#define FLASH_FREQ_LO           (4000000)
#define FLASH_FREQ_HI           (60000000)

#define FLASH_FCU_INT_ENABLE    FLASH.FAEINT.BYTE = 0x90;   \
                                FLASH.FRDYIE.BYTE = 0x01;
#define FLASH_FCU_INT_DISABLE   FLASH.FAEINT.BYTE = 0x00;   \
                                FLASH.FRDYIE.BYTE = 0x00;

#define FLASH_ACCESS_WINDOW_END_NEXT_REG_VALUE  (0x800)

/***********************************************************************************************************************
Typedef definitions
***********************************************************************************************************************/

/***********************************************************************************************************************
Exported global variables
***********************************************************************************************************************/

/***********************************************************************************************************************
Exported global functions (to be accessed by other files)
***********************************************************************************************************************/
extern flash_err_t get_cmdlk_err(void);
extern flash_interrupt_event_t get_cmdlk_err_event(void);
extern uint8_t R_CF_GetCurrentSwapState(void);
extern void R_CF_SetCurrentSwapState(uint8_t value);
extern uint8_t R_CF_GetCurrentStartupArea(void);
extern flash_err_t R_CF_ToggleStartupArea(void);
extern flash_err_t R_CF_GetAccessWindow(flash_access_window_config_t  *pAccessInfo);
extern flash_err_t R_CF_SetAccessWindow(flash_access_window_config_t  *pAccessInfo);

#ifdef FLASH_HAS_APP_SWAP
extern flash_err_t flash_toggle_banksel_reg(void);
#endif

#endif // FLASH_TYPE == 4
#endif // FLASH_API_IF_FCU_64M_HEADER_FILE


