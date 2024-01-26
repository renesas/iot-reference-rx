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
* OTA
  * Update device firmware using AWS

The preceding demos use the following technical elements of the AWS IoT:

* [AWS IoT Jobs](https://docs.aws.amazon.com/iot/latest/developerguide/iot-jobs.html)
* [MQTT File Delivery](https://docs.aws.amazon.com/iot/latest/developerguide/mqtt-based-file-delivery.html)

### Supported Boards

* [CK-RX65N](https://www.renesas.com/products/microcontrollers-microprocessors/rx-32-bit-performance-efficiency-mcus/ck-rx65n-cloud-kit-based-rx65n-mcu-group) with:
  * Ethernet
  * Cellular(CAT M1)
    * [RYZ014A](https://www.renesas.com/br/en/products/wireless-connectivity/cellular-iot-modules/ryz014a-lte-cat-m1-cellular-iot-module)
  * Wi-Fi
    * [DA16600](https://www.renesas.com/us/en/products/wireless-connectivity/wi-fi/low-power-wi-fi/da16600mod-ultra-low-power-wi-fi-bluetooth-low-energy-combo-modules-battery-powered-iot-devices)

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

The following table indicates name and version of [FIT modules](https://www.renesas.com/software-tool/fit) which are used in this reference and version of [RX Driver Package](https://www.renesas.com/software-tool/rx-driver-package) in which each FIT module is packaged.

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
|r_fwup|2.01|--|
|r_wifi_da16xxx|1.10|1.10|

### Data Flash Usage
RX family of MCU has internal Data Flash Memory, and this references are using the Data Flash to store the data for connecting to the Cloud service.  
| Area | Description | Contents |Start address<br>[Size] | Section name |
|------|-------------|----------|--------------|--------------|
|LittleFS management area|It's consist of the filesystem LittleFS,<br>default size is 8960 bytes.<br>You can change size by `LFS_FLASH_BLOCK_COUNT`.|IoT const data<br><ul><li>thingname</li><li>endpoint</li><li>claim credentials</li><li>device credentials</li><li>provisioning template</li><li>codesigncert</li><li>root ca</li></ul>|0x00100000<br><br>[8960 bytes<br>(*Default*)]|C_LITTLEFS_MANAGEMENT_AREA|
|Free area|It's not used by the demo.<br>Therefore, it's free area for user application.|User application data|0x00102300<br>(*Default*)<br><br>[23808 bytes<br>(*Default*)]|C_USER_APPLICATION_AREA|

* LittleFS management area
  * The demo project uses a maximum of 8960 bytes of Data Flash from address 0x00100000 to 0x001022FF using LittleFS.
    * If this area is less than 8960 bytes, the demo will not work properly.
  * You must **NOT** overwrite other data against IoT const data in this area.
  * If you intend to read/write user application data in this area, increase the value of `LFS_FLASH_BLOCK_COUNT` in the "Projects\\...\\frtos_config\\rm_littlefs_flash_config.h".
    * `LFS_FLASH_BLOCK_COUNT` must be specified 71 ( == 9088 bytes) or more and 256 (32768 bytes, it is Data Flash size) or less.
    * You must use LittleFS's API to read/write this area.
  * When increasing `LFS_FLASH_BLOCK_COUNT`, you must also reset the address of the section for free area(C_USER_APPLICATION_AREA) considering the increased LittleFS management area.  
    **Open the project by e2 studio of IDE** --> **Right-click on the project at the project explore** --> **properties** --> **C/C++ Build** --> **Settings** --> **Section**
    * If you do not set up a section, the demo may not work properly.
* Free area
  * The remaining area on Data Flash, 23808 bytes of Data Flash area at address 0x00102300 or above, can be used as user application area.
    * You must use FLASH FIT module's API to write this area.

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
* CLI task cannot run after starting the demo to avoid the SCI conflict.
* OTA demo will not work properly unless `otaconfigMAX_NUM_BLOCKS_REQUEST` is set to "1".  
  If `otaconfigMAX_NUM_BLOCKS_REQUEST` is set to anything other than 1, a build error will occur.  
* The following macros are not supported by this source code.  
  If you build the above macros, a build error will occur.  
  `LFS_NO_MALLOC`  
  `LFS_THREADSAFE`
* Limitations on the xGetMQTTAgentState() function  
  In the following case, the xGetMQTTAgentState() function for monitoring the communication status returns the state of established MQTT connection with AWS (`MQTT_AGENT_STATE_CONNECTED`) without detecting the disconnection:  
The hook function *1 is called by occuring an error of a TCP_Sockets API *2 (disconnection with AWS) inner the xGetMQTTAgentState(), then this hook restores the connection to established state.  
  *1 The hook function defined in USER_TCP_HOOK_FUNCTION macro in src/frtos_config/user_tcp_hook_config.h  
  *2 TCP_Sockets API is a function defined in TCP_Sockets_xxxx

* Notes on redundant linker section after generating code with Smart Configurator.  
  After generating code with Smart Configurator, sections 'C_FIRMWARE_UPDATE_CONTROL_BLOCK' and 'C_FIRMWARE_UPDATE_CONTROL_BLOCK_MIRROR' will be created at address 0x00100000.  
  These sections are redundant, it does not impact memory usage of the project.    
  This behavior is according to the specification of r_tsip_rx FIT module (from version 1.17.l).  

* Notes on bootloader to application transition.  
  When transitioning from the bootloader sample program to the application, the settings of the bootloader's peripheral functions are taken over by the application.  
  For more information, check chapter 7 of the following document.   
  [RX Family Firmware Update module Using Firmware Integration Technology Application Notes Rev.2.01 - Sample Code](https://www.renesas.com/search?keywords=R01AN6850)


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
| `BL_ERASE_BUFFER_AREA_AFTER_VERIFIED` | After the firmware update is completed, the old firmware written on the Buffer side is deleted after the verification of new firmware. | 1 (0:Disable,1:Enable)|

  
Also, if both `BL_UPDATE_MODE` and `BL_INITIAL_IMAGE_INSTALL` above are disabled (Disable), the definition `BL_UART_RTS` for UART flow control is disabled.
In this sample, both `BL_UPDATE_MODE` and `BL_INITIAL_IMAGE_INSTALL` are set to Disable, so the default `BL_UART_RTS` is disabled.

### Configuration values changed from the default in the FIT Modules

* The configuration values of the FIT modules that have been changed from the default values are listed in the table below.
* However, each projects is evaluated only with preset values, including configuration values that have not been changed from the default values.
* If changed, the program may not work properly.

  #### CK-RX65N Ethernet Projects

  | FIT module | Config name | Default Value | Project value | Reason for change |
  |------------|-------------|---------------|---------------|-------------------|
  | r_bsp      | BSP_CFG_HEAP_BYTES | 0x400 | 0x1000 | Because LittleFS and fleet provisioning demo uses malloc which is not an OS feature.<br>Also, because the default value cannot secure enough heap memory. |
  |            | BSP_CFG_CODE_FLASH_BANK_MODE | 1 | 0 | This project uses the Dual bank function. |
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
  | r_fwup     | FWUP_CFG_UPDATE_MODE | 1 | 0 | This project uses Dual bank function. |
  |            | FWUP_CFG_FUNCTION_MODE | 0 | 1 | This project is user program. |
  |            | FWUP_CFG_MAIN_AREA_ADDR_L | 0xFFE00000U | 0xFFF00000U | This value is set according to the RX65N ROM 2MB product. |
  |            | FWUP_CFG_BUF_AREA_ADDR_L | 0xFFEF8000U | 0xFFE00000U | This value is set according to the RX65N ROM 2MB product. |
  |            | FWUP_CFG_AREA_SIZE | 0xF8000U | 0xF0000U | This value is set according to the RX65N ROM 2MB product. |

  #### CK-RX65N Cellular-RYZ014A Projects

  | FIT module | Config name | Default Value | Project value | Reason for change |
  |------------|-------------|---------------|---------------|-------------------|
  | r_bsp      | BSP_CFG_HEAP_BYTES | 0x400 | 0x1000 | Because LittleFS and fleet provisioning demo uses malloc which is not an OS feature.<br>Also, because the default value cannot secure enough heap memory. |
  |            | BSP_CFG_CODE_FLASH_BANK_MODE | 1 | 0 | This project uses the Dual bank function. |
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
  | r_fwup     | FWUP_CFG_UPDATE_MODE | 1 | 0 | This project uses Dual bank function. |
  |            | FWUP_CFG_FUNCTION_MODE | 0 | 1 | This project is user program. |
  |            | FWUP_CFG_MAIN_AREA_ADDR_L | 0xFFE00000U | 0xFFF00000U | This value is set according to the RX65N ROM 2MB product. |
  |            | FWUP_CFG_BUF_AREA_ADDR_L | 0xFFEF8000U | 0xFFE00000U | This value is set according to the RX65N ROM 2MB product. |
  |            | FWUP_CFG_AREA_SIZE | 0xF8000U | 0xF0000U | This value is set according to the RX65N ROM 2MB product. |

  #### CK-RX65N Wi-Fi - DA16600 Projects

  | FIT module | Config name | Default Value | Project value | Reason for change |
  |------------|-------------|---------------|---------------|-------------------|
  | r_bsp      | BSP_CFG_HEAP_BYTES | 0x400 | 0x1000 | Because LittleFS and fleet provisioning demo uses malloc which is not an OS feature.<br>Also, because the default value cannot secure enough heap memory. |
  |            | BSP_CFG_CODE_FLASH_BANK_MODE | 1 | 0 | This project uses the Dual bank function. |
  |            | BSP_CFG_RTOS_USED | 0 | 1 | This project uses FreeRTOS. |
  |            | BSP_CFG_SCI_UART_TERMINAL_ENABLE	| 0 | 1 | This project uses SCI UART terminals. |
  |            | BSP_CFG_SCI_UART_TERMINAL_CHANNEL | 8 | 5 | This project uses SCI CH5 as the SCI UART terminal. |
  | r_flash_rx | FLASH_CFG_CODE_FLASH_ENABLE | 0 | 1 | OTA library rewrites code flash. |
  |            | FLASH_CFG_DATA_FLASH_BGO | 0 | 1 | LittleFS is implemented to rewrite data flash using BGO functionality. |
  |            | FLASH_CFG_CODE_FLASH_BGO | 0 | 1 | OTA library is implemented to rewrite code flash using BGO functionality. |
  |            | FLASH_CFG_CODE_FLASH_RUN_FROM_ROM | 0 | 1 | OTA library is implemented to execute code that rewrites the code flash from another bank. |
  | r_wifi_da16xxx | WIFI_CFG_DA16600_SUPPORT | 0 | 1 | Enable support for DA16600 |
  |                | WIFI_CFG_SCI_CHANNEL | 0 | 1| PMOD1 using SCI6 channel |
  |                | WIFI_CFG_CTS_PORT | 2 | J | PMOD1_1 is PJ3 |
  |                | WIFI_CFG_CTS_PIN | 3 | 3 | PMOD1_1 is PJ3 |
  |                | WIFI_CFG_RTS_PORT | 2 | J | PMOD1_1 is PJ3 |
  |                | WIFI_CFG_RTS_PIN | 3 | 3 | PMOD1_1 is PJ3 |
  |                | WIFI_CFG_PFS_SET_VALUE | 0x0BU | 0x0AU | PFS for Jx is 0x0AU |
  |                | WIFI_CFG_RESET_PORT | A | 5 | PMOD1_8 is P55 |
  |                | WIFI_CFG_RESET_PIN | 1 | 5 | PMOD1_8 is P55 |
  | r_sci_rx   | SCI_CFG_CH5_INCLUDED | 0 | 1 | SCI CH5 is used as the SCI UART terminal. |
  |            | SCI_CFG_CH6_INCLUDED | 0 | 1 | SCI CH6 is used to communicate with the DA16600 module. |
  |            | SCI_CFG_CH6_TX_BUFSIZ | 80 | 2180 | The TX buffer size needs to be increased to communicate with DA16600. |
  |            | SCI_CFG_CH6_RX_BUFSIZ | 80 | 8192 | The RX buffer size needs to be increased to communicate with DA16600. |
  |            | SCI_CFG_TEI_INCLUDED | 0 | 1 | Transmit end interrupt is used. |
  | r_fwup     | FWUP_CFG_UPDATE_MODE | 1 | 0 | This project uses Dual bank function. |
  |            | FWUP_CFG_FUNCTION_MODE | 0 | 1 | This project is user program. |
  |            | FWUP_CFG_MAIN_AREA_ADDR_L | 0xFFE00000U | 0xFFF00000U | This value is set according to the RX65N ROM 2MB product. |
  |            | FWUP_CFG_BUF_AREA_ADDR_L | 0xFFEF8000U | 0xFFE00000U | This value is set according to the RX65N ROM 2MB product. |
  |            | FWUP_CFG_AREA_SIZE | 0xF8000U | 0xF0000U | This value is set according to the RX65N ROM 2MB product. |

  #### CK-RX65N Bootloader Projects

  | FIT module | Config name | Default Value | Project value | Reason for change |
  |------------|-------------|---------------|---------------|-------------------|
  | r_bsp      | BSP_CFG_USER_CHARPUT_ENABLED | 0 | 1 | Use with log output function. |
  |            | BSP_CFG_CODE_FLASH_BANK_MODE | 1 | 0 | This project uses the Dual bank function. |
  | r_flash_rx | FLASH_CFG_CODE_FLASH_ENABLE | 0 | 1 | Bootloader rewrites the code flash. |
  |            | FLASH_CFG_CODE_FLASH_RUN_FROM_ROM | 0 | 1 | Bootloader is implemented to execute code that rewrites the code flash from another bank. |
  | r_sci_rx   | SCI_CFG_CH5_INCLUDED | 0 | 1 | SCI CH5 is used to write firmware and output log information. |
  | r_fwup     | FWUP_CFG_UPDATE_MODE | 1 | 0 | This project uses the Dual bank function. |
  |            | FWUP_CFG_MAIN_AREA_ADDR_L | 0xFFE00000U | 0xFFF00000U | This value is set according to the RX65N ROM 2MB product. |
  |            | FWUP_CFG_BUF_AREA_ADDR_L | 0xFFEF8000U | 0xFFE00000U | This value is set according to the RX65N ROM 2MB product. |
  |            | FWUP_CFG_AREA_SIZE | 0xF8000U | 0xF0000U | This value is set according to the RX65N ROM 2MB product. |

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
