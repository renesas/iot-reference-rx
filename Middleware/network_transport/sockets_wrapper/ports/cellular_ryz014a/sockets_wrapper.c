/*
 * FreeRTOS V202112.00
 * Copyright (C) 2020 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 * Modifications Copyright (C) 2023-2025 Renesas Electronics Corporation or its affiliates.
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
#include "list.h"
#include "semphr.h"
#include "task.h"
#include "core_pkcs11.h"
#include "iot_crypto.h"
#include "tcp_sockets_wrapper.h"

/* Renesas includes. */
#include "r_cellular_if.h"
#include "user_tcp_hook_config.h"

#define MALLOC_SOCKET_CREATION_ERROR -50
#define NO_SOCKET_CREATION_ERROR -51
#define FORCE_RESET   1
#define NO_FORCE_RESET 0
typedef struct xSOCKETContext
{
	uint32_t receiveTimeout;
	uint32_t sendTimeout;
    uint32_t socket_no;
} cellularSocketWrapper_t,* xSOCKETContextPtr_t;
extern st_cellular_ctrl_t cellular_ctrl;
extern e_cellular_err_t SocketErrorHook( e_cellular_err_t error, bool force_reset );
extern void CloseSocket(uint32_t socket_number);
volatile uint32_t count_module_comm = 0;
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

    socketStatus = R_CELLULAR_CreateSocket(&cellular_ctrl, CELLULAR_PROTOCOL_TCP, CELLULAR_PROTOCOL_IPV4);

    /* No socket created */
	if(0 >= socketStatus)
	{
		LogError( ( "Failed to create new socket." ) );

		/* SocketStatus can be 0 in this case. */
		if (0 == socketStatus)
		{
			socketStatus = NO_SOCKET_CREATION_ERROR;
		}
		else
		{
			/* If CELLULAR_ERR_MODULE_TIMEOUT or CELLULAR_ERR_MODULE_COM, reset cellular module and return these errors*/
			(void)USER_TCP_HOOK_FUNCTION(socketStatus, FORCE_RESET);
		}
		*pTcpSocket = NULL;
		return socketStatus;
	}
    else
	{
    	/* Create Malloc for pxContext */
        pxContext = pvPortMalloc(sizeof(cellularSocketWrapper_t));

        /* Failure on Malloc creation */
        if (NULL == pxContext)
        {
        	LogError( ( "Failed to allocate new socket context." ) );
        	/* Due to created a socket , need to close it before going to TCP_Socket_Disconnect.
        	 * If not, there is no heap allocation, TCP_Socket_Disconnect can not close the created socket
        	 */

        	CloseSocket(socketStatus);

        	*pTcpSocket = NULL;
        	return MALLOC_SOCKET_CREATION_ERROR;
        }
        else
        {

        	(void)memset( pxContext, 0, sizeof( cellularSocketWrapper_t ));
			LogInfo ( ( "Created new TCP socket." ) );
			/* Assign socket number, timeout to send and receive */
			pxContext->socket_no = socketStatus;
			pxContext->receiveTimeout = receiveTimeoutMs;
			pxContext->sendTimeout = sendTimeoutMs;
        }

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

    /* Assign failure to clean up on TCP_Socket_Disconnect. */
	if( CELLULAR_SUCCESS != socketStatus )
	{
		/* If CELLULAR_ERR_MODULE_TIMEOUT or CELLULAR_ERR_MODULE_COM, reset cellular module and return these errors*/
		(void)USER_TCP_HOOK_FUNCTION(socketStatus, FORCE_RESET);
	}
	else
	{
		LogInfo( ( "Established TCP connection with %s.", pHostName ) );
	}
	/* Set the socket. */
	*pTcpSocket = (Socket_t )pxContext;

	return socketStatus;

}


int32_t TCP_Sockets_Recv( Socket_t xSocket,
                      void * pvBuffer,
                      size_t xBufferLength )
{


	xSOCKETContextPtr_t pxContext =  (xSOCKETContextPtr_t) xSocket; /*lint !e9087 cast used for portability. */
	BaseType_t receive_byte = R_CELLULAR_ReceiveSocket(&cellular_ctrl, pxContext->socket_no, (uint8_t *)pvBuffer, xBufferLength, pxContext->receiveTimeout);

	if (0 >receive_byte)
	{
		if ((CELLULAR_ERR_OTHER_API_RUNNING  == receive_byte)
					|| (CELLULAR_ERR_OTHER_ATCOMMAND_RUNNING  == receive_byte))
		{
			/*Assign to 0 */
			receive_byte = 0;
			/* Reset the counter of CELLULAR_ERR_MODULE_COM */
			count_module_comm = 0;
		}
		else if(CELLULAR_ERR_SOCKET_NOT_READY == receive_byte)
		{
			/* Reset the counter of CELLULAR_ERR_MODULE_COM */
			count_module_comm = 0;
			CloseSocket(pxContext->socket_no);
		}
		else
		{
			/* If CELLULAR_ERR_MODULE_COM comes continuously or
			 * CELLULAR_ERR_MODULE_TIMEOUT, reset cellular module
			 */
			receive_byte =  USER_TCP_HOOK_FUNCTION(receive_byte, NO_FORCE_RESET);
		}
	}
	else
	{
		/* Reset the counter of CELLULAR_ERR_MODULE_COM */
		count_module_comm = 0;
	}

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

	if (0 > send_byte)
	{

		if ((CELLULAR_ERR_NOT_CONNECT == send_byte) ||(CELLULAR_ERR_OTHER_API_RUNNING  == send_byte)
				|| (CELLULAR_ERR_OTHER_ATCOMMAND_RUNNING  == send_byte))
		{
			send_byte = 0;
			/* Reset the counter of CELLULAR_ERR_MODULE_COM */
			count_module_comm = 0;
		}
		else if(CELLULAR_ERR_SOCKET_NOT_READY == send_byte)
		{
			/* Reset the counter of CELLULAR_ERR_MODULE_COM */
			count_module_comm = 0;
			CloseSocket(pxContext->socket_no);
		}
		else
		{
			/* If CELLULAR_ERR_MODULE_COM comes continuously or
			 * CELLULAR_ERR_MODULE_TIMEOUT, reset cellular module
			 */
			send_byte = USER_TCP_HOOK_FUNCTION(send_byte, NO_FORCE_RESET);
		}
	}
	else
	{
		/* Reset the counter of CELLULAR_ERR_MODULE_COM */
		count_module_comm = 0;
	}
	return send_byte;
}

void TCP_Sockets_Disconnect( Socket_t tcpSocket )
{
	xSOCKETContextPtr_t pxContext = ( xSOCKETContextPtr_t ) tcpSocket; /*lint !e9087 cast used for portability. */

	if ( NULL != pxContext )
	{
		if ( 0 != pxContext->socket_no )
		{
			CloseSocket(pxContext->socket_no);
		}
		vPortFree( pxContext );
		pxContext = NULL;
	}
}
