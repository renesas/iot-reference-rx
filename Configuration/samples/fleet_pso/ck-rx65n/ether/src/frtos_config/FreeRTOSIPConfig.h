/*
 * FreeRTOS Kernel V10.3.0
 * Copyright (C) 2020 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 * Modifications Copyright (C) 2023 Renesas Electronics Corporation. or its affiliates.
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

/* Set to 1 to print out debug messages.  If ipconfigHAS_DEBUG_PRINTF is set to
 * 1 then FreeRTOS_debug_printf should be defined to the function used to print
 * out the debugging messages. */
#define ipconfigHAS_DEBUG_PRINTF    0
#if ( ipconfigHAS_DEBUG_PRINTF == 1 )
    #define FreeRTOS_debug_printf( X )    configPRINTF( X )
#endif

/* Set to 1 to print out non debugging messages, for example the output of the
 * FreeRTOS_netstat() command, and ping replies.  If ipconfigHAS_PRINTF is set to 1
 * then FreeRTOS_printf should be set to the function used to print out the
 * messages. */
#define ipconfigHAS_PRINTF    1
#if ( ipconfigHAS_PRINTF == 1 )
    #define FreeRTOS_printf( X )    configPRINTF( X )
#endif

/* Define the byte order of the target MCU (the MCU FreeRTOS+TCP is executing
 * on).  Valid options are pdFREERTOS_BIG_ENDIAN and pdFREERTOS_LITTLE_ENDIAN. */
#define ipconfigBYTE_ORDER                         pdFREERTOS_LITTLE_ENDIAN

/* If the network card/driver includes checksum offloading (IP/TCP/UDP checksums)
 * then set ipconfigDRIVER_INCLUDED_RX_IP_CHECKSUM to 1 to prevent the software
 * stack repeating the checksum calculations. */
#define ipconfigDRIVER_INCLUDED_RX_IP_CHECKSUM     1

/* Several API's will block until the result is known, or the action has been
 * performed, for example FreeRTOS_send() and FreeRTOS_recv().  The timeouts can be
 * set per socket, using setsockopt().  If not set, the times below will be
 * used as defaults. */
#define ipconfigSOCK_DEFAULT_RECEIVE_BLOCK_TIME    ( 10000 )
#define ipconfigSOCK_DEFAULT_SEND_BLOCK_TIME       ( 10000 )

/* Include support for DNS caching.  For TCP, having a small DNS cache is very
 * useful.  When a cache is present, ipconfigDNS_REQUEST_ATTEMPTS can be kept low
 * and also DNS may use small timeouts.  If a DNS reply comes in after the DNS
 * socket has been destroyed, the result will be stored into the cache.  The next
 * call to FreeRTOS_gethostbyname() will return immediately, without even creating
 * a socket.
 */
#define ipconfigUSE_DNS_CACHE                      ( 1 )
#define ipconfigDNS_CACHE_ADDRESSES_PER_ENTRY      ( 6 )
#define ipconfigDNS_REQUEST_ATTEMPTS               ( 2 )

/* The IP stack executes it its own task (although any application task can make
 * use of its services through the published sockets API). ipconfigUDP_TASK_PRIORITY
 * sets the priority of the task that executes the IP stack.  The priority is a
 * standard FreeRTOS task priority so can take any value from 0 (the lowest
 * priority) to (configMAX_PRIORITIES - 1) (the highest priority).
 * configMAX_PRIORITIES is a standard FreeRTOS configuration parameter defined in
 * FreeRTOSConfig.h, not FreeRTOSIPConfig.h. Consideration needs to be given as to
 * the priority assigned to the task executing the IP stack relative to the
 * priority assigned to tasks that use the IP stack. */
#define ipconfigIP_TASK_PRIORITY                   ( configMAX_PRIORITIES - 2 )

/* The size, in words (not bytes), of the stack allocated to the FreeRTOS+TCP
 * task.  This setting is less important when the FreeRTOS Win32 simulator is used
 * as the Win32 simulator only stores a fixed amount of information on the task
 * stack.  FreeRTOS includes optional stack overflow detection, see:
 * http://www.freertos.org/Stacks-and-stack-overflow-checking.html. */
#define ipconfigIP_TASK_STACK_SIZE_WORDS           ( configMINIMAL_STACK_SIZE * 5 )

/* ipconfigRAND32() is called by the IP stack to generate random numbers for
 * things such as a DHCP transaction number or initial sequence number.  Random
 * number generation is performed via this macro to allow applications to use their
 * own random number generation method.  For example, it might be possible to
 * generate a random number by sampling noise on an analogue input. */
uint32_t ulRand(void);
#define ipconfigRAND32()    ulRand()

/* If ipconfigUSE_NETWORK_EVENT_HOOK is set to 1 then FreeRTOS+TCP will call the
 * network event hook at the appropriate times.  If ipconfigUSE_NETWORK_EVENT_HOOK
 * is not set to 1 then the network event hook will never be called. See:
 * http://www.FreeRTOS.org/FreeRTOS-Plus/FreeRTOS_Plus_UDP/API/vApplicationIPNetworkEventHook.shtml.
 */
#define ipconfigUSE_NETWORK_EVENT_HOOK           1

/* Sockets have a send block time attribute.  If FreeRTOS_sendto() is called but
 * a network buffer cannot be obtained then the calling task is held in the Blocked
 * state (so other tasks can continue to executed) until either a network buffer
 * becomes available or the send block time expires.  If the send block time expires
 * then the send operation is aborted.  The maximum allowable send block time is
 * capped to the value set by ipconfigMAX_SEND_BLOCK_TIME_TICKS.  Capping the
 * maximum allowable send block time prevents prevents a deadlock occurring when
 * all the network buffers are in use and the tasks that process (and subsequently
 * free) the network buffers are themselves blocked waiting for a network buffer.
 * ipconfigMAX_SEND_BLOCK_TIME_TICKS is specified in RTOS ticks.  A time in
 * milliseconds can be converted to a time in ticks by dividing the time in
 * milliseconds by portTICK_PERIOD_MS. */
#define ipconfigUDP_MAX_SEND_BLOCK_TIME_TICKS    ( 5000 / portTICK_PERIOD_MS )

/* If ipconfigUSE_DHCP is 1 then FreeRTOS+TCP will attempt to retrieve an IP
 * address, netmask, DNS server address and gateway address from a DHCP server.  If
 * ipconfigUSE_DHCP is 0 then FreeRTOS+TCP will use a static IP address.  The
 * stack will revert to using the static IP address even when ipconfigUSE_DHCP is
 * set to 1 if a valid configuration cannot be obtained from a DHCP server for any
 * reason.  The static configuration used is that passed into the stack by the
 * FreeRTOS_IPInit() function call. */
#define ipconfigUSE_DHCP                         1
#define ipconfigDHCP_REGISTER_HOSTNAME           1
#define ipconfigDHCP_USES_UNICAST                1
#define ipconfigDHCP_SEND_DISCOVER_AFTER_AUTO_IP 0

/* If ipconfigDHCP_USES_USER_HOOK is set to 1 then the application writer must
 * provide an implementation of the DHCP callback function,
 * xApplicationDHCPUserHook(). */
#define ipconfigUSE_DHCP_HOOK                    0

/* When ipconfigUSE_DHCP is set to 1, DHCP requests will be sent out at
 * increasing time intervals until either a reply is received from a DHCP server
 * and accepted, or the interval between transmissions reaches
 * ipconfigMAXIMUM_DISCOVER_TX_PERIOD.  The IP stack will revert to using the
 * static IP address passed as a parameter to FreeRTOS_IPInit() if the
 * re-transmission time interval reaches ipconfigMAXIMUM_DISCOVER_TX_PERIOD without
 * a DHCP reply being received. */
#define ipconfigMAXIMUM_DISCOVER_TX_PERIOD     ( 120000 / portTICK_PERIOD_MS )

/* The ARP cache is a table that maps IP addresses to MAC addresses.  The IP
 * stack can only send a UDP message to a remove IP address if it knowns the MAC
 * address associated with the IP address, or the MAC address of the router used to
 * contact the remote IP address.  When a UDP message is received from a remote IP
 * address the MAC address and IP address are added to the ARP cache.  When a UDP
 * message is sent to a remote IP address that does not already appear in the ARP
 * cache then the UDP message is replaced by a ARP message that solicits the
 * required MAC address information.  ipconfigARP_CACHE_ENTRIES defines the maximum
 * number of entries that can exist in the ARP table at any one time. */
