/**********************************************************************************************************************
 * Copyright (C) 2023 Renesas Electronics Corporation. All rights reserved.
 *
 * DISCLAIMER
 * This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products. No
 * other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all
 * applicable laws, including copyright laws.
 * THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
 * THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED. TO THE MAXIMUM
 * EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES
 * SHALL BE LIABLE FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO THIS
 * SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
 * Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability of
 * this software. By using this software, you agree to the additional terms and conditions found by accessing the
 * following link:
 * http://www.renesas.com/disclaimer
 *********************************************************************************************************************/

#ifndef FRTOS_CONFIG_USER_TCP_HOOK_CONFIG_H_
#define FRTOS_CONFIG_USER_TCP_HOOK_CONFIG_H_

/* The limit of number for WIFI_ERR_MODULE_COM when it comes continuously.
 * If counter is over than this limit, hook the reset */
#define USER_COMM_ERROR_TRIES		3

/*Connection retry limit*/
#define USER_RECONNECT_TRIES 		3

/*Socket closing retry limit*/
#define USER_CLOSE_SOCKET_TRIES		3

/*Enable SocketErrorHook*/
#define USER_TCP_HOOK_ENABLED   	1 /* Default */

#endif /* FRTOS_CONFIG_USER_TCP_HOOK_CONFIG_H_ */
