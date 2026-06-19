/*
* Copyright (C) 2016-2025 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/
/***********************************************************************************************************************
* File Name    : r_flash_group.h
* Description  : This is a private header file used internally by the FLASH module. It should not be modified or
*                included by the user in their application.
***********************************************************************************************************************/
/***********************************************************************************************************************
* History : DD.MM.YYYY Version Description
*           05.10.2016 1.00    First Release
*           31.10.2017 1.10    Added function prototype for r_flash_close().
*           19.04.2019 4.00    Removed support for flash type 2.
*           19.07.2019 4.20    Added volatile to g_current_parameters.
*           20.03.2025 5.22    Changed the disclaimer in program sources
***********************************************************************************************************************/

#ifndef RX_FLASH_GROUP_HEADER_FILE
#define RX_FLASH_GROUP_HEADER_FILE

#include "r_flash_rx_if.h"

/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/

/***********************************************************************************************************************
Typedef definitions
***********************************************************************************************************************/

/***********************************************************************************************************************
Exported global variables
***********************************************************************************************************************/
extern void (*flash_ready_isr_handler)(void *);        // Function pointer for Flash Ready ISR
extern void (*flash_error_isr_handler)(void *);        // Function pointer for Flash Error ISR
extern flash_int_cb_args_t g_flash_int_ready_cb_args;  // Callback argument structure for flash READY interrupt
extern flash_int_cb_args_t g_flash_int_error_cb_args;  // Callback argument structure for flash ERROR interrupt
extern volatile current_param_t g_current_parameters;

/***********************************************************************************************************************
Exported global functions (to be accessed by other files)
***********************************************************************************************************************/
extern flash_err_t r_flash_open(void);
extern flash_err_t r_flash_close(void);
extern flash_err_t r_flash_erase(flash_block_address_t block_start_address, uint32_t num_blocks);
extern flash_err_t check_cf_block_total(flash_block_address_t block_start_address, uint32_t num_blocks);
#ifndef FLASH_NO_BLANK_CHECK
extern flash_err_t r_flash_blankcheck(uint32_t address, uint32_t num_bytes, flash_res_t *result);
#endif
extern flash_err_t r_flash_write(uint32_t src_address, uint32_t dest_address, uint32_t num_bytes);
extern flash_err_t r_flash_control(flash_cmd_t cmd,  void  *pcfg);

#endif  // RX_FLASH_GROUP_HEADER_FILE

