/*
* Copyright (c) 2023-2025 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/

/***********************************************************************************************************************
 * File Name    : r_common_api.h
 * Description  : This header defines common API types and status codes for managing Flash memory operations.
 **********************************************************************************************************************/

#ifndef COMMON_API_R_COMMON_API_H_
#define COMMON_API_R_COMMON_API_H_

/**********************************************************************************************************************
 Typedef definitions
 *********************************************************************************************************************/
typedef enum CommonAPI_Status
{
    COMAPI_STATE_CLOSE = 0u,
    COMAPI_STATE_OPEN,
    COMAPI_STATE_RUNNING
} CommonAPI_Status_t;

typedef enum e_commonapi_err
{
    COMMONAPI_ERR = -1,
    COMMONAPI_SUCCESS = 0,
} e_commonapi_err_t;

#endif /* COMMON_API_R_COMMON_API_H_ */