#define ipconfigARP_CACHE_ENTRIES                 6

/* ARP requests that do not result in an ARP response will be re-transmitted a
 * maximum of ipconfigMAX_ARP_RETRANSMISSIONS times before the ARP request is
 * aborted. */
#define ipconfigMAX_ARP_RETRANSMISSIONS           ( 5 )

/* ipconfigMAX_ARP_AGE defines the maximum time between an entry in the ARP
 * table being created or refreshed and the entry being removed because it is stale.
 * New ARP requests are sent for ARP cache entries that are nearing their maximum
 * age.  ipconfigMAX_ARP_AGE is specified in tens of seconds, so a value of 150 is
 * equal to 1500 seconds (or 25 minutes). */
#define ipconfigMAX_ARP_AGE                       150

/* Implementing FreeRTOS_inet_addr() necessitates the use of string handling
 * routines, which are relatively large.  To save code space the full
 * FreeRTOS_inet_addr() implementation is made optional, and a smaller and faster
 * alternative called FreeRTOS_inet_addr_quick() is provided.  FreeRTOS_inet_addr()
 * takes an IP in decimal dot format (for example, "192.168.0.1") as its parameter.
 * FreeRTOS_inet_addr_quick() takes an IP address as four separate numerical octets
 * (for example, 192, 168, 0, 1) as its parameters.  If
 * ipconfigINCLUDE_FULL_INET_ADDR is set to 1 then both FreeRTOS_inet_addr() and
 * FreeRTOS_indet_addr_quick() are available.  If ipconfigINCLUDE_FULL_INET_ADDR is
 * not set to 1 then only FreeRTOS_indet_addr_quick() is available. */
#define ipconfigINCLUDE_FULL_INET_ADDR            1

/* ipconfigNUM_NETWORK_BUFFER_DESCRIPTORS defines the total number of network buffer that
 * are available to the IP stack.  The total number of network buffers is limited
 * to ensure the total amount of RAM that can be consumed by the IP stack is capped
 * to a pre-determinable value. */
#define ipconfigNUM_NETWORK_BUFFER_DESCRIPTORS    ETHER_CFG_EMAC_TX_DESCRIPTORS

/* A FreeRTOS queue is used to send events from application tasks to the IP
 * stack.  ipconfigEVENT_QUEUE_LENGTH sets the maximum number of events that can
 * be queued for processing at any one time.  The event queue must be a minimum of
 * 5 greater than the total number of network buffers. */
#define ipconfigEVENT_QUEUE_LENGTH     ( ipconfigNUM_NETWORK_BUFFER_DESCRIPTORS + 5 )

/* The address of a socket is the combination of its IP address and its port
 * number.  FreeRTOS_bind() is used to manually allocate a port number to a socket
 * (to 'bind' the socket to a port), but manual binding is not normally necessary
 * for client sockets (those sockets that initiate outgoing connections rather than
 * wait for incoming connections on a known port number).  If
 * ipconfigALLOW_SOCKET_SEND_WITHOUT_BIND is set to 1 then calling
 * FreeRTOS_sendto() on a socket that has not yet been bound will result in the IP
 * stack automatically binding the socket to a port number from the range
 * socketAUTO_PORT_ALLOCATION_START_NUMBER to 0xffff.  If
 * ipconfigALLOW_SOCKET_SEND_WITHOUT_BIND is set to 0 then calling FreeRTOS_sendto()
 * on a socket that has not yet been bound will result in the send operation being
 * aborted. */
#define ipconfigALLOW_SOCKET_SEND_WITHOUT_BIND         1

/* Defines the Time To Live (TTL) values used in outgoing UDP packets. */
#define ipconfigUDP_TIME_TO_LIVE                       128
/* Also defined in FreeRTOSIPConfigDefaults.h. */
#define ipconfigTCP_TIME_TO_LIVE                       128

/* USE_TCP: Use TCP and all its features. */
#define ipconfigUSE_TCP                                ( 1 )

/* USE_WIN: Let TCP use windowing mechanism. */
#define ipconfigUSE_TCP_WIN                            ( 1 )

/* The MTU is the maximum number of bytes the payload of a network frame can
 * contain.  For normal Ethernet V2 frames the maximum MTU is 1500.  Setting a
 * lower value can save RAM, depending on the buffer management scheme used.  If
 * ipconfigCAN_FRAGMENT_OUTGOING_PACKETS is 1 then (ipconfigNETWORK_MTU - 28) must
 * be divisible by 8. */
#define ipconfigNETWORK_MTU                            1500U

/* Set ipconfigUSE_DNS to 1 to include a basic DNS client/resolver.  DNS is used
 * through the FreeRTOS_gethostbyname() API function. */
#define ipconfigUSE_DNS                                1

/* If ipconfigREPLY_TO_INCOMING_PINGS is set to 1 then the IP stack will
 * generate replies to incoming ICMP echo (ping) requests. */
#define ipconfigREPLY_TO_INCOMING_PINGS                1

/* If ipconfigSUPPORT_OUTGOING_PINGS is set to 1 then the
 * FreeRTOS_SendPingRequest() API function is available. */
#define ipconfigSUPPORT_OUTGOING_PINGS                 0

/* If ipconfigSUPPORT_SELECT_FUNCTION is set to 1 then the FreeRTOS_select()
 * (and associated) API function is available. */
#define ipconfigSUPPORT_SELECT_FUNCTION                0

/* If ipconfigFILTER_OUT_NON_ETHERNET_II_FRAMES is set to 1 then Ethernet frames
 * that are not in Ethernet II format will be dropped.  This option is included for
 * potential future IP stack developments. */
#define ipconfigFILTER_OUT_NON_ETHERNET_II_FRAMES      1

/* If ipconfigETHERNET_DRIVER_FILTERS_FRAME_TYPES is set to 1 then it is the
 * responsibility of the Ethernet interface to filter out packets that are of no
 * interest.  If the Ethernet interface does not implement this functionality, then
 * set ipconfigETHERNET_DRIVER_FILTERS_FRAME_TYPES to 0 to have the IP stack
 * perform the filtering instead (it is much less efficient for the stack to do it
 * because the packet will already have been passed into the stack).  If the
 * Ethernet driver does all the necessary filtering in hardware then software
 * filtering can be removed by using a value other than 1 or 0. */
#define ipconfigETHERNET_DRIVER_FILTERS_FRAME_TYPES    0 //XXX

/* The windows simulator cannot really simulate MAC interrupts, and needs to
 * block occasionally to allow other tasks to run. */
//#define configWINDOWS_MAC_INTERRUPT_SIMULATOR_DELAY    ( 20 / portTICK_PERIOD_MS )

/* Advanced only: in order to access 32-bit fields in the IP packets with
 * 32-bit memory instructions, all packets will be stored 32-bit-aligned,
 * plus 16-bits. This has to do with the contents of the IP-packets: all
 * 32-bit fields are 32-bit-aligned, plus 16-bit. */
#define ipconfigPACKET_FILLER_SIZE                     2U

/* Define the size of the pool of TCP window descriptors.  On the average, each
 * TCP socket will use up to 2 x 6 descriptors, meaning that it can have 2 x 6
 * outstanding packets (for Rx and Tx).  When using up to 10 TP sockets
 * simultaneously, one could define TCP_WIN_SEG_COUNT as 120. */
#define ipconfigTCP_WIN_SEG_COUNT                      240

/* Each TCP socket has a circular buffers for Rx and Tx, which have a fixed
 * maximum size.  Define the size of Rx buffer for TCP sockets. */
#define ipconfigTCP_RX_BUFFER_LENGTH                   ( 1460 )

/* Define the size of Tx buffer for TCP sockets. */
#define ipconfigTCP_TX_BUFFER_LENGTH                   ( 1460 )

/* When using call-back handlers, the driver may check if the handler points to
 * real program memory (RAM or flash) or just has a random non-zero value. */
#define ipconfigIS_VALID_PROG_ADDRESS( x )    ( ( x ) != NULL )

