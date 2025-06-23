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

/***********************************************************************************************************************
* Function Name: R_SCI_PinSet_SCI6
* Description  : This function initializes pins for r_sci_rx module
* Arguments    : none
* Return Value : none
***********************************************************************************************************************/
void R_SCI_PinSet_SCI6()
{
    R_BSP_RegisterProtectDisable(BSP_REG_PROTECT_MPC);

    /* Set RXD6/SMISO6/SSCL6 pin */
    MPC.P01PFS.BYTE = 0x0AU;
    PORT0.PMR.BIT.B1 = 1U;

    /* Set TXD6/SMOSI6/SSDA6 pin */
    MPC.P00PFS.BYTE = 0x0AU;
    PORT0.PMR.BIT.B0 = 1U;

    /* Set CTS6#/RTS6#/SS6# pin */
    MPC.PJ3PFS.BYTE = 0x0AU;
    PORTJ.PMR.BIT.B3 = 1U;

    R_BSP_RegisterProtectEnable(BSP_REG_PROTECT_MPC);
}

