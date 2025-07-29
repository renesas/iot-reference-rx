# Cellular(RYZ014A), PubSub/MQTT with Over-the-air(OTA) update sample project

## Overview

This document provides information about the demo with the following specifications:

* Connection: Cellular (RYZ014A)
* Demo Type: OTA update

> **Note:**  
> For information about the "*PubSub/MQTT sample project*", see [**"ryz014a_pubsub_information.md"**](ryz014a_pubsub_information.md).  
> For more information about setting up and running the demo, see [**"Getting_Started_Guide.md"**](../../Getting_Started_Guide.md).

## About the PubSub Demo using Cellular connection

In this demo, we will use Cellular to connect to AWS and perform an OTA(Over-the-air) firmware update.

### Tasks performed

This demo performs the following tasks:

| Task Name                 | Detail | stack size<BR>(Byte) | Priority |
|---------------------------|--------|----------------------|----------|
| Logging task              | This task uses another task, such as UART, to actually perform the printing output.                              | 4608 | 2 |
| UART command console task | This task provides a command console function to realize CLI using UART.                                         | 4608 | 1 |
| Cellular receive task     | This task handles the reception of the cellular module.                                                          | 512  | 6 |
| Cellular ring task        | This task handles ring for the cellular module.                                                                  | 512  | 6 |
| main task                 | This task is the application entry point from power-on reset.                                                    | 512  | 1 |
| MQTT-agent task           | This task uses the MQTT agent API to communicate with the MQTT broker over the same MQTT connection.             | 6144 | 2 |
| PubSub task               | This task uses the MQTT Agent API to send a unique MQTT payload to a unique topic over the same MQTT connection. | 2048 | 1 |
| OTA Demo task             | This task connects to the MQTT broker, creates the MQTT Agent task and calls the Ota demo loop.                  | 2304 | 1 |
| OTA Agent task            | This task manages the OTA firmware updates for the device.                                                       | 8192 | 2 |

### Memory usage

| Compiler | RAM size | ROM size |
|----------|----------|----------|
| CC-RX    | 344KB    | 376KB    |
| GCC      | 352KB    | 363KB    |

### Confirmed Operation Environment

| Compiler | Version  | Details |
|----------|----------|---------|
| CC-RX    | V3.07.00 | Optimization: Level 2 |
| GCC      | GCC for Renesas RX v8.3.0.202411 | Optimization options:<br>&emsp;- Optimize for Debug<br>&emsp;- Function sections<br>&emsp;- Data sections<br>&emsp;- Enable garbage collection of unused input sections (-gc-sections) |

## Explanation specific to this demo

### Hardware setup

* For common hardware setup instructions, see [**Getting Started Guide: step 2**](../../Getting_Started_Guide.md#step-2-hardware-setup).
* For RYZ014A specific hardware setup instructions, please see [**ryz014a_pubsub_information.md: Hardware setup**](ryz014a_pubsub_information.md#hardware-setup).

### Software setup

* For common software setup instructions, see [**Getting Started Guide: step 4-2**](../../Getting_Started_Guide.md#step-4-2-run-pubsubmqtt-with-over-the-airota-update-sample-project).
* For RYZ014A specific software setup instructions, please see [**ryz014a_pubsub_information.md: Software setup**](ryz014a_pubsub_information.md#software-setup).

## Troubleshooting

### 1. Troubleshooting for OTA project

* [OTA Troubleshooting](/Projects/aws_ether_ck_rx65n_v2/ether_ota_information.md#troubleshooting)
