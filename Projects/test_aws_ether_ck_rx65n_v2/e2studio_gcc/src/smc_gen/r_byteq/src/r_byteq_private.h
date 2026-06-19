/***********************************************************************************************************************
* Copyright (c) 2013 - 2025 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
***********************************************************************************************************************/
/***********************************************************************************************************************
* File Name    : r_private.h
* Description  : Definitions internal to byte queue module 
************************************************************************************************************************
* History : DD.MM.YYYY Version Description   
*         : 24.07.2013 1.0     Initial Release        
*         : 30.09.2015 1.50    Added dependency to BSP
*         : 15.03.2025 2.11    Updated disclaimer.
***********************************************************************************************************************/

#ifndef BYTEQ_PRIVATE_H
#define BYTEQ_PRIVATE_H

/***********************************************************************************************************************
Includes   <System Includes> , "Project Includes"
***********************************************************************************************************************/
#include "platform.h"


/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/


/*****************************************************************************
Typedef definitions
******************************************************************************/

/* QUEUE CONTROL BLOCK */

typedef struct st_byteq_ctrl    // Byte Queue Control Block (for handle)
{
    uint8_t     *buffer;        // pointer to buffer
    uint16_t    size;           // buffer size
    uint16_t    count;          // number data bytes in queue
    uint16_t    in_index;       // index used by Put function to add data
    uint16_t    out_index;      // index used by Get function to remove data
} byteq_ctrl_t;


#endif /* BYTEQ_PRIVATE_H */
