/* Generated configuration header file - do not edit */
/***********************************************************************************************************************
* Copyright (c) 2013 - 2025 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
***********************************************************************************************************************/
/***********************************************************************************************************************
* File Name    : r_irq_rx_config.h
* Device(s)    : Renesas RX Family
* Tool-Chain   : Renesas RX Standard Toolchain
* H/W Platform :
* Description  : User configurable options and setup definitions for the RX IRQ support module.
***********************************************************************************************************************/
/***********************************************************************************************************************
* History : DD.MM.YYYY  Version Description
*         : 15.07.2013  1.00    First Release
*         : 04.04.2014  1.20    Using the definition of VECT_ICU_IRQ15 to check the number of IRQs.
*         : 30.09.2015  1.70    Using the definition of VECT_ICU_IRQ7  to check the number of IRQs.
*         : 28.09.2018  2.30    Updated the coding rules of Renesas.
*         : 15.08.2022  4.30    Fixed to comply with GSCE Coding Standards Rev.6.5.0.
*         : 29.05.2023  4.40    Deleted the description of FIT configurator.
*         : 31.12.2024  4.70    Added support Nested Interrupt.
*         : 15.03.2025  4.71    Updated disclaimer.
***********************************************************************************************************************/
#ifndef IRQ_CONFIG_H
#define IRQ_CONFIG_H
/***********************************************************************************************************************
Includes   <System Includes> , "Project Includes"
***********************************************************************************************************************/
#include "platform.h"
#include "r_irq_rx_if.h"

/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/

/* Set this value to 1 to enable locking functions for the IRQ APIs, otherwise set to 0 to ignore locking.
 * If locking is enabled, the application must obtain a lock on the selected IRQ before making calls to the API
 * for that IRQ. If locking is disabled, any caller has unrestricted access to any IRQ function. Disabling is
 * provided for systems that require faster and smaller code, however care must be taken to prevent reentrancy or
 * conflict over resource usage.
 */
#define IRQ_CFG_REQUIRE_LOCK    (1)

/* Checking of arguments passed to IRQ API functions can be enable or disabled.
 * Disabling argument checking is provided for systems that absolutely require faster and smaller code.
 * By default the module is configured to use the setting of the system-wide BSP_CFG_PARAM_CHECKING_ENABLE macro.
 * This can be overridden for the local module by redefining IRQ_CFG_PARAM_CHECKING.
 * To control parameter checking locally, set IRQ_CFG_PARAM_CHECKING to 1 to enable it, otherwise set to 0 skip checking.
 *
 * Use this option with caution.
 */

#define IRQ_CFG_PARAM_CHECKING  (BSP_CFG_PARAM_CHECKING_ENABLE)


/* *********************************************************************************************************************
 * IRQ enabling and port assignments
 *
 * If you are using Smart Configurator for pin assignment, use the following:
 *   #include "r_irq_rx_pinset.h"
 *   #ifndef R_IRQ_RX_H
 *   #error "Please check IRQ port assignments in Smart Configurator"
 *   #endif
 * and comment out the following:
 *   #define IRQ_PORT_IRQ0_PORT     ('m') 
 *   #define IRQ_PORT_IRQ0_BIT      ('n')
 *          ...
 *   #define IRQ_PORT_IRQ15_PORT     ('m') 
 *   #define IRQ_PORT_IRQ15_BIT      ('n')
 * --------------------------------------------------------------------------------------------
 * Use these defines to select IRQs that are to be used in your system.
 * This will eliminate the generation of code for unused IRQs.
 *
 * IRQs intended for use must have an I/O port and bit assigned. Use these definitions to make the assignments.
 * These are decoded by the API to map to the port input data registers.
 * Set these as required according to the following format:
 * #define IRQ_PORT_IRQ0_PORT     ('m') where m is the port number and the IRQ number replaces *
 * ##define IRQ_PORT_IRQ0_BIT     ('n') where n is the bit number and the IRQ number replaces *
 * Port assignments here must match the port configuration settings performed externally for them.
***********************************************************************************************************************/
#include "r_irq_rx_pinset.h"
#ifndef R_IRQ_RX_H
#error "Please check IRQ port assignments in Smart Configurator"
#endif

