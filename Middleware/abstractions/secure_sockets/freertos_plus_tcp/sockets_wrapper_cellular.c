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

#include "sockets_wrapper_cellular.h"



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

typedef struct freertos_sockaddr
{
/* _HT_ On 32- and 64-bit architectures, the addition of the two uint8_t
* fields sin_len and sin_family doesn't make the structure bigger, due to alignment.
* These fields are only inserted as a preparation for IPv6
* and are not used in the IPv4-only release. */
    uint8_t sin_len;    /**< length of this structure. */
    uint8_t sin_family; /**< FREERTOS_AF_INET. */
    uint16_t sin_port;  /**< The port. */
    uint32_t sin_addr;  /**< The IP address. */
}freertos_sockaddr_t;

/*-----------------------------------------------------------*/
/* Cellular socket wrapper needs application provide the cellular handle and pdn context id. */
/* User of cellular socket wrapper should provide this variable. */
/* coverity[misra_c_2012_rule_8_6_violation] */
extern st_cellular_ctrl_t cellular_ctrl;


#define SOCKETS_AF_INET     ( 2 )           /*!< IPv4 Internet Protocols. */
#define SOCKETS_PF_INET     SOCKETS_AF_INET /*!< IPv4 Internet Protocol. */
#define SOCKETS_AF_INET6    ( 10 )          /*!< IPv6 Internet Protocols. This option is currently not supported. */
/**@} */
/*-----------------------------------------------------------*/


#define socketsconfigBYTE_ORDER pdFREERTOS_BIG_ENDIAN
/**
 * @brief Convert an unsigned thirty-two-bit value from host endianness to network
 * endianness.
 *
 * @param[in] usIn The unsigned thirty-two-bit value to convert.
 */
#if defined( socketsconfigBYTE_ORDER ) && ( socketsconfigBYTE_ORDER == pdLITTLE_ENDIAN )
    #define SOCKETS_htonl( ulIn )    ( ( uint32_t ) ( ( ( ulIn & 0xFF ) << 24 ) | ( ( ulIn & 0xFF00 ) << 8 ) | ( ( ulIn & 0xFF0000 ) >> 8 ) | ( ( ulIn & 0xFF000000 ) >> 24 ) ) )
#else
    #define SOCKETS_htonl( usIn )    ( ( uint32_t ) ( usIn ) )
#endif

/**
 * @brief Convert an unsigned thirty-two-bit value from network endianness to host
 * endianness.
 *
 * @param[in] usIn The unsigned thirty-two-bit value to convert.
 */
#define SOCKETS_ntohl( usIn )    SOCKETS_htonl( usIn )


/**
 * @brief Convert an unsigned sixteen-bit value from host endianness to network
 * endianness.
 *
 * @param[in] usIn The unsigned sixteen-bit value to convert.
 */

#if defined( socketsconfigBYTE_ORDER ) && ( socketsconfigBYTE_ORDER == pdLITTLE_ENDIAN )
    #define SOCKETS_htons( usIn )    ( ( uint16_t ) ( ( ( usIn ) << 8U ) | ( ( usIn ) >> 8U ) ) )
#else
    #define SOCKETS_htons( usIn )    ( ( uint16_t ) ( usIn ) )
#endif


/**
 * @brief Convert an unsigned sixteen-bit value from network endianness to host
 * endianness.
 *
 * @param[in] usIn The unsigned sixteen-bit value to convert.
 */
#define SOCKETS_ntohs( usIn )    SOCKETS_htons( usIn )

/**
 * @brief Convert an IP address expressed as four separate numeric octets into a an IP address expressed as a 32-bit number in network byte order
 * (for example 192, 168, 0, 100)
 *
 * @param[in] ucOctet0 0th IP Octet
 * @param[in] ucOctet1 1st IP Octet
 * @param[in] ucOctet2 2nd IP Octet
 * @param[in] ucOctet3 3rd IP Octet
 */
