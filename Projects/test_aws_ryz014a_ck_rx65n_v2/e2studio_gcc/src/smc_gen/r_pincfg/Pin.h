/*
* Copyright (c) 2016 - 2025 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/

/***********************************************************************************************************************
* File Name        : Pin.h
* Version          : 1.0.2
* Device(s)        : R5F565NEHxFB
* Description      : This file implements SMC pin code generation.
***********************************************************************************************************************/

#ifndef PIN_H
#define PIN_H

/***********************************************************************************************************************
Macro definitions (Register bit)
***********************************************************************************************************************/

/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/

/* User's guide for symbolic name.
 * The generated symbolic names can be used in the user application as follows: 
 *
 * Example: Toggle LED1 at Pin P54.
 *  There are 2 ways to toggle LED1
 *   1) Using symbolic name macro
 *    Assuming the symbolic name for P54 is "LED1", the generated macro definition will be:
 *         #define LED1    5,4
 *
 *    To use this macro defintion to toggle the LED1, call the symbolic name APIs:
 *         PIN_WRITE(LED1) = ~PIN_READ(LED1)
 *
 *   2) Not using symbolic name macro
 *    Call the symbolic name APIs directly
 *         PIN_WRITE(5,4) = ~PIN_READ(5,4)
 */

/* Pin write helper */
#define PIN_WRITE_HELPER(x,y)                    ((PORT##x.PODR.BIT.B##y))
/* Pin read helper */
#define PIN_READ_HELPER(x,y)                     ((PORT##x.PIDR.BIT.B##y))

#if !(defined(__CCRX__) && defined(__cplusplus))
/* Pin write API */
#define PIN_WRITE(...)                           (PIN_WRITE_HELPER(__VA_ARGS__))
/* Pin read API */
#define PIN_READ(...)                            (PIN_READ_HELPER(__VA_ARGS__))
#else
/* CC-RX' C++ mode does not support variadic macros */
/* Pin write API */
#define PIN_WRITE(args)                           (PIN_WRITE_HELPER(args))
/* Pin read API */
#define PIN_READ(args)                            (PIN_READ_HELPER(args))
#endif

/***********************************************************************************************************************
Typedef definitions
***********************************************************************************************************************/

/***********************************************************************************************************************
Global functions
***********************************************************************************************************************/
void R_Pins_Create(void);
/* Start user code for function. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
#endif
