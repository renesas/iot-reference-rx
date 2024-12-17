/***********************************************************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products.
* No other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all
* applicable laws, including copyright laws. 
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING THIS SOFTWARE, WHETHER EXPRESS, IMPLIED
* OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
* NON-INFRINGEMENT.  ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY
* LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE FOR ANY DIRECT,
* INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR
* ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability 
* of this software. By using this software, you agree to the additional terms and conditions found by accessing the 
* following link:
* http://www.renesas.com/disclaimer
*
* Copyright (C) 2024 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/
/***********************************************************************************************************************
* File Name    : r_ether_rx_pinset.c
* Version      : 1.0.2
* Device(s)    : R5F565NEHxFB
* Tool-Chain   : RXC toolchain
* Description  : Setting of port and mpc registers
***********************************************************************************************************************/

/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "r_ether_rx_pinset.h"
#include "platform.h"

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/

/***********************************************************************************************************************
* Function Name: R_ETHER_PinSet_ETHERC0_RMII
* Description  : This function initializes pins for r_ether_rx module
* Arguments    : none
* Return Value : none
***********************************************************************************************************************/
void R_ETHER_PinSet_ETHERC0_RMII()
{
    R_BSP_RegisterProtectDisable(BSP_REG_PROTECT_MPC);

    /* Set REF50CK0 pin */
    MPC.P76PFS.BYTE = 0x12U;
    PORT7.PMR.BIT.B6 = 1U;

    /* Set RMII0_TXD_EN pin */
    MPC.PB4PFS.BYTE = 0x12U;
    PORTB.PMR.BIT.B4 = 1U;

    /* Set RMII0_TXD1 pin */
    MPC.P82PFS.BYTE = 0x12U;
    PORT8.PMR.BIT.B2 = 1U;

    /* Set RMII0_TXD0 pin */
    MPC.P81PFS.BYTE = 0x12U;
    PORT8.PMR.BIT.B1 = 1U;

    /* Set RMII0_RXD1 pin */
    MPC.P74PFS.BYTE = 0x12U;
    PORT7.PMR.BIT.B4 = 1U;

    /* Set RMII0_RXD0 pin */
    MPC.P75PFS.BYTE = 0x12U;
    PORT7.PMR.BIT.B5 = 1U;

    /* Set RMII0_RX_ER pin */
    MPC.P77PFS.BYTE = 0x12U;
    PORT7.PMR.BIT.B7 = 1U;

    /* Set RMII0_CRS_DV pin */
    MPC.P83PFS.BYTE = 0x12U;
    PORT8.PMR.BIT.B3 = 1U;

    /* Set ET0_MDC pin */
    MPC.P72PFS.BYTE = 0x11U;
    PORT7.PMR.BIT.B2 = 1U;

    /* Set ET0_MDIO pin */
    MPC.P71PFS.BYTE = 0x11U;
    PORT7.PMR.BIT.B1 = 1U;

    /* Set ET0_LINKSTA pin */
    MPC.PA5PFS.BYTE = 0x11U;
    PORTA.PMR.BIT.B5 = 1U;

    R_BSP_RegisterProtectEnable(BSP_REG_PROTECT_MPC);
}

