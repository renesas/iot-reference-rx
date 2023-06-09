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

/* Include header that defines log levels. */
#include "logging_levels.h"

/* Logging configuration for the Sockets. */
#ifndef LIBRARY_LOG_NAME
    #define LIBRARY_LOG_NAME     "SocketsWrapper"
#endif
#ifndef LIBRARY_LOG_LEVEL
    #define LIBRARY_LOG_LEVEL    LOG_INFO
#endif

extern void vLoggingPrintf( const char * pcFormatString,
                            ... );

#include "logging_stack.h"

/* Standard includes. */
#include <string.h>

/* FreeRTOS includes. */
#include "FreeRTOS.h"

/* FreeRTOS+TCP includes. */
#include "FreeRTOS_IP.h"
#include "FreeRTOS_Sockets.h"
#include "FreeRTOS_DNS.h"

/* TCP Sockets Wrapper include.*/
/* Let sockets wrapper know that Socket_t is defined already. */
#define SOCKET_T_TYPEDEFED
#include "tcp_sockets_wrapper.h"

/* FreeRTOS includes. */
#include "core_pkcs11.h"
#include "iot_crypto.h"

/**
 * @brief Maximum number of times to call FreeRTOS_recv when initiating a graceful shutdown.
 */
#ifndef FREERTOS_SOCKETS_WRAPPER_SHUTDOWN_LOOPS
    #define FREERTOS_SOCKETS_WRAPPER_SHUTDOWN_LOOPS    ( 3 )
#endif

/**
 * @brief negative error code indicating a network failure.
 */
#define FREERTOS_SOCKETS_WRAPPER_NETWORK_ERROR    ( -1 )

/**
 * @brief Establish a connection to server.
 *
 * @param[out] pTcpSocket The output parameter to return the created socket descriptor.
 * @param[in] pHostName Server hostname to connect to.
 * @param[in] pServerInfo Server port to connect to.
 * @param[in] receiveTimeoutMs Timeout (in milliseconds) for transport receive.
 * @param[in] sendTimeoutMs Timeout (in milliseconds) for transport send.
 *
 * @note A timeout of 0 means infinite timeout.
 *
 * @return Non-zero value on error, 0 on success.
 */
BaseType_t TCP_Sockets_Connect( Socket_t * pTcpSocket,
                                const char * pHostName,
                                uint16_t port,
                                uint32_t receiveTimeoutMs,
                                uint32_t sendTimeoutMs )
{
    Socket_t tcpSocket = FREERTOS_INVALID_SOCKET;
    BaseType_t socketStatus = 0;
    struct freertos_sockaddr serverAddress = { 0 };
    TickType_t transportTimeout = 0;

    configASSERT( pTcpSocket != NULL );
    configASSERT( pHostName != NULL );

    /* Create a new TCP socket. */
    tcpSocket = FreeRTOS_socket( FREERTOS_AF_INET, FREERTOS_SOCK_STREAM, FREERTOS_IPPROTO_TCP );

    if( tcpSocket == FREERTOS_INVALID_SOCKET )
    {
        LogError( ( "Failed to create new socket." ) );
        socketStatus = FREERTOS_SOCKETS_WRAPPER_NETWORK_ERROR;
    }
    else
    {
        LogDebug( ( "Created new TCP socket." ) );

        /* Connection parameters. */
        serverAddress.sin_family = FREERTOS_AF_INET;
        serverAddress.sin_port = FreeRTOS_htons( port );
        serverAddress.sin_addr = ( uint32_t ) FreeRTOS_gethostbyname( pHostName );
        serverAddress.sin_len = ( uint8_t ) sizeof( serverAddress );

        /* Check for errors from DNS lookup. */
        if( serverAddress.sin_addr == 0U )
        {
            LogError( ( "Failed to connect to server: DNS resolution failed: Hostname=%s.",
                        pHostName ) );
            socketStatus = FREERTOS_SOCKETS_WRAPPER_NETWORK_ERROR;
        }
    }

    if( socketStatus == 0 )
    {
        /* Establish connection. */
        LogDebug( ( "Creating TCP Connection to %s.", pHostName ) );
        socketStatus = FreeRTOS_connect( tcpSocket, &serverAddress, sizeof( serverAddress ) );

        if( socketStatus != 0 )
        {
            LogError( ( "Failed to connect to server: FreeRTOS_Connect failed: ReturnCode=%d,"
                        " Hostname=%s, Port=%u.",
                        socketStatus,
                        pHostName,
                        port ) );
        }
    }

    if( socketStatus == 0 )
    {
        /* Set socket receive timeout. */
        transportTimeout = pdMS_TO_TICKS( receiveTimeoutMs );
        /* Setting the receive block time cannot fail. */
        ( void ) FreeRTOS_setsockopt( tcpSocket,
                                      0,
                                      FREERTOS_SO_RCVTIMEO,
                                      &transportTimeout,
                                      sizeof( TickType_t ) );

        /* Set socket send timeout. */
        transportTimeout = pdMS_TO_TICKS( sendTimeoutMs );
        /* Setting the send block time cannot fail. */
        ( void ) FreeRTOS_setsockopt( tcpSocket,
                                      0,
                                      FREERTOS_SO_SNDTIMEO,
                                      &transportTimeout,
                                      sizeof( TickType_t ) );
    }

    /* Clean up on failure. */
    if( socketStatus != 0 )
    {
        if( tcpSocket != FREERTOS_INVALID_SOCKET )
        {
            ( void ) FreeRTOS_closesocket( tcpSocket );
        }
    }
    else
    {
        /* Set the socket. */
        *pTcpSocket = tcpSocket;
        LogInfo( ( "Established TCP connection with %s.", pHostName ) );
    }

    return socketStatus;
}

