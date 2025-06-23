/*
* Copyright (c) 2024-2025 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/

/***********************************************************************************************************************
 * File Name    : r_common_api_tsip.h
 * Description  : This header file provides the API definitions for managing TSIP (Trusted Secure IP) operations.
 **********************************************************************************************************************/

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