/***********************************************************************************************************************
Digital Filtering
***********************************************************************************************************************/
/* To enable digital noise filtering with the selected IRQ.
 * Set the value to 1 to enable the filter or 0 to disable it. */
#define IRQ_CFG_FILT_EN_IRQ0       (0)   /* Filtering disabled on IRQ 0. */
#define IRQ_CFG_FILT_EN_IRQ1       (0)   /* Filtering disabled on IRQ 1. */
#define IRQ_CFG_FILT_EN_IRQ2       (0)   /* Filtering disabled on IRQ 2. */
#define IRQ_CFG_FILT_EN_IRQ3       (0)   /* Filtering disabled on IRQ 3. */
#define IRQ_CFG_FILT_EN_IRQ4       (0)   /* Filtering disabled on IRQ 4. */
#define IRQ_CFG_FILT_EN_IRQ5       (0)   /* Filtering disabled on IRQ 5. */
#define IRQ_CFG_FILT_EN_IRQ6       (0)   /* Filtering disabled on IRQ 6. */
#define IRQ_CFG_FILT_EN_IRQ7       (0)   /* Filtering disabled on IRQ 7. */
#define IRQ_CFG_FILT_EN_IRQ8       (0)   /* Filtering disabled on IRQ 8. */
#define IRQ_CFG_FILT_EN_IRQ9       (0)   /* Filtering disabled on IRQ 9. */
#define IRQ_CFG_FILT_EN_IRQ10      (0)   /* Filtering disabled on IRQ 10. */
#define IRQ_CFG_FILT_EN_IRQ11      (0)   /* Filtering disabled on IRQ 11. */
#define IRQ_CFG_FILT_EN_IRQ12      (0)   /* Filtering disabled on IRQ 12. */
#define IRQ_CFG_FILT_EN_IRQ13      (0)   /* Filtering disabled on IRQ 13. */
#define IRQ_CFG_FILT_EN_IRQ14      (0)   /* Filtering disabled on IRQ 14. */
#define IRQ_CFG_FILT_EN_IRQ15      (0)   /* Filtering disabled on IRQ 15. */

/* Example settings for IRQ input pin digital filtering sample clock divisors for each IRQ.
 * Change as required by selecting a divisor value from the IRQ_CFG_PCLK_DIVxx definitions.
 * Filtering must be enabled for the corresponding IRQ for these settings to be effective.  */