/**
 * @brief End connection to server.
 *
 * @param[in] tcpSocket The socket descriptor.
 */
void TCP_Sockets_Disconnect( Socket_t tcpSocket )
{
    BaseType_t waitForShutdownLoopCount = 0;
    uint8_t pDummyBuffer[ 2 ];

    if( tcpSocket != FREERTOS_INVALID_SOCKET )
    {
        /* Initiate graceful shutdown. */
        ( void ) FreeRTOS_shutdown( tcpSocket, FREERTOS_SHUT_RDWR );

        /* Wait for the socket to disconnect gracefully (indicated by FreeRTOS_recv()
         * returning a FREERTOS_EINVAL error) before closing the socket. */
        while( FreeRTOS_recv( tcpSocket, pDummyBuffer, sizeof( pDummyBuffer ), 0 ) >= 0 )
        {
            /* We don't need to delay since FreeRTOS_recv should already have a timeout. */

            if( ++waitForShutdownLoopCount >= FREERTOS_SOCKETS_WRAPPER_SHUTDOWN_LOOPS )
            {
                break;
            }
        }

        ( void ) FreeRTOS_closesocket( tcpSocket );
    }
}

/**
 * @brief Transmit data to the remote socket.
 *
 * The socket must have already been created using a call to TCP_Sockets_Connect().
 *
 * @param[in] xSocket The handle of the sending socket.
 * @param[in] pvBuffer The buffer containing the data to be sent.
 * @param[in] xDataLength The length of the data to be sent.
 *
 * @return
 * * On success, the number of bytes actually sent is returned.
 * * If an error occurred, a negative value is returned. @ref SocketsErrors
 */