/* Include support for TCP keep-alive messages. */
#define ipconfigTCP_KEEP_ALIVE                   ( 1 )
#define ipconfigTCP_KEEP_ALIVE_INTERVAL          ( 20 ) /* Seconds. */

/* The socket semaphore is used to unblock the MQTT task. */
#define ipconfigSOCKET_HAS_USER_SEMAPHORE        ( 0 )

#define ipconfigSOCKET_HAS_USER_WAKE_CALLBACK    ( 1 )
#define ipconfigUSE_CALLBACKS                    ( 0 )

#define ipconfigZERO_COPY_TX_DRIVER              ( 0 )
#define ipconfigZERO_COPY_RX_DRIVER              ( 0 )

/* Possible optimisation for expert users - requires network driver support.
 * When ipconfigUSE_LINKED_RX_MESSAGES is set to non-zero value then
 * instead of passing received packets into the IP task one at a time the
 * network interface can chain received packets together and pass them into
 * the IP task in one go.  The packets are chained using the pxNextBuffer
 * member. This optimisation is useful when there is high network traffic.
 * When ipconfigUSE_LINKED_RX_MESSAGES is set to 0 then only one buffer will
 * be sent at a time.  This is the default way for +TCP to pass messages from
 * the MAC to the TCP/IP stack. */
#define ipconfigUSE_LINKED_RX_MESSAGES           ( 0 )

#define portINLINE                               __inline

void vApplicationMQTTGetKeys( const char ** ppcRootCA,
                              const char ** ppcClientCert,
                              const char ** ppcClientPrivateKey );

/* Let NetworkInterface check the free space in the working queue for the IP-task. */
#define ipconfigCHECK_IP_QUEUE_SPACE            1

/* This file must be included only after the configuration files FreeRTOSConfig.h and
 * FreeRTOSIPConfig.h have been included already.
 * Mentioned header files are private to the project, whereas this header
 * file is part of FreeRTOS+TCP.
 *
 * The complete documentation of the configuration parameters can be found here:
 *
 * https://www.freertos.org/FreeRTOS-Plus/FreeRTOS_Plus_TCP/TCP_IP_Configuration.html
 *
 */

/* The error numbers defined in this file will be moved to the core FreeRTOS
 * code in future versions of FreeRTOS - at which time the following header file
 * will be removed. */
#include "FreeRTOS_errno_TCP.h"

/* This file provides default values for configuration options that are missing
 * from the FreeRTOSIPConfig.h configuration header file. */

/* These macros are used to define away static keyword for CBMC proofs */
#ifndef _static
    #define _static    static
#endif

/* Through time some macro names have changed. This always
 * happened for a good reason: clarity or consistency.
 * Here below there are some ifdef's that will issue an error if any of the
 * deprecated macros is still defined. */

/* Ensure defined configuration constants are using the most up to date naming. */
#ifdef tcpconfigIP_TIME_TO_LIVE
    #error now called: ipconfigTCP_TIME_TO_LIVE
#endif

#ifdef updconfigIP_TIME_TO_LIVE
    #error now called: ipconfigUDP_TIME_TO_LIVE
#endif

#ifdef ipFILLER_SIZE
    #error now called: ipconfigPACKET_FILLER_SIZE
#endif

#ifdef dnsMAX_REQUEST_ATTEMPTS
    #error now called: ipconfigDNS_REQUEST_ATTEMPTS
#endif

#ifdef ipconfigUDP_TASK_PRIORITY
    #error now called: ipconfigIP_TASK_PRIORITY
#endif

#ifdef ipconfigUDP_TASK_STACK_SIZE_WORDS
    #error now called: ipconfigIP_TASK_STACK_SIZE_WORDS
#endif

#ifdef ipconfigDRIVER_INCLUDED_RX_IP_FILTERING
    #error now called: ipconfigETHERNET_DRIVER_FILTERS_PACKETS
#endif

#ifdef ipconfigMAX_SEND_BLOCK_TIME_TICKS
    #error now called: ipconfigUDP_MAX_SEND_BLOCK_TIME_TICKS
#endif

#ifdef ipconfigUSE_RECEIVE_CONNECT_CALLBACKS
    #error now called: ipconfigUSE_CALLBACKS
#endif

#ifdef ipconfigNUM_NETWORK_BUFFERS
    #error now called: ipconfigNUM_NETWORK_BUFFER_DESCRIPTORS
#endif

#ifdef ipconfigTCP_HANG_PROT
    #error now called: ipconfigTCP_HANG_PROTECTION
#endif

#ifdef ipconfigTCP_HANG_PROT_TIME
    #error now called: ipconfigTCP_HANG_PROTECTION_TIME
#endif

#ifdef FreeRTOS_lprintf
    #error now called: FreeRTOS_debug_printf
#endif

#ifdef  ipconfigBUFFER_ALLOC_FIXED_SIZE
    #error ipconfigBUFFER_ALLOC_FIXED_SIZE was dropped and replaced by a const value, declared in BufferAllocation[12].c
#endif

#ifdef  ipconfigNIC_SEND_PASSES_DMA
    #error now called: ipconfigZERO_COPY_TX_DRIVER
#endif

#ifdef  HAS_TX_CRC_OFFLOADING
    #error now called: ipconfigHAS_TX_CRC_OFFLOADING
#endif

#ifdef  HAS_RX_CRC_OFFLOADING
    #error now called: ipconfigHAS_RX_CRC_OFFLOADING
#endif

#ifdef ipconfigTCP_RX_BUF_LEN
    #error ipconfigTCP_RX_BUF_LEN is now called ipconfigTCP_RX_BUFFER_LENGTH
#endif

#ifdef ipconfigTCP_TX_BUF_LEN
    #error ipconfigTCP_TX_BUF_LEN is now called ipconfigTCP_TX_BUFFER_LENGTH
#endif

#ifdef ipconfigDHCP_USES_USER_HOOK
    #error ipconfigDHCP_USES_USER_HOOK and its associated callback have been superseded - see http: /*www.FreeRTOS.org/FreeRTOS-Plus/FreeRTOS_Plus_TCP/TCP_IP_Configuration.html#ipconfigUSE_DHCP_HOOK */
#endif

/* The macro 'ipconfigBYTE_ORDER' must be defined as either 'pdFREERTOS_LITTLE_ENDIAN'
 * or as 'pdFREERTOS_BIG_ENDIAN'.  See also 'projdefs.h'.
 */
#ifndef ipconfigBYTE_ORDER
    #error The macro 'ipconfigBYTE_ORDER' must be defined at this point
#endif

/* So far the warnings about deprecated configuration macros. */

/*-----------------------------------------------------------*/

/* The IP stack executes it its own task (although any application task can make
 * use of its services through the published sockets API). ipconfigUDP_TASK_PRIORITY
 * sets the priority of the task that executes the IP stack.  The priority is a
 * standard FreeRTOS task priority so can take any value from 0 (the lowest
 * priority) to (configMAX_PRIORITIES - 1) (the highest priority).
 * configMAX_PRIORITIES is a standard FreeRTOS configuration parameter defined in
 * FreeRTOSConfig.h, not FreeRTOSIPConfig.h. Consideration needs to be given as to
 * the priority assigned to the task executing the IP stack relative to the
 * priority assigned to tasks that use the IP stack. */
#ifndef ipconfigIP_TASK_PRIORITY
    #define ipconfigIP_TASK_PRIORITY    ( configMAX_PRIORITIES - 2 )
#endif

/* The size, in words (not bytes), of the stack allocated to the FreeRTOS+TCP
 * task.  This setting is less important when the FreeRTOS Win32 simulator is used
 * as the Win32 simulator only stores a fixed amount of information on the task
 * stack.  FreeRTOS includes optional stack overflow detection, see:
 * http://www.freertos.org/Stacks-and-stack-overflow-checking.html */
#ifndef ipconfigIP_TASK_STACK_SIZE_WORDS
    #define ipconfigIP_TASK_STACK_SIZE_WORDS    ( configMINIMAL_STACK_SIZE * 5 )
#endif

/* Include all API's and code that is needed for the TCP protocol.
 * When defined as zero, the application is UDP-only. */
#ifndef ipconfigUSE_TCP
    #define ipconfigUSE_TCP    ( 1 )
#endif

#if ( ipconfigUSE_TCP != 0 )

