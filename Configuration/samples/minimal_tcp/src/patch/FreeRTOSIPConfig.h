/*
 * FreeRTOS+TCP V4.2.2
 * Copyright (C) 2024 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 * Modifications Copyright (C) 2024-2025 Renesas Electronics Corporation or its affiliates.
 *
 * SPDX-License-Identifier: MIT
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
 * http://aws.amazon.com/freertos
 * http://www.FreeRTOS.org
 */


/*****************************************************************************
*
* See the following URL for configuration information.
* http://www.freertos.org/FreeRTOS-Plus/FreeRTOS_Plus_TCP/TCP_IP_Configuration.html
*
*****************************************************************************/
#include "r_ether_rx_config.h"

#ifndef FREERTOS_IP_CONFIG_H
#define FREERTOS_IP_CONFIG_H

/* Define the byte order of the target MCU (the MCU FreeRTOS+TCP is executing
 * on).  Valid options are pdFREERTOS_BIG_ENDIAN and pdFREERTOS_LITTLE_ENDIAN. */
#define ipconfigBYTE_ORDER                         pdFREERTOS_LITTLE_ENDIAN

/* Set to 1 to print out debug messages. If ipconfigHAS_DEBUG_PRINTF is set to
   1 then FreeRTOS_debug_printf should be defined to the function used to print
   out the debugging messages. */
#define ipconfigHAS_DEBUG_PRINTF    0
#if( ipconfigHAS_DEBUG_PRINTF == 1 )
    #define FreeRTOS_debug_printf( MSG )    configPRINTF( MSG )
#endif

/* Set to 1 to print out application messages. If ipconfigHAS_PRINTF is set to
   1 then FreeRTOS_printf should be defined to the function used to print
out the application messages. */
#define ipconfigHAS_PRINTF  1
#if( ipconfigHAS_PRINTF == 1 )
    #define FreeRTOS_printf( MSG )    configPRINTF( MSG )
#endif

/* 'ipconfigUSE_NETWORK_EVENT_HOOK' indicates if an application hook is available
 * called 'vApplicationIPNetworkEventHook()'.  This function will be called when
 * the network goes up and when it goes down.  See also FREERTOS_IP.h for further
 * explanation. */
#define ipconfigUSE_NETWORK_EVENT_HOOK    1

/* Define the number of entries in the ARP cache table. */
#define ipconfigARP_CACHE_ENTRIES    6

/* 'ipconfigNUM_NETWORK_BUFFER_DESCRIPTORS' is an important macro: it
 * determines the number of network buffers that are available in the
 * entire application.
 * Note that the default of 45 may be pretty high for smaller
 * applications.
 * Also note that when the network interface uses zero-copy reception
 * ( ipconfigZERO_COPY_RX_DRIVER ), it will reserve a set of network
 * buffers permanently.
 * For zero-copy transmission, no network buffers are permanently
 * "reserved" for transmission.
 */
#define ipconfigNUM_NETWORK_BUFFER_DESCRIPTORS    ETHER_CFG_EMAC_TX_DESCRIPTORS

/* Related to the macro 'ipconfigEVENT_QUEUE_LENGTH' here above:
 * when developing a new networking application, it can be helpful
 * to monitor the length of the message queue of the IP-task.
 * This code is only enabled when 'ipconfigCHECK_IP_QUEUE_SPACE'
 * is set to 1.  See also the function 'uxGetMinimumIPQueueSpace()'.
 */
#define ipconfigCHECK_IP_QUEUE_SPACE    1

/* Set ipconfigUSE_LLMNR to 1 to include LLMNR. */
#define ipconfigUSE_LLMNR    0

/* Set ipconfigUSE_NBNS to 1 to include NBNS. */
#define ipconfigUSE_NBNS    0

/* DHCP servers have a table with information about each clients.  One
 * of the fields in this table contains the host name of the DHCP clients.
 * When 'ipconfigDHCP_REGISTER_HOSTNAME' is defined as non-zero, the DHCP
 * driver will call 'pcApplicationHostnameHook()' to obtain the name of
 * the embedded device.
 */
#define ipconfigDHCP_REGISTER_HOSTNAME    1

/* The results of DNS lookup's can be stored in a cache table. */
#define ipconfigUSE_DNS_CACHE    1

/* When ipconfigDRIVER_INCLUDED_RX_IP_CHECKSUM is enabled,
 * the network interface is responsible for checking the checksums
 * of the incoming packets.
 * This can be either done in hardware, or by calling the checksum
 * functions.
 */
#define ipconfigDRIVER_INCLUDED_RX_IP_CHECKSUM    1

/* The macro 'ipconfigSOCKET_HAS_USER_WAKE_CALLBACK' allows to use a call-back
 * function that will be called at the moment one of the above events occurs.
 * Use the socket option 'FREERTOS_SO_WAKEUP_CALLBACK' to install a function
 * of the type 'void callback( Socket_t pxSocket )'.
 * Note that the call-back function runs in the IP-task, so very little things
 * can be done.  Better not to call any networking API, because that could
 * easily lead to a deadlock situation.
 */
#define ipconfigSOCKET_HAS_USER_WAKE_CALLBACK    1

/* TCP only: if the 'ipconfigTCP_KEEP_ALIVE' macro is defined as 1,
 * sockets in state "ESTABLISHED" can be protected using keep-alive packets.
 * These packets will be sent as soon as there hasn't been any activity
 * for a while.
 * The macro 'ipconfigTCP_KEEP_ALIVE_INTERVAL' determines the interval at
 * which keep-alive packets are sent.
 */
#define ipconfigTCP_KEEP_ALIVE    1

/* Configuration for FreeRTOS TCP minimal sample project. */

/* Allows DHCP to be enabled by setting by setting `endpoint->bits.bWantDHCP`.
 *
 * When successful, DHCP will assign an IP-address, a netmask, a gateway
 * address, and one or more DNS addresses to the endpoint. DHCP must be able to
 * receive an options field of 312 bytes, the fixed part of the DHCP packet is
 * 240 bytes, and the IP/UDP headers take 28 bytes.
 */
#define ipconfigUSE_DHCP                           ipconfigENABLE

/* During the DHCP process, the driver will call an application hook
 * if 'ipconfigUSE_DHCP_HOOK' is non-zero.  It lets the application decide
 * if the DHCP offer shall be accepted.
 */
#define ipconfigUSE_DHCP_HOOK                      ipconfigENABLE

/* Include support for FreeRTOS_SendPingRequest() and
 * FreeRTOS_SendPingRequestIPv6()
 */
#define ipconfigSUPPORT_OUTGOING_PINGS             ipconfigENABLE

/* Enables the APIs that are backward compatible with single end point IPv4
 * version V3.x.x or older.
 */
#define ipconfigIPv4_BACKWARD_COMPATIBLE    ipconfigENABLE

/* Include support for the IPv6 protocol.
 * If disabled, the application must enable IPv4.
 */
#define ipconfigUSE_IPv6    ipconfigDISABLE

#endif /* FREERTOS_IP_CONFIG_H */
