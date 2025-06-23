/*
* Copyright (c) 2016 - 2025 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/

/***********************************************************************************************************************
* File Name        : r_cg_hardware_setup.c
* Version          : 1.2.150
* Device(s)        : R5F565NEHxFB
* Description      : Initialization file for code generation configurations.
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
#include "r_smc_cgc.h"
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
* Function Name: r_undefined_exception
* Description  : This function is undefined interrupt service routine
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

#if BSP_CFG_BOOTLOADER_PROJECT == 0
/* Disable the following function in the bootloader project. */
void r_undefined_exception(void)
{
    /* Start user code for r_undefined_exception. Do not edit comment generated here */
    /* End user code. Do not edit comment generated here */
}
#endif /* BSP_CFG_BOOTLOADER_PROJECT == 0 */

/***********************************************************************************************************************
* Function Name: R_Systeminit
* Description  : This function initializes every configuration
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

void R_Systeminit(void)
{
    /* Enable writing to registers related to operating modes, LPC, CGC and software reset */
    SYSTEM.PRCR.WORD = 0xA50BU;

    /* Enable writing to MPC pin function control registers */
    MPC.PWPR.BIT.B0WI = 0U;
    MPC.PWPR.BIT.PFSWE = 1U;

#if BSP_CFG_BOOTLOADER_PROJECT == 0
    /* Disable the following codes in the bootloader project. */
    /* Write 0 to the target bits in the POECR2 registers */
    POE3.POECR2.WORD = 0x0000U;
#endif /* BSP_CFG_BOOTLOADER_PROJECT == 0 */

    /* Initialize clocks settings */
    R_CGC_Create();

    /* Set interrupt settings */
    R_Interrupt_Create();

#if BSP_CFG_BOOTLOADER_PROJECT == 0
    /* Disable the following codes in the bootloader project. */
    /* Register undefined interrupt */
    R_BSP_InterruptWrite(BSP_INT_SRC_UNDEFINED_INTERRUPT,(bsp_int_cb_t)r_undefined_exception);
#endif /* BSP_CFG_BOOTLOADER_PROJECT == 0 */

    /* Disable writing to MPC pin function control registers */
    MPC.PWPR.BIT.PFSWE = 0U;
    MPC.PWPR.BIT.B0WI = 1U;

    /* Enable protection */
    SYSTEM.PRCR.WORD = 0xA500U;
}

/* Start user code for adding. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

