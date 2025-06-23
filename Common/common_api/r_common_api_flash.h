/*
* Copyright (c) 2023-2025 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/

/***********************************************************************************************************************
 * File Name    : r_common_api_flash.h
 * Description  : This header file declares the common API functions and definitions for managing Flash memory operations.
 **********************************************************************************************************************/

/**********************************************************************************************************************
 Includes   <System Includes> , "Project Includes"
 *********************************************************************************************************************/
#include "FreeRTOS.h"
#include "queue.h"
#include "semphr.h"
#include "r_common_api.h"
#include "r_flash_rx_if.h"

#ifndef COMMON_API_R_COMMON_API_FLASH_H_
#define COMMON_API_R_COMMON_API_FLASH_H_

#define DATA_FLASH_UPDATE_STATE_INITIALIZE          (0)
#define DATA_FLASH_UPDATE_STATE_ERASE               (1)
#define DATA_FLASH_UPDATE_STATE_ERASE_WAIT_COMPLETE (2)
#define DATA_FLASH_UPDATE_STATE_WRITE               (3)
#define DATA_FLASH_UPDATE_STATE_WRITE_WAIT_COMPLETE (4)
#define DATA_FLASH_UPDATE_STATE_FINALIZE            (5)
#define DATA_FLASH_UPDATE_STATE_FINALIZE_COMPLETED  (6)
#define DATA_FLASH_UPDATE_STATE_ERROR               (103)
#define DATA_FLASH_UPDATE_STATE_UNINITIALIZE        (0xFF)

typedef struct _update_data_flash_control_block
{
    uint32_t status;
} UPDATA_DATA_FLASH_CONTROL_BLOCK;

/* Resources for FLASH Libraries */
extern xSemaphoreHandle xSemaphoreFlashAccess;

/* Function Name: R_Demo_Common_API_Flash_Open */
/******************************************************************************************************************//**
 * @brief CommonAPI open function for Flash.
 * @param[in] void
 * @return e_commonapi_err_t
 * @retval COMMONAPI_SUCCESS        success
 * @retval COMMONAPI_ERR            fail
 *********************************************************************************************************************/
e_commonapi_err_t R_Demo_Common_API_Flash_Open (void);

/* Function Name: R_Demo_Common_API_Flash_Close */
/******************************************************************************************************************//**
 * @brief CommonAPI close function for Flash.
 * @param[in] void
 * @return e_commonapi_err_t
 * @retval COMMONAPI_SUCCESS        success
 * @retval COMMONAPI_ERR            fail
 *********************************************************************************************************************/
e_commonapi_err_t R_Demo_Common_API_Flash_Close (void);

/* Function Name: flashing_callback */
/******************************************************************************************************************//**
 * @brief Callback function which is called from Renesas API's interrupt service routine.
 * @param[in] void *pvArgs
 * @return void
 *********************************************************************************************************************/
void flashing_callback (void *pvArgs);

#endif /* COMMON_API_R_COMMON_API_FLASH_H_ */
