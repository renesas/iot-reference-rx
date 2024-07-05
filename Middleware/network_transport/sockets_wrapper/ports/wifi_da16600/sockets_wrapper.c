/*
 * FreeRTOS V202112.00
 * Copyright (C) 2020 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
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
 * https://www.FreeRTOS.org
 * https://github.com/FreeRTOS
 *
 */

/**
 * @file sockets_wrapper.c
 * @brief FreeRTOS Sockets connect and disconnect wrapper implementation.
 */

/* Standard includes. */
#include <string.h>
#include <stdio.h>

/* FreeRTOS includes. */
#include "FreeRTOS.h"
#include "tcp_sockets_wrapper.h"

/* FreeRTOS includes. */
#include "FreeRTOS.h"
#include "list.h"
#include "semphr.h"
#include "task.h"
#include "core_pkcs11.h"
#include "iot_crypto.h"
#include "r_wifi_da16xxx_if.h"
#include "user_tcp_hook_config.h"

#define MALLOC_SOCKET_CREATION_ERROR -50
#define NO_SOCKET_CREATION_ERROR -51

/* Cast uint32_t -> uint8_t */
typedef union
{
    uint32_t    ul;
    struct
    {
        uint8_t hh;
        uint8_t hl;
        uint8_t lh;
        uint8_t ll;
    } b;
} u_cast_t;

typedef struct xSOCKETContext
{
    Socket_t xSocket;
    uint32_t ulSendTimeout;
    uint32_t ulRecvTimeout;
    uint32_t socket_no;
} SSOCKETContext_t, * SSOCKETContextPtr_t;

/**
 * @brief Maximum number of sockets.
 *
 * 16 total sockets
 */
#define MAX_NUM_SSOCKETS    (WIFI_CFG_TCP_CREATABLE_SOCKETS)

/**
 * @brief Number of secure sockets allocated.
 *
 * Keep a count of the number of open sockets.
 */
static uint32_t s_sockets_num_allocated = 0;

/*-----------------------------------------------------------*/
#define FORCE_RESET     1
#define NO_FORCE_RESET  0
static volatile uint32_t count_module_comm = 0;
static wifi_err_t SocketErrorHook( wifi_err_t error, bool force_reset );
static wifi_err_t CloseSocket(uint32_t socket_number);

#if (0 == USER_TCP_HOOK_ENABLED)
static wifi_err_t SocketErrorHook( wifi_err_t error, bool force_reset )
{
    (void) force_reset;
    return error;
}
#else
static wifi_err_t SocketErrorHook( wifi_err_t error, bool force_reset )
{
    uint32_t reconnect_tries = 0;

    if (WIFI_ERR_MODULE_COM != error && WIFI_ERR_MODULE_TIMEOUT != error)
    {
        count_module_comm = 0;
        return error;
    }

    if (FORCE_RESET == force_reset)
    {
        LogInfo(("Start resetting Wi-Fi Hardware due to error = %d \r\n", error));
        for (reconnect_tries = 0; reconnect_tries < USER_RECONNECT_TRIES; reconnect_tries++)
        {
            LogInfo(("Tried to connect %d times \r\n", reconnect_tries + 1));
            if (WIFI_SUCCESS == R_WIFI_DA16XXX_HardwareReset())
            {
                break;
            }
        }
        if (USER_RECONNECT_TRIES <= reconnect_tries)
        {
            LogError(("Failed after tried to connect %d times", reconnect_tries));
        }
        return error;
    }
    else
    {
    	if (WIFI_ERR_MODULE_COM == error)
    	{
            count_module_comm++;
            LogInfo(("Wi-Fi has error = %d in %d times \r\n", error, count_module_comm));
            if (USER_COMM_ERROR_TRIES > count_module_comm)
            {
                return WIFI_SUCCESS;
            }
    	}

    	if ((USER_COMM_ERROR_TRIES <= count_module_comm) || (WIFI_ERR_MODULE_COM != error))
    	{
            if (USER_COMM_ERROR_TRIES <= count_module_comm)
            {
                LogInfo(("Start no force reset Wi-Fi Hardware due to the continuation of %d error \r\n", error));
            }

            if ( WIFI_ERR_MODULE_COM != error )
            {
                LogInfo(("Start no force reset Wi-Fi Hardware due to error = %d \r\n", error));
            }
            LogInfo(("Resetting Wi-Fi Hardware \r\n"));

            count_module_comm = 0;
            for (reconnect_tries = 0; reconnect_tries < USER_RECONNECT_TRIES; reconnect_tries++)
            {
                LogInfo(("Tried to connect %d times \r\n", reconnect_tries + 1));
                if (WIFI_SUCCESS == R_WIFI_DA16XXX_HardwareReset())
                {
                    break;
                }
            }
            if (USER_RECONNECT_TRIES <= reconnect_tries)
            {
                LogError(("Failed after tried to connect %d times", reconnect_tries));
            }
    	}

        return error;
    }
}
#endif

