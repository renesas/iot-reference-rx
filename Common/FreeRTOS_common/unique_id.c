/*
* Copyright (c) 2023-2025 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/

/***********************************************************************************************************************
 * File Name    : unique_id.c
 * Description  : This file provides the implementation for retrieving the unique device ID.
 **********************************************************************************************************************/

/**********************************************************************************************************************
 Includes   <System Includes> , "Project Includes"
 *********************************************************************************************************************/
#include "platform.h"
#include "unique_id.h"

/**********************************************************************************************************************
 * Function Name: get_unique_id
 * Description  : .
 * Argument     : uuid
 * Return Value : .
 *********************************************************************************************************************/
bool get_unique_id(uuid_param_t * uuid)
{
    uuid->uuid0 = FLASHCONST.UIDR0;
    uuid->uuid1 = FLASHCONST.UIDR1;
    uuid->uuid2 = FLASHCONST.UIDR2;
    uuid->uuid3 = FLASHCONST.UIDR3;

    return true;
}
/*****************************************************************************************
End of function get_unique_id
****************************************************************************************/