int32_t TCP_Sockets_Send( Socket_t xSocket,
                          const void * pvBuffer,
                          size_t xBufferLength )
{
    BaseType_t xSendStatus;
    int xReturnStatus = TCP_SOCKETS_ERRNO_ERROR;

    configASSERT( xSocket != NULL );
    configASSERT( pvBuffer != NULL );

    xSendStatus = FreeRTOS_send( xSocket, pvBuffer, xBufferLength, 0 );

    switch( xSendStatus )
    {
        /* Socket was closed or just got closed. */
        case -pdFREERTOS_ERRNO_ENOTCONN:
            xReturnStatus = TCP_SOCKETS_ERRNO_ENOTCONN;
            break;

        /* Not enough memory for the socket to create either an Rx or Tx stream. */
        case -pdFREERTOS_ERRNO_ENOMEM:
            xReturnStatus = TCP_SOCKETS_ERRNO_ENOMEM;
            break;

        /* Socket is not valid, is not a TCP socket, or is not bound. */
        case -pdFREERTOS_ERRNO_EINVAL:
            xReturnStatus = TCP_SOCKETS_ERRNO_EINVAL;
            break;

        /* Socket received a signal, causing the read operation to be aborted. */
        case -pdFREERTOS_ERRNO_EINTR:
            xReturnStatus = TCP_SOCKETS_ERRNO_EINTR;
            break;

        /* A timeout occurred before any data could be sent as the TCP buffer was full. */
        case -pdFREERTOS_ERRNO_ENOSPC:
            xReturnStatus = TCP_SOCKETS_ERRNO_ENOSPC;
            break;

        default:
            xReturnStatus = ( int ) xSendStatus;
            break;
    }

    return xReturnStatus;
}

/**
 * @brief Receive data from a TCP socket.
 *
 * The socket must have already been created using a call to TCP_Sockets_Connect().
 *
 * @param[in] xSocket The handle of the socket from which data is being received.
 * @param[out] pvBuffer The buffer into which the received data will be placed.
 * @param[in] xBufferLength The maximum number of bytes which can be received.
 * pvBuffer must be at least xBufferLength bytes long.
 *
 * @return
 * * If the receive was successful then the number of bytes received (placed in the
 *   buffer pointed to by pvBuffer) is returned.
 * * If a timeout occurred before data could be received then 0 is returned (timeout
 *   is set using @ref SOCKETS_SO_RCVTIMEO).
 * * If an error occurred, a negative value is returned. @ref SocketsErrors
 */
int32_t TCP_Sockets_Recv( Socket_t xSocket,
                          void * pvBuffer,
                          size_t xBufferLength )
{
    BaseType_t xRecvStatus;
    int xReturnStatus = TCP_SOCKETS_ERRNO_ERROR;

    configASSERT( xSocket != NULL );
    configASSERT( pvBuffer != NULL );

    xRecvStatus = FreeRTOS_recv( xSocket, pvBuffer, xBufferLength, 0 );

    switch( xRecvStatus )
    {
        /* Socket was closed or just got closed. */
        case -pdFREERTOS_ERRNO_ENOTCONN:
            xReturnStatus = TCP_SOCKETS_ERRNO_ENOTCONN;
            break;

        /* Not enough memory for the socket to create either an Rx or Tx stream. */
        case -pdFREERTOS_ERRNO_ENOMEM:
            xReturnStatus = TCP_SOCKETS_ERRNO_ENOMEM;
            break;

        /* Socket is not valid, is not a TCP socket, or is not bound. */
        case -pdFREERTOS_ERRNO_EINVAL:
            xReturnStatus = TCP_SOCKETS_ERRNO_EINVAL;
            break;

        /* Socket received a signal, causing the read operation to be aborted. */
        case -pdFREERTOS_ERRNO_EINTR:
            xReturnStatus = TCP_SOCKETS_ERRNO_EINTR;
            break;

        default:
            xReturnStatus = ( int ) xRecvStatus;
            break;
    }

    return xReturnStatus;
}

