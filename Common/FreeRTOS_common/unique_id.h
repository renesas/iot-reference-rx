/*
* Copyright (c) 2023-2025 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/

/***********************************************************************************************************************
 * File Name    : unique_id.h
 * Description  : This file provides the definition of the unique ID API and data structures.
 **********************************************************************************************************************/

/**********************************************************************************************************************
 Includes   <System Includes> , "Project Includes"
 *********************************************************************************************************************/
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
