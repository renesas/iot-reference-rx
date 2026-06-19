/*
 * AWS IoT Core MQTT File Streams Embedded C v1.1.0
 * Copyright (C) 2023 Amazon.com, Inc. and its affiliates. All Rights Reserved.
 * Modifications Copyright (C) 2025 Renesas Electronics Corporation or its affiliates.
 * SPDX-License-Identifier: MIT
 *
 * Licensed under the MIT License. See the LICENSE accompanying this file
 * for the specific language governing permissions and limitations under
 * the License.
 */

/**
 * @file MQTTFileDownloader_config.h
 * @brief Configs for MQTT stream.
 */

#ifndef MQTT_FILE_DOWNLOADER_CONFIG_H
#define MQTT_FILE_DOWNLOADER_CONFIG_H

/**
 * Configure the Maximum size of the data payload. The smallest value is 256 bytes,
 * maximum is 128KB.
 */
#ifndef mqttFileDownloader_CONFIG_BLOCK_SIZE
#define mqttFileDownloader_CONFIG_BLOCK_SIZE    (4096U)
#endif

/**
 * @brief The maximum number of data blocks requested from OTA streaming service.
 *
 *  This configuration parameter is sent with data requests and represents the maximum number of
 *  data blocks the service will send in response. The maximum limit for this must be calculated
 *  from the maximum data response limit (128 KB from service) divided by the block size.
 *  For example if block size is set as 1 KB then the maximum number of data blocks that we can
 *  request is 128/1 = 128 blocks. Configure this parameter to this maximum limit or lower based on
 *  how many data blocks response is expected for each data requests.
 *  Please note that this must be set larger than zero.
 *
 */
#ifndef mqttFileDownloader_MAX_NUM_BLOCKS_REQUEST
#define mqttFileDownloader_MAX_NUM_BLOCKS_REQUEST (1U)
#endif

#if mqttFileDownloader_MAX_NUM_BLOCKS_REQUEST > 1
#error "Requesting multiple data blocks is not supported in this version of the library. Please set mqttFileDownloader_MAX_NUM_BLOCKS_REQUEST to 1."
#endif

#endif /* #ifndef MQTT_FILE_DOWNLOADER_DEFAULT_H */
