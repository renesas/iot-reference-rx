/*
 * r_common_api_sci.h v1.0.0
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
 *********************************************************************************************************************/
#include "FreeRTOS.h"
#include "queue.h"
#include "semphr.h"
#include "r_common_api_sci_cfg.h"
#include "r_common_api.h"

#ifndef COMMON_API_R_COMMON_API_SCI_H_
#define COMMON_API_R_COMMON_API_SCI_H_

/* Function Name: R_Demo_Common_API_SCI5_Open */
/******************************************************************************************************************//**
 * @brief CommonAPI open function for sci5.
 * @param[in] void
 * @return e_commonapi_err_t
 * @retval COMMONAPI_SUCCESS        success
 * @retval COMMONAPI_ERR            fail
 *********************************************************************************************************************/
e_commonapi_err_t R_Demo_Common_API_SCI5_Open(void);

/* Function Name: R_Demo_Common_API_SCI5_Close */
/******************************************************************************************************************//**
 * @brief CommonAPI close function for sci5.
 * @param[in] void
 * @return e_commonapi_err_t
 * @retval COMMONAPI_SUCCESS        success
 * @retval COMMONAPI_ERR            fail
 *********************************************************************************************************************/
e_commonapi_err_t R_Demo_Common_API_SCI5_Close(void);

/* Function Name: vSerialSciCallback */
/******************************************************************************************************************//**
 * @brief Callback function which is called from Renesas API's interrupt service routine.
 * @param[in] void *pvArgs
 * @return void
 *********************************************************************************************************************/
void vSerialSciCallback( void *pvArgs );

/* Resources for CLI Libraries */
extern QueueHandle_t xRxQueue;

#endif /* COMMON_API_R_COMMON_API_SCI_H_ */