#define IRQ_CFG_FILT_PLCK_IRQ0    (IRQ_CFG_PCLK_DIV64)  /* Filter sample clock divisor for IRQ 0 = PCLK/64. */
#define IRQ_CFG_FILT_PLCK_IRQ1    (IRQ_CFG_PCLK_DIV64)  /* Filter sample clock divisor for IRQ 1 = PCLK/64. */
#define IRQ_CFG_FILT_PLCK_IRQ2    (IRQ_CFG_PCLK_DIV64)  /* Filter sample clock divisor for IRQ 2 = PCLK/64. */
#define IRQ_CFG_FILT_PLCK_IRQ3    (IRQ_CFG_PCLK_DIV1)   /* Filter sample clock divisor for IRQ 3 = PCLK. */
#define IRQ_CFG_FILT_PLCK_IRQ4    (IRQ_CFG_PCLK_DIV1)   /* Filter sample clock divisor for IRQ 4 = PCLK. */
#define IRQ_CFG_FILT_PLCK_IRQ5    (IRQ_CFG_PCLK_DIV1)   /* Filter sample clock divisor for IRQ 5 = PCLK. */
#define IRQ_CFG_FILT_PLCK_IRQ6    (IRQ_CFG_PCLK_DIV1)   /* Filter sample clock divisor for IRQ 6 = PCLK. */
#define IRQ_CFG_FILT_PLCK_IRQ7    (IRQ_CFG_PCLK_DIV1)   /* Filter sample clock divisor for IRQ 7 = PCLK. */
#define IRQ_CFG_FILT_PLCK_IRQ8    (IRQ_CFG_PCLK_DIV1)   /* Filter sample clock divisor for IRQ 8 = PCLK. */
#define IRQ_CFG_FILT_PLCK_IRQ9    (IRQ_CFG_PCLK_DIV1)   /* Filter sample clock divisor for IRQ 9 = PCLK. */
#define IRQ_CFG_FILT_PLCK_IRQ10   (IRQ_CFG_PCLK_DIV64)  /* Filter sample clock divisor for IRQ 10 = PCLK/64. */
#define IRQ_CFG_FILT_PLCK_IRQ11   (IRQ_CFG_PCLK_DIV1)   /* Filter sample clock divisor for IRQ 11 = PCLK. */
#define IRQ_CFG_FILT_PLCK_IRQ12   (IRQ_CFG_PCLK_DIV1)   /* Filter sample clock divisor for IRQ 12 = PCLK. */
#define IRQ_CFG_FILT_PLCK_IRQ13   (IRQ_CFG_PCLK_DIV1)   /* Filter sample clock divisor for IRQ 13 = PCLK. */
#define IRQ_CFG_FILT_PLCK_IRQ14   (IRQ_CFG_PCLK_DIV1)   /* Filter sample clock divisor for IRQ 14 = PCLK. */
#define IRQ_CFG_FILT_PLCK_IRQ15   (IRQ_CFG_PCLK_DIV1)   /* Filter sample clock divisor for IRQ 15 = PCLK. */

/* To enable nested interrupt with the selected IRQ.
 * Set the value to 1 to enable the nested interrupt or 0 to disable it. */
#define IRQ_CFG_NESTED_INT_EN_IRQ0       (0)   /* Nested interrupt disabled on IRQ 0. */
#define IRQ_CFG_NESTED_INT_EN_IRQ1       (0)   /* Nested interrupt disabled on IRQ 1. */
#define IRQ_CFG_NESTED_INT_EN_IRQ2       (0)   /* Nested interrupt disabled on IRQ 2. */
#define IRQ_CFG_NESTED_INT_EN_IRQ3       (0)   /* Nested interrupt disabled on IRQ 3. */
#define IRQ_CFG_NESTED_INT_EN_IRQ4       (0)   /* Nested interrupt disabled on IRQ 4. */
#define IRQ_CFG_NESTED_INT_EN_IRQ5       (0)   /* Nested interrupt disabled on IRQ 5. */
#define IRQ_CFG_NESTED_INT_EN_IRQ6       (0)   /* Nested interrupt disabled on IRQ 6. */
#define IRQ_CFG_NESTED_INT_EN_IRQ7       (0)   /* Nested interrupt disabled on IRQ 7. */
#define IRQ_CFG_NESTED_INT_EN_IRQ8       (0)   /* Nested interrupt disabled on IRQ 8. */
#define IRQ_CFG_NESTED_INT_EN_IRQ9       (0)   /* Nested interrupt disabled on IRQ 9. */
#define IRQ_CFG_NESTED_INT_EN_IRQ10      (0)   /* Nested interrupt disabled on IRQ 10. */
#define IRQ_CFG_NESTED_INT_EN_IRQ11      (0)   /* Nested interrupt disabled on IRQ 11. */
#define IRQ_CFG_NESTED_INT_EN_IRQ12      (0)   /* Nested interrupt disabled on IRQ 12. */
#define IRQ_CFG_NESTED_INT_EN_IRQ13      (0)   /* Nested interrupt disabled on IRQ 13. */
#define IRQ_CFG_NESTED_INT_EN_IRQ14      (0)   /* Nested interrupt disabled on IRQ 14. */
#define IRQ_CFG_NESTED_INT_EN_IRQ15      (0)   /* Nested interrupt disabled on IRQ 15. */

/***********************************************************************************************************************
Typedef definitions
***********************************************************************************************************************/


#endif /* IRQ_CONFIG_H */
