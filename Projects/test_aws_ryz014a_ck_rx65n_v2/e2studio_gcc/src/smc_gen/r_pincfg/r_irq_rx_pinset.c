/*
* Copyright (c) 2016 - 2025 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/

/***********************************************************************************************************************
* File Name    : r_irq_rx_pinset.c
* Version      : 1.0.2
* Device(s)    : R5F565NEHxFB
* Tool-Chain   : RXC toolchain
* Description  : Setting of port and mpc registers
***********************************************************************************************************************/

/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "r_irq_rx_pinset.h"
#include "platform.h"

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
/***********************************************************************************************************************
* Function Name: R_ICU_PinSet
* Description  : This function initializes pins for r_irq_rx module
* Arguments    : none
* Return Value : none
***********************************************************************************************************************/

void R_ICU_PinSet()
{
    R_BSP_RegisterProtectDisable(BSP_REG_PROTECT_MPC);
    /* Set IRQ0 pin */
    PORTD.PMR.BYTE &= ~(1U<<0);
    PORTD.PDR.BYTE &= ~(1U<<0);
    MPC.PD0PFS.BYTE = 0x40U;
    /* Set IRQ2 pin */
    PORTD.PMR.BYTE &= ~(1U<<2);
    PORTD.PDR.BYTE &= ~(1U<<2);
    MPC.PD2PFS.BYTE = 0x40U;
    /* Set IRQ6 pin */
    PORTE.PMR.BYTE &= ~(1U<<6);
    PORTE.PDR.BYTE &= ~(1U<<6);
    MPC.PE6PFS.BYTE = 0x40U;
    /* Set IRQ7 pin */
    PORTE.PMR.BYTE &= ~(1U<<7);
    PORTE.PDR.BYTE &= ~(1U<<7);
    MPC.PE7PFS.BYTE = 0x40U;
    /* Set IRQ11 pin */
    PORT0.PMR.BYTE &= ~(1U<<3);
    PORT0.PDR.BYTE &= ~(1U<<3);
    MPC.P03PFS.BYTE = 0x40U;
    /* Set IRQ13 pin */
    PORT4.PMR.BYTE &= ~(1U<<5);
    PORT4.PDR.BYTE &= ~(1U<<5);
    MPC.P45PFS.BYTE = 0x40U;
    /* Set IRQ14 pin */
    PORT4.PMR.BYTE &= ~(1U<<6);
    PORT4.PDR.BYTE &= ~(1U<<6);
    MPC.P46PFS.BYTE = 0x40U;
    /* Set IRQ15 pin */
    PORT6.PMR.BYTE &= ~(1U<<7);
    PORT6.PDR.BYTE &= ~(1U<<7);
    MPC.P67PFS.BYTE = 0x40U;

    R_BSP_RegisterProtectEnable(BSP_REG_PROTECT_MPC);
}