static CK_RV prvSocketsGetCryptoSession( SemaphoreHandle_t * pxSessionLock,
                                         CK_SESSION_HANDLE * pxSession,
                                         CK_FUNCTION_LIST_PTR_PTR ppxFunctionList )
{
    CK_RV xResult = CKR_OK;
    CK_C_GetFunctionList pxCkGetFunctionList = NULL;
    static CK_SESSION_HANDLE xPkcs11Session = 0;
    static CK_FUNCTION_LIST_PTR pxPkcs11FunctionList = NULL;
    static StaticSemaphore_t xStaticSemaphore;
    static SemaphoreHandle_t xSessionLock = NULL;
    CK_ULONG ulCount = 0;
    CK_SLOT_ID * pxSlotIds = NULL;

    /* Check if one-time initialization of the lock is needed.*/
    portENTER_CRITICAL();

    if( NULL == xSessionLock )
    {
        xSessionLock = xSemaphoreCreateMutexStatic( &xStaticSemaphore );
    }

    *pxSessionLock = xSessionLock;
    portEXIT_CRITICAL();

    /* Check if one-time initialization of the crypto handle is needed.*/
    xSemaphoreTake( xSessionLock, portMAX_DELAY );

    if( 0 == xPkcs11Session )
    {
        /* One-time initialization. */

        /* Ensure that the PKCS#11 module is initialized. We don't keep the
         * scheduler stopped here, since we don't want to make assumptions about hardware
         * requirements for accessing a crypto module. */

        pxCkGetFunctionList = C_GetFunctionList;
        xResult = pxCkGetFunctionList( &pxPkcs11FunctionList );

        if( CKR_OK == xResult )
        {
            xResult = xInitializePkcs11Token();
        }

        /* Get the crypto token slot count. */
        if( CKR_OK == xResult )
        {
            xResult = pxPkcs11FunctionList->C_GetSlotList( CK_TRUE,
                                                           NULL,
                                                           &ulCount );
        }

        /* Allocate memory to store the token slots. */
        if( CKR_OK == xResult )
        {
            pxSlotIds = ( CK_SLOT_ID * ) pvPortMalloc( sizeof( CK_SLOT_ID ) * ulCount );

            if( NULL == pxSlotIds )
            {
                xResult = CKR_HOST_MEMORY;
            }
        }

        /* Get all of the available private key slot identities. */
        if( CKR_OK == xResult )
        {
            xResult = pxPkcs11FunctionList->C_GetSlotList( CK_TRUE,
                                                           pxSlotIds,
                                                           &ulCount );
        }

        /* Start a session with the PKCS#11 module. */
        if( CKR_OK == xResult )
        {
            xResult = pxPkcs11FunctionList->C_OpenSession( pxSlotIds[ 0 ],
                                                           CKF_SERIAL_SESSION,
                                                           NULL,
                                                           NULL,
                                                           &xPkcs11Session );
        }
    }

    *pxSession = xPkcs11Session;
    *ppxFunctionList = pxPkcs11FunctionList;
    xSemaphoreGive( xSessionLock );

    if( NULL != pxSlotIds )
    {
        vPortFree( pxSlotIds );
    }

    return xResult;
}
BaseType_t xApplicationGetRandomNumber( uint32_t * pulNumber )
{
    CK_RV xResult = 0;
    SemaphoreHandle_t xSessionLock = NULL;
    CK_SESSION_HANDLE xPkcs11Session = 0;
    CK_FUNCTION_LIST_PTR pxPkcs11FunctionList = NULL;
    uint32_t ulRandomValue = 0;
    BaseType_t xReturn; /* Return pdTRUE if successful */

    xResult = prvSocketsGetCryptoSession( &xSessionLock,
                                          &xPkcs11Session,
                                          &pxPkcs11FunctionList );

    if( 0 == xResult )
    {
        /* Request a sequence of cryptographically random byte values using
         * PKCS#11. */
        xResult = pxPkcs11FunctionList->C_GenerateRandom( xPkcs11Session,
                                                          ( CK_BYTE_PTR ) &ulRandomValue,
                                                          sizeof( ulRandomValue ) );
    }

    /* Check if any of the API calls failed. */
    if( 0 == xResult )
    {
        xReturn = pdTRUE;
        *( pulNumber ) = ulRandomValue;
    }
    else
    {
        xReturn = pdFALSE;
        *( pulNumber ) = 0uL;
    }

    return xReturn;
}
/*-----------------------------------------------------------*/

/**
 * @brief Generate a TCP Initial Sequence Number that is reasonably difficult
 * to predict, per https://tools.ietf.org/html/rfc6528.
 */
