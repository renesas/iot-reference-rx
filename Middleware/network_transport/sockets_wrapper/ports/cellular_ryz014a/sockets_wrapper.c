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
#include "FreeRTOSIPConfig.h"
#include "list.h"
#include "semphr.h"
#include "task.h"
#include "core_pkcs11.h"
#include "iot_crypto.h"
#include "r_cellular_if.h"

typedef struct xSOCKETContext
{
	Socket_t xSocket;
	uint32_t receiveTimeout;
	uint32_t sendTimeout;
    uint32_t socket_no;
} cellularSocketWrapper_t,* xSOCKETContextPtr_t;
extern st_cellular_ctrl_t cellular_ctrl;
/**@} */
/*-----------------------------------------------------------*/

/*-----------------------------------------------------------*/

BaseType_t TCP_Sockets_Connect( Socket_t * pTcpSocket,
                            const char * pHostName,
                            uint16_t port,
                            uint32_t receiveTimeoutMs,
                            uint32_t sendTimeoutMs )
{
	xSOCKETContextPtr_t pxContext = NULL;
	BaseType_t socketStatus = 0;
    st_cellular_ipaddr_t ip_addr = {0};

    configASSERT( pTcpSocket != NULL );
    configASSERT( pHostName != NULL );

    pxContext = pvPortMalloc(sizeof(cellularSocketWrapper_t));
    memset( pxContext, 0, sizeof( cellularSocketWrapper_t ) );
    socketStatus = R_CELLULAR_CreateSocket(&cellular_ctrl, CELLULAR_PROTOCOL_TCP, CELLULAR_PROTOCOL_IPV4);

    if(0 >= socketStatus)
	{
    	LogError( ( "Failed to create new socket." ) );
    	return socketStatus;
	}
	else
	{
		LogInfo ( ( "Created new TCP socket." ) );
		pxContext->socket_no = socketStatus;
		pxContext->xSocket = (Socket_t)pxContext;
		pxContext->receiveTimeout = receiveTimeoutMs;
		pxContext->sendTimeout = sendTimeoutMs;

		socketStatus = R_CELLULAR_DnsQuery(&cellular_ctrl, (uint8_t *)pHostName, CELLULAR_PROTOCOL_IPV4, &ip_addr);

		if( (CELLULAR_SUCCESS != socketStatus))
		{
			LogError( ( "Failed to connect to server: DNS resolution failed: Hostname=%s.",
						pHostName ) );
		}
	}

    if( CELLULAR_SUCCESS == socketStatus )
	{
		/* Establish connection. */
		LogDebug( ( "Creating TCP Connection to %s.", pHostName ) );
		socketStatus = R_CELLULAR_ConnectSocket(&cellular_ctrl, pxContext->socket_no, ip_addr.ipv4, port);

		if( CELLULAR_SUCCESS != socketStatus )
		{
			LogError( ( "Failed to connect to server: Connect failed: ReturnCode=%d,"
						" Hostname=%s, Port=%u.",
						socketStatus,
						pHostName,
						port ) );
		}
	}

    /* Clean up on failure. */
	if( CELLULAR_SUCCESS != socketStatus )
	{
		LogError( ( "Close Socket: Socket Number = %d.",pxContext->socket_no ) );
		R_CELLULAR_ShutdownSocket(&cellular_ctrl, pxContext->socket_no);
		R_CELLULAR_CloseSocket(&cellular_ctrl, pxContext->socket_no);
		vPortFree( pxContext );
	}
	else
	{
		/* Set the socket. */
		*pTcpSocket = (Socket_t )pxContext;
		LogInfo( ( "Established TCP connection with %s.", pHostName ) );
	}

	return socketStatus;

}


int32_t TCP_Sockets_Recv( Socket_t xSocket,
                      void * pvBuffer,
                      size_t xBufferLength )
{

	BaseType_t receive_byte;
	xSOCKETContextPtr_t pxContext =  (xSOCKETContextPtr_t) xSocket; /*lint !e9087 cast used for portability. */

   receive_byte = R_CELLULAR_ReceiveSocket(&cellular_ctrl, pxContext->socket_no, (uint8_t *)pvBuffer, xBufferLength, pxContext->receiveTimeout);

   return receive_byte;
}

/*-----------------------------------------------------------*/

/* This function sends the data until timeout or data is completely sent to server.
 * Send timeout unit is TickType_t. Any timeout value greater than UINT32_MAX_MS_TICKS
 * or portMAX_DELAY will be regarded as MAX delay. In this case, this function
 * will not return until all bytes of data are sent successfully or until an error occurs. */
int32_t TCP_Sockets_Send( Socket_t xSocket,
                      const void * pvBuffer,
                      size_t xDataLength )
{

	xSOCKETContextPtr_t pxContext =  (xSOCKETContextPtr_t) xSocket; /*lint !e9087 cast used for portability. */

	BaseType_t send_byte = R_CELLULAR_SendSocket(&cellular_ctrl, pxContext->socket_no, (uint8_t *)pvBuffer, xDataLength, pxContext->sendTimeout);
	return send_byte;

}

void TCP_Sockets_Disconnect( Socket_t tcpSocket )
{
	xSOCKETContextPtr_t pxContext = ( xSOCKETContextPtr_t ) tcpSocket; /*lint !e9087 cast used for portability. */

	e_cellular_err_t cellular_ret = R_CELLULAR_ShutdownSocket(&cellular_ctrl, pxContext->socket_no);
	cellular_ret = R_CELLULAR_CloseSocket(&cellular_ctrl, pxContext->socket_no);
	(void) cellular_ret;
	vPortFree( pxContext );
}
