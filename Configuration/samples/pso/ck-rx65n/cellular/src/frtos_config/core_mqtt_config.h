/*
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
 */

#ifndef CORE_MQTT_CONFIG_H_
#define CORE_MQTT_CONFIG_H_

/**************************************************/
/******* DO NOT CHANGE the following order ********/
/**************************************************/

/* *INDENT-OFF* */
#ifdef __cplusplus
    extern "C" {
#endif
/* *INDENT-ON* */

/**
 * @ingroup mqtt_constants
 * @brief Maximum number of vectors in subscribe and unsubscribe packet.
 */
#ifndef MQTT_SUB_UNSUB_MAX_VECTORS
    #define MQTT_SUB_UNSUB_MAX_VECTORS    ( 4U )
#endif

/* Include logging header files and define logging macros in the following order:
 * 1. Include the header file "logging_levels.h".
 * 2. Define the LIBRARY_LOG_NAME and LIBRARY_LOG_LEVEL macros depending on
 * the logging configuration for MQTT.
 * 3. Include the header file "logging_stack.h", if logging is enabled for MQTT.
 */

/**
 * @brief The number of retries for receiving CONNACK.
 *
 * The MQTT_MAX_CONNACK_RECEIVE_RETRY_COUNT will be used only when the
 * timeoutMs parameter of #MQTT_Connect is passed as 0 . The transport
 * receive for CONNACK will be retried MQTT_MAX_CONNACK_RECEIVE_RETRY_COUNT
 * times before timing out. A value of 0 for this config will cause the
 * transport receive for CONNACK  to be invoked only once.
 *
 * <b>Possible values:</b> Any positive 16 bit integer. <br>
 * <b>Default value:</b> `5`
 */
#ifndef MQTT_MAX_CONNACK_RECEIVE_RETRY_COUNT
/* Default value for the CONNACK receive retries. */
    #define MQTT_MAX_CONNACK_RECEIVE_RETRY_COUNT    ( 5U )
#endif

/**
 * @brief Maximum number of milliseconds to wait for a ping response to a ping
 * request as part of the keep-alive mechanism.
 *
 * If a ping response is not received before this timeout, then
 * #MQTT_ProcessLoop will return #MQTTKeepAliveTimeout.
 *
 * @note If this value is more than half of the keep alive interval, and the
 * server does not receive the previous ping request, then it is likely that the
 * server will disconnect the client before #MQTTKeepAliveTimeout can be returned.
 *
 * @note If a dummy implementation of the #MQTTGetCurrentTimeFunc_t timer function,
 * is supplied to the library, then the keep-alive mechanism is not supported by the
 * #MQTT_ProcessLoop API function. In that case, the value of #MQTT_PINGRESP_TIMEOUT_MS
 * is irrelevant to the behavior of the library.
 *
 * <b>Possible values:</b> Any positive integer up to SIZE_MAX. <br>
 * <b>Default value:</b> `5000`
 */
#ifndef MQTT_PINGRESP_TIMEOUT_MS
/* Wait 5 seconds by default for a ping response. */
    #define MQTT_PINGRESP_TIMEOUT_MS    ( 5000U )
#endif

/**
 * @brief Maximum number of milliseconds of TX inactivity to wait
 * before initiating a PINGREQ
 *
 * @note If this value is less than the keep alive interval than
 * it will be used instead.
 *
 * <b>Possible values:</b> Any positive integer up to SIZE_MAX. <br>
 * <b>Default value:</b> '30000'
 */
#ifndef PACKET_TX_TIMEOUT_MS
    #define PACKET_TX_TIMEOUT_MS    ( 30000U )
#endif

/**
 * @brief Maximum number of milliseconds of RX inactivity to wait
 * before initiating a PINGREQ
 *
 * <b>Possible values:</b> Any positive integer up to SIZE_MAX. <br>
 * <b>Default value:</b> '30000'
 *
 */
#ifndef PACKET_RX_TIMEOUT_MS
    #define PACKET_RX_TIMEOUT_MS    ( 30000U )
#endif

#include "logging_levels.h"

/* Logging configuration for the MQTT library. */
#ifndef LIBRARY_LOG_NAME
    #define LIBRARY_LOG_NAME    "MQTT"
#endif

#ifndef LIBRARY_LOG_LEVEL
    #define LIBRARY_LOG_LEVEL    LOG_INFO
#endif

#include "logging_stack.h"

/************ End of logging configuration ****************/

/**
 * @brief The maximum number of MQTT PUBLISH messages that may be pending
 * acknowledgement at any time.
 *
 * QoS 1 and 2 MQTT PUBLISHes require acknowledgment from the server before
 * they can be completed. While they are awaiting the acknowledgment, the
 * client must maintain information about their state. The value of this
 * macro sets the limit on how many simultaneous PUBLISH states an MQTT
 * context maintains.
 */
#define MQTT_STATE_ARRAY_MAX_COUNT                   10U

#define MQTT_AGENT_COMMAND_QUEUE_LENGTH              ( 25 )

#define MQTT_COMMAND_CONTEXTS_POOL_SIZE              ( 10 )

/**
 * @brief The maximum number of subscriptions to track for a single connection.
 *
 * @note The MQTT agent keeps a record of all existing MQTT subscriptions.
 * MQTT_AGENT_MAX_SIMULTANEOUS_SUBSCRIPTIONS sets the maximum number of
 * subscriptions records that can be maintained at one time.  The higher this
 * number is the greater the agent's RAM consumption will be.
 */
#define MQTT_AGENT_MAX_SIMULTANEOUS_SUBSCRIPTIONS    ( 10 )

/**
 * @brief Size of statically allocated buffers for holding subscription filters.
 *
 * @note Subscription filters are strings such as "/my/topicname/#".  These
 * strings are limited to a maximum of MQTT_AGENT_MAX_SUBSCRIPTION_FILTER_LENGTH
 * characters. The higher this number is the greater the agent's RAM consumption
 * will be.
 */
#define MQTT_AGENT_MAX_SUBSCRIPTION_FILTER_LENGTH    ( 100 )

/**
 * @brief Dimensions the buffer used to serialize and deserialize MQTT packets.
 * @note Specified in bytes.  Must be large enough to hold the maximum
 * anticipated MQTT payload.
 */
#define MQTT_AGENT_NETWORK_BUFFER_SIZE               ( 5000 )

/**
 * @brief The maximum duration between non-empty network reads while
 * receiving an MQTT packet via the #MQTT_ProcessLoop or #MQTT_ReceiveLoop
 * API functions.
 *
 * When an incoming MQTT packet is detected, the transport receive function
 * may be called multiple times until all of the expected number of bytes of the
 * packet are received. This timeout represents the maximum polling duration that
 * is allowed without any data reception from the network for the incoming packet.
 *
 * If the timeout expires, the #MQTT_ProcessLoop and #MQTT_ReceiveLoop functions
 * return #MQTTRecvFailed.
 *
 * @note If a dummy implementation of the #MQTTGetCurrentTimeFunc_t timer function,
 * is supplied to the library, then #MQTT_RECV_POLLING_TIMEOUT_MS MUST be set to 0.
 *
 * <b>Possible values:</b> Any positive 32 bit integer. Recommended to use a
 * small timeout value. <br>
 * <b>Default value:</b> `10`
 *
 */
#ifndef MQTT_RECV_POLLING_TIMEOUT_MS
    #define MQTT_RECV_POLLING_TIMEOUT_MS    ( 500 )
#endif

/**
 * @brief The maximum duration allowed to send an MQTT packet over the transport
 * interface.
 *
 * When sending an MQTT packet, the transport send or writev functions may be
 * called multiple times until all of the required number of bytes are sent.
 * This timeout represents the maximum duration that is allowed to send the MQTT
 * packet while calling the transport send or writev functions.
 *
 * If the timeout expires, #MQTTSendFailed will be returned by the public API
 * functions.
 *
 * @note If a dummy implementation of the #MQTTGetCurrentTimeFunc_t timer function,
 * is supplied to the library, then #MQTT_SEND_TIMEOUT_MS MUST be set to 0.
 *
 * <b>Possible values:</b> Any positive 32 bit integer. <br>
 * <b>Default value:</b> `20000`
 *
 */
#ifndef MQTT_SEND_TIMEOUT_MS
    #define MQTT_SEND_TIMEOUT_MS    ( 20000U )
#endif

/**
 * @brief Maximum wait time in milliseconds for MQTT agent on the input queue.
 * If there is no events(commands) on the queue for the period, then MQTT agent
 * switches to receive packet from network.
 *
 * Since the transport interface is non blocking, the queue wait time is set to
 * a interval such that agent does not polls busy on the socket.
 *
 */
#define MQTT_AGENT_MAX_EVENT_QUEUE_WAIT_TIME         ( 50 )
#endif

#ifdef MQTT_SEND_RETRY_TIMEOUT_MS
    #error MQTT_SEND_RETRY_TIMEOUT_MS is deprecated. Instead use MQTT_SEND_TIMEOUT_MS.
#endif

/* *INDENT-OFF* */
#ifdef __cplusplus
    }
#endif
/* *INDENT-ON* */
