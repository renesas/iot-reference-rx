/*
 * FreeRTOS FreeRTOS LTS Qualification Tests preview
 * Copyright (C) 2022 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 * Modifications Copyright (C) 2025 Renesas Electronics Corporation or its affiliates.
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
 * @file test_param_config.h
 * @brief This setup the test parameters for LTS qualification test.
 */

#ifndef TEST_PARAM_CONFIG_H
#define TEST_PARAM_CONFIG_H

/**
 * @brief Flag to enable or disable provisioning mode for the demo.
 * Enabling the flags starts a CLI task, so that user can perform provisioning of the device through
 * a serial terminal. Provisioning involves running commands to fetch or set the PKI and configuration
 * information for the device to connect to broker and perform OTA updates. Disabling the flag results
 * in disabling the CLI task and execution of the demo tasks in normal device operation mode.
 */
#define appmainPROVISIONING_MODE                  ( 1 )

/**
 * @brief Configuration that indicates if the device should generate a key pair.
 *
 * @note When FORCE_GENERATE_NEW_KEY_PAIR is set to 1, the device should generate
 * a new on-device key pair and output public key. When set to 0, the device
 * should keep existing key pair.
 *
 * #define FORCE_GENERATE_NEW_KEY_PAIR   0
 */

/**
 * @brief Endpoint of the MQTT broker to connect to in mqtt test.
 *
 * #define MQTT_SERVER_ENDPOINT   "PLACE_HOLDER"
 */
#define MQTT_SERVER_ENDPOINT   "PLACE_HOLDER"

/**
 * @brief Port of the MQTT broker to connect to in mqtt test.
 *
 * #define MQTT_SERVER_PORT       (8883)
 */
#define MQTT_SERVER_PORT        ( 8883 )

/**
 * @brief Endpoint of the echo server to connect to in transport interface test.
 *
 * #define ECHO_SERVER_ENDPOINT   "PLACE_HOLDER"
 */
#define ECHO_SERVER_ENDPOINT    "192.168.1.1"

/**
 * @brief Port of the echo server to connect to in transport interface test.
 *
 * #define ECHO_SERVER_PORT       (9000)
 */
#define ECHO_SERVER_PORT        ( 9000 )

/**
 * @brief Root certificate of the echo server.
 *
 * @note This certificate should be PEM-encoded.
 *
 * Must include the PEM header and footer:
 * "-----BEGIN CERTIFICATE-----\n"\
 * "...base64 data...\n"\
 * "-----END CERTIFICATE-----\n"
 *
 * #define ECHO_SERVER_ROOT_CA "PLACE_HOLDER"
 */
#define ECHO_SERVER_ROOT_CA "PLACE_HOLDER"


/**
 * @brief Client certificate to connect to echo server.
 *
 * @note This certificate should be PEM-encoded.
 *
 * Must include the PEM header and footer:
 * "-----BEGIN CERTIFICATE-----\n"\
 * "...base64 data...\n"\
 * "-----END CERTIFICATE-----\n"
 *
 * #define TRANSPORT_CLIENT_CERTIFICATE NULL
 */
#define TRANSPORT_CLIENT_CERTIFICATE (NULL)

/**
 * @brief Client private key to connect to echo server.
 *
 * @note This is should only be used for testing purpose.
 *
 * For qualification, the key should be generated on-device.
 *
 * #define TRANSPORT_CLIENT_PRIVATE_KEY  NULL
 */
/**
 * @brief Port of the MQTT broker to connect to in mqtt test.
 *
 * #define MQTT_SERVER_PORT       (8883)
 */
#define TRANSPORT_CLIENT_PRIVATE_KEY  (NULL)

/**
 * @brief Client certificate to connect to MQTT server.
 *
 * @note This certificate should be PEM-encoded.
 *
 * Must include the PEM header and footer:
 * "-----BEGIN CERTIFICATE-----\n"\
 * "...base64 data...\n"\
 * "-----END CERTIFICATE-----\n"
 *
 * #define MQTT_CLIENT_CERTIFICATE NULL
 */
#define MQTT_CLIENT_CERTIFICATE "PLACE_HOLDER"

