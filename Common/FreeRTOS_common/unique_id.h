/*
 * unique_id.h v1.0.0
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

#include "platform.h"

#ifndef FREERTOS_COMMON_UNIQUE_ID_H_
#define FREERTOS_COMMON_UNIQUE_ID_H_

#define UUID_NUMBER_OF_REGISTER     ( 4 )
#define UUID_MAX_SIZE               ( sizeof( uint32_t) * UUID_NUMBER_OF_REGISTER )

typedef struct uuid_param_t
{
    uint32_t uuid0;
    uint32_t uuid1;
    uint32_t uuid2;
    uint32_t uuid3;
} uuid_param_t;

/**********************************************************************************************************************
 * Function Name: get_unique_id
 * Description  : .
 * Argument     : uuid
 * Return Value : .
 *********************************************************************************************************************/
bool get_unique_id (uuid_param_t * uuid);

#endif /* FREERTOS_COMMON_UNIQUE_ID_H_ */
