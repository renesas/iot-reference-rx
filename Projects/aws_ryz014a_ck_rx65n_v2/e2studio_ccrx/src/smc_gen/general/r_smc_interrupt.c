/*
* Copyright (c) 2016 - 2025 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/

/***********************************************************************************************************************
* File Name        : r_smc_interrupt.c
* Version          : 1.2.51
* Device(s)        : R5F565NEHxFB
* Description      : This file implements interrupt setting.
***********************************************************************************************************************/

/***********************************************************************************************************************
Pragma directive
***********************************************************************************************************************/
/* Start user code for pragma. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "r_cg_macrodriver.h"
#include "r_smc_interrupt.h"
/* Start user code for include. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
/* Start user code for global. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
* Function Name: R_Interrupt_Create
* Description  : This function Used to set the fast interrupt or group interrupt 
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

void R_Interrupt_Create(void)
{
    /* Disable group BL0 interrupt*/
    IEN(ICU,GROUPBL0) = 0U;
    

    /* Set group BL0 interrupt priority level */
    IPR(ICU,GROUPBL0) = _03_ICU_PRIORITY_LEVEL3;

    /* Enable group BL0 interrupt */
    IEN(ICU,GROUPBL0) = 1U;
}

/* Start user code for adding. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
