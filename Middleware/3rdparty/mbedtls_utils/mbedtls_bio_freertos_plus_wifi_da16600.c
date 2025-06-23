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
 * @file mbedtls_bio_freertos_plus_wifi_da16600.c
 * @brief Implements mbed TLS platform send/receive functions for the TCP sockets wrapper.
 */

/* MbedTLS includes. */
#if !defined( MBEDTLS_CONFIG_FILE )
    #include "mbedtls/mbedtls_config.h"
#else
    #include MBEDTLS_CONFIG_FILE
#endif
#include "threading_alt.h"
#include "mbedtls/entropy.h"
#include "mbedtls/ssl.h"

/* TCP Sockets Wrapper include.*/
#include "tcp_sockets_wrapper.h"

/* MbedTLS Bio TCP sockets wrapper include. */
#include "mbedtls_bio_tcp_sockets_wrapper.h"

/**
 * @brief Sends data over TCP socket.
 *
 * @param[in] ctx The network context containing the socket handle.
 * @param[in] buf Buffer containing the bytes to send.
 * @param[in] len Number of bytes to send from the buffer.
 *
 * @return Number of bytes sent on success; else a negative value.
 */
int xMbedTLSBioTCPSocketsWrapperSend( void * ctx,
                                      const unsigned char * buf,
                                      size_t len )
{
    int32_t xReturnStatus;

    configASSERT( ctx != NULL );
    configASSERT( buf != NULL );

    xReturnStatus = TCP_Sockets_Send( ( Socket_t ) ctx, buf, len );

    switch( xReturnStatus )
    {
        case 0:
            xReturnStatus = MBEDTLS_ERR_SSL_WANT_WRITE;
            break;
        default:
            break;
    }

    return ( int ) xReturnStatus;
}

/**
 * @brief Receives data from TCP socket.
 *
 * @param[in] ctx The network context containing the socket handle.
 * @param[out] buf Buffer to receive bytes into.
 * @param[in] len Number of bytes to receive from the network.
 *
 * @return Number of bytes received if successful; Negative value on error.
 */
int xMbedTLSBioTCPSocketsWrapperRecv( void * ctx,
                                      unsigned char * buf,
                                      size_t len )
{
    int32_t xReturnStatus;

    configASSERT( ctx != NULL );
    configASSERT( buf != NULL );

    xReturnStatus = TCP_Sockets_Recv( ( Socket_t ) ctx, buf, len );

    switch( xReturnStatus )
    {
        /* A timeout occurred before any data could be received. */
        case 0:
            xReturnStatus = MBEDTLS_ERR_SSL_WANT_READ;
            break;

        default:
            break;
    }

    return ( int ) xReturnStatus;
}
