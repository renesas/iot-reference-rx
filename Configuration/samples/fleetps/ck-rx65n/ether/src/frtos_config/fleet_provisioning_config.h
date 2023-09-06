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

#ifndef FLEET_PROVISIONING_CONFIG_H_
#define FLEET_PROVISIONING_CONFIG_H_

/**************************************************/
/******* DO NOT CHANGE the following order ********/
/**************************************************/

/* Include logging header files and define logging macros in the following order:
 * 1. Include the header file "logging_levels.h".
 * 2. Define the LIBRARY_LOG_NAME and LIBRARY_LOG_LEVEL macros.
 * 3. Include the header file "logging_stack.h".
 */

#include "logging_levels.h"

/* Logging configuration for the Fleet Provisioning library. */
#ifndef LIBRARY_LOG_NAME
    #define LIBRARY_LOG_NAME    "FleetProvisioning"
#endif

#ifndef LIBRARY_LOG_LEVEL
    #define LIBRARY_LOG_LEVEL    LOG_INFO
#endif

/* Prototype for the function used to print to console on Windows simulator
 * of FreeRTOS.
 * The function prints to the console before the network is connected;
 * then a UDP port after the network has connected. */
extern void vLoggingPrintf( const char * pcFormatString,
                            ... );

/* Map the SdkLog macro to the logging function to enable logging
 * on Windows simulator. */
#ifndef SdkLog
    #define SdkLog( message )    vLoggingPrintf message
#endif

#include "logging_stack.h"

/* The macro definition for FLEET_PROVISIONING_DO_NOT_USE_CUSTOM_CONFIG is for
 * Doxygen documentation only. */

/**
 * @brief Define this macro to build the AWS IoT Fleet Provisioning Library
 * without the custom config file fleet_provisioning_config.h.
 *
 * Without the custom config, the the AWS IoT Fleet Provisioning Library builds
 * with default values of config macros defined in the
 * fleet_provisioning_config_defaults.h file.
 *
 * If a custom config file is provided, then
 * FLEET_PROVISIONING_DO_NOT_USE_CUSTOM_CONFIG must not be defined.
 *
 * <b>Default value</b>: FLEET_PROVISIONING_DO_NOT_USE_CUSTOM_CONFIG is
 * <b>not</b> defined by default and the library expects a
 * fleet_provisioning_config.h file.
 */
#ifdef DOXYGEN
    #define FLEET_PROVISIONING_DO_NOT_USE_CUSTOM_CONFIG
#endif

/**
 * @brief Macro used in the Fleet Provisioning library to log error messages.
 *
 * To enable error logging, this macro should be mapped to an
 * application-specific logging implementation.
 *
 * @note This logging macro is called in the Fleet Provisioning library with
 * parameters wrapped in double parentheses to be ISO C89/C90 standard
 * compliant. For a reference POSIX implementation of the logging macros, refer
 * to the fleet_provisioning_config.h file, and the logging-stack in demos
 * folder of the [AWS IoT Embedded C SDK
 * repository](https://github.com/aws/aws-iot-device-sdk-embedded-C).
 *
 * <b>Default value</b>: Error logs are turned off, and no code is generated for
 * calls to the macro in the Fleet Provisioning library on compilation.
 */
#ifndef LogError
    #define LogError( message )
#endif

/**
 * @brief Macro used in the Fleet Provisioning library to log warning messages.
 *
 * To enable warning logging, this macro should be mapped to an
 * application-specific logging implementation.
 *
 * @note This logging macro is called in the Fleet Provisioning library with
 * parameters wrapped in double parentheses to be ISO C89/C90 standard
 * compliant. For a reference POSIX implementation of the logging macros, refer
 * to the fleet_provisioning_config.h file, and the logging-stack in demos
 * folder of the [AWS IoT Embedded C SDK
 * repository](https://github.com/aws/aws-iot-device-sdk-embedded-C).
 *
 * <b>Default value</b>: Warning logs are turned off, and no code is generated
 * for calls to the macro in the Fleet Provisioning library on compilation.
 */
#ifndef LogWarn
    #define LogWarn( message )
#endif

/**
 * @brief Macro used in the Fleet Provisioning library to log info messages.
 *
 * To enable info logging, this macro should be mapped to an
 * application-specific logging implementation.
 *
 * @note This logging macro is called in the Fleet Provisioning library with
 * parameters wrapped in double parentheses to be ISO C89/C90 standard
 * compliant. For a reference POSIX implementation of the logging macros, refer
 * to the fleet_provisioning_config.h file, and the logging-stack in demos
 * folder of the [AWS IoT Embedded C SDK
 * repository](https://github.com/aws/aws-iot-device-sdk-embedded-C).
 *
 * <b>Default value</b>: Info logs are turned off, and no code is generated for
 * calls to the macro in the Fleet Provisioning library on compilation.
 */
#ifndef LogInfo
    #define LogInfo( message )
#endif

/**
 * @brief Macro used in the Fleet Provisioning library to log debug messages.
 *
 * To enable debug logging, this macro should be mapped to an
 * application-specific logging implementation.
 *
 * @note This logging macro is called in the Fleet Provisioning library with
 * parameters wrapped in double parentheses to be ISO C89/C90 standard
 * compliant. For a reference POSIX implementation of the logging macros, refer
 * to the fleet_provisioning_config.h file, and the logging-stack in demos
 * folder of the [AWS IoT Embedded C SDK
 * repository](https://github.com/aws/aws-iot-device-sdk-embedded-C).
 *
 * <b>Default value</b>: Debug logs are turned off, and no code is generated for
 * calls to the macro in the Fleet Provisioning library on compilation.
 */
#ifndef LogDebug
    #define LogDebug( message )
#endif

/************ End of logging configuration ****************/

#endif /* ifndef FLEET_PROVISIONING_CONFIG_H_ */
