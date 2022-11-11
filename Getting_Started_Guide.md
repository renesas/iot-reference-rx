This includes demonstration tasks for the following AWS services:
* [AWS IoT Device Shadow](https://docs.aws.amazon.com/iot/latest/developerguide/iot-device-shadows.html)
* [AWS IoT Device Defender](https://docs.aws.amazon.com/iot/latest/developerguide/device-defender.html)
* [AWS IoT Jobs](https://docs.aws.amazon.com/iot/latest/developerguide/iot-jobs.html)
* [MQTT File Delivery](https://docs.aws.amazon.com/iot/latest/developerguide/mqtt-based-file-delivery.html)
* [AWS IoT OTA Update](https://docs.aws.amazon.com/freertos/latest/userguide/freertos-ota-dev.html)

The demo projects both connect to AWS IoT core via the included Ethernet or Cellular and use the [CoreMQTT-Agent](https://github.com/FreeRTOS/coreMQTT-Agent) library to share a single MQTT connection among multiple tasks. These tasks publish environemnt and motion sensor data from a subset of the sensor available on the development board, and demonstrate use of the AWS IoT Device Shadow and Device Defender services.

## AWS IoT Core Demo Tasks
* MQTT Agent
* IoT Defender
* OTA Update
* Environment Sensor Publishing

## Cloning the Repository
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

## 1 Prerequisites

### 1.1 Hardware Requirements

* CK-RX65N 
* Cellular RYZ014A


### 1.2 Software Requirements

* e2studio 2022-07. To download, visit the
     [e2studio page](https://www.renesas.com/us/en/software-tool/e-studio#download).
     (A user account is required to download.)

* [OpenSSL for Windows](https://www.openssl.org/) to create the OTA signing
    key and certificate. If you have git installed on your machine, you can also use the openssl.exe
    that comes with the git installation.

* A serial terminal application, such as [Tera Term](https://ttssh2.osdn.jp/index.html.en).


## 2 Hardware and Software Setup



1. Plug in the Cellular RYZ014A to the PMOD1 on the  CK-RX65N board for using cellular
2. Plug in the Ethernet cab to  the CK-RX65N board for using ethernet


## 3 Poting project 
### 3.1 Structure skeleton

### 3.2 Buid mbed network transport implementation
#### 3.2.1 Using Ethernet
Using [FreeRTOS-Plus-TCP] layer to poting ethernet

* Copy [socket_wrapper/freertos_plus_tcp](https://github.com/FreeRTOS/FreeRTOS/tree/main/FreeRTOS-Plus/Source/Application-Protocols/network_transport/sockets_wrapper/freertos_plus_tcp) to Middleware/abstractions/secure_sockets
* Copy [using_mbedtls_pkcs11](https://github.com/FreeRTOS/FreeRTOS/tree/main/FreeRTOS-Plus/Source/Application-Protocols/network_transport/using_mbedtls_pkcs11) to Middleware/mbedstls_utils
* Copy [mbedtls_freertos](https://github.com/FreeRTOS/FreeRTOS/tree/main/FreeRTOS-Plus/Source/Utilities/mbedtls_freertos) to Middleware/mbedstls_utils. 
*Note:Using mbedtls_bio_freertos_tcp.c*

#### 3.2.1 Using Cellular
Using .h [FreeRTOS-Plus-TCP] layer to port

* Copy and modify [socket_wrapper/cellular](https://github.com/FreeRTOS/FreeRTOS/tree/main/FreeRTOS-Plus/Source/Application-Protocols/network_transport/sockets_wrapper/cellular) to Middleware/abstractions/secure_sockets
* Copy [using_mbedtls_pkcs11](https://github.com/FreeRTOS/FreeRTOS/tree/main/FreeRTOS-Plus/Source/Application-Protocols/network_transport/using_mbedtls_pkcs11) to Middleware/mbedstls_utils
* Copy [mbedtls_freertos](https://github.com/FreeRTOS/FreeRTOS/tree/main/FreeRTOS-Plus/Source/Utilities/mbedtls_freertos) to Middleware/mbedstls_utils. 
*Note:Using mbedtls_bio_freertos_cellular.c*

| RX MCU and Board | Old Structure|New Structure |
| :----- | :---: |:---: |
| SecureSocketsTransport | SecureSocketsTransport_Connect<br>SecureSocketsTransport_Disconnect<br>SecureSocketsTransport_Recv<br>SecureSocketsTransport_Send|TLS_FreeRTOS_Connect<br>TLS_FreeRTOS_Disconnect<br>TLS_FreeRTOS_recv<br>TLS_FreeRTOS_send|
| Sockets | SOCKETS_Socket<br>SOCKETS_Connect<br>SOCKETS_Recv<br>SOCKETS_Send<br>SOCKETS_Close<br>SOCKETS_Shutdown<br>SOCKETS_Bind|Sockets_Connect<br>Sockets_Disconnect<br>Sockets_Send<br>Sockets_Recv|
| TLS layer | TLS_Init<br>TLS_Connect<br>TLS_Send<br>TLS_Recv<br>TLS_Cleanup|initMbedtls<br>tlsSetup<br>|


#####  Modify [socket_wrapper/cellular](http://global-infra-jp-main.dgn.renesas.com/vannam.dinh/iot-reference-rx/-/blob/main/Middleware/abstractions/secure_sockets/freertos_plus_tcp/sockets_wrapper_cellular.c): 
* Because sockets_wrapper.c is using [FreeRTOS-Plus-TCP] layer APIs, we need to change to use RYZ014A cellular API into each of function belows : Sockets_Connect, Sockets_Recv, Sockets_Send, Sockets_Disconnect


1. Sockets_Connect
* Create socket [R_CELLULAR_CreateSocket]
* Convert an IP address [SOCKETS_GetHostByName]
* Connect to socket [R_CELLULAR_ConnectSocket]
2. Sockets_Recv
* Call R_CELLULAR_ReceiveSocket to receive data
3. Sockets_Send
* Call R_CELLULAR_SendSocket to send data
4. Sockets_Disconnect
* Call R_CELLULAR_ShutdownSocket
* Call R_CELLULAR_CloseSocket