/* 'ipconfigUSE_TCP_WIN' enables support for TCP sliding windows.  When
 * defined as zero, each TCP packet must be acknowledged individually.
 * That will be slower, but it will result less code. */
    #ifndef ipconfigUSE_TCP_WIN
        #define ipconfigUSE_TCP_WIN    ( 1 )
    #endif

/* The total number of outstanding TCP segments, either outgoing or incoming.
 * This only applies when 'ipconfigUSE_TCP_WIN' is enabled. */
    #ifndef ipconfigTCP_WIN_SEG_COUNT
        #define ipconfigTCP_WIN_SEG_COUNT    ( 240 )
    #endif

/* When non-zero, TCP will not send RST packets in reply to
 * TCP packets which are unknown, or out-of-order.
 * This is an option used for testing.  It is recommended to
 * define it as '0'. */
    #ifndef ipconfigIGNORE_UNKNOWN_PACKETS
        #define ipconfigIGNORE_UNKNOWN_PACKETS    ( 0 )
    #endif
#endif /* if ipconfigUSE_TCP */

/*
 * For debugging/logging: check if the port number is used for e.g. telnet.
 * Some events will not be logged for telnet connections
 * because it would produce logging about the transmission of the logging...
 * This macro will only be used if FreeRTOS_debug_printf() is defined for logging.
 * Note that the parameter 'xPort' is already converted to host-endian.
 */
#ifndef ipconfigTCP_MAY_LOG_PORT
    #define ipconfigTCP_MAY_LOG_PORT( xPort )    ( ( xPort ) != 23U )
#endif

/* Determine the number of clock ticks that the API's FreeRTOS_recv() and
 * FreeRTOS_recvfrom() must wait for incoming data. */
#ifndef ipconfigSOCK_DEFAULT_RECEIVE_BLOCK_TIME
    #define ipconfigSOCK_DEFAULT_RECEIVE_BLOCK_TIME    portMAX_DELAY
#endif

/* Determine the number of clock ticks that FreeRTOS_send() must wait
 * for space in the transmission buffer.
 * For FreeRTOS_sendto(), it limits how long the application
 * should wait for a network buffer to become available.
 */
#ifndef ipconfigSOCK_DEFAULT_SEND_BLOCK_TIME
    #define ipconfigSOCK_DEFAULT_SEND_BLOCK_TIME    portMAX_DELAY
#endif

/* When sending a UDP packet, a network buffer must be obtained.  This macro
 * will limit the maximum waiting time that is configured with e.g.
 * ipconfigSOCK_DEFAULT_SEND_BLOCK_TIME, or when setting the socket option
 * FREERTOS_SO_SNDTIMEO.
 * It serves as a protection against a possible dead-lock; a task waiting
 * for itself to release a network buffer.
 */
#ifndef ipconfigUDP_MAX_SEND_BLOCK_TIME_TICKS
    #define ipconfigUDP_MAX_SEND_BLOCK_TIME_TICKS    ( 5000 / portTICK_PERIOD_MS )
#endif


/*
 * FreeRTOS general logging routine (proposal)
 * Used in some utility functions such as FreeRTOS_netstat() and FreeRTOS_PrintARPCache()
 *
 *     extern void my_printf( const char * pcFormat, ... );
 *     #define FreeRTOS_printf( MSG )           my_printf MSG
 *
 * The FreeRTOS_printf() must be thread-safe but does not have to be interrupt-safe
 */
#ifdef ipconfigHAS_PRINTF
    #if ( ipconfigHAS_PRINTF == 0 )
        #ifdef FreeRTOS_printf
            #error Do not define FreeRTOS_print if ipconfigHAS_PRINTF is set to 0
        #endif /* ifdef FreeRTOS_debug_printf */
    #endif /* ( ipconfigHAS_PRINTF == 0 ) */
#endif /* ifdef ipconfigHAS_PRINTF */

#ifndef FreeRTOS_printf
    #define FreeRTOS_printf( MSG )    do {} while( ipFALSE_BOOL )
    /* MISRA Ref 20.5.1 [Use of undef] */
    /* More details at: https://github.com/FreeRTOS/FreeRTOS-Plus-TCP/blob/main/MISRA.md#rule-2051 */
    /* coverity[misra_c_2012_rule_20_5_violation] */
    #undef ipconfigHAS_PRINTF
    #define ipconfigHAS_PRINTF    0
#endif

/*
 * In cases where a lot of logging is produced, FreeRTOS_flush_logging( )
 * will be called to give the logging module a chance to flush the data.
 */
#ifndef FreeRTOS_flush_logging
    #define FreeRTOS_flush_logging()    do {} while( ipFALSE_BOOL )
#endif

/* Malloc functions.  Within most applications of FreeRTOS, the couple
 * pvPortMalloc()/vPortFree() will be used.
 * If there are different types of RAM, the user may decide to use a different
 * memory allocator for different purposes:
 * MallocLarge is used to allocate large TCP buffers (for Rx/Tx)
 * MallocSocket is used to allocate the space for the sockets
 */
#ifndef pvPortMallocLarge
    #define pvPortMallocLarge( x )    pvPortMalloc( x )
#endif

#ifndef vPortFreeLarge
    #define vPortFreeLarge( ptr )    vPortFree( ptr )
#endif

#ifndef pvPortMallocSocket
    #define pvPortMallocSocket( x )    pvPortMalloc( x )
#endif

#ifndef vPortFreeSocket
    #define vPortFreeSocket( ptr )    vPortFree( ptr )
#endif

/*
 * At several places within the library, random numbers are needed:
 * - DHCP:    For creating a DHCP transaction number
 * - TCP:     Set the Initial Sequence Number: this is the value of the first outgoing
 *            sequence number being used when connecting to a peer.
 *            Having a well randomized ISN is important to avoid spoofing
 * - UDP/TCP: for setting the first port number to be used, in case a socket
 *            uses a 'random' or anonymous port number
 *
 * The function is now **deprecated**, in stead the following function must be provided:
 * BaseType_t xApplicationGetRandomNumber( uint32_t *pulValue );
 * It will return pdPASS if the random number could be created, otherwise pdFAIL.
 */
#ifndef ipconfigRAND32
    #define ipconfigRAND32()    rand()
#endif

/* 'ipconfigUSE_NETWORK_EVENT_HOOK' indicates if an application hook is available
 * called 'vApplicationIPNetworkEventHook()'.  This function will be called when
 * the network goes up and when it goes down.  See also FREERTOS_IP.h for further
 * explanation. */
#ifndef ipconfigUSE_NETWORK_EVENT_HOOK
    #define ipconfigUSE_NETWORK_EVENT_HOOK    1
#endif

/* Define the number of entries in the ARP cache table. */
#ifndef ipconfigARP_CACHE_ENTRIES
    #define ipconfigARP_CACHE_ENTRIES    6
#endif

/* The number of times an ARP request is sent when looking
 * up an IP-address.
 * The name should have been 'max transmissions', and not
 * 'max re-transmissions'. */
#ifndef ipconfigMAX_ARP_RETRANSMISSIONS
    #define ipconfigMAX_ARP_RETRANSMISSIONS    ( 5U)
#endif

/* The maximum age of an entry in the ARP cache table can be
 * calculated as 'ipARP_TIMER_PERIOD_MS' x 'ipconfigMAX_ARP_AGE'.
 * The macro 'ipARP_TIMER_PERIOD_MS' is local to FreeRTOSIP.c
 * but it can be overwritten from FreeRTOSIPConfig.h
 * The default is 10000 x 150 = 1500000 ms or 1500 seconds
 */
#ifndef ipconfigMAX_ARP_AGE
    #define ipconfigMAX_ARP_AGE    150U
#endif

/* 'ipconfigUSE_ARP_REVERSED_LOOKUP' when non-zero, the function
 * eARPGetCacheEntryByMac() will be included in the code.
 */
#ifndef ipconfigUSE_ARP_REVERSED_LOOKUP
    #define ipconfigUSE_ARP_REVERSED_LOOKUP    0
#endif

/* 'ipconfigUSE_ARP_REMOVE_ENTRY' when non-zero, the function
 * ulARPRemoveCacheEntryByMac() will be included in the code.
 */
#ifndef ipconfigUSE_ARP_REMOVE_ENTRY
    #define ipconfigUSE_ARP_REMOVE_ENTRY    0
