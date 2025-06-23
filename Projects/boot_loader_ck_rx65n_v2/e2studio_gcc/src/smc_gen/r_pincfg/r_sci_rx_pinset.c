/*
* Copyright (c) 2016 - 2025 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/

/***********************************************************************************************************************
* File Name    : r_sci_rx_pinset.c
* Version      : 1.0.2
* Device(s)    : R5F565NEHxFB
* Tool-Chain   : RXC toolchain
* Description  : Setting of port and mpc registers
***********************************************************************************************************************/

/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "r_sci_rx_pinset.h"
#include "platform.h"

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/

/***********************************************************************************************************************
* Function Name: R_SCI_PinSet_SCI5
* Description  : This function initializes pins for r_sci_rx module
* Arguments    : none
* Return Value : none
***********************************************************************************************************************/
void R_SCI_PinSet_SCI5()
{
    R_BSP_RegisterProtectDisable(BSP_REG_PROTECT_MPC);

    /* Set RXD5/SMISO5/SSCL5 pin */
    MPC.PC2PFS.BYTE = 0x0AU;
    PORTC.PMR.BIT.B2 = 1U;

    /* Set TXD5/SMOSI5/SSDA5 pin */
    MPC.PC3PFS.BYTE = 0x0AU;
    PORTC.PMR.BIT.B3 = 1U;

    R_BSP_RegisterProtectEnable(BSP_REG_PROTECT_MPC);
}