uint32_t ulApplicationGetNextSequenceNumber( uint32_t ulSourceAddress,
                                             uint16_t usSourcePort,
                                             uint32_t ulDestinationAddress,
                                             uint16_t usDestinationPort )
{
    CK_RV xResult = CKR_OK;
    SemaphoreHandle_t xSessionLock = NULL;
    CK_SESSION_HANDLE xPkcs11Session = 0;
    CK_FUNCTION_LIST_PTR pxPkcs11FunctionList = NULL;
    CK_MECHANISM xMechSha256 = { 0 };
    uint8_t ucSha256Result[ cryptoSHA256_DIGEST_BYTES ];
    CK_ULONG ulLength = sizeof( ucSha256Result );
    uint32_t ulNextSequenceNumber = 0;
    static uint64_t ullKey;
    static CK_BBOOL xKeyIsInitialized = CK_FALSE;

    /* Acquire a crypto session handle. */
    xResult = prvSocketsGetCryptoSession( &xSessionLock,
                                          &xPkcs11Session,
                                          &pxPkcs11FunctionList );

    if( CKR_OK == xResult )
    {
        xSemaphoreTake( xSessionLock, portMAX_DELAY );

        if( CK_FALSE == xKeyIsInitialized )
        {
            /* One-time initialization, per boot, of the random seed. */
            xResult = pxPkcs11FunctionList->C_GenerateRandom( xPkcs11Session,
                                                              ( CK_BYTE_PTR ) &ullKey,
                                                              sizeof( ullKey ) );

            if( xResult == CKR_OK )
            {
                xKeyIsInitialized = CK_TRUE;
            }
        }

        xSemaphoreGive( xSessionLock );
    }

    /* Lock the shared crypto session. */
    xSemaphoreTake( xSessionLock, portMAX_DELAY );

    /* Start a hash. */
    if( CKR_OK == xResult )
    {
        xMechSha256.mechanism = CKM_SHA256;
        xResult = pxPkcs11FunctionList->C_DigestInit( xPkcs11Session, &xMechSha256 );
    }

    /* Hash the seed. */
    if( CKR_OK == xResult )
    {
        xResult = pxPkcs11FunctionList->C_DigestUpdate( xPkcs11Session,
                                                        ( CK_BYTE_PTR ) &ullKey,
                                                        sizeof( ullKey ) );
    }

    /* Hash the source address. */
    if( CKR_OK == xResult )
    {
        xResult = pxPkcs11FunctionList->C_DigestUpdate( xPkcs11Session,
                                                        ( CK_BYTE_PTR ) &ulSourceAddress,
                                                        sizeof( ulSourceAddress ) );
    }

    /* Hash the source port. */
    if( CKR_OK == xResult )
    {
        xResult = pxPkcs11FunctionList->C_DigestUpdate( xPkcs11Session,
                                                        ( CK_BYTE_PTR ) &usSourcePort,
                                                        sizeof( usSourcePort ) );
    }

    /* Hash the destination address. */
    if( CKR_OK == xResult )
    {
        xResult = pxPkcs11FunctionList->C_DigestUpdate( xPkcs11Session,
                                                        ( CK_BYTE_PTR ) &ulDestinationAddress,
                                                        sizeof( ulDestinationAddress ) );
    }

    /* Hash the destination port. */
    if( CKR_OK == xResult )
    {
        xResult = pxPkcs11FunctionList->C_DigestUpdate( xPkcs11Session,
                                                        ( CK_BYTE_PTR ) &usDestinationPort,
                                                        sizeof( usDestinationPort ) );
    }

    /* Get the hash. */
    if( CKR_OK == xResult )
    {
        xResult = pxPkcs11FunctionList->C_DigestFinal( xPkcs11Session,
                                                       ucSha256Result,
                                                       &ulLength );
    }

    xSemaphoreGive( xSessionLock );

    /* Use the first four bytes of the hash result as the starting point for
     * all initial sequence numbers for connections based on the input 4-tuple. */
    if( CKR_OK == xResult )
    {
        memcpy( &ulNextSequenceNumber,
                ucSha256Result,
                sizeof( ulNextSequenceNumber ) );

        /* Add the tick count of four-tick intervals. In theory, per the RFC
         * (see above), this approach still allows server equipment to optimize
         * handling of connections from the same device that haven't fully timed out. */
        ulNextSequenceNumber += xTaskGetTickCount() / 4;
    }

    return ulNextSequenceNumber;
}
/*-----------------------------------------------------------*/
