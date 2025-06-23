# Cellular(RYZ014A), PubSub/MQTT sample project

## Overview

This document provides information about the demo with the following specifications:

* Connection: Cellular (RYZ014A)
* Demo Type: PubSub/MQTT

> **Note:**  
> For information about the "*PubSub/MQTT with Over-the-air(OTA) update sample project*", see [**"ryz014a_ota_information.md"**](ryz014a_ota_information.md).  
> For more information about setting up and running the demo, see [**"Getting_Started_Guide.md"**](../../../Getting_Started_Guide.md).

## About the PubSub Demo using Cellular connection

This demo provides a demonstration of connecting to AWS with Cellular and sending and receiving MQTT messages.

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

### Memory usage

| Compiler | RAM size | ROM size |
|----------|----------|----------|
| CC-RX    | 321KB    | 336KB    |
| GCC      | 330KB    | 331KB    |

### Confirmed Operation Environment

| Compiler | Version  | Details |
|----------|----------|---------|
| CC-RX    | V3.07.00 | Optimization: Level 2 |
| GCC      | GCC for Renesas RX v8.3.0.202411 | Optimization options:<br>&emsp;- Optimize for Debug<br>&emsp;- Function sections<br>&emsp;- Data sections<br>&emsp;- Enable garbage collection of unused input sections (-gc-sections) |

## Explanation specific to this demo

### Hardware setup

For common hardware setup procedures, see [**Getting Started Guide: step 2**](../../../Getting_Started_Guide.md#step-2-hardware-setup).

#### Board setup

Connect a RYZ014A with LTE antenna and SIM card to device board.

1. Connect the LTE antenna to the RYZ014A. The antenna is packaged with the purchase of RYZ014A.
2. **The SIM card is required to be activated in advance** (refer to your SIM card's guide), then insert it to the RYZ014A.
3. Connect the RYZ014A to device board. In case of CK-RX65N, connected to the PMOD1 of it.
4. Connect a USB cable to the RYZ014A for supplemental power supply.

> **Note:**
>
> * Depending on the country you live in or provider of SIM card, you may not use RYZ014A due to the supported bands. Refer to manual of RYZ014A about supported bands for details.  
> Also see [Settings of Bands](#settings-of-bands) for band settings.

#### Board settings image

![2](https://github.com/renesas/iot-reference-rx/wiki/getting_started_guide_image/step2_v2_cell.png?raw=true)  

### Software setup

For common software setup procedures, see [**Getting Started Guide: step 4-1-1**](../../../Getting_Started_Guide.md#step-4-1-1-software-setup-for-pubsub-demo).

#### Settings of the hook function

The socket wrapper layer provides a hook function to reset the cellular module when an error occurs.  
It is recommended to reset the module if the FIT module API `R_Cellular` returns errors `CELLULAR_ERR_MODULE_TIMEOUT`, `CELLULAR_ERR_MODULE_COM`.  
Otherwise, the module may not communicate.  
You can change `USER_TCP_HOOK_ENABLED` in "*Projects\\<project_name>\\e2 studio_ccrx\\src\\frtos_config\\user_tcp_hook_config.h*" to enable or disable this hook.

You can use hook as-is if you only run the demos.  
In our current implementation, cellular module is reset in this hook when the following conditions:

* On TCP_Sockets_Connect,
  * When both errors occur 1 time.
* On TCP_Sockets_Recv and TCP_Sockets_Send,
  * When `CELLULAR_ERR_MODULE_TIMEOUT` occurs 1 time.
  * When `CELLULAR_ERR_MODULE_COM` occurs continuously 3 times.
    * You can change retry numbers by configuring `USER_COMM_ERROR_TRIES`.

#### Settings of access point

Use the RX Smart Configurator to configure the SIM card settings.  
Open the RX Smart Configurator as shown in the image below and set the four parameters `"Access point name"`, `"Access point login ID"`, `"Access point password"`, and `"SIM card PIN card PIN code"`.  
The settings will vary depending on the SIM card that you are using, so please check the SIM information.

* `"Access point name"` : Set the access point name of the SIM
* `"Access point login ID"` : Set the user name of the SIM
* `"Access point password"` : Set SIM card password
* `"SIM  card PIN code"` : Set SIM card PIN code

If you are using the truephone SIM that comes with CK-RX65N v2, it will work if you only enter the Access point name.

![4-1-1](https://github.com/renesas/iot-reference-rx/wiki/getting_started_guide_image/step4_1_1_Input_APN_l3_2.PNG?raw=true)

#### Settings of bands

Configure settings related to the bands supported by your cellular module.
In case of RYZ014A, configure the following macro in "*Middleware\\network_transport\\sockets_wrapper\\ports\\cellular_ryz014a\\TCP_socket_hook.c*"

* `CELLULAR_BAND_CONFIG`: Set `"1,2,4,5,8,12,13,14,17,18,19,20,25,26,28,66"`

## Troubleshooting

N/A