static wifi_err_t CloseSocket(uint32_t socket_number)
{
    int32_t count;
    wifi_err_t ret;

    for (count = 0; count < USER_CLOSE_SOCKET_TRIES; count++)
    {
        /* Close sockets. */
        ret = R_WIFI_DA16XXX_CloseSocket(socket_number);
        if (ret == WIFI_SUCCESS)
        {
            break;
        }
        LogInfo(("Try to close in %d times.", count));
    }

    return ret;
}

/*-----------------------------------------------------------*/

BaseType_t TCP_Sockets_Connect(Socket_t * pTcpSocket,
                               const char * pHostName,
                               uint16_t port,
                               uint32_t receiveTimeoutMs,
                               uint32_t sendTimeoutMs)
{
    BaseType_t socketStatus = TCP_SOCKETS_ERRNO_NONE;
    wifi_err_t ret;
    SSOCKETContextPtr_t pxContext = NULL;
    uint32_t socketId = 0;
    uint8_t ipAddress[4] = { 0 };

    /* Ensure that only supported values are supplied. */
    configASSERT( pTcpSocket != NULL );
    configASSERT( pHostName != NULL );

    R_WIFI_DA16XXX_GetAvailableSocket(&socketId);

    if((s_sockets_num_allocated > WIFI_CFG_TCP_CREATABLE_SOCKETS) || (socketId == UINT8_MAX))
    {
        LogError(("There is no available socket.\r\n"));
        socketStatus = NO_SOCKET_CREATION_ERROR;
        *pTcpSocket = NULL;
        return socketStatus;
    }

    ret = R_WIFI_DA16XXX_CreateSocket(socketId, WIFI_SOCKET_TYPE_TCP_CLIENT, 4);
    if(WIFI_SUCCESS != ret)
    {
        LogError(("Failed to create WiFi sockets. %d\r\n", ret));
        socketStatus = NO_SOCKET_CREATION_ERROR;
    }
    else
    {
        /* Allocate the internal context structure. */
        if( NULL == ( pxContext = pvPortMalloc( sizeof( SSOCKETContext_t ) ) ) )
        {
            LogError(("Create malloc error\r\n"));
            /* Closing created socket */
            CloseSocket(socketId);
            *pTcpSocket = NULL;
            socketStatus = MALLOC_SOCKET_CREATION_ERROR;
        }
        else
        {
            /* Create the wrapped socket. */
            memset( pxContext, 0, sizeof( SSOCKETContext_t ) );
            pxContext->socket_no     = socketId;
            pxContext->xSocket       = (Socket_t) pxContext;
            pxContext->ulRecvTimeout = receiveTimeoutMs;
            pxContext->ulSendTimeout = sendTimeoutMs;

            s_sockets_num_allocated++;
            LogInfo(( "Created new TCP socket." ));

            ret = R_WIFI_DA16XXX_DnsQuery((uint8_t *)pHostName, ipAddress);
            if(WIFI_SUCCESS != ret)
            {
                LogError( ( "Failed to connect to server: DNS resolution failed: Hostname=%s.",
                            pHostName ) );
                socketStatus = (BaseType_t) ret;
            }
        }
    }

    if (WIFI_SUCCESS == socketStatus)
    {
        ret = R_WIFI_DA16XXX_TcpConnect(pxContext->socket_no, ipAddress, port);
        if( WIFI_SUCCESS != ret )
        {
            LogError( ( "Failed to connect to server: Connect failed: ReturnCode=%d,"
                        " Hostname=%u.%u.%u.%u, Port=%u.",
                        ret,
                        ipAddress[0], ipAddress[1], ipAddress[2], ipAddress[3],
                        port) );
            socketStatus = (BaseType_t) ret;
        }
    }

    if( WIFI_SUCCESS != socketStatus )
    {
        SocketErrorHook(socketStatus, FORCE_RESET);
        LogError(( "Failed to create new socket." ));
    }
    else
    {
        LogInfo( ( "Established TCP connection with %s.", pHostName ) );
    }

    /* Set the socket. */
    *pTcpSocket = (Socket_t )pxContext;

    return socketStatus;
}