#endif

/* Normally, the ARP table will only store IP-addresses that are located
 * in the local subnet.
 * When enabled, the option 'ipconfigARP_STORES_REMOTE_ADDRESSES' will allow
 * that remote IP-addresses will also be stored, along with the MAC-address of
 * the gateway.
 */
#ifndef ipconfigARP_STORES_REMOTE_ADDRESSES
    #define ipconfigARP_STORES_REMOTE_ADDRESSES    0
#endif

/* 'ipconfigINCLUDE_FULL_INET_ADDR' used to determine if
 * the function 'FreeRTOS_inet_addr()' is included.
 * The macro is now deprecated and the function is included
 * unconditionally.
 */
#ifndef ipconfigINCLUDE_FULL_INET_ADDR
    #define ipconfigINCLUDE_FULL_INET_ADDR    1
#endif

/* This is about how new packets are passed from the network interface
 * to the IP-task.  By default they will be sent one-by-one.
 * When 'ipconfigUSE_LINKED_RX_MESSAGES' is non-zero, each message
 * buffer gets a 'pxNextBuffer' field, to that linked packets can be passed
 * to the IP-task in a single call to 'xSendEventStructToIPTask()'.
 * Note that this only works if the Network Interface also supports this
 * option.
 */
#ifndef ipconfigUSE_LINKED_RX_MESSAGES
    #define ipconfigUSE_LINKED_RX_MESSAGES    0
#endif

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
#ifndef ipconfigNUM_NETWORK_BUFFER_DESCRIPTORS
    #define ipconfigNUM_NETWORK_BUFFER_DESCRIPTORS    ETHER_CFG_EMAC_TX_DESCRIPTORS
#endif

/* Every task, and also the network interface can send messages
 * to the IP-task by calling API's.  These messages pass through a
 * queue which has a maximum size of 'ipconfigEVENT_QUEUE_LENGTH'
 * items.
 * When developing an application, it is important to monitor the
 * actual usage of the queue.  See 'ipconfigCHECK_IP_QUEUE_SPACE'
 * here below.
 */
#ifndef ipconfigEVENT_QUEUE_LENGTH
    #define ipconfigEVENT_QUEUE_LENGTH    ( ipconfigNUM_NETWORK_BUFFER_DESCRIPTORS + 5U )
#endif

#if ( ipconfigEVENT_QUEUE_LENGTH < ( ipconfigNUM_NETWORK_BUFFER_DESCRIPTORS + 5U ) )
    #error The ipconfigEVENT_QUEUE_LENGTH parameter must be at least ipconfigNUM_NETWORK_BUFFER_DESCRIPTORS + 5
#endif

/* Related to the macro 'ipconfigEVENT_QUEUE_LENGTH' here above:
 * when developing a new networking application, it can be helpful
 * to monitor the length of the message queue of the IP-task.
 * This code is only enabled when 'ipconfigCHECK_IP_QUEUE_SPACE'
 * is set to 1.  See also the function 'uxGetMinimumIPQueueSpace()'.
 */
#ifndef ipconfigCHECK_IP_QUEUE_SPACE
    #define ipconfigCHECK_IP_QUEUE_SPACE    1
#endif

/* When defined as non-zero, this macro allows to use a socket
 * without first binding it explicitly to a port number.
 * In that case, it will be bound to a random free port number. */
#ifndef ipconfigALLOW_SOCKET_SEND_WITHOUT_BIND
    #define ipconfigALLOW_SOCKET_SEND_WITHOUT_BIND    1
#endif

/* Configuration to control whether packets with IP options,
 * received over the network, should be passed up to the
 * software stack OR should be dropped.
 * If set to 1, the stack accepts IP packets that contain IP options, but does
 * not process the options (IP options are not supported).
 * If set to 0, the stack will drop IP packets that contain IP options.
 */
#ifndef ipconfigIP_PASS_PACKETS_WITH_IP_OPTIONS
    #define ipconfigIP_PASS_PACKETS_WITH_IP_OPTIONS    1
#endif

/* Configuration to control whether all outgoing IP datagrams get their
 * "don't fragment" flag set.
 * If set to 1, the stack will set the "don't fragment" flag on all outgoing IP
 * packets.  If a packet needs to be fragmented somewhere along it's path, it will get
 * discarded instead of fragmented.
 * If set to 0, the stack will clear the "don't fragment" flag an all outgoing IP
 * packets therefore allowing fragmentation if it is needed.
 */
#ifndef ipconfigFORCE_IP_DONT_FRAGMENT
    #define ipconfigFORCE_IP_DONT_FRAGMENT    0
#endif

/* Configuration to control whether UDP packets with
 * checksum value of zero should be passed up the software
 * stack OR should be dropped.
 * When enabled, the stack will accept UDP packets that have their checksum
 * value set to 0.
 * When disabled, the stack will drop UDP packets that have their checksum
 * value set to 0, and issue some logging.
 */
#ifndef ipconfigUDP_PASS_ZERO_CHECKSUM_PACKETS
    #define ipconfigUDP_PASS_ZERO_CHECKSUM_PACKETS    0
#endif

/* Define the value of the TTL field in outgoing UDP packets. */
#ifndef ipconfigUDP_TIME_TO_LIVE
    #define ipconfigUDP_TIME_TO_LIVE    128
#endif

/* Define the value of the TTL field in outgoing TCP packets. */
#ifndef ipconfigTCP_TIME_TO_LIVE
    #define ipconfigTCP_TIME_TO_LIVE    128
#endif

/* Define the value of the TTL field in outgoing TCP packets. */
/* The default of 64 is recommend in RFC 1700. */
#ifndef ipconfigICMP_TIME_TO_LIVE
    #define ipconfigICMP_TIME_TO_LIVE    64
#endif

/* TCP only: when measuring the Smoothed Round Trip Time (SRTT),
 * the result will be rounded up to a minimum value.
 * The default has always been 50, but a value of 1000
 * is recommended ( see RFC6298 ) because hosts often delay the
 * sending of ACK packets with 200 ms. */
#ifndef ipconfigTCP_SRTT_MINIMUM_VALUE_MS
    #define ipconfigTCP_SRTT_MINIMUM_VALUE_MS    50
#endif

/* Make positive to define the maximum number of packets which will be buffered
 * in a UDP socket.
 * Can be overridden with the socket option 'FREERTOS_SO_UDP_MAX_RX_PACKETS'.
 */
#ifndef ipconfigUDP_MAX_RX_PACKETS
    #define ipconfigUDP_MAX_RX_PACKETS    0U
#endif

/* Define the priority of the IP-task.  It is recommended to use this
 * order of priorities:
 * Highest : network interface, handling transmission and reception.
 * Medium  : the IP-task handling API calls from the application.
 * Lower   : the tasks that make use of the IP-stack.
 * For other tasks any priority can be chosen.
 */
#ifndef ipconfigIP_TASK_PRIORITY
    #define ipconfigIP_TASK_PRIORITY    ( configMAX_PRIORITIES - 2 )
#endif

/* The size, in words (not bytes), of the stack allocated to the FreeRTOS+TCP
 * task.  This setting is less important when the FreeRTOS Win32 simulator is used
 * as the Win32 simulator only stores a fixed amount of information on the task
 * stack.  FreeRTOS includes optional stack overflow detection, see:
 * http://www.freertos.org/Stacks-and-stack-overflow-checking.html. */
#ifndef ipconfigIP_TASK_STACK_SIZE_WORDS
    #define ipconfigIP_TASK_STACK_SIZE_WORDS    ( configMINIMAL_STACK_SIZE * 5 )
#endif

/* When non-zero, the module FreeRTOS_DHCP.c will be included and called.
 * Note that the application can override decide to ignore the outcome
 * of the DHCP negotiation and use a static IP-address. */
#ifndef ipconfigUSE_DHCP
    #define ipconfigUSE_DHCP    1
#endif

/* During the DHCP process, the driver will call an application hook
 * if 'ipconfigUSE_DHCP_HOOK' is non-zero.  It lets the application decide
 * if the DHCP offer shall be accepted.
 */
#ifndef ipconfigUSE_DHCP_HOOK
    #define ipconfigUSE_DHCP_HOOK    0
#endif

