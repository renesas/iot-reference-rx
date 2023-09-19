/*
 * r_common_api.h v1.0.0
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
