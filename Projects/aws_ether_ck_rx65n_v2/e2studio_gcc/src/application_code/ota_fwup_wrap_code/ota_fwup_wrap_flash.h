/*
* Copyright (c) 2024-2025 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/
/**********************************************************************************************************************
 * File Name    : ota_fwup_wrap_flash.h
 * Description  : User-defined functions for the Firmware update module.
 **********************************************************************************************************************
 * History : DD.MM.YYYY Version Description
 *         : 09.04.2023 2.00    First Release
 *********************************************************************************************************************/

/**********************************************************************************************************************
 Includes   <System Includes> , "Project Includes"
 *********************************************************************************************************************/

#ifndef APPLICATION_CODE_OTA_FWUP_WRAP_CODE_OTA_FWUP_WRAP_FLASH_H_
#define APPLICATION_CODE_OTA_FWUP_WRAP_CODE_OTA_FWUP_WRAP_FLASH_H_

#include "FreeRTOS.h"
#include "r_fwup_if.h"
#include "r_flash_rx_if.h"
#include "r_common_api_flash.h"
#include "demo_config.h"
#include "r_fwup_wrap_com.h"

/* User-defined wrapper functions */
e_fwup_err_t ota_flash_open_function(void);
void ota_flash_close_function(void);
e_fwup_err_t ota_flash_erase_function(uint32_t addr, uint32_t num_blocks);
e_fwup_err_t ota_flash_write_function(uint32_t src_addr, uint32_t dest_addr, uint32_t num_bytes);
e_fwup_err_t ota_flash_read_function(uint32_t buf_addr, uint32_t src_addr, uint32_t size);
e_fwup_err_t ota_bank_swap_function(void);

#endif /* APPLICATION_CODE_OTA_FWUP_WRAP_CODE_OTA_FWUP_WRAP_FLASH_H_ */