/**
 * @brief Client private key to connect to MQTT server.
*
* @note This is should only be used for testing purpose.
*
* For qualification, the key should be generated on-device.
*
* #define MQTT_CLIENT_PRIVATE_KEY  NULL
*/
#define MQTT_CLIENT_PRIVATE_KEY  "PLACE_HOLDER"

/**
 * @brief The IoT Thing name for the device for OTA test and MQTT test.
 *
 * #define IOT_THING_NAME  "PLACE_HOLDER"
 */
#define IOT_THING_NAME                                   "PLACE_HOLDER"

/**
 * @brief The MQTT client identifier used in MQTT test.  Each client identifier
 * must be unique; so edit as required to ensure that no two clients connecting to
 * the same broker use the same client identifier.
 *
 * #define MQTT_TEST_CLIENT_IDENTIFIER                "insert here."
 */
#define MQTT_TEST_CLIENT_IDENTIFIER                IOT_THING_NAME

/**
 * @brief Network buffer size specified in bytes. Must be large enough to hold the maximum
 * anticipated MQTT payload.
 *
 * #define MQTT_TEST_NETWORK_BUFFER_SIZE            ( 5000 )
 */
#define MQTT_TEST_NETWORK_BUFFER_SIZE    ( 5000 )

/**
 * @brief Client private key to connect to echo server.
 *
 * @note This is should only be used for testing purpose.
 *
 * For qualification, the key should be generated on-device.
 *
 * #define TRANSPORT_CLIENT_PRIVATE_KEY  NULL
 */
#define PKCS11_TEST_RSA_KEY_SUPPORT                      ( 0 )
#define PKCS11_TEST_EC_KEY_SUPPORT                       ( 1 )
#define PKCS11_TEST_IMPORT_PRIVATE_KEY_SUPPORT           ( 0 )
#define PKCS11_TEST_GENERATE_KEYPAIR_SUPPORT             ( 0 )
#define PKCS11_TEST_PREPROVISIONED_SUPPORT               ( 1 )

#define PKCS11_TEST_JITP_CODEVERIFY_ROOT_CERT_SUPPORTED   (0)

#define OTA_RSA_SHA1                                     (1)
#define OTA_RSA_SHA256                                   (2)
#define OTA_ECDSA_SHA256                                 (3)

/**
 * @brief Certificate type for OTA PAL test.
 * Valid options are: OTA_RSA_SHA1, OTA_RSA_SHA256, OTA_ECDSA_SHA256.
 *
 * #define OTA_PAL_TEST_CERT_TYPE OTA_ECDSA_SHA256
 */
#define OTA_PAL_TEST_CERT_TYPE                           OTA_ECDSA_SHA256

/**
 * @brief Path to cert for OTA test PAL. Used to verify signature.
 * If applicable, the device must be pre-provisioned with this certificate. Please see
 * test/common/ota/test_files for the set of certificates.
 */
#define OTA_PAL_CERTIFICATE_FILE                         "ecdsa-sha256-signer.crt.pem"

/**
 * @brief Some devices have a hard-coded name for the firmware image to boot.
 */
#define OTA_PAL_FIRMWARE_FILE                            "dummy.bin"

/**
 * @brief Some boards OTA PAL layers will use the file names passed into it for the
 * image and the certificates because their non-volatile memory is abstracted by a
 * file system. Set this to 1 if that is the case for your device.
 */
#define OTA_PAL_USE_FILE_SYSTEM                          (0)



/**
 * @brief 1 if using PKCS #11 to access the code sign certificate from NVM.
 */
#define OTA_PAL_READ_CERTIFICATE_FROM_NVM_WITH_PKCS11    (1)

#define OTA_APP_VERSION_MAJOR                            (0)

#define OTA_APP_VERSION_MINOR                            (9)

#define OTA_APP_VERSION_BUILD                            (1)

#define OUTGOING_PUBLISH_RECORD_COUNT                    ( 10 )
#define INCOMING_PUBLISH_RECORD_COUNT                    ( 10 )

#define MQTT_TEST_PROCESS_LOOP_TIMEOUT_MS                (1000)
#endif /* TEST_PARAM_CONFIG_H */
