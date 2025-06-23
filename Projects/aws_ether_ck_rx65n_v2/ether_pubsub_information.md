# Ethernet, PubSub/MQTT sample project

## Overview

This document provides information about the demo with the following specifications:

* Connection: Ethernet
* Demo Type: PubSub/MQTT

> **Note:**  
> For information about the "*PubSub/MQTT with Over-the-air(OTA) update sample project*", see [**"ether_ota_information.md"**](ether_ota_information.md).  
> For more information about setting up and running the demo, see [**"Getting_Started_Guide.md"**](../../../Getting_Started_Guide.md).

## About the PubSub Demo using Ethernet connection

This demo provides a demonstration of connecting to AWS with ethernet and sending and receiving MQTT messages.

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

### Memory usage

| Compiler | RAM size | ROM size |
|----------|----------|----------|
| CC-RX    | 327KB    | 355KB    |
| GCC      | 335KB    | 352KB    |

### Confirmed Operation Environment

| Compiler | Version  | Details |
|----------|----------|---------|
| CC-RX    | V3.07.00 | Optimization: Level 2 |
| GCC      | GCC for Renesas RX v8.3.0.202411 | Optimization options:<br>&emsp;- Optimize for Debug<br>&emsp;- Function sections<br>&emsp;- Data sections<br>&emsp;- Enable garbage collection of unused input sections (-gc-sections) |

## Explanation specific to this demo

### Hardware setup

For hardware setup instructions, see [**Getting Started Guide: step 2**](../../../Getting_Started_Guide.md#step-2-hardware-setup).

### Software setup

For software setup instructions, see [**Getting Started Guide: step 4-1-1**](../../../Getting_Started_Guide.md#step-4-1-1-software-setup-for-pubsub-demo).

## Troubleshooting

N/A
