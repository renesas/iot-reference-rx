/*
* Copyright (c) 2016 - 2025 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/

/***********************************************************************************************************************
* File Name        : r_cg_macrodriver.h
* Version          : 1.2.150
* Device(s)        : R5F565NEHxFB
* Description      : Macro header file for code generation.
***********************************************************************************************************************/

#ifndef MACRODRIVER_H
#define MACRODRIVER_H

/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "platform.h"
#include "r_smc_interrupt.h"
#include <machine.h>

/***********************************************************************************************************************
Macro definitions (Register bit)
***********************************************************************************************************************/

/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/
#ifndef __TYPEDEF_CG__

/* Status list definition */
#define MD_STATUSBASE        (0x00U)
#define MD_OK                (MD_STATUSBASE + 0x00U) /* register setting OK */
#define MD_SPT               (MD_STATUSBASE + 0x01U) /* IIC stop */
#define MD_NACK              (MD_STATUSBASE + 0x02U) /* IIC no ACK */
#define MD_BUSY1             (MD_STATUSBASE + 0x03U) /* busy 1 */
#define MD_BUSY2             (MD_STATUSBASE + 0x04U) /* busy 2 */

/* Error list definition */
#define MD_ERRORBASE         (0x80U)
#define MD_ERROR             (MD_ERRORBASE + 0x00U)  /* error */
#define MD_ARGERROR          (MD_ERRORBASE + 0x01U)  /* error argument input error */
#define MD_ERROR1            (MD_ERRORBASE + 0x02U)  /* error 1 */
#define MD_ERROR2            (MD_ERRORBASE + 0x03U)  /* error 2 */
#define MD_ERROR3            (MD_ERRORBASE + 0x04U)  /* error 3 */
#define MD_ERROR4            (MD_ERRORBASE + 0x05U)  /* error 4 */
#define MD_ERROR5            (MD_ERRORBASE + 0x06U)  /* error 5 */

#endif

/***********************************************************************************************************************
Typedef definitions
***********************************************************************************************************************/
#ifndef __TYPEDEF_CG__
    typedef unsigned short      MD_STATUS;
    #define __TYPEDEF_CG__
#endif

/***********************************************************************************************************************
Global functions
***********************************************************************************************************************/
void R_Systeminit(void);
/* Start user code for function. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
#endif

