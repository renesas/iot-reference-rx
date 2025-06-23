/*
 * coreMQTT v2.1.1
 * Copyright (C) 2022 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
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
 */

#ifndef CORE_MQTT_CONFIG_H_
#define CORE_MQTT_CONFIG_H_

/* *INDENT-OFF* */
#ifdef __cplusplus
    extern "C" {
#endif
/* *INDENT-ON* */

/**************************************************/
/******* DO NOT CHANGE the following order ********/
/**************************************************/

/* Include logging header files and define logging macros in the following order:
 * 1. Include the header file "logging_levels.h".
 * 2. Define the LIBRARY_LOG_NAME and LIBRARY_LOG_LEVEL macros depending on
 * the logging configuration for MQTT.
 * 3. Include the header file "logging_stack.h", if logging is enabled for MQTT.
 */

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
#define MQTT_RECV_POLLING_TIMEOUT_MS    ( 1000U )

/* *INDENT-OFF* */
#ifdef __cplusplus
    }
#endif
/* *INDENT-ON* */

#endif /* ifndef CORE_MQTT_CONFIG_H_ */