/* DHCP servers have a table with information about each clients.  One
 * of the fields in this table contains the host name of the DHCP clients.
 * When 'ipconfigDHCP_REGISTER_HOSTNAME' is defined as non-zero, the DHCP
 * driver will call 'pcApplicationHostnameHook()' to obtain the name of
 * the embedded device.
 */
#ifndef ipconfigDHCP_REGISTER_HOSTNAME
    #define ipconfigDHCP_REGISTER_HOSTNAME    1
#endif

/*
 * Only applicable when DHCP is in use:
 * If no DHCP server responds, use "Auto-IP" : the
 * device will allocate a random LinkLayer IP address.
 */
#ifndef ipconfigDHCP_FALL_BACK_AUTO_IP
    #define ipconfigDHCP_FALL_BACK_AUTO_IP    ( 0 )
#endif

/* When a link-layer address is assigned, the driver will test
 * if it is already taken by a different device by sending ARP
 * requests.  Therefore, 'ipconfigARP_USE_CLASH_DETECTION' must
 * be defined as non-zero.
 */
#if ( ipconfigDHCP_FALL_BACK_AUTO_IP != 0 )
    #ifndef ipconfigARP_USE_CLASH_DETECTION
        #define ipconfigARP_USE_CLASH_DETECTION    1
    #else
        #if ( ipconfigARP_USE_CLASH_DETECTION != 1 )
            #error ipconfigARP_USE_CLASH_DETECTION should be defined as 1 when AUTO_IP is used.
        #endif
    #endif
#endif

/* If ipconfigDHCP_FALL_BACK_AUTO_IP is not used, the code for
 * clash detection is not needed.
 */
#ifndef ipconfigARP_USE_CLASH_DETECTION
    #define ipconfigARP_USE_CLASH_DETECTION    0
#endif

/* An important macro: 'ipconfigNETWORK_MTU' determines the Maximum
 * transmission unit, which is a network packet minus the size of the
 * 14-byte Ethernet header.
 */
#ifndef ipconfigNETWORK_MTU
    #define ipconfigNETWORK_MTU    1500U
#else
    /* A sanity check to avoid a possible overflow of size_t. */
    #if ipconfigNETWORK_MTU > ( SIZE_MAX >> 1 )
        /* MISRA Ref 20.5.1 [Use of undef] */
        /* More details at: https://github.com/FreeRTOS/FreeRTOS-Plus-TCP/blob/main/MISRA.md#rule-2051 */
        /* coverity[misra_c_2012_rule_20_5_violation] */
        #undef ipconfigNETWORK_MTU
        #define ipconfigNETWORK_MTU    ( SIZE_MAX >> 1 )
    #endif
#endif

#if ( ipconfigNETWORK_MTU < 46 )
    #error ipconfigNETWORK_MTU must be at least 46.
#endif

/* The maximum segment size used by TCP, it is the maximum size of
 * the TCP payload per packet.
 * For IPv4: when MTU equals 1500, the MSS equals 1460.
 * It is recommended to use the default value defined here.
 *
 * In FreeRTOS_TCP_IP.c, there is a local macro called 'tcpREDUCED_MSS_THROUGH_INTERNET'.
 * When a TCP connection is made outside the local network, the MSS
 * will be reduced to 'tcpREDUCED_MSS_THROUGH_INTERNET' before the connection
 * is made.
 */
#ifndef ipconfigTCP_MSS
    #define ipconfigTCP_MSS    ( ipconfigNETWORK_MTU - ( ipSIZE_OF_IPv4_HEADER + ipSIZE_OF_TCP_HEADER ) )
#endif

/* This macro defines the minimum size of an outgoing Ethernet packet.
 * When zero, there is no minimum.
 * When non-zero, the packet will be extended to the minimum size.
 * The extra bytes will be cleared.
 * In many projects a value of 60 is used.
 * The 32-bit CRC added in the physical layer is not included.
 */
#ifndef ipconfigETHERNET_MINIMUM_PACKET_BYTES
    #define ipconfigETHERNET_MINIMUM_PACKET_BYTES    0
#endif

/* Each TCP socket has circular stream buffers for reception and transmission,
 * which have a fixed maximum size.
 * The defaults for these sizes are defined here, although
 * they can be overridden at runtime by calling FreeRTOS_setsockopt(),
 * and use 'FREERTOS_SO_RCVBUF', 'FREERTOS_SO_SNDBUF' or
 * 'FREERTOS_SO_WIN_PROPERTIES'.
 * A stream buffer will only be created when needed.  A TCP server socket
 * will never create buffers.
 */
#ifndef ipconfigTCP_RX_BUFFER_LENGTH
    /* When MTU equals 1500, the buffer length defaults to 5840 bytes */
    #define ipconfigTCP_RX_BUFFER_LENGTH    ( 1460 )
#endif

/* Define the size of Tx stream buffer for TCP sockets.
 */
#ifndef ipconfigTCP_TX_BUFFER_LENGTH
    /* When MTU equals 1500, the buffer length defaults to 5840 bytes */
    #define ipconfigTCP_TX_BUFFER_LENGTH    ( 1460 )
#endif

/* 'ipconfigMAXIMUM_DISCOVER_TX_PERIOD' is about DHCP.
 * It determines the maximum time (in clock-ticks) that the DHCP client
 * will wait for an offer from a DHCP server. */
#ifndef ipconfigMAXIMUM_DISCOVER_TX_PERIOD
    #ifdef _WINDOWS_
        #define ipconfigMAXIMUM_DISCOVER_TX_PERIOD    ( pdMS_TO_TICKS( 999U ) )
    #else
        #define ipconfigMAXIMUM_DISCOVER_TX_PERIOD    ( pdMS_TO_TICKS( 30000U ) )
    #endif /* _WINDOWS_ */
#endif /* ipconfigMAXIMUM_DISCOVER_TX_PERIOD */

#if ( ipconfigUSE_DNS == 0 )
    /* The DNS module will not be included. */
    #if ( ( ipconfigUSE_LLMNR != 0 ) || ( ipconfigUSE_NBNS != 0 ) )
        /* LLMNR and NBNS depend on DNS because those protocols share a lot of code. */
        #error When either LLMNR or NBNS is used, ipconfigUSE_DNS must be defined
    #endif
#endif

/* By default, the DNS client is included.  Note that LLMNR and
 * NBNS also need the code from FreeRTOS_DNS.c
 */
#ifndef ipconfigUSE_DNS
    #define ipconfigUSE_DNS    1
#endif

/* When looking up a host with DNS, this macro determines how long the
 * call to FreeRTOS_recvfrom() will wait for a reply.
 * When there is no reply, the request will be repeated up to
 * 'ipconfigDNS_REQUEST_ATTEMPTS' attempts. */
#ifndef ipconfigDNS_RECEIVE_BLOCK_TIME_TICKS
    #define ipconfigDNS_RECEIVE_BLOCK_TIME_TICKS    pdMS_TO_TICKS( 5000U )
#endif

/* When looking up a host with DNS, this macro determines how long the
 * call to FreeRTOS_sendto() will block to wait for a free buffer. */
#ifndef ipconfigDNS_SEND_BLOCK_TIME_TICKS
    #define ipconfigDNS_SEND_BLOCK_TIME_TICKS    pdMS_TO_TICKS( 500U )
#endif

/* The number of times a DNS request will be send before giving up. */
#ifndef ipconfigDNS_REQUEST_ATTEMPTS
    #define ipconfigDNS_REQUEST_ATTEMPTS    2
#endif

/* The results of DNS lookup's can be stored in a cache table. */
#ifndef ipconfigUSE_DNS_CACHE
    #define ipconfigUSE_DNS_CACHE    1
#endif

#if ( ipconfigUSE_DNS_CACHE != 0 )

/* Per https://tools.ietf.org/html/rfc1035, 253 is the maximum string length
 * of a DNS name.  The following default accounts for a null terminator. */
    #ifndef ipconfigDNS_CACHE_NAME_LENGTH
        #define ipconfigDNS_CACHE_NAME_LENGTH    254U
    #endif

/* The number of entries in the DNS cache table.
 * The default of 1 is maybe too economic. */
    #ifndef ipconfigDNS_CACHE_ENTRIES
        #define ipconfigDNS_CACHE_ENTRIES    1U
    #endif

