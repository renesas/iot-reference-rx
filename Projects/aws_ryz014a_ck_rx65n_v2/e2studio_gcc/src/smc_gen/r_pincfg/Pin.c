/*
* Copyright (c) 2016 - 2025 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/

/***********************************************************************************************************************
* File Name        : Pin.c
* Version          : 1.0.2
* Device(s)        : R5F565NEHxFB
* Description      : This file implements SMC pin code generation.
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
/* Start user code for include. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
/* Start user code for global. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
* Function Name: R_Pins_Create
* Description  : This function initializes Smart Configurator pins
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

void R_Pins_Create(void)
{
    R_BSP_RegisterProtectDisable(BSP_REG_PROTECT_MPC);

    /* Set AN115 pin */
    PORT9.PMR.BYTE &= 0xFDU;
    PORT9.PDR.BYTE &= 0xFDU;
    MPC.P91PFS.BYTE = 0x80U;

    /* Set AN117 pin */
    PORT9.PMR.BYTE &= 0xF7U;
    PORT9.PDR.BYTE &= 0xF7U;
    MPC.P93PFS.BYTE = 0x80U;

    /* Set CTS6# pin */
    MPC.PJ3PFS.BYTE = 0x0AU;
    PORTJ.PMR.BYTE |= 0x08U;

    /* Set IRQ0 pin */
    MPC.PD0PFS.BYTE = 0x40U;
    PORTD.PMR.BYTE &= 0xFEU;
    PORTD.PDR.BYTE &= 0xFEU;

    /* Set IRQ2 pin */
    MPC.PD2PFS.BYTE = 0x40U;
    PORTD.PMR.BYTE &= 0xFBU;
    PORTD.PDR.BYTE &= 0xFBU;

    /* Set IRQ6 pin */
    MPC.PE6PFS.BYTE = 0x40U;
    PORTE.PMR.BYTE &= 0xBFU;
    PORTE.PDR.BYTE &= 0xBFU;

    /* Set IRQ7 pin */
    MPC.PE7PFS.BYTE = 0x40U;
    PORTE.PMR.BYTE &= 0x7FU;
    PORTE.PDR.BYTE &= 0x7FU;

    /* Set IRQ11 pin */
    MPC.P03PFS.BYTE = 0x40U;
    PORT0.PMR.BYTE &= 0xF7U;
    PORT0.PDR.BYTE &= 0xF7U;

    /* Set IRQ13 pin */
    MPC.P45PFS.BYTE = 0x40U;
    PORT4.PMR.BYTE &= 0xDFU;
    PORT4.PDR.BYTE &= 0xDFU;

    /* Set IRQ14 pin */
    MPC.P46PFS.BYTE = 0x40U;
    PORT4.PMR.BYTE &= 0xBFU;
    PORT4.PDR.BYTE &= 0xBFU;

    /* Set IRQ15 pin */
    MPC.P67PFS.BYTE = 0x40U;
    PORT6.PMR.BYTE &= 0x7FU;
    PORT6.PDR.BYTE &= 0x7FU;

    /* Set RXD5 pin */
    MPC.PC2PFS.BYTE = 0x0AU;
    PORTC.PMR.BYTE |= 0x04U;

    /* Set RXD6 pin */
    MPC.P01PFS.BYTE = 0x0AU;
    PORT0.PMR.BYTE |= 0x02U;

    /* Set TXD5 pin */
    PORTC.PODR.BYTE |= 0x08U;
    MPC.PC3PFS.BYTE = 0x0AU;
    PORTC.PDR.BYTE |= 0x08U;
    // PORTC.PMR.BIT.B3 = 1U; // Please set the PMR bit after TE bit is set to 1.

    /* Set TXD6 pin */
    PORT0.PODR.BYTE |= 0x01U;
    MPC.P00PFS.BYTE = 0x0AU;
    PORT0.PDR.BYTE |= 0x01U;
    // PORT0.PMR.BIT.B0 = 1U; // Please set the PMR bit after TE bit is set to 1.

    R_BSP_RegisterProtectEnable(BSP_REG_PROTECT_MPC);
}

