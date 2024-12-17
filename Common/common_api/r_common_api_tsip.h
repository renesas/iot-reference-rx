/*
 * r_common_api_tsip.h v1.0.0
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
#include "r_common_api.h"

#include "r_tsip_rx_if.h"
#include "r_tsip_rx_config.h"

#ifndef COMMON_API_R_COMMON_API_TSIP_H_
#define COMMON_API_R_COMMON_API_TSIP_H_

/* Resources for TSIP Libraries */
extern xSemaphoreHandle xSemaphoreTSIPAccess;

/* Function Name: R_Demo_Common_API_TSIP_Open */
/******************************************************************************************************************//**
 * @brief CommonAPI open function for TSIP.
 * @param[in] void
 * @return e_commonapi_err_t
 * @retval COMMONAPI_SUCCESS        success
 * @retval COMMONAPI_ERR            fail
 *********************************************************************************************************************/
e_commonapi_err_t R_Demo_Common_API_TSIP_Open (tsip_tls_ca_certification_public_key_index_t *ki1, tsip_update_key_ring_t *ki2);

/* Function Name: R_Demo_Common_API_TSIP_Close */
/******************************************************************************************************************//**
 * @brief CommonAPI close function for TSIP.
 * @param[in] void
 * @return e_commonapi_err_t
 * @retval COMMONAPI_SUCCESS        success
 * @retval COMMONAPI_ERR            fail
 *********************************************************************************************************************/
e_commonapi_err_t R_Demo_Common_API_TSIP_Close (void);

#endif /* COMMON_API_R_COMMON_API_TSIP_H_ */
