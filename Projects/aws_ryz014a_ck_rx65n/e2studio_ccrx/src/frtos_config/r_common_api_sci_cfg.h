/*
 * r_common_api_sci_cfg.h v1.0.0
 * Copyright (C) 2023 Renesas Electronics Corporation. or its affiliates.
 * All rights reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * http://www.renesas.com
 */
/**********************************************************************************************************************
 Includes   <System Includes> , "Project Includes"
 *********************************************************************************************************************/
#include "platform.h"
#include "r_sci_rx_if.h"

#ifndef COMMON_API_R_COMMON_API_CFG_H_
#define COMMON_API_R_COMMON_API_CFG_H_

/**********************************************************************************************************************
 Macro definitions
 *********************************************************************************************************************/
#define USER_SCI_UART_TERMINAL_CHANNEL      5   /* Edit here if you want to switch to another channel */

/* The code below here is a preprocessor macro. Do not change the user. */
#if !defined(USER_SCI_UART_TERMINAL_CHANNEL)
#error "Error! Need to define USER_SCI_UART_TERMINAL_CHANNEL in r_common_api_aci_cfg.h"
#elif USER_SCI_UART_TERMINAL_CHANNEL == (0)
#define U_SCI_UART_CLI_PINSET()  R_SCI_PinSet_SCI0()
#define U_SCI_UART_CLI_SCI_CH          SCI_CH0
#elif USER_SCI_UART_TERMINAL_CHANNEL == (1)
#define U_SCI_UART_CLI_PINSET()  R_SCI_PinSet_SCI1()
#define U_SCI_UART_CLI_SCI_CH          SCI_CH1
#elif USER_SCI_UART_TERMINAL_CHANNEL == (2)
#define U_SCI_UART_CLI_PINSET()  R_SCI_PinSet_SCI2()
#define U_SCI_UART_CLI_SCI_CH          SCI_CH2
#elif USER_SCI_UART_TERMINAL_CHANNEL == (3)
#define U_SCI_UART_CLI_PINSET()  R_SCI_PinSet_SCI3()
#define U_SCI_UART_CLI_SCI_CH          SCI_CH3
#elif USER_SCI_UART_TERMINAL_CHANNEL == (4)
#define U_SCI_UART_CLI_PINSET()  R_SCI_PinSet_SCI4()
#define U_SCI_UART_CLI_SCI_CH          SCI_CH4
#elif USER_SCI_UART_TERMINAL_CHANNEL == (5)
#define U_SCI_UART_CLI_PINSET()  R_SCI_PinSet_SCI5()
#define U_SCI_UART_CLI_SCI_CH          SCI_CH5
#elif USER_SCI_UART_TERMINAL_CHANNEL == (6)
#define U_SCI_UART_CLI_PINSET()  R_SCI_PinSet_SCI6()
#define U_SCI_UART_CLI_SCI_CH          SCI_CH6
#elif USER_SCI_UART_TERMINAL_CHANNEL == (7)
#define U_SCI_UART_CLI_PINSET()  R_SCI_PinSet_SCI7()
#define U_SCI_UART_CLI_SCI_CH          SCI_CH7
#elif USER_SCI_UART_TERMINAL_CHANNEL == (8)
#define U_SCI_UART_CLI_PINSET()  R_SCI_PinSet_SCI8()
#define U_SCI_UART_CLI_SCI_CH          SCI_CH8
#elif USER_SCI_UART_TERMINAL_CHANNEL == (9)
#define U_SCI_UART_CLI_PINSET()  R_SCI_PinSet_SCI9()
#define U_SCI_UART_CLI_SCI_CH          SCI_CH9
#elif USER_SCI_UART_TERMINAL_CHANNEL == (10)
#define U_SCI_UART_CLI_PINSET()  R_SCI_PinSet_SCI10()
#define U_SCI_UART_CLI_SCI_CH          SCI_CH10
#elif USER_SCI_UART_TERMINAL_CHANNEL == (11)
#define U_SCI_UART_CLI_PINSET()  R_SCI_PinSet_SCI11()
#define U_SCI_UART_CLI_SCI_CH          SCI_CH11
#elif USER_SCI_UART_TERMINAL_CHANNEL == (12)
#define U_SCI_UART_CLI_PINSET()  R_SCI_PinSet_SCI12()
#define U_SCI_UART_CLI_SCI_CH          SCI_CH12
#else
#error "Error! Invalid setting for USER_SCI_UART_TERMINAL_CHANNEL in r_common_api_aci_cfg.h"
#endif

/* Macros for calling SCI Open/Close functions */
#define COMMON_API_SCI_OPEN(x)      __COMMON_API_SCI_OPEN(x)
#define __COMMON_API_SCI_OPEN(x)    R_Demo_Common_API_SCI ## x ##_Open()

#define COMMON_API_SCI_CLOSE(x)     __COMMON_API_SCI_CLOSE(x)
#define __COMMON_API_SCI_CLOSE(x)   R_Demo_Common_API_SCI ## x ##_Close()

#endif /* COMMON_API_R_COMMON_API_CFG_H_ */
