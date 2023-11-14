# FreeRTOS RX Family IoT Reference

## Introduction

This product provides the reference of IoT solution with using [RX family](https://www.renesas.com/products/microcontrollers-microprocessors/rx-32-bit-performance-efficiency-mcus), [AWS](https://aws.amazon.com), and [FreeRTOS](https://www.freertos.org/RTOS.html). The RX family is a kind of MCUs provided by the [Renesas](https://www.renesas.com).
You can easily try to run AWS IoT demos using RX family while working with our various other products.
Refer to the [Getting Started Guide](Getting_Started_Guide.md) for step by step instructions on setting up your development environment and running demos.

## Product Specifications

This reference is consist of demo applications, FreeRTOS kernel, middleware provided by AWS and 3rd party, middleware and drivers for RX family provided as the [RX Driver Package](https://github.com/renesas/rx-driver-package) by the Renesas, files to collaborate Renesas tools such as the IDE [e2 studio](https://www.renesas.com/software-tool/e-studio), and etc.
Summary of specifications explains in the following chapters.

### Using AWS Services

* [AWS IoT core](https://aws.amazon.com/iot-core/)

### Demos

* PubSub
  * Simple MQTT communication with [AWS Management Console](https://aws.amazon.com/console)
* Fleet Provisioning
  * Generating and securely delivering device certificates and private keys to your devices by AWS when they connect to AWS IoT for the first time
* OTA demo
  * Update device firmware using AWS

The preceding demos use the following technical elements of the AWS IoT:

* [AWS IoT Jobs](https://docs.aws.amazon.com/iot/latest/developerguide/iot-jobs.html)
* [MQTT File Delivery](https://docs.aws.amazon.com/iot/latest/developerguide/mqtt-based-file-delivery.html)

### Supported Boards

* [CK-RX65N](https://www.renesas.com/products/microcontrollers-microprocessors/rx-32-bit-performance-efficiency-mcus/ck-rx65n-cloud-kit-based-rx65n-mcu-group) with:
  * Ethernet
  * Cellular(CAT M1)
    * [RYZ014A](https://www.renesas.com/br/en/products/wireless-connectivity/cellular-iot-modules/ryz014a-lte-cat-m1-cellular-iot-module)

### Open Source Software (OSS) Components

The following table indicates name and version of OSS which are used in this reference. The column *LTS Until* indicates the support period as LTS.

| Library                     | Version             | LTS Until  | LTS Repo URL                                                                |
|-------------------------    |---------------------|------------|---------------------------------------------------------------------------  |
| FreeRTOS Cellular Interface              | 1.3.0              | 10/31/2024 | <https://github.com/FreeRTOS/FreeRTOS-Cellular-Interface>                        |
| FreeRTOS Kernel             | 10.5.1              | 10/31/2024 | <https://github.com/FreeRTOS/FreeRTOS-Kernel>                        |
| FreeRTOS-Plus-TCP             | 3.1.0              | 10/31/2024 | <https://github.com/FreeRTOS/FreeRTOS-Kernel>                        |
| backoffAlgorithm             | 1.3.0              | 10/31/2024 | <https://github.com/FreeRTOS/backoffAlgorithm>                        |
| coreHTTP Client             | 3.0.0              | 10/31/2024 | <https://github.com/FreeRTOS/coreHTTP>                        |
| coreJSON             | 3.2.0              | 10/31/2024 | <https://github.com/FreeRTOS/coreJSON>                        |
| coreMQTT Client             | 2.1.1              | 10/31/2024 | <https://github.com/FreeRTOS/coreMQTT>                        |
| coreMQTT Agent             | 1.2.0              | 10/31/2024 | <https://github.com/FreeRTOS/coreMQTT-Agent>                        |
| corePKCS11             | 3.5.0              | 10/31/2024 | <https://github.com/FreeRTOS/corePKCS11>                        |
| coreSNTP             | 1.2.0              | 10/31/2024 | <https://github.com/FreeRTOS/coreSNTP>                        |
| AWS IoT Device Defender             | 1.3.0              | 10/31/2024 | <https://github.com/aws/Device-Defender-for-AWS-IoT-embedded-sdk>                        |
| AWS IoT Device Shadow             | 1.3.0              | 10/31/2024 | <https://github.com/aws/Device-Shadow-for-AWS-IoT-embedded-sdk>                        |
| AWS IoT Fleet Provisioning             | 1.1.0              | 10/31/2024 | <https://github.com/aws/Fleet-Provisioning-for-AWS-IoT-embedded-sdk>                        |
| AWS IoT Jobs              | 1.3.0              | 10/31/2024 | <https://github.com/aws/Jobs-for-AWS-IoT-embedded-sdk>                        |
| AWS SigV4             | 1.2.0              | 10/31/2024 | <https://github.com/aws/SigV4-for-AWS-IoT-embedded-sdk>                        |
| AWS IoT Over-the-air             | 3.4.0              | 10/31/2024 | <https://github.com/aws/ota-for-aws-iot-embedded-sdk>                        |
| tinycbor             | 0.5.2              | -- | <https://github.com/intel/tinycbor>                        |
| mbedtls             | 3.2.1              | -- | <https://github.com/Mbed-TLS>                        |
| mbedtls_config             | 3.2.1              | -- | --                        |
| mbedtls_utils             | 3.2.1              | -- | --                        |
| littlefs             | 2.5.1              | -- | <https://github.com/littlefs-project/littlefs>                        |
| FreeRTOS-Plus network_transport             | --              | -- | <https://www.freertos.org/network-interface.html>                        |
| FreeRTOS-Plus-CLI             | 1.0.4              | -- | <http://www.freertos.org/cli>                        |
| mbed-crypto             | 1.1.2              | -- | <https://github.com/renesas/mbed-crypto>                        |
| Logging Interface             | 1.1.3              | -- | <https://github.com/aws/amazon-freertos/tree/main/libraries/logging>                        |
| pkcs11             | 2-40-errata-1              | -- | <https://github.com/amazon-freertos/pkcs11>                        |
| tinycrypt             | 0.2.8              | -- | <https://github.com/intel/tinycrypt>                        |

### FIT Modules and RX Driver Package

The following table indicates name and version of [FIT modules](https://www.renesas.com/software-tool/fit) which are used in this reference and version of [RX Driver Package](https://www.renesas.com/software-tool/rx-driver-package) which each FIT modules are included in.

| FIT module | Revision of FIT module | Version of RX Driver Package |
|------------|---------|-------------------|
|r_bsp|7.20|1.36|
|r_s12ad_rx|5.00|1.36 - 1.37|
|r_byteq|2.00|1.33 - 1.36|
|r_cellular|1.11|1.40|
|r_ether_rx|1.23|1.36 - 1.40|
|r_flash_rx|5.00|1.39|
|r_sci_rx|4.40|1.36|
|r_tsip_rx|1.17.l|--|
|r_irq_rx|4.00|1.34|

### Data Flash Usage

| Area | Description | Contents |Start address<br>[Size] | Section name |
|------|-------------|----------|--------------|--------------|
|LittleFS management area|It's consist of the filesystem LittleFS,<br>default size is 8960 bytes.<br>You can change size by `LFS_FLASH_BLOCK_COUNT`.|IoT const data<br><ul><li>thingname</li><li>endpoint</li><li>claim credentials</li><li>device credentials</li><li>provisioning template</li><li>codesigncert</li><li>root ca</li></ul>|0x00100000<br><br>[8960 bytes<br>(*Default*)]|C_LITTLEFS_MANAGEMENT_AREA|
|Free area|It's not used by the demo.<br>Therefore, it's free area for user application.|User application data|0x00102300<br>(*Default*)<br><br>[23808 bytes<br>(*Default*)]|C_USER_APPLICATION_AREA|

* LittleFS management area
  * The demo project uses a maximum of 8960 bytes of Data Flash from address 0x00100000 to 0x001022FF using LittleFS.
    * If this area is less than 8960 bytes, the demo will not work properly.
  * You must **NOT** overwrite IoT const data in this area.
  * If you intend to read/write user application data in this area, increase the value of `LFS_FLASH_BLOCK_COUNT` in the "Projects\\...\\frtos_config\\rm_littlefs_flash_config.h".
    * `LFS_FLASH_BLOCK_COUNT` must be specified 71 ( == 9088 bytes) or more and 256 (32768 bytes, it is Data Flash size) or less.
    * You must use LittleFS's API to read/write this area.
  * When increasing LFS_FLASH_BLOCK_COUNT, you must also set the address of the section for user applications (C_USER_APPLICATION_AREA).  
    **Right-click on the project** --> **properties** --> **C/C++ Build** --> **Settings** --> **Section**  
    * If you do not set up a section, the demo may not work properly.
* Free area
  * The remaining area on Data Flash, 23808 bytes of Data Flash area at address 0x00102300 or above, can be used as user application area.
    * You must use FLASH FIT module's API to read/write this area.

#### Data Flash Memory Map

The following figure indicates how memory in Data Flash in CK-RX65N is used for each example of `LFS_FLASH_BLOCK_COUNT` value.

```text
In case of `LFS_FLASH_BLOCK_COUNT` == 70 (8960 bytes)
 +----------------------------+-------------------------+ <- 0x00100000
 |  LittleFS management area  |  IoT const data         |    <8960 bytes>
 +----------------------------+-------------------------+ <- 0x00102300
 |  Free area                 |  User application data  |    <23808 bytes>
 |                            |                         |
 +----------------------------+-------------------------+ <- 0x00107FFF

In case of `LFS_FLASH_BLOCK_COUNT` == 170 (21760 bytes)
 +----------------------------+-------------------------+ <- 0x00100000
 |  LittleFS management area  |  IoT const data         |    <21760 bytes>
 |                            |  User application data  |
 +----------------------------+-------------------------+ <- 0x00105500
 |  Free area                 |  User application data  |    <11008 bytes>
 +----------------------------+-------------------------+ <- 0x00107FFF
```

## Limitation

* ROM/RAM size is not optimized.
* CLI task cannot run after starting the demo to avoid the SCI conflict.
* The project generation for OTA demo feature on e2 studio can not be supported.*1  
  *1 For PubSub and Fleet Provisioning demo, it is possible to generate a new demo from e2studio. Please refer to the following for details on how to generate them.  
  https://en-support.renesas.com/knowledgeBase/21115016
* In the Cellular-RYZ014A project, OTA will not work properly unless "otaconfigMAX_NUM_BLOCKS_REQUEST" is set to "1".
* The following macros are not supported by this source code.  
  If you build the above macros, a build error will occur.  
  `LFS_NO_MALLOC`  
* The following macros are not supported in this project.  
  `LFS_NO_INTRINSICS`  
  This project is designed for RX65N. The RX65N is an RXv2 core and does not implement the CTZ or CLZ instructions.
* Limitations on the xGetMQTTAgentState() function  
  When monitoring the communication status using the xGetMQTTAgentState() function, the disconnection with AWS cannot be detected in the following cases.  
  Case where disconnection cannot be detected: An error occurs in the TCP_Sockets API*2 and disconnection with AWS occurs --> The Hook function is called and the MQTT connection with AWS is restored to the state where it was established.  
  The disconnection that occurred in the above case cannot be detected by the xGetMQTTAgentState() function.
  The function returns the state in which the MQTT connection with AWS is established (MQTT_AGENT_STATE_CONNECTED).  
  *2 TCP_Sockets API is a function defined in TCP_Sockets_xxxx  
* Notes on bootloader to application transition.  
  When transitioning from the bootloader sample program to the application, the settings of the bootloader's peripheral functions are taken over by the application.  
  For more information, check chapter 6.4 of the following document.   
  [RX Family Firmware Update module Using Firmware Integration Technology Application Notes](https://www.renesas.com/search?keywords=R01AN6850)


### About bootloader macros
The following macros have been added in FreeRTOS-v202210.01-LTS-rx-1.1.0.
The macros enable/disable the processes implemented on boot_loader.c.  
For details of the process, please read the source code in boot_loader.c.
The macros are defined in boot_loader.h.
A summary of the macros is shown below.  

| Macro name | Description | Initial value |
| ---------- | ----------- | ------------- |
| `BL_UPDATE_MODE` | If only the bootloader is written to the device, pressing the switch will install the initial firmware. | 0 (0:Disable,1:Enable) |
| `BL_INITIAL_IMAGE_INSTALL` | If the execution plane is empty, initial FW is installed. | 0 (0:Disable,1:Enable)|
| `BL_ERASE_BUFFER_AREA_AFTER_VERIFIED` | After the firmware update is completed, the old firmware written on the Buffer side is deleted after the first code verification. | 1 (0:Disable,1:Enable)|

  
Also, if both `BL_UPDATE_MODE` and `BL_INITIAL_IMAGE_INSTALL` above are disabled (Disable), the definition `BL_UART_RTS` for UART flow control is disabled.
In this sample, both `BL_UPDATE_MODE` and `BL_INITIAL_IMAGE_INSTALL` are set to Disable, so the default `BL_UART_RTS` is disabled.

### Fixed configuration values of FIT Modules

* There are some config values of FIT Modules that cannot be changed in order for the program to work properly.
* The config values of the following FIT Modules of tables cannot be changed.
* If changed, the program may not work properly.

  #### CK-RX65N Ethernet Projects

  | FIT module | Config name | Default Value | Project value | Reason for change |
  |------------|-------------|---------------|---------------|-------------------|
  | r_bsp      | BSP_CFG_HEAP_BYTES | 0x400 | 0x1000 | Because fleet provisioning demo uses malloc which is not an OS feature.<br>Also, because the default value cannot secure enough heap memory. |
  |            | BSP_CFG_CODE_FLASH_BANK_MODE | 1 | 0 | This project uses the dual bank feature. |
  |            | BSP_CFG_RTOS_USED | 0 | 1 | This project uses FreeRTOS. |
  |            | BSP_CFG_SCI_UART_TERMINAL_ENABLE	| 0 | 1 | This project uses SCI UART terminals. |
  |            | BSP_CFG_SCI_UART_TERMINAL_CHANNEL | 8 | 5 | This project uses SCI CH5 as the SCI UART terminal. |
  | r_ether_rx | ETHER_CFG_MODE_SEL | 0 | 1 | This value depends on the CK-RX65N PHY-LSI and circuit specifications. |
  |            | ETHER_CFG_CH0_PHY_ADDRESS | 0 | 5 | This value depends on the CK-RX65N PHY-LSI and circuit specifications. |
  |            | ETHER_CFG_CH0_PHY_ACCESS | 1 | 0 | This value depends on the CK-RX65N PHY-LSI and circuit specifications. |
  |            | ETHER_CFG_LINK_PRESENT | 0 | 1 | This value depends on the CK-RX65N PHY-LSI and circuit specifications. |
  |            | ETHER_CFG_USE_PHY_ICS1894_32 | 0 | 1 | This value depends on the CK-RX65N PHY-LSI and circuit specifications. |
  | r_flash_rx | FLASH_CFG_CODE_FLASH_ENABLE | 0 | 1 | OTA library rewrites code flash. |
  |            | FLASH_CFG_DATA_FLASH_BGO | 0 | 1 | LittleFS is implemented to rewrite data flash using BGO functionality. |
  |            | FLASH_CFG_CODE_FLASH_BGO | 0 | 1 | OTA library is implemented to rewrite code flash using BGO functionality. |
  |            | FLASH_CFG_CODE_FLASH_RUN_FROM_ROM | 0 | 1 | OTA library is implemented to execute code that rewrites the code flash from another bank. |
  | r_sci_rx   | SCI_CFG_CH5_INCLUDED | 0 | 1 | SCI CH5 is used as the SCI UART terminal. |
  |            | SCI_CFG_TEI_INCLUDED | 0 | 1 | Transmit end interrupt is used. |

  #### CK-RX65N Cellular-RYZ014A Projects

  | FIT module | Config name | Default Value | Project value | Reason for change |
  |------------|-------------|---------------|---------------|-------------------|
  | r_bsp      | BSP_CFG_HEAP_BYTES | 0x400 | 0x1000 | Because fleet provisioning demo uses malloc which is not an OS feature.<br>Also, because the default value cannot secure enough heap memory. |
  |            | BSP_CFG_CODE_FLASH_BANK_MODE | 1 | 0 | This project uses the dual bank feature. |
  |            | BSP_CFG_RTOS_USED | 0 | 1 | This project uses FreeRTOS. |
  |            | BSP_CFG_SCI_UART_TERMINAL_ENABLE	| 0 | 1 | This project uses SCI UART terminals. |
  |            | BSP_CFG_SCI_UART_TERMINAL_CHANNEL | 8 | 5 | This project uses SCI CH5 as the SCI UART terminal. |
  | r_flash_rx | FLASH_CFG_CODE_FLASH_ENABLE | 0 | 1 | OTA library rewrites code flash. |
  |            | FLASH_CFG_DATA_FLASH_BGO | 0 | 1 | LittleFS is implemented to rewrite data flash using BGO functionality. |
  |            | FLASH_CFG_CODE_FLASH_BGO | 0 | 1 | OTA library is implemented to rewrite code flash using BGO functionality. |
  |            | FLASH_CFG_CODE_FLASH_RUN_FROM_ROM | 0 | 1 | OTA library is implemented to execute code that rewrites the code flash from another bank. |
  | r_sci_rx   | SCI_CFG_CH5_INCLUDED | 0 | 1 | SCI CH5 is used as the SCI UART terminal. |
  |            | SCI_CFG_CH6_INCLUDED | 0 | 1 | SCI CH6 is used to communicate with the RYZ014A module. |
  |            | SCI_CFG_CH6_TX_BUFSIZ | 80 | 2180 | The TX buffer size needs to be increased to communicate with RYZ014A. |
  |            | SCI_CFG_CH6_RX_BUFSIZ | 80 | 8192 | The RX buffer size needs to be increased to communicate with RYZ014A. |
  |            | SCI_CFG_TEI_INCLUDED | 0 | 1 | Transmit end interrupt is used. |

## Contribution

See [CONTRIBUTING](CONTRIBUTING.md) for more information.

## License

* Source code located in the *Projects*, *Common*, *Middleware/AWS*, and *Middleware/FreeRTOS* directories are available under the terms of the MIT License. See the LICENSE file for more details.
* Other libraries located in the *Middleware* directories are available under the terms specified in each source file.
* Each Renesas FIT Modules in RX driver package located in the *Projects/xxx/xxx/src/smc_gen* are available under the terms of the basically MIT License. See the doc/license of following URL for more details.
[https://github.com/renesas/rx-driver-package](https://github.com/renesas/rx-driver-package)

## Support

Visit the following official webpage if having any technical questions.

* [English window](https://en-support.renesas.com/dashboard)
* [Japanese window](https://ja-support.renesas.com/dashboard)
* [Chinese window](https://zh-support.renesas.com/dashboard)
