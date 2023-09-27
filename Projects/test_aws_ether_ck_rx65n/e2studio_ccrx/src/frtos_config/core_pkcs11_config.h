/*
 * corePKCS11 v3.5.0
 * Copyright (C) 2020 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 * Modifications Copyright (C) 2023 Renesas Electronics Corporation. or its affiliates.
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

/**
 * @file core_pkcs11_config.h
 * @brief List of configuration macros for the corePKCS11 library along with
 * their custom values.
 */

#ifndef CORE_PKCS11_CONFIG_H_
#define CORE_PKCS11_CONFIG_H_

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
 * the logging configuration for PKCS #11.
 * 3. Include the header file "logging_stack.h", if logging is enabled for PKCS #11.
 */
#include "logging_levels.h"

/* Logging configuration for the PKCS #11 library. */
#ifndef LIBRARY_LOG_NAME
    #define LIBRARY_LOG_NAME    "PKCS11"
#endif

#ifndef LIBRARY_LOG_LEVEL
    #define LIBRARY_LOG_LEVEL    LOG_ERROR
#endif

#include "logging_stack.h"
#include "test_execution_config.h"
#include "test_param_config.h"
/************ End of logging configuration ****************/

/**
* @brief Maximum number of sessions that can be stored
* by the PKCS #11 module.
*
* @note The windows test port has an abnormally large value in order to have
* enough sessions to successfully run all the model based PKCS #11 tests.
*
* <b>Possible values:</b> Any positive integer.<br>
* <b>Default value:</b> 10
*/
#define pkcs11configMAX_NUM_OBJECTS    8

/**
* @brief Set to 1 if a PAL destroy object is implemented.
*
* If set to 0, no PAL destroy object is implemented, and this functionality
* is implemented in the common PKCS #11 layer.
*
* <b>Possible values:</b> `0` or `1`<br>
* <b>Default value:</b> `0`
*/
#define pkcs11configPAL_DESTROY_SUPPORTED    1

/**
* @brief Set to 1 if OTA image verification via PKCS #11 module is supported.
*
* If set to 0, OTA code signing certificate is built in via
* aws_ota_codesigner_certificate.h.
*
* <b>Possible values:</b> `0` or `1`<br>
* <b>Default value:</b> `0`
*/
#define pkcs11configOTA_SUPPORTED    1

#define PKCS11_TEST_LABEL_DEVICE_PRIVATE_KEY_FOR_TLS     pkcs11configLABEL_DEVICE_PRIVATE_KEY_FOR_TLS
#define PKCS11_TEST_LABEL_DEVICE_PUBLIC_KEY_FOR_TLS      pkcs11configLABEL_DEVICE_PUBLIC_KEY_FOR_TLS
#define PKCS11_TEST_LABEL_DEVICE_CERTIFICATE_FOR_TLS     pkcs11configLABEL_DEVICE_CERTIFICATE_FOR_TLS

/* *INDENT-OFF* */
#ifdef __cplusplus
    }
#endif
/* *INDENT-ON* */

#endif /* CORE_PKCS11_CONFIG_H_ include guard. */