int32_t TCP_Sockets_Recv(Socket_t xSocket,
                         void * pvBuffer,
                         size_t xBufferLength)
{
    BaseType_t receive_byte;
    SSOCKETContextPtr_t pxContext = ( SSOCKETContextPtr_t ) xSocket; /*lint !e9087 cast used for portability. */

    receive_byte = R_WIFI_DA16XXX_ReceiveSocket(pxContext->socket_no, pvBuffer, xBufferLength, pxContext->ulRecvTimeout);
    if (receive_byte < 0)
    {
        if ((receive_byte == WIFI_ERR_NOT_CONNECT) || (receive_byte == WIFI_ERR_TAKE_MUTEX))
        {
            receive_byte = 0;
            /* reset the counter of WIFI_ERR_MODULE_COM */
            count_module_comm = 0;
        }
        else if (receive_byte == WIFI_ERR_SOCKET_NUM)
        {
            /* reset the counter of WIFI_ERR_MODULE_COM */
            count_module_comm = 0;
            CloseSocket(pxContext->socket_no);
        }
        else
        {
            receive_byte = SocketErrorHook(receive_byte, NO_FORCE_RESET);
        }
    }
    else
    {
        /* reset the counter of WIFI_ERR_MODULE_COM */
        count_module_comm = 0;
    }
    return receive_byte;
}

int32_t TCP_Sockets_Send(Socket_t xSocket,
                         const void * pvBuffer,
                         size_t xDataLength)
{
    BaseType_t send_byte;
    SSOCKETContextPtr_t pxContext = ( SSOCKETContextPtr_t ) xSocket; /*lint !e9087 cast used for portability. */

    send_byte = R_WIFI_DA16XXX_SendSocket(pxContext->socket_no, (uint8_t *)pvBuffer, xDataLength, pxContext->ulSendTimeout);
    if (send_byte < 0)
    {
        if ((send_byte == WIFI_ERR_NOT_CONNECT) || (send_byte == WIFI_ERR_TAKE_MUTEX))
        {
            send_byte = 0;
            /* reset the counter of WIFI_ERR_MODULE_COM */
            count_module_comm = 0;
        }
        else if (send_byte == WIFI_ERR_SOCKET_NUM)
        {
            /* reset the counter of WIFI_ERR_MODULE_COM */
            count_module_comm = 0;
            CloseSocket(pxContext->socket_no);
        }
        else
        {
            send_byte = SocketErrorHook(send_byte, NO_FORCE_RESET);
        }
    }
    else
    {
        /* reset the counter of WIFI_ERR_MODULE_COM */
        count_module_comm = 0;
    }
    return send_byte;
}

void TCP_Sockets_Disconnect(Socket_t tcpSocket)
{
    SSOCKETContextPtr_t pxContext   = ( SSOCKETContextPtr_t ) tcpSocket; /*lint !e9087 cast used for portability. */
    wifi_err_t ret;

    if(( NULL == pxContext ) || (tcpSocket == SOCKETS_INVALID_SOCKET) || (pxContext->socket_no >= MAX_NUM_SSOCKETS))
    {
        return;
    }

    ret = CloseSocket(pxContext->socket_no);

    if (WIFI_SUCCESS == ret)
    {
        LogInfo(( "Closed Socket: Socket Number = %d.", pxContext->socket_no ));
    }
    else
    {
        LogInfo(( "Try to close but failed to close Socket: Socket Number = %d with %d.", pxContext->socket_no, ret ));
    }

    vPortFree(pxContext);
    pxContext = NULL;

    s_sockets_num_allocated--;
}
