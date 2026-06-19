/* Generated configuration header file - do not edit */
/***********************************************************************************************************************
* Copyright (c) 2013 - 2025 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
***********************************************************************************************************************/
/***********************************************************************************************************************
* File Name     : r_byteq_config.h
* Description   : Configures the byte queue memory allocation
************************************************************************************************************************
* History : DD.MM.YYYY Version Description  
*         : 24.07.2013 1.00    Initial Release
*         : 11.21.2014 1.20    Removed dependency to BSP
*         : 30.09.2015 1.50    Added dependency to BSP
*         : 01.06.2018 1.70    Changed the default value of the following macro definition.
*                               - BYTEQ_CFG_MAX_CTRL_BLKS - Changed the default value from 4 to 32.
*         : 31.03.2021 1.90    Updated for queue protection.
*         : 29.10.2021 2.00    Updated for critical section protection.
*         : 15.03.2025 2.11    Updated disclaimer.
***********************************************************************************************************************/
#ifndef BYTEQ_CONFIG_H
#define BYTEQ_CONFIG_H

/***********************************************************************************************************************
Includes   <System Includes> , "Project Includes"
***********************************************************************************************************************/
#include "platform.h"

/***********************************************************************************************************************
Configuration Options
***********************************************************************************************************************/

/* SPECIFY WHETHER TO INCLUDE CODE FOR API PARAMETER CHECKING
   Available settings:
   BSP_CFG_PARAM_CHECKING_ENABLE:
       Utilizes the system default setting
   1:
       Includes parameter checking
   0:
       Compiles out parameter checking
*/
#define BYTEQ_CFG_PARAM_CHECKING_ENABLE     (BSP_CFG_PARAM_CHECKING_ENABLE)

/* SPECIFY IF SHOULD USE MALLOC() TO ALLOCATE MEMORY FOR QUEUE CONTROL BLOCKS */
#define BYTEQ_CFG_USE_HEAP_FOR_CTRL_BLKS    (0)

/* SPECIFY NUMBER OF STATIC QUEUE CONTROL BLOCKS TO SUPPORT */
/* valid only when BYTEQ_USE_HEAP_FOR_CTRL_BLKS is set to 0 */
#define BYTEQ_CFG_MAX_CTRL_BLKS             (32)

/* Selects to use disable interrupt to protect queue. */
#define BYTEQ_CFG_PROTECT_QUEUE             (0)

/* Selects to use disable interrupt to protect critical section. */
#define BYTEQ_CFG_CRITICAL_SECTION          (0)

#endif /* BYTEQ_CONFIG_H */