#endif /* ipconfigUSE_DNS_CACHE != 0 */

/* When accessing services which have multiple IP addresses, setting this
 * greater than 1 can improve reliability by returning different IP address
 * answers on successive calls to FreeRTOS_gethostbyname(). */
#ifndef ipconfigDNS_CACHE_ADDRESSES_PER_ENTRY
    #define ipconfigDNS_CACHE_ADDRESSES_PER_ENTRY    6
#endif

/* When 'ipconfigDNS_USE_CALLBACKS' is defined, a function 'FreeRTOS_gethostbyname_a()'
 * will become available.
 * It is used for asynchronous DNS lookups.
 * This function will start a DNS-lookup and set an application hook.
 * This hook will be called when either the URL has been found, or when
 * a time-out has been reached.
 * Note that the function 'FreeRTOS_gethostbyname_a()' will not make use of the
 * macros 'ipconfigDNS_SEND_BLOCK_TIME_TICKS', 'ipconfigDNS_RECEIVE_BLOCK_TIME_TICKS'
 * or 'ipconfigDNS_REQUEST_ATTEMPTS'.
 */
#ifndef ipconfigDNS_USE_CALLBACKS
    #define ipconfigDNS_USE_CALLBACKS    0
#endif

/* Include support for LLMNR: Link-local Multicast Name Resolution. */
#ifndef ipconfigUSE_LLMNR
    #define ipconfigUSE_LLMNR    ( 0 )
#endif

/* Include support for NBNS: NetBIOS Name Server. */
#ifndef ipconfigUSE_NBNS
    #define ipconfigUSE_NBNS    0
#endif

/* It is recommended to let the application respond to incoming ping
 * requests. */
#ifndef ipconfigREPLY_TO_INCOMING_PINGS
    #define ipconfigREPLY_TO_INCOMING_PINGS    1
#endif

/* Add code for outgoing pings as well. */
#ifndef ipconfigSUPPORT_OUTGOING_PINGS
    #define ipconfigSUPPORT_OUTGOING_PINGS    0
#endif

/* A MISRA note: The macros 'ipconfigETHERNET_DRIVER_FILTERS_FRAME_TYPES'
 * and 'ipconfigETHERNET_DRIVER_FILTERS_PACKETS' are too long:
 * the first 32 bytes are equal, which might cause problems
 * for some compilers. */

/* Beside that, there is some overlap between the following 3 macros.  Here is
 * a summary:
 *
 * 1) ipconfigETHERNET_DRIVER_FILTERS_FRAME_TYPES
 *        When disabled, the IP-task will call 'eConsiderFrameForProcessing()'
 *        to check incoming packets.
 * 2) ipconfigETHERNET_DRIVER_FILTERS_PACKETS
 *        When disabled, the IP-task will perform sanity checks on the IP-header,
 *        also checking the target IP address.
 *        Also when disabled, xPortHasUDPSocket() won't be included.  That means
 *        that the IP-task can access the 'xBoundUDPSocketsList' without locking.
 * 3) ipconfigFILTER_OUT_NON_ETHERNET_II_FRAMES
 *        When enabled, the function 'eConsiderFrameForProcessing()' will also
 *        check if the Ethernet frame type is acceptable.
 */

/* The following macro determines if the network interface will
 * do checks on the incoming packets.  When false, the IP-task will
 * perform these checks in the function eConsiderFrameForProcessing().
 *
 * It should be noted that it is most efficient to drop unwanted packets
 * as early as possible.
 */

#ifndef ipconfigETHERNET_DRIVER_FILTERS_FRAME_TYPES
    #define ipconfigETHERNET_DRIVER_FILTERS_FRAME_TYPES    0
#endif

/* When ipconfigETHERNET_DRIVER_FILTERS_PACKETS is enabled, the network
 * interface will inspect the incoming packets to see if they can be
 * accepted.
 * When enabled, the function 'xPortHasUDPSocket()' will become available.
 * That function checks if there is a UDP socket listening to a
 * given port number.
 */
#ifndef ipconfigETHERNET_DRIVER_FILTERS_PACKETS
    #define ipconfigETHERNET_DRIVER_FILTERS_PACKETS    ( 0 )
#endif

/* When defined as 1, the driver will drop all packets with an unknown
 * frame type.
 * This macro is only looked at when 'ipconfigETHERNET_DRIVER_FILTERS_FRAME_TYPES'
 * is disabled.
 */
#ifndef ipconfigFILTER_OUT_NON_ETHERNET_II_FRAMES
    #define ipconfigFILTER_OUT_NON_ETHERNET_II_FRAMES    1
#endif


/* The macro 'ipconfigINCLUDE_EXAMPLE_FREERTOS_PLUS_TRACE_CALLS' was
 * introduced to enable a tracing system.
 * Currently it is only used in BufferAllocation_2.c.
 */
#ifndef configINCLUDE_TRACE_RELATED_CLI_COMMANDS
    #define ipconfigINCLUDE_EXAMPLE_FREERTOS_PLUS_TRACE_CALLS    0
#else
    #define ipconfigINCLUDE_EXAMPLE_FREERTOS_PLUS_TRACE_CALLS    configINCLUDE_TRACE_RELATED_CLI_COMMANDS
#endif

/* This macro will be called in every loop the IP-task makes.  It may be
 * replaced by user-code that triggers a watchdog */
#ifndef ipconfigWATCHDOG_TIMER
    #define ipconfigWATCHDOG_TIMER()
#endif

/* The option 'ipconfigUSE_CALLBACKS' allows to assign specific application
 * hooks to a socket.  Each hook will be set with a specific socket option:
 *
 *  FREERTOS_SO_TCP_CONN_HANDLER  * Callback for (dis) connection events.
 *                                * Supply pointer to 'F_TCP_UDP_Handler_t'
 *  FREERTOS_SO_TCP_RECV_HANDLER  * Callback for receiving TCP data.
 *                                * Supply pointer to 'F_TCP_UDP_Handler_t'
 *  FREERTOS_SO_TCP_SENT_HANDLER  * Callback for sending TCP data.
 *                                * Supply pointer to 'F_TCP_UDP_Handler_t'
 *  FREERTOS_SO_UDP_RECV_HANDLER  * Callback for receiving UDP data.
 *                                * Supply pointer to 'F_TCP_UDP_Handler_t'
 *  FREERTOS_SO_UDP_SENT_HANDLER  * Callback for sending UDP data.
 *                                * Supply pointer to 'F_TCP_UDP_Handler_t'
 *
 * Note that the call-back functions will run in the IP-task, so very little
 * things can be done.  Better not to call any networking API, because that
 * could easily lead to a deadlock situation.
 */
#ifndef ipconfigUSE_CALLBACKS
    #define ipconfigUSE_CALLBACKS    ( 0 )
#endif

/* Replace this macro with a test returning non-zero if the memory pointer to by
 * pxAddress is valid memory which can contain executable code.
 * In fact this is an extra safety measure: if a handler points to invalid memory,
 * it will not be called.
 * The parameter 'pxAddress' is in fact a pointer to a function.
 */
#if ( ipconfigUSE_CALLBACKS != 0 )
    #ifndef ipconfigIS_VALID_PROG_ADDRESS
        #define ipconfigIS_VALID_PROG_ADDRESS( pxAddress )    ( ( pxAddress ) != NULL )
    #endif
#endif

/* The macro 'ipconfigHAS_INLINE_FUNCTIONS' is now deprecated. */
#ifndef ipconfigHAS_INLINE_FUNCTIONS
    #define ipconfigHAS_INLINE_FUNCTIONS    ( 1 )
#endif

/* Since all code is made compatible with the MISRA rules, the inline functions
 * disappear.  'portINLINE' should normally be defined in FreeRTOSCOnfig.h
 */
#ifndef portINLINE
    #define portINLINE    inline
#endif

/* When non-zero, the buffers passed to xNetworkInterfaceOutput() will be passed
 * directly to DMA.  As soon as sending is ready, the buffers must be released by
 * calling vReleaseNetworkBufferAndDescriptor(). */
#ifndef ipconfigZERO_COPY_TX_DRIVER
    #define ipconfigZERO_COPY_TX_DRIVER    ( 0 )
#endif

