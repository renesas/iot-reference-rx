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

* The demo project uses a maximum of 8960 bytes of Data Flash from address 0x00100000 to 0x001022FF using LittleFS.
  * This area cannot be overwritten by user.
  * If you intend to add more Data Flash allocation to LittleFS, please increase the value of `LFS_FLASH_BLOCK_COUNT`.
  * If `LFS_FLASH_BLOCK_COUNT` is specified less than 8960 bytes, the demo will not work properly.
* Therefore, 23808 bytes of Data Flash area at address 0x00102300 or above can be used as user application area.

#### Data Flash memory map

The following table indicates how memory in Data Flash in CK-RX65N is used when `LFS_FLASH_BLOCK_COUNT` equals to 8960.

<table>
    <thead>
        <tr>
            <th>Description</th>
            <th>Content</th>
            <th>Start Address<br>[Area size]</th>
            <th>Section Name</th>
        </tr>
    </thead>
    <tbody>
        <tr>
            <td>LittleFS management area</td>
            <td>IoT const data<br>(Thingname, endpoint, claim credentials, <br>device credentials)</td>
            <td>0x00100000<br>[8960 Bytes]</td>
            <td>C_LITTLEFS_MANAGEMENT_AREA</td>
        </tr>
        <tr>
            <td>User application area</td>
            <td>Free area<br>(Free area for user application)</td>
            <td>0x00102300<br>[23808 Bytes]</td>
            <td>C_USER_APPLICATION_AREA</td>
        </tr>
    </tbody>
</table>

## Limitation

* OTA update is not supported.
* ROM/RAM size is not optimized.
* CLI task cannot run after starting the demo to avoid the SCI conflict.
* The project generation feature on e2 studio can not be supported.

## Contribution

See [CONTRIBUTING](CONTRIBUTING.md) for more information.

## License

* Source code located in the *Projects*, *Common*, *Middleware/AWS*, and *Middleware/FreeRTOS* directories are available under the terms of the MIT License. See the LICENSE file for more details.
* Other libraries located in the *Drivers* and *Middleware* directories are available under the terms specified in each source file.
* Each Renesas FIT Modules in RX driver package located in the Projects/xxx/xxx/src/smc_gen are available under the terms of the basically MIT License. See the doc/license of following URL for more details.
[https://github.com/renesas/rx-driver-package](https://github.com/renesas/rx-driver-package)

## Support

Visit the following official webpage if having any technical questions.

* [English window](https://en-support.renesas.com/dashboard)
* [Japanese window](https://ja-support.renesas.com/dashboard)
* [Chinese window](https://zh-support.renesas.com/dashboard)