#if defined( socketsconfigBYTE_ORDER ) && ( socketsconfigBYTE_ORDER == pdLITTLE_ENDIAN )

    #define SOCKETS_inet_addr_quick( ucOctet0, ucOctet1, ucOctet2, ucOctet3 ) \
    ( ( ( ( uint32_t ) ( ucOctet3 ) ) << 24UL ) |                             \
      ( ( ( uint32_t ) ( ucOctet2 ) ) << 16UL ) |                             \
      ( ( ( uint32_t ) ( ucOctet1 ) ) << 8UL ) |                              \
      ( ( uint32_t ) ( ucOctet0 ) ) )

/**
 * @brief Convert an IP address expressed as a 32-bit number in network byte order to a string in decimal dot notation.
 * (for example "192.168.0.100")
 *
 * @param[in] ulIPAddress An IP address expressed as a 32-bit value in network byte order.
 * @param[in] pucBuffer A pointer to a buffer into which the IP address will be written in decimal dot notation.
 */
    #define SOCKETS_inet_ntoa( ulIPAddress, pucBuffer )               \
    sprintf( ( char * ) ( pucBuffer ), "%u.%u.%u.%u",                 \
             ( ( unsigned ) ( ( ulIPAddress ) & 0xffUL ) ),           \
             ( ( unsigned ) ( ( ( ulIPAddress ) >> 8 ) & 0xffUL ) ),  \
             ( ( unsigned ) ( ( ( ulIPAddress ) >> 16 ) & 0xffUL ) ), \
             ( ( unsigned ) ( ( ulIPAddress ) >> 24 ) ) )

#else /* socketsconfigBYTE_ORDER. */

    #define SOCKETS_inet_addr_quick( ucOctet0, ucOctet1, ucOctet2, ucOctet3 ) \
    ( ( ( ( uint32_t ) ( ucOctet0 ) ) << 24UL ) |                             \
      ( ( ( uint32_t ) ( ucOctet1 ) ) << 16UL ) |                             \
      ( ( ( uint32_t ) ( ucOctet2 ) ) << 8UL ) |                              \
      ( ( uint32_t ) ( ucOctet3 ) ) )

/**
 * @brief Convert an IP address expressed as a 32-bit number in network byte order to a string in decimal dot notation.
 * (for example "192.168.0.100")
 *
 * @param[in] ulIPAddress An IP address expressed as a 32-bit value in network byte order.
 * @param[in] pucBuffer A pointer to a buffer into which the IP address will be written in decimal dot notation.
 */
    #define SOCKETS_inet_ntoa( ulIPAddress, pucBuffer )               \
    sprintf( ( char * ) ( pucBuffer ), "%u.%u.%u.%u",                 \
             ( ( unsigned ) ( ( ulIPAddress ) >> 24 ) ),              \
             ( ( unsigned ) ( ( ( ulIPAddress ) >> 16 ) & 0xffUL ) ), \
             ( ( unsigned ) ( ( ( ulIPAddress ) >> 8 ) & 0xffUL ) ),  \
             ( ( unsigned ) ( ( ulIPAddress ) & 0xffUL ) ) )

#endif /* socketsconfigBYTE_ORDER. */
/*-----------------------------------------------------------*/

static xSOCKETContextPtr_t pxContext = NULL;


