# Wi-Fi(DA16600), PubSub/MQTT sample project

## Overview

This document provides information about the demo with the following specifications:

* Connection: Wi-Fi (DA16600)
* Demo Type: PubSub/MQTT

> **Note:**  
> For information about the "*PubSub/MQTT with Over-the-air(OTA) update sample project*", see [**"da16600_ota_information.md"**](da16600_ota_information.md).  
> For more information about setting up and running the demo, see [**"Getting_Started_Guide.md"**](../../Getting_Started_Guide.md).

## About the PubSub Demo using Wi-Fi connection

This demo provides a demonstration of connecting to AWS with Wi-Fi and sending and receiving MQTT messages.

### Tasks performed

This demo performs the following tasks:

| Task Name                 | Detail | stack size<BR>(Byte) | Priority |
|---------------------------|--------|----------------------|----------|
| Logging task              | This task uses another task, such as UART, to actually perform the printing output.                              | 4608 | 2 |
| UART command console task | This task provides a command console function to realize CLI using UART.                                         | 4608 | 1 |
| main task task            | This task is the application entry point from power-on reset.                                                    | 512  | 1 |
| MQTT-agent task           | This task uses the MQTT agent API to communicate with the MQTT broker over the same MQTT connection.             | 6144 | 2 |
| PubSub task               | This task uses the MQTT Agent API to send a unique MQTT payload to a unique topic over the same MQTT connection. | 2048 | 1 |

### Memory usage

| Compiler | RAM size | ROM size |
|----------|----------|----------|
| CC-RX    | 359KB    | 323KB    |
| GCC      | 364KB    | 318KB    |

### Confirmed Operation Environment

| Compiler | Version  | Details |
|----------|----------|---------|
| CC-RX    | V3.07.00 | Optimization: Level 2 |
| GCC      | GCC for Renesas RX v8.3.0.202411 | Optimization options:<br>&emsp;- Optimize for Debug<br>&emsp;- Function sections<br>&emsp;- Data sections<br>&emsp;- Enable garbage collection of unused input sections (-gc-sections) |

## Explanation specific to this demo

### Hardware setup

For common hardware setup instructions, see [**Getting Started Guide: step 2**](../../Getting_Started_Guide.md#step-2-hardware-setup).

#### Board setup

Connect the DA16600 to device board. In case of CK-RX65N v2, connected to the PMOD1 of it.  

>**Note:**
>
> * Depending on the country you live in, you may not use DA16600 due to wrong country code or timezone.  
> Refer to [Settings of Country code and GMT timezone](#settings-of-country-code-and-gmt-timezone) for settings.

#### Board settings image

![2](https://github.com/renesas/iot-reference-rx/wiki/getting_started_guide_image/step2_v2_wifi.png?raw=true)  

### Software setup

For common software setup instructions, see [**Getting Started Guide: step 4-1-1**](../../Getting_Started_Guide.md#step-4-1-1-software-setup-for-pubsub-demo).

#### Settings of the hook function

The socket wrapper layer provides a hook function to reset Wi-Fi module when an error occurs.  
It is recommended to reset the module if the FIT module API "R_WIFI_xxx" returns errors `WIFI_ERR_MODULE_TIMEOUT` or `WIFI_ERR_MODULE_COM`. Otherwise, the module may not communicate.

You can change `USER_TCP_HOOK_ENABLED` in "Projects\\<project_name>\\e2 studio_ccrx\\src\\frtos_config\\user_tcp_hook_config.h" to enable or disable this hook.

You can use hook as-is if you only run the demos.
In our current implementation, Wi-Fi module is reset in this hook when the following conditions:

* On TCP_Sockets_Connect,
  * When both errors occur 1 time.
    > * **Note:** You can change retry numbers by configuring `USER_RECONNECT_TRIES` (hardware reset attempts).
* On TCP_Sockets_Recv and TCP_Sockets_Send,
  * When `WIFI_ERR_MODULE_TIMEOUT` occurs 1 time.
  * When `WIFI_ERR_MODULE_COM` occurs continuously 3 times.
    > * **Note:** You can change retry numbers by configuring `USER_COMM_ERROR_TRIES` as well as `USER_RECONNECT_TRIES` (hardware reset attempts).

#### Settings of Country code and GMT timezone

Use the RX Smart Configurator to configure the Country code and GMT timezone.  
Open the RX Smart Configurator as shown in the image below and set the 2 parameters `"WIFI_CFG_COUNTRY_CODE"` and `"WIFI_CFG_SNTP_UTC_OFFSET"`.

* `"WIFI_CFG_COUNTRY_CODE"` : Country code defined in ISO 3166-1 alpha-2 standard. Such as KR, US, JP, and CH.
* `"WIFI_CFG_SNTP_UTC_OFFSET"` : GMT timezone offset in hours (-12 ~ 12).

![4-1-1](https://github.com/renesas/iot-reference-rx/wiki/getting_started_guide_image/step4_1_1_Input_WFCC_TZONE.PNG?raw=true)

#### Settings of Wi-Fi network

Configure settings of Wi-Fi network for Wi-Fi DA16600 module. Configure the following macro in "src\\application_code\\include\\aws_clientcredential.h"

* `clientcredentialWIFI_SSID` : Set the access point name of Wi-Fi network
* `clientcredentialWIFI_PASSWORD` : Set the Wi-Fi network password

```c
/*
 * @brief Wi-Fi network to join.
 *
 * @todo If you are using Wi-Fi, set this to your network name.
 */
#define clientcredentialWIFI_SSID                    ""
```

```c
/*
 * @brief Password needed to join Wi-Fi network.
 * @todo If you are using WPA, set this to your network password.
 */
#define clientcredentialWIFI_PASSWORD                ""
```

## Troubleshooting

### Improve Wi-Fi hardware reset operation when connection error happens

If an error occurs during Wi-Fi connection establishment and Wi-Fi DA16600 module (specifically for US159-DA16600EVZ) cannot fix itself after reset attempts due to hardware limitation (Wi-Fi reset pin need to be delayed in low level state more than 2.7 seconds before pulling to high level state for properly resetting), changing delay value to the following "correction value" may improve the problem.

* Path: `src\smc_gen\r_wifi_da16xxx\r_wifi_da16xxx_common.c`
* Default value: `500`  
![trouble_shoot_wifi_reset_1](https://github.com/renesas/iot-reference-rx/wiki/getting_started_guide_image/trouble_shoot_wifi_reset_1.png?raw=true)

* Correction value: `3000`(delay time should be larger than 2700)  
![trouble_shoot_wifi_reset_2](https://github.com/renesas/iot-reference-rx/wiki/getting_started_guide_image/trouble_shoot_wifi_reset_2.png?raw=true)

> Note:
>
> * If the above value is set, the execution time of the demo may increase. Should adjust the value of time according to your communication environment.
