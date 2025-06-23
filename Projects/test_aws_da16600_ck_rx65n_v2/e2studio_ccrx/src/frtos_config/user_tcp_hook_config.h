/*
* Copyright (c) 2023-2025 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/

/***********************************************************************************************************************
* File Name    : user_tcp_hook_config.h
 * Description  : The custom header file for hook function configuration to reset Wi-Fi module when errors occur.
**********************************************************************************************************************/

#ifndef FRTOS_CONFIG_USER_TCP_HOOK_CONFIG_H_
#define FRTOS_CONFIG_USER_TCP_HOOK_CONFIG_H_

/* The limit of number for WIFI_ERR_MODULE_COM when it comes continuously.
 * If counter is over than this limit, hook the reset */
#define USER_COMM_ERROR_TRIES       (3)

/*Connection retry limit*/
#define USER_RECONNECT_TRIES        (3)

/*Socket closing retry limit*/
#define USER_CLOSE_SOCKET_TRIES     (3)

/*Enable SocketErrorHook*/
#define USER_TCP_HOOK_ENABLED       (1) /* Default */

#endif /* FRTOS_CONFIG_USER_TCP_HOOK_CONFIG_H_ */
