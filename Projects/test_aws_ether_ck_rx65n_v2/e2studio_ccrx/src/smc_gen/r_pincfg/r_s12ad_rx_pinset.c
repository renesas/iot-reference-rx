/*
* Copyright (c) 2016 - 2025 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/

/***********************************************************************************************************************
* File Name    : r_s12ad_rx_pinset.c
* Version      : 1.0.2
* Device(s)    : R5F565NEHxFB
* Tool-Chain   : RXC toolchain
* Description  : Setting of port and mpc registers
***********************************************************************************************************************/

/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "r_s12ad_rx_pinset.h"
#include "platform.h"

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/

/***********************************************************************************************************************
* Function Name: R_ADC_PinSet_S12AD1
* Description  : This function initializes pins for r_s12ad_rx module
* Arguments    : none
* Return Value : none
***********************************************************************************************************************/
void R_ADC_PinSet_S12AD1()
{
    R_BSP_RegisterProtectDisable(BSP_REG_PROTECT_MPC);

    /* Set AN115 pin */
    PORT9.PCR.BIT.B1 = 0U;
    PORT9.PDR.BIT.B1 = 0U;
    PORT9.PMR.BIT.B1 = 0U;
    MPC.P91PFS.BYTE = 0x80U;

    /* Set AN117 pin */
    PORT9.PCR.BIT.B3 = 0U;
    PORT9.PDR.BIT.B3 = 0U;
    PORT9.PMR.BIT.B3 = 0U;
    MPC.P93PFS.BYTE = 0x80U;

    R_BSP_RegisterProtectEnable(BSP_REG_PROTECT_MPC);
}