/* When enabled, the network interface will pass the network buffers directly to
 * the DMA descriptors. When a packet has been received, the function
 * pxPacketBuffer_to_NetworkBuffer() will translate a buffer address to a
 * network packet, so it can be passed to the IP-task. */
#ifndef ipconfigZERO_COPY_RX_DRIVER
    #define ipconfigZERO_COPY_RX_DRIVER    ( 0 )
#endif

/* When ipconfigDRIVER_INCLUDED_TX_IP_CHECKSUM is defined as non-zero,
 * the network interface is responsible for setting the checksums
 * of the outgoing packets.
 * This can be either done in hardware, or by calling the checksum
 * routines from 'xNetworkInterfaceOutput()'.
 */
#ifndef ipconfigDRIVER_INCLUDED_TX_IP_CHECKSUM
    #define ipconfigDRIVER_INCLUDED_TX_IP_CHECKSUM    0
#endif

/* When ipconfigDRIVER_INCLUDED_RX_IP_CHECKSUM is enabled,
 * the network interface is responsible for checking the checksums
 * of the incoming packets.
 * This can be either done in hardware, or by calling the checksum
 * functions.
 */
#ifndef ipconfigDRIVER_INCLUDED_RX_IP_CHECKSUM
    #define ipconfigDRIVER_INCLUDED_RX_IP_CHECKSUM    1
#endif

/* The macro 'ipconfigSOCKET_HAS_USER_SEMAPHORE' is rarely used, yet it
 * can be very useful.  IT applies to both TCP and UDP sockets.
 *
 * The application can register a semaphore ( of type 'SemaphoreHandle_t' )
 * in a socket with the option 'FREERTOS_SO_SET_SEMAPHORE'.
 * Once set, the semaphore will be signalled after every important socket event:
 * READ, WRITE, EXCEPTION.
 * Note that a READ event is also generated for a TCP socket in listen mode,
 * and a WRITE event is generated when a call to connect() has succeeded.
 * Beside that, READ and WRITE are the normal events that occur when
 * data has been received or delivered.
 */
#ifndef ipconfigSOCKET_HAS_USER_SEMAPHORE
    #define ipconfigSOCKET_HAS_USER_SEMAPHORE    0
#endif

/* The macro 'ipconfigSOCKET_HAS_USER_WAKE_CALLBACK' allows to use a call-back
 * function that will be called at the moment one of the above events occurs.
 * Use the socket option 'FREERTOS_SO_WAKEUP_CALLBACK' to install a function
 * of the type 'void callback( Socket_t pxSocket )'.
 * Note that the call-back function runs in the IP-task, so very little things
 * can be done.  Better not to call any networking API, because that could
 * easily lead to a deadlock situation.
 */
#ifndef ipconfigSOCKET_HAS_USER_WAKE_CALLBACK
    #define ipconfigSOCKET_HAS_USER_WAKE_CALLBACK    1
#endif

/* Yet another possibility that makes it easy to handle multiple socket in
 * a single task FreeRTOS_select().  The code for this functionality will
 * be included when 'ipconfigSUPPORT_SELECT_FUNCTION' is defined as non-zero.
 */
#ifndef ipconfigSUPPORT_SELECT_FUNCTION
    #define ipconfigSUPPORT_SELECT_FUNCTION    0
#endif

/* If the select function will be called simultaneously from more that one task
 * e.g. one for 'eSELECT_READ', and the other for the other flags, it is strongly
 * recommended to define 'ipconfigSELECT_USES_NOTIFY'.
 * The problem is 'eSELECT_CALL_IP': 2 tasks would be waiting for the same bit
 * in the event group, and both will try to reset this bit.
 * When ipconfigSELECT_USES_NOTIFY is used, the IP-task will wakeup the calling
 * task by notifying it.
 */
#ifndef ipconfigSELECT_USES_NOTIFY
    #define ipconfigSELECT_USES_NOTIFY    0
#endif

/* TCP only: if the 'ipconfigTCP_KEEP_ALIVE' macro is defined as 1,
 * sockets in state "ESTABLISHED" can be protected using keep-alive packets.
 * These packets will be sent as soon as there hasn't been any activity
 * for a while.
 * The macro 'ipconfigTCP_KEEP_ALIVE_INTERVAL' determines the interval at
 * which keep-alive packets are sent.
 */
#ifndef ipconfigTCP_KEEP_ALIVE
    #define ipconfigTCP_KEEP_ALIVE    1
#endif

/* The period of non-activity ( in seconds ) after which the driver will
 * start sending a keep-alive packet to the TCP peer.  The default is 20 seconds.
 */
#ifndef ipconfigTCP_KEEP_ALIVE_INTERVAL
    #define  ipconfigTCP_KEEP_ALIVE_INTERVAL    20U
#endif

/* Another less used option: signals.  This macro makes it possible to interrupt
 * a blocking call to one of the API's by calling either FreeRTOS_SignalSocket() or
 * FreeRTOS_SignalSocketFromISR() for that socket.
 * When an API got interrupted, it will return the error value -pdFREERTOS_ERRNO_EINTR.
 */
#ifndef ipconfigSUPPORT_SIGNALS
    #define ipconfigSUPPORT_SIGNALS    0
#endif

/* Hang protection can help reduce the impact of SYN floods.
 * When a SYN packet comes in, it will first be checked if there is a listening
 * socket for the port number. If not, it will be replied to with a RESET packet.
 * If there is a listing socket for that port number, a new socket will be created.
 * This socket will be owned temporarily by the IP-task.  Only when the SYN/ACK
 * handshake is finished, the new socket will be passed to the application,
 * resulting in a successful call to FreeRTOS_accept().
 * The option 'ipconfigTCP_HANG_PROTECTION' will make sure that the socket will be
 * deleted in case the SYN-handshake doesn't come to a good end.
 * See also ipconfigTCP_HANG_PROTECTION_TIME time.
 */
#ifndef ipconfigTCP_HANG_PROTECTION
    #define ipconfigTCP_HANG_PROTECTION    1
#endif

/* ipconfigTCP_HANG_PROTECTION_TIME defines the maximum time that a socket stays
 * in one of these "in-between" states:
 *
 * eCONNECT_SYN, eSYN_FIRST, eSYN_RECEIVED, eFIN_WAIT_1, eFIN_WAIT_2, eCLOSING,
 * eLAST_ACK, or eTIME_WAIT.
 */
#ifndef ipconfigTCP_HANG_PROTECTION_TIME
    #define ipconfigTCP_HANG_PROTECTION_TIME    30U
#endif

/* Initially, 'ipconfigTCP_IP_SANITY' was introduced to include all code that checks
 * the correctness of the algorithms.  However, it is only used in BufferAllocation_1.c
 * When defined as non-zero, some extra code will check the validity of network buffers.
 */
#ifndef ipconfigTCP_IP_SANITY
    #define ipconfigTCP_IP_SANITY    0
#endif

/* Expert option: define a value for 'ipBUFFER_PADDING'.
 * When 'ipconfigBUFFER_PADDING' equals 0,
 * 'ipBUFFER_PADDING' will get a default value of 8 + 2 bytes. */
#ifndef ipconfigBUFFER_PADDING
    #define ipconfigBUFFER_PADDING    0U
#endif

/* This library treats the network packets as structs, containing 16- and 32-bit
 * variables.  However, due to the size of the Ethernet header, the 32-byte
 * variables are badly aligned.
 * This is corrected with the macro 'ipconfigPACKET_FILLER_SIZE' which has a default
 * of two.  Thanks to this offset, ( almost ) all 32-bit numbers can be read and
 * written in a single assembler instruction.
 */
#ifndef ipconfigPACKET_FILLER_SIZE
    #define ipconfigPACKET_FILLER_SIZE    2U
#endif

/* Set to 1 if you plan on processing custom Ethernet protocols or protocols
 * that are not yet supported by the FreeRTOS+TCP stack.  If set to 1,
 * the user must define eFrameProcessingResult_t eApplicationProcessCustomFrameHook( NetworkBufferDescriptor_t * const pxNetworkBuffer )
 * which will be called by the stack for any frame with an unsupported EtherType. */
#ifndef ipconfigPROCESS_CUSTOM_ETHERNET_FRAMES
    #define ipconfigPROCESS_CUSTOM_ETHERNET_FRAMES    0
#endif

#endif /* FREERTOS_IP_CONFIG_H */
