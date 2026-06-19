/*
* Copyright (c) 2016 - 2025 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/

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

#define IRQ_PORT_IRQ0_PORT     ('D')
#define IRQ_PORT_IRQ0_BIT      ('0')
#define IRQ_PORT_IRQ1_PORT     ('m')
#define IRQ_PORT_IRQ1_BIT      ('n')
#define IRQ_PORT_IRQ2_PORT     ('D')
#define IRQ_PORT_IRQ2_BIT      ('2')
#define IRQ_PORT_IRQ3_PORT     ('m')
#define IRQ_PORT_IRQ3_BIT      ('n')
#define IRQ_PORT_IRQ4_PORT     ('m')
#define IRQ_PORT_IRQ4_BIT      ('n')
#define IRQ_PORT_IRQ5_PORT     ('m')
#define IRQ_PORT_IRQ5_BIT      ('n')
#define IRQ_PORT_IRQ6_PORT     ('E')
#define IRQ_PORT_IRQ6_BIT      ('6')
#define IRQ_PORT_IRQ7_PORT     ('E')
#define IRQ_PORT_IRQ7_BIT      ('7')
#define IRQ_PORT_IRQ8_PORT     ('m')
#define IRQ_PORT_IRQ8_BIT      ('n')
#define IRQ_PORT_IRQ9_PORT     ('m')
#define IRQ_PORT_IRQ9_BIT      ('n')
#define IRQ_PORT_IRQ10_PORT     ('m')
#define IRQ_PORT_IRQ10_BIT      ('n')
#define IRQ_PORT_IRQ11_PORT     ('0')
#define IRQ_PORT_IRQ11_BIT      ('3')
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
