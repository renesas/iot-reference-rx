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
* File Name    : r_irq_rx_pinset.h
* Version      : 1.0.2
* Device(s)    : R5F565NEHxFB
* Tool-Chain   : RXC toolchain
* Description  : Setting of port and mpc registers
***********************************************************************************************************************/

#ifndef R_IRQ_RX_H
#define R_IRQ_RX_H

/***********************************************************************************************************************
Includes
***********************************************************************************************************************/

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/

#define IRQ_PORT_IRQ0_PORT     ('m')
#define IRQ_PORT_IRQ0_BIT      ('n')
#define IRQ_PORT_IRQ1_PORT     ('D')
#define IRQ_PORT_IRQ1_BIT      ('1')
#define IRQ_PORT_IRQ2_PORT     ('D')
#define IRQ_PORT_IRQ2_BIT      ('2')
#define IRQ_PORT_IRQ3_PORT     ('m')
#define IRQ_PORT_IRQ3_BIT      ('n')
#define IRQ_PORT_IRQ4_PORT     ('m')
#define IRQ_PORT_IRQ4_BIT      ('n')
#define IRQ_PORT_IRQ5_PORT     ('m')
#define IRQ_PORT_IRQ5_BIT      ('n')
#define IRQ_PORT_IRQ6_PORT     ('m')
#define IRQ_PORT_IRQ6_BIT      ('n')
#define IRQ_PORT_IRQ7_PORT     ('E')
#define IRQ_PORT_IRQ7_BIT      ('7')
#define IRQ_PORT_IRQ8_PORT     ('m')
#define IRQ_PORT_IRQ8_BIT      ('n')
#define IRQ_PORT_IRQ9_PORT     ('m')
#define IRQ_PORT_IRQ9_BIT      ('n')
#define IRQ_PORT_IRQ10_PORT     ('m')
#define IRQ_PORT_IRQ10_BIT      ('n')
#define IRQ_PORT_IRQ11_PORT     ('m')
#define IRQ_PORT_IRQ11_BIT      ('n')
#define IRQ_PORT_IRQ12_PORT     ('m')
#define IRQ_PORT_IRQ12_BIT      ('n')
#define IRQ_PORT_IRQ13_PORT     ('4')
#define IRQ_PORT_IRQ13_BIT      ('5')
#define IRQ_PORT_IRQ14_PORT     ('4')
#define IRQ_PORT_IRQ14_BIT      ('6')
#define IRQ_PORT_IRQ15_PORT     ('6')
#define IRQ_PORT_IRQ15_BIT      ('7')

void R_ICU_PinSet();

#endif
