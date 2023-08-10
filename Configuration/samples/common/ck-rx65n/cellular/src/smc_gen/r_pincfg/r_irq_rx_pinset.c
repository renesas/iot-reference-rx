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
* Copyright (C) 2023 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/
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

    /* Set IRQ1 pin */
    PORTD.PMR.BIT.B1 = 0U;
    PORTD.PDR.BIT.B1 = 0U;
    MPC.PD1PFS.BYTE = 0x40U;

    /* Set IRQ2 pin */
    PORTD.PMR.BIT.B2 = 0U;
    PORTD.PDR.BIT.B2 = 0U;
    MPC.PD2PFS.BYTE = 0x40U;

    /* Set IRQ7 pin */
    PORTE.PMR.BIT.B7 = 0U;
    PORTE.PDR.BIT.B7 = 0U;
    MPC.PE7PFS.BYTE = 0x40U;

    /* Set IRQ13 pin */
    PORT4.PMR.BIT.B5 = 0U;
    PORT4.PDR.BIT.B5 = 0U;
    MPC.P45PFS.BYTE = 0x40U;

    /* Set IRQ14 pin */
    PORT4.PMR.BIT.B6 = 0U;
    PORT4.PDR.BIT.B6 = 0U;
    MPC.P46PFS.BYTE = 0x40U;

    /* Set IRQ15 pin */
    PORT6.PMR.BIT.B7 = 0U;
    PORT6.PDR.BIT.B7 = 0U;
    MPC.P67PFS.BYTE = 0x40U;

    R_BSP_RegisterProtectEnable(BSP_REG_PROTECT_MPC);
}

