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