BaseType_t Sockets_Connect( Socket_t * pTcpSocket,
                            const char * pHostName,
                            uint16_t port,
                            uint32_t receiveTimeoutMs,
                            uint32_t sendTimeoutMs )
{
	int32_t lStatus = SOCKETS_ERROR_NONE;
	e_cellular_err_t ret;

    freertos_sockaddr_t serverAddress = { 0 };

    pxContext = pvPortMalloc( sizeof( cellularSocketWrapper_t ) ) ;
    memset( pxContext, 0, sizeof( cellularSocketWrapper_t ) );
//    xSemaphoreTake( xUcInUse, xMaxSemaphoreBlockTime);
    LogInfo( ( "Creating TCP Connection to %s.", pHostName ) );
    ret = R_CELLULAR_CreateSocket(&cellular_ctrl, CELLULAR_SOCKET_IP_PROTOCOL_TCP, CELLULAR_SOCKET_IP_VERSION_4);
    if(0 > ret)
	{
		configPRINTF(("create error: %d\r\n",ret));
		lStatus = SOCKETS_SOCKET_ERROR;
	}
	else
	{
		pxContext->socket_no = ret;
		pxContext->xSocket = pxContext;
		pxContext->receiveTimeout = 10000;
		pxContext->sendTimeout = 10000;
	}
    if( SOCKETS_ERROR_NONE != lStatus )
		{
			if(NULL != pxContext)
			{
				vPortFree( pxContext );
			}
			/* Give back the socketInUse mutex. */
			lStatus = SOCKETS_INVALID_SOCKET;
		}

    /* Connection parameters. */
    serverAddress.sin_family = SOCKETS_AF_INET;
    serverAddress.sin_port = SOCKETS_htons( port );
    serverAddress.sin_addr = SOCKETS_GetHostByName( pHostName );
    serverAddress.sin_len = ( uint8_t ) sizeof( serverAddress );


	/* Check for errors from DNS lookup. */
	if( serverAddress.sin_addr == 0U )
	{
		LogError( ( "Failed to connect to server: DNS resolution failed: Hostname=%s.",
					pHostName ) );

	}
	else
	{
		LogInfo( ( "Connecting to TCP Connection %s.", pHostName ) );
		ret = R_CELLULAR_ConnectSocket(&cellular_ctrl, pxContext->socket_no, SOCKETS_GetHostByName( pHostName ), SOCKETS_htons(serverAddress.sin_port));
	}

    if( SOCKETS_ERROR_NONE != ret )
	{
	return SOCKETS_INVALID_SOCKET;
	}
	else
	{

		LogInfo( ( "TCP connected." ) );
		pxContext->receiveTimeout = receiveTimeoutMs;
		pxContext->sendTimeout = sendTimeoutMs;
		*pTcpSocket = (Socket_t )pxContext;
		return ret;
	}
}


int32_t Sockets_Recv( Socket_t xSocket,
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
int32_t Sockets_Send( Socket_t xSocket,
                      const void * pvBuffer,
                      size_t xDataLength )
{
	BaseType_t send_byte;
	xSOCKETContextPtr_t pxContext =  (xSOCKETContextPtr_t) xSocket; /*lint !e9087 cast used for portability. */

	send_byte = R_CELLULAR_SendSocket(&cellular_ctrl, pxContext->socket_no, (uint8_t *)pvBuffer, xDataLength, pxContext->sendTimeout);
	return send_byte;

}

uint32_t SOCKETS_GetHostByName( const char * pcHostName )
{
	uint32_t ulAddr = 0;
	uint32_t ret;

	ret = R_CELLULAR_DnsQuery(&cellular_ctrl, (uint8_t *)pcHostName, &ulAddr);
	if(0 == ret)
	{
		ulAddr = SOCKETS_htonl( ulAddr );
	}
	return ulAddr;
}

void Sockets_Disconnect( Socket_t tcpSocket )
{
	xSOCKETContextPtr_t pxContext = ( xSOCKETContextPtr_t ) tcpSocket; /*lint !e9087 cast used for portability. */

	e_cellular_err_t cellular_ret;

	cellular_ret = R_CELLULAR_ShutdownSocket(&cellular_ctrl, pxContext->socket_no);
	if( ( NULL != pxContext ) && ( SOCKETS_INVALID_SOCKET != pxContext ) )
	{

		R_CELLULAR_CloseSocket(&cellular_ctrl, pxContext->socket_no);
		vPortFree( pxContext );

	}
	else
	{
		return SOCKETS_EINVAL;
	}

	return pdFREERTOS_ERRNO_NONE;
}
