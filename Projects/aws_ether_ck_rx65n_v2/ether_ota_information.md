# Ethernet, PubSub/MQTT with Over-the-air(OTA) update sample project

## Overview

This document provides information about the demo with the following specifications:

* Connection: Ethernet
* Demo Type: OTA update

> **Note:**  
> For information about the "*PubSub/MQTT sample project*", see [**"ether_pubsub_information.md"**](ether_pubsub_information.md).  
> For more information about setting up and running the demo, see [**"Getting_Started_Guide.md"**](../../Getting_Started_Guide.md).

## About the PubSub Demo using Ethernet connection

In this demo, we will use Ethernet to connect to AWS and perform an OTA(Over-the-air) firmware update.

### Tasks performed

This demo performs the following tasks:

| Task Name                 | Detail | stack size<BR>(Byte) | Priority |
|---------------------------|--------|----------------------|----------|
| Logging task              | This task uses another task, such as UART, to actually perform the printing output.                              | 4608 | 2 |
| UART command console task | This task provides a command console function to realize CLI using UART.                                         | 4608 | 1 |
| Ether Receive check task  | This task checks for incoming data from the Ethernet.                                                            | 512  | 6 |
| IP task                   | This task handles the Ethernet communication.                                                                    | 768  | 5 |
| main task                 | This task is the application entry point from power-on reset.                                                    | 512  | 1 |
| MQTT-agent task           | This task uses the MQTT agent API to communicate with the MQTT broker over the same MQTT connection.             | 6144 | 2 |
| PubSub task               | This task uses the MQTT Agent API to send a unique MQTT payload to a unique topic over the same MQTT connection. | 2048 | 1 |
| OTA Demo task             | This task connects to the MQTT broker, creates the MQTT Agent task and calls the Ota demo loop.                  | 2304 | 1 |
| OTA Agent task            | This task manages the OTA firmware updates for the device.                                                       | 8192 | 2 |

### Memory usage

| Compiler | RAM size | ROM size |
|----------|----------|----------|
| CC-RX    | 349KB    | 395KB    |
| GCC      | 357KB    | 385KB    |

### Confirmed Operation Environment

| Compiler | Version  | Details |
|----------|----------|---------|
| CC-RX    | V3.07.00 | Optimization: Level 2 |
| GCC      | GCC for Renesas RX v8.3.0.202411 | Optimization options:<br>&emsp;- Optimize for Debug<br>&emsp;- Function sections<br>&emsp;- Data sections<br>&emsp;- Enable garbage collection of unused input sections (-gc-sections) |

## Explanation specific to this demo

### Hardware setup

For hardware setup instructions, see [**Getting Started Guide: step 2**](../../Getting_Started_Guide.md#step-2-hardware-setup).

### Software setup

For software setup instructions, see [**Getting Started Guide: step 4-2**](../../Getting_Started_Guide.md#step-4-2-run-pubsubmqtt-with-over-the-airota-update-sample-project).

## Troubleshooting

### 1. Operation when OTA update firmware is abnormal

If the device cannot connect to the network when running the updated firmware, the self-test will not be performed and the updated firmware will be executed as is.  
In this case, the old firmware will remain in the flash memory, so please consider measures such as rollback to the old firmware.

### 2. Differences in behavior depending on LTS version after running OTA demo

Please note that the behavior when launching the OTA demo differs depending on the LTS version.  

  | LTS version    | detail |
  |----------------|--------|
  | v202210.01-LTS | After the *OTA demo starts running*, an OTA request message will be sent once.<BR>The *demo* will wait for the preconfigured *duration* to *poll for an OTA job*.<BR>After the preconfigured duration has elapsed, the device will enter suspended state. |
  | v202406.01-LTS | After the OTA demo *starts running*, OTA request messages *will be* sent to AWS periodically until an OTA job is *detected*.<BR>(There is no *configuration* value to specify the duration to wait for an OTA job.)|