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

    /* Set ET0_LINKSTA pin */
    MPC.PA5PFS.BYTE = 0x11U;
    PORTA.PMR.BYTE |= 0x20U;

    /* Set ET0_MDC pin */
    MPC.P72PFS.BYTE = 0x11U;
    PORT7.PMR.BYTE |= 0x04U;

    /* Set ET0_MDIO pin */
    MPC.P71PFS.BYTE = 0x11U;
    PORT7.PMR.BYTE |= 0x02U;

    /* Set REF50CK0 pin */
    MPC.P76PFS.BYTE = 0x12U;
    PORT7.PMR.BYTE |= 0x40U;

    /* Set RMII0_CRS_DV pin */
    MPC.P83PFS.BYTE = 0x12U;
    PORT8.PMR.BYTE |= 0x08U;

    /* Set RMII0_RXD0 pin */
    MPC.P75PFS.BYTE = 0x12U;
    PORT7.PMR.BYTE |= 0x20U;

    /* Set RMII0_RXD1 pin */
    MPC.P74PFS.BYTE = 0x12U;
    PORT7.PMR.BYTE |= 0x10U;

    /* Set RMII0_RX_ER pin */
    MPC.P77PFS.BYTE = 0x12U;
    PORT7.PMR.BYTE |= 0x80U;

    /* Set RMII0_TXD0 pin */
    MPC.P81PFS.BYTE = 0x12U;
    PORT8.PMR.BYTE |= 0x02U;

    /* Set RMII0_TXD1 pin */
    MPC.P82PFS.BYTE = 0x12U;
    PORT8.PMR.BYTE |= 0x04U;

    /* Set RMII0_TXD_EN pin */
    MPC.PB4PFS.BYTE = 0x12U;
    PORTB.PMR.BYTE |= 0x10U;

    /* Set RXD5 pin */
    MPC.PC2PFS.BYTE = 0x0AU;
    PORTC.PMR.BYTE |= 0x04U;

    /* Set TXD5 pin */
    PORTC.PODR.BYTE |= 0x08U;
    MPC.PC3PFS.BYTE = 0x0AU;
    PORTC.PDR.BYTE |= 0x08U;
    // PORTC.PMR.BIT.B3 = 1U; // Please set the PMR bit after TE bit is set to 1.

    R_BSP_RegisterProtectEnable(BSP_REG_PROTECT_MPC);
}

