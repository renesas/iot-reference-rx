# FreeRTOS RX Family IoT Reference
## Introduction
This project demonstrates how to integrate modular [ FreeRTOS kernel ](https://www.freertos.org/RTOS.html) and [ libraries ](https://github.com/renesas/iot-reference-rx#supported-board) which can run RX Family official device drivers package [ RX Driver Package ](https://github.com/renesas/rx-driver-package).

Refer to the [ Getting Started Guide ](Getting_Started_Guide.md) for step by step instructions on setting up your development environment.

This includes demonstration tasks for the following AWS services:
* [AWS IoT Device Shadow](https://docs.aws.amazon.com/iot/latest/developerguide/iot-device-shadows.html)
* [AWS IoT Device Defender](https://docs.aws.amazon.com/iot/latest/developerguide/device-defender.html)
* [AWS IoT Jobs](https://docs.aws.amazon.com/iot/latest/developerguide/iot-jobs.html)
* [MQTT File Delivery](https://docs.aws.amazon.com/iot/latest/developerguide/mqtt-based-file-delivery.html)
* [AWS IoT OTA Update](https://docs.aws.amazon.com/freertos/latest/userguide/freertos-ota-dev.html)

The demo projects both connect to AWS IoT core via the included Ethernet or Cellular and use the [CoreMQTT-Agent](https://github.com/FreeRTOS/coreMQTT-Agent) library to share a single MQTT connection among multiple tasks. These tasks publish environemnt and motion sensor data from a subset of the sensor available on the development board, and demonstrate use of the AWS IoT Device Shadow and Device Defender services.

### Supported Board
#### Ethernet
* [RSK-RX65N](https://www.renesas.com/products/microcontrollers-microprocessors/rx-32-bit-performance-efficiency-mcus/rx65n-2mb-starter-kit-plus-renesas-starter-kit-rx65n-2mb)

#### Ethernet
* [CK-RX65N](https://www.renesas.com/products/microcontrollers-microprocessors/rx-32-bit-performance-efficiency-mcus/ck-rx65n-cloud-kit-based-rx65n-mcu-group)

## AWS IoT Core Demo Tasks
* MQTT Agent
* IoT Defender
* OTA Update
* Environment Sensor Publishing
* Motion Sensor Publishing

## Key Software Components
### FreeRTOS TCP/IP Stack
See [ FreeRTOS TCP/IP ](https://github.com/FreeRTOS/FreeRTOS-Plus-TCP) for details.

### Mbedtls 3.1.0 TLS and Cryptography library <todo: confirm which version renesas would use>
See [ MbedTLS ](https://github.com/Mbed-TLS/mbedtls/tree/d65aeb37349ad1a50e0f6c9b694d4b5290d60e49) for details.

### Command Line Interface (CLI)
The CLI interface located in the Common/cli directory is used to provision the device. It also provides other Unix-like utilities. See [Common/cli](Common/cli/ReadMe.md) for details.

### Key-Value Store <todo: confirm which s/w renesas would use>
The key-value store located in the Common/kvstore directory is used to store runtime configuration values in non-volatile flash memory.
See [Common/kvstore](Common/kvstore/ReadMe.md) for details.

### PkiObject API <todo: confirm which scheme renesas would use>
The PkiObject API takes care of some of the mundane tasks in converting between different representations of cryptographic objects such as public keys, private keys, and certificates. See [Common/crypto](Common/crypto/ReadMe.md) for details.

### Mbedtls Transport <todo: confirm renesas can do this>
The *Common/net/mbedtls_transport.c* file contains a transport layer implementation for coreMQTT and coreHTTP which uses mbedtls to encrypt the connection in a way supported by AWS IoT Core.

Optionally, client key / certificate authentication may be used with the mbedtls transport or this parameter may be set to NULL if not needed.
### Cloning the Repository
For long path name:
```
git config --global core.longpaths true
```
To clone using HTTPS:
```
git clone https://github.com/renesas/iot-reference-rx.git --recurse-submodules
```
Using SSH:
```
git clone git@github.com:renesas/iot-reference-rx --recurse-submodules
```
If you have downloaded the repo without using the `--recurse-submodules` argument, you should run:
```
git submodule update --init --recursive
```
## Running the demos
To get started running demos, see the [Getting Started Guide](Getting_Started_Guide.md).

## Contribution
See [CONTRIBUTING](https://github.com/renesas/iot-reference-rx/blob/main/CONTRIBUTING.md) for more information.

## License
Source code located in the *Projects*, *Common*, *Middleware/AWS*, and *Middleware/FreeRTOS* directories are available under the terms of the MIT License. See the LICENSE file for more details.

Other libraries located in the *Drivers* and *Middleware* directories are available under the terms specified in each source file.
