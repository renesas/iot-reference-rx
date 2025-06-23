# Getting Started Guide

* This document explains demo and how to run them.  
* This demo supports the FreeRTOS 202406.01 LTS libraries.  
* This demo only works with CK-RX65N v2.

## Summary of demos

If demo does not work properly, please refer to [Troubleshooting](#troubleshooting).

The following table describes the different combinations of demos.

|Demo Name|Based AWS IoT contents|Description|Each information|
| ---- | ---- | ---- | ---- |
|PubSub/MQTT|[coreMQTT demos](https://docs.aws.amazon.com/freertos/latest/userguide/mqtt-demo.html)|It demonstrates simple MQTT communication between device and AWS server.|- [Ethernet project](/Projects/aws_ether_ck_rx65n_v2/ether_pubsub_information.md)<BR>- [Cellular project](/Projects/aws_ryz014a_ck_rx65n_v2/ryz014a_pubsub_information.md)<BR>- [Wi-Fi project](/Projects/aws_da16600_ck_rx65n_v2/da16600_pubsub_information.md)|
|PubSub/MQTT with Over-the-air(OTA)| [OTA tutorial](https://docs.aws.amazon.com/freertos/latest/userguide/dev-guide-ota-workflow.html) | It provides the steps to update the firmware on your device.|- [Ethernet project](/Projects/aws_ether_ck_rx65n_v2/ether_ota_information.md)<BR>- [Cellular project](/Projects/aws_ryz014a_ck_rx65n_v2/ryz014a_ota_information.md)<BR>- [Wi-Fi project](/Projects/aws_da16600_ck_rx65n_v2/da16600_ota_information.md)<BR>- [Boot loader project](/Projects/boot_loader_ck_rx65n_v2/bootloader_information.md)|

Each demo is independent as a FreeRTOS's task. It means multiple demos can be run at the same time.

The demos connect to AWS IoT core via the Ethernet, Cellular, or Wi-Fi with MQTT protocol by using the [CoreMQTT-Agent](https://github.com/FreeRTOS/coreMQTT-Agent) library to share a single MQTT connection among multiple tasks.  
These tasks establish a server and client-authenticated TLS connection, and demonstrates the subscribe-publish workflow of MQTT.  

> **Note** :
>
> * Fleet Provisioning demo is not supported in this version.  
> * TCP minimal will only be created when you create as a new project (PG) and usage is described in [minimal_tcp/README.md](https://github.com/renesas/iot-reference-rx/blob/main/Configuration/samples/minimal_tcp/README.md).

<details>
<summary><h2>How to run demos</h2></summary>

This chapter explains step by step instructions for running demos by importing project into e2 studio.  
When you create as a new project (PG) according to the FAQ below, you can skip step1 and step3.  
<https://github.com/renesas/iot-reference-rx/wiki/Creating-and-importing-a-FreeRTOS-project#create-a-new-freertos-project>  

### Prerequisites

#### Hardware requirements

* [CK-RX65N v2](https://www.renesas.com/products/microcontrollers-microprocessors/rx-32-bit-performance-efficiency-mcus/ck-rx65n-cloud-kit-based-rx65n-mcu-group)
  * Ethernet  
  * [RYZ014A](https://www.renesas.com/br/en/products/wireless-connectivity/cellular-iot-modules/ryz014a-lte-cat-m1-cellular-iot-module)[(Obsolete)](https://www.renesas.com/document/eln/plc-240004-end-life-eol-process-select-part-numbers?r=1503996) as Cellular (CAT-M1) module if you use Cellular communication
    * This product is no longer in production and technical support may be limited.  
  * [DA16600](https://www.renesas.com/us/en/products/wireless-connectivity/wi-fi/low-power-wi-fi/da16600mod-ultra-low-power-wi-fi-bluetooth-low-energy-combo-modules-battery-powered-iot-devices) as Wi-Fi module if you use Wi-Fi communication

#### Software requirements

* IDE: [e2 studio](https://www.renesas.com/software-tool/e-studio#download) 2025-04
* Compiler:
  * [CC-RX](https://www.renesas.com/software-tool/cc-compiler-package-rx-family) V3.07.00
  * [GCC](https://llvm-gcc-renesas.com/rx-download-toolchains/) for Renesas RX v8.3.0.202411  
* Code generator: [RX Smart Configurator](https://www.renesas.com/software-tool/rx-smart-configurator)
  * It is installed with e2 studio.
* Serial terminal application: such as Tera Term
  * Recommend using [Tera Term v4.108](https://github.com/TeraTermProject/teraterm/releases/tag/v4.108)

#### Choosing the sample project

Each connectivity has a corresponding sample project as described in the following table:

| RX MCU and Board | Connectivity | Project name | Boot loader | Compiler | Remark |
|:-----------------|:-------------|:-------------|:------------|:---------|:-------|
| CK-RX65N v2 | Ethernet | aws_ether_ck_rx65n_v2 | boot_loader_ck_rx65n_v2 | CC-RX/GCC |
| CK-RX65N v2 | Cellular (Cat-M1)(Obsolete) | aws_ryz014a_ck_rx65n_v2 | boot_loader_ck_rx65n_v2 |CC-RX/GCC|   |
| CK-RX65N v2 | Wi-Fi (DA16600) | aws_da16600_ck_rx65n_v2 | -- | CC-RX/GCC | See Note |

> **Note**: The Wi-Fi (DA16600) project only supports PubSub/MQTT sample project.  

The following are combinations of demos that can be tried for each procedure.

|Operating Procedure|Macro Settings|PubSub|Fleet Provisioning|OTA|
|---|---|---|---|---|
|[Step 4-1:<BR>Run PubSub/MQTT sample project](#step-4-1-run-pubsubmqtt-sample-project)|`ENABLE_FLEET_PROVISIONING_DEMO (0)`<BR>`ENABLE_OTA_UPDATE_DEMO (0)`|✓|-|-|
|[Step 4-2:<BR>Run PubSub/MQTT with Over-the-air(OTA) update sample project](#step-4-2-run-pubsubmqtt-with-over-the-airota-update-sample-project)|`ENABLE_FLEET_PROVISIONING_DEMO (0)`<BR>`ENABLE_OTA_UPDATE_DEMO (1)`|✓|-|✓|

* The macro configuration is done in the file \src\frtos_config\demo_config.h.  
* Be sure to complete Step 1 to 3 before performing Step 4 in the table above.

---

### Step 1: Downloading this product

At first step, prepare this product on your local environment.
Clone this repository by the following commands.

* For long path name:  

  ```text
  git config --global core.longpaths true
  ```

* Run the clone command (depending on your environment):
  * To clone using HTTPS:  

    ```text
    git clone https://github.com/renesas/iot-reference-rx.git -b v202406.01-LTS-rx-1.1.0 --recurse-submodules
    ```  

  * To clone using  SSH:

    ```text
    git clone git@github.com:renesas/iot-reference-rx -b v202406.01-LTS-rx-1.1.0 --recurse-submodules
    ```  

### Step 2: Hardware setup

#### To connect for communication

1. Hardware setup in case of Ethernet
    * Connect an internet-connected Ethernet cable to the Ethernet port attached on device board.
2. Hardware setup in case of Cellular
    * [**RYZ014A hardware Setup**](/Projects/aws_ryz014a_ck_rx65n_v2/ryz014a_pubsub_information.md#hardware-setup)
3. Hardware setup in case of Wi-Fi
    * [**DA16600 hardware Setup**](/Projects/aws_da16600_ck_rx65n_v2/da16600_pubsub_information.md#hardware-setup)

#### To connect for power supply, debugging, and logging

For more details, refer to the [manual of CK-RX65N v2](https://www.renesas.com/document/mat/ck-rx65n-v2-users-manual?r=1611756).

1. Set up jumpers of device board as follows:
    * J2 Short for invalidating MCU current measurement point
    * J11 Open for single-chip mode as MCU boot option
    * J16 1-2 Short for debugging
2. For power supply and debugging, connect an USB cable between the debug connector (J14) on the CK-RX65N v2 and your PC with installed e2 studio.  
    This connector has a role of both of power supply and debugging.
3. For receiving debug logs, connect an USB cable between the USB-serial connector (J10) on the CK-RX65N v2 and your PC with serial terminal application to receive logs.

> **Note**: that debug logs mean serial output data which is coded in the demo program and Renesas driver software.  
> It is not directly related to debugging on e2 studio.

* Board settings image for ethernet :  
![2](https://github.com/renesas/iot-reference-rx/wiki/getting_started_guide_image/step2_v2_ether.png?raw=true)
* [Board settings image for cellular](/Projects/aws_ryz014a_ck_rx65n_v2/ryz014a_pubsub_information.md#board-settings-image)
* [Board settings image for Wi-Fi](/Projects/aws_da16600_ck_rx65n_v2/da16600_pubsub_information.md#board-settings-image)

### Step 3: Import project into e2 studio

Import demo project into IDE; e2 studio.

1. Open e2 studio.
1. Choose workspace and click **Launch**.
1. **File** -> **Import...** -> **Existing Project into WorkSpace**.
1. Click **Browse...** and choose **aws_ether_ck_rx65n_v2** (for Ethernet) demo.  
  In the project folder, there is a folder corresponding to your compiler. Select the **e2studio_ccrx** folder.  
   * The **e2studio_gcc** folder contains the GCC compiler project.  
   * The **flash_project** folder contains a project for writing firmware to the RX65N flash memory using the [**Renesas Flash Programmer**](https://www.renesas.com/software-tool/renesas-flash-programmer-programming-gui?srsltid=AfmBOooUVDTEBakGjnRCPWrDtsjxbOEhQTCX3KR6HLw2oXfya7HAwy2_) during OTA execution.
  
    ![3-4](https://github.com/renesas/iot-reference-rx/wiki/getting_started_guide_image/step3_4_project_import_l3_2.png?raw=true)

    > **Note:**  
    > Ensure that **"copy project into workspace"** is **not selected**.  
    > ![3-4](https://github.com/renesas/iot-reference-rx/wiki/getting_started_guide_image/step3_4_project_import2_l3_1.png?raw=true)  
1. Click **Finish** to import the project.

### Step 4: Run demos

#### Step 4-1: Run PubSub/MQTT sample project

In this chapter, the "PubSub/MQTT sample project" will be referred to as the "PubSub Demo".

##### Step 4-1-1: Software setup for PubSub Demo

###### Configure combination of demos for PubSub Demo

Configure settings to select combination of demos.  
In "*Projects\\<project_name>\\e2studio_ccrx\\src\\frtos_config\\demo_config.h*", configure both of `ENABLE_FLEET_PROVISIONING_DEMO` and `ENABLE_OTA_UPDATE_DEMO`, which allow to select the different supported demo combinations:

> **Note:**  
>
> * In the above, *<project_name>* refers to the specific project name described in `'Choosing the sample project'`.
> * The default demo application is "PubSub Demo", disable the OTA and Fleet Provisiong demos to run PubSub only demo.

* `ENABLE_FLEET_PROVISIONING_DEMO`: (0)
* `ENABLE_OTA_UPDATE_DEMO`: (0)  
![4-1-1](https://github.com/renesas/iot-reference-rx/wiki/getting_started_guide_image/step4_1_1config_l3_2.png?raw=true)

###### Download the FIT module

FIT modules can be downloaded using the RX Smart Configurator.  
If a gray icon appears in the component tree in the Smart Configurator, the corresponding component does not exist in your environment and you will need to download the required component.  
If it is already installed, skip the download step.  
Click the module with the gray icon and select **Downloading it** to download the missing module.  
![4-1-1](https://github.com/renesas/iot-reference-rx/wiki/getting_started_guide_image/Step4_1_1_download_FIT_2.png?raw=true)  

##### Settings for each connectivity

###### 1. Using Cellular

* [Settings of the hook function](/Projects/aws_ryz014a_ck_rx65n_v2/ryz014a_pubsub_information.md#settings-of-the-hook-function)
* [Settings of access point](/Projects/aws_ryz014a_ck_rx65n_v2/ryz014a_pubsub_information.md#settings-of-access-point)
* [Settings of bands](/Projects/aws_ryz014a_ck_rx65n_v2/ryz014a_pubsub_information.md#settings-of-bands)

###### 2. Using Wi-Fi

* [Settings of the hook function](/Projects/aws_da16600_ck_rx65n_v2/da16600_pubsub_information.md#settings-of-the-hook-function)
* [Settings of Country code and GMT timezone](/Projects/aws_da16600_ck_rx65n_v2/da16600_pubsub_information.md#settings-of-country-code-and-gmt-timezone)
* [Settings of Wi-Fi network](/Projects/aws_da16600_ck_rx65n_v2/da16600_pubsub_information.md#settings-of-wi-fi-network)

##### Step 4-1-2: Building for PubSub Demo

Build firmware image by the builder with e2 studio.  
In the **Project Explorer** pane of e2 studio, Right click on the project and select **Build Project**. Wait for completing building.

> **Note:** Or choose **Project**->**Build Project** menu to build.

![4-1-1](https://github.com/renesas/iot-reference-rx/wiki/getting_started_guide_image/4_1_3_build_l3_2.png?raw=true)

##### Step 4-1-3: Create RSA key pair and device certificate for PubSub Demo

RSA key pair and device certificate can be generated with AWS Console when creating new iot-thing.  
For more information about create iot-thing with **Auto-generate a new certificate**, please refer to the following webpage:  
<https://github.com/renesas/iot-reference-rx/wiki/Register-device-to-AWS-IoT>  

If you follow the steps in the URL, the following three files will be generated.

* xxxx-certificate.pem.crt
* xxxx-public.pem.key
* xxxx-private.pem.key

##### Step 4-1-4: Set up serial terminal application for PubSub Demo

Configure Tera Term settings as following if you use it:

* New line configuration: Setup->Terminal->New-line
  * Receive : AUTO
  * Transmit: CR+LF  

  ![4-1-4](https://github.com/renesas/iot-reference-rx/wiki/getting_started_guide_image/step4_1_5teraterm_setting1.PNG?raw=true)
* Serial port settings: Setup->Serial port
  * Port : Your COM
* Speed : 115200
* Data : 8 bit
* Parity : none
* Stop bits: 1 bit
* Flow control : none  
  ![4-1-4](https://github.com/renesas/iot-reference-rx/wiki/getting_started_guide_image/step4_1_5teraterm_setting2.PNG?raw=true)

##### Step 4-1-5: AWS MQTT test client settings  

You can run the PubSub Demo to see the communication messages in AWS.  
To do this, you must first set up the **MQTT test client**.

* Open the **MQTT test client** website.  
  <https://us-east-2.console.aws.amazon.com/iot/home?region=us-east-2#/test>

> **Note:** The above URL is an example, change it to the AWS site that matches your region.

* Click **Subscribe to a topic**, enter `#` in the **Topic filter**, and click the `Subscribe` button.  
  ![4-1-7](https://github.com/renesas/iot-reference-rx/wiki/getting_started_guide_image/step4_1_7_2.PNG?raw=true)
* You can check the communication messages by running the demo.  
  For instructions on how to check the messages, see [Step 4-1-9](#step-4-1-9-check-messages-with-the-aws-mqtt-test-client).

##### Step 4-1-6: Flashing for PubSub Demo

In the **Project Explorer** panel of e2 studio, right click on the project and select **Debug As** --> **Renesas GDB Hardware Debugging**.  
 ![4-1-5](https://github.com/renesas/iot-reference-rx/wiki/getting_started_guide_image/step4_1_5debug_start_l3_2.png?raw=true)  

If the window below appears, press "Switch".  
 ![4-1-5](https://github.com/renesas/iot-reference-rx/wiki/getting_started_guide_image/step4_1_5debug_message.PNG?raw=true)

Press the following button to start debugging.  
The first command of [step 4-1-7](#step-4-1-7-input-via-cli-for-pubsub-demo) must be entered on Tera Term within 10 seconds after the start of debugging.  
It is recommended that you have Tera Term ready for easy input.  
 ![4-1-5](https://github.com/renesas/iot-reference-rx/wiki/getting_started_guide_image/step4_1_5_press_start_l3_2.png?raw=true)

##### Step 4-1-7: Input via CLI for PubSub Demo

CLI runs on the serial terminal application when starting demo.  
Input the following commands to the serial terminal application to start communication to AWS.

The summarized flow of the command input is as follows:

1. Switch to CLI mode.
1. Set IoT information (thing name and MQTT endpoint).
1. Set security information (device certificate and private key).
1. Commit preceding changes.
1. Reset and restart demo.

###### Switch to CLI mode

CLI automatically runs just after starting demo.  
First, Switch to CLI mode by inputting the following command:

```text
> CLI
Going to FreeRTOS-CLI !
```

 ![4-1-6](https://github.com/renesas/iot-reference-rx/wiki/getting_started_guide_image/step4_1_6_1_l3_2.PNG?raw=true)  

> **Note:**
>
> * CLI commands are case-sensitive, please make sure to use the correct case.
> * CLI will timeout after some seconds. After the timeout, user application of demo runs.

###### Set IoT information

Next, format the Data Flash:

```text
> format
Format OK !
```

 ![4-1-6](https://github.com/renesas/iot-reference-rx/wiki/getting_started_guide_image/step4_1_6_2.PNG?raw=true)  

Next, configure the desired thing name / mqtt device identifier:

```text
> conf set thingname <iot-thing name>
OK.
```

 ![4-1-6](https://github.com/renesas/iot-reference-rx/wiki/getting_started_guide_image/step4_1_6_3thingname.PNG?raw=true)  

Next, set the mqtt endpoint to the endpoint for your account:

```text
> conf set endpoint <endpoint for your account>
OK.
```

 ![4-1-6](https://github.com/renesas/iot-reference-rx/wiki/getting_started_guide_image/step4_1_6_4endpoint_l3_2.PNG?raw=true)  

> **Note:** You can determine the endpoint for your AWS account with the ```aws iot describe-endpoint``` command or on the *Domain configurations* page of the AWS IoT Core console.

###### Set security information of target device

Next, set the device certificate:  
Drag and drop "xxxx-certificate.pem.crt" generated in [Step 4-1-3: Create RSA key pair and device certificate for PubSub Demo](#step-4-1-3-create-rsa-key-pair-and-device-certificate-for-pubsub-demo) to Tera Term.

```text
> conf set cert <Drag and drop crt file>
```

 ![4-1-6](https://github.com/renesas/iot-reference-rx/wiki/getting_started_guide_image/step4_1_6_5cert_l3_2.PNG?raw=true)  

> **Note:** About \<device certificate\>, if you use Tera Term, drag and drop the certificate file onto the Terminal screen after entered `conf set cert` , then select `Send File (Paste content of file)` on the screen that appears, and press `OK`.  
> The contents of the certificate file will be entered.

Next, set the device private key:  
Drag and drop "xxxx-private.pem.key" generated in [Step 4-1-3: Create RSA key pair and device certificate for PubSub Demo](#step-4-1-3-create-rsa-key-pair-and-device-certificate-for-pubsub-demo) to Tera Term.

```text
> conf set key <Drag and drop key file>
```

 ![4-1-6](https://github.com/renesas/iot-reference-rx/wiki/getting_started_guide_image/step4_1_6_6key_l3_2.PNG?raw=true)

> **Note:** About \<private key\>, if you use Tera Term, drag and drop the certificate file onto the Terminal screen after entered `conf set key` , then select `Send File (Paste content of file)` on the screen that appears, and press `OK`.  
> The contents of the certificate file will be entered.

###### Commit configuration changes

Next, commit the staged configuration changes to Data Flash:

```text
> conf commit
```

![4-1-6](https://github.com/renesas/iot-reference-rx/wiki/getting_started_guide_image/step4_1_6_7.PNG?raw=true)  

###### Reset the target device and restart demo

Finally, reset the target device once by the following command to restart demo:

```text
> reset
```

![4-1-6](https://github.com/renesas/iot-reference-rx/wiki/getting_started_guide_image/step4_1_6_8reset_l3_2.png?raw=true)

##### Step 4-1-8: Expected output for PubSub Demo

After demo restarts, wait for the CLI to timeout without inputting anything.
Then demo is performed with the following output.

* **PubSub Demo Task 0** and **PubSub Demo Task 1** can complete successfully.  
![4-1-7](https://github.com/renesas/iot-reference-rx/wiki/getting_started_guide_image/step4_1_7_1_l3_1.png?raw=true)

> **Note:** The PubSub Demo Task 0 and the PubSub Demo Task 1 will unsubscribe from the demo topic after publishing demo messages 10 times each.

##### Step 4-1-9: Check messages with the AWS MQTT test client

Once you configure the MQTT test client in [Step 4-1-5](#step-4-1-5-aws-mqtt-test-client-settings), you will be able to see the messages sent from the PubSub Demo on AWS.

* Display the MQTT test client website you set in [Step 4-1-5](#step-4-1-5-aws-mqtt-test-client-settings).
* You can get message "*Task x publishing message x*" published to **MQTT test client** console.  
If successful, it will be displayed on AWS console as shown below.![4-1-7](https://github.com/renesas/iot-reference-rx/wiki/getting_started_guide_image/step4_1_7_3.PNG?raw=true)  

---

#### Step 4-2: Run PubSub/MQTT with Over-the-air(OTA) update sample project

* Details for *PubSub/MQTT with Over-the-air(OTA) update sample project* are provided in the special application note (document number: R01AN7662).  
  Please check the following webpage:
  * <https://www.renesas.com/document/apn/rx-family-how-implement-freertos-ota-using-amazon-web-services-v202406-lts-rx>  
* About how to run this demo, see the chapter "*2. Prerequisites*" and beyond in preceding application note.
* Although this application note focuses on an Ethernet project, the Cellular and Wi-Fi projects also work as described in the application note.
* When running this demo, please check your RX Smart Configurator settings.  
  If a gray icon appears in the component tree in the Smart Configurator, the corresponding component is not present in your environment.  
  To download the required modules, see the "[**Download the FIT module**](#download-the-fit-module)" section.
* For more information about the bootloader used in OTA, see:  
  [**bootloader_information.md**](/Projects/boot_loader_ck_rx65n_v2/bootloader_information.md)

---
</details>

<details>
<summary><h2>Troubleshooting</h2></summary>

### 1. Improve transfer speed for OTA Update

* `mqttexampleTRANSPORT_RECV_TIMEOUT_MS` can be set to 150 or higher  to improve OTA execution time.
* Set the default value to 150 or higher after setting the sending and receiving timeout values separately for sending and receiving.

### 2. Improve timeout when connection error happens

If an error occurs during communication due to the communication environment, changing each definition to the following "correction value" may improve the problem.  

|Definition Name|Description|default value|correction value|path|
| ---- | ---- | ---- | ---- | ---- |
|mqttexampleTRANSPORT_RECV_TIMEOUT_MS| Timeout in milliseconds to receive in the transport layer. You try to change the value to more bigger when you met TLS handshake error. Changes in this value affect the time of MQTT communication. So you should adjust the value of time according to your communication environment. |450|`750`|Demos\mqtt_agent\mqtt_agent_task.c|
|MQTT_AGENT_MAX_EVENT_QUEUE_WAIT_TIME|Time in milliseconds that the MQTT agent task will wait in the Blocked state (so not using any CPU time) for a command to arrive in its command queue before exiting the blocked state so it can call MQTT_ProcessLoop().|50U|`1000U`|src\frtos_config\core_mqtt_agent_config.h|

However, each definition affects the execution time of the demo.  
If the above values are set, the execution time of the demo may increase.  

### 3. How to change MAC Address

The MAC address used in the sample project can be changed using the Smart Configurator.  
The MAC address assigned to your board can be found on a sticker on the board's Ethernet connector.  
If the sticker is not attached, please check [Tool News](https://www.renesas.com/document/tnn/notification-information-about-ck-rx65n-mac-address?r=1611756).  
![TroubleShooting_0_1_MAC_Address_Board](https://github.com/renesas/iot-reference-rx/wiki/getting_started_guide_image/TroubleShooting_0_1_MAC_Address_Board_v2.png)  
MAC Address can be changed from FreeRTOS Kernel of the Smart Configurator.  
Change Components-> FreeRTOS Kernel-> MAC address X (X = 0 to 5).  
After changing the settings, be sure to **Generate Code** and **Build**.  
![TroubleShooting_0_2_MAC_Address](https://github.com/renesas/iot-reference-rx/wiki/getting_started_guide_image/TroubleShooting_0_2_MAC_Address.png)  

### 4. LittleFS error message appears at runtime after first firmware flash

After writing the firmware, when you run the program, the following error message may appear after the CLI menu appears:

``` log
/xxxx/iot-reference-rx/Middleware/3rdparty/littlefs/lfs.c:1225:error: Corrupted dir pair at {0x1, 0x0}
```

This message that appears immediately after writing firmware is not an error message, so please continue.
> **Note:**  
> This message appears if the data flash file system has not been previously initialized.  
> After this message appears, the data flash is initialized and the file system is mounted.

### 5. Troubleshooting for Cellular project

* [Cellular Troubleshooting](/Projects/aws_ryz014a_ck_rx65n_v2/ryz014a_pubsub_information.md#troubleshooting)

### 6. Troubleshooting for Wi-Fi project

* [Wi-Fi Troubleshooting](/Projects/aws_da16600_ck_rx65n_v2/da16600_pubsub_information.md#troubleshooting)

### 7. Troubleshooting for OTA project

* [OTA Troubleshooting](/Projects/aws_ether_ck_rx65n_v2/ether_ota_information.md#troubleshooting)

</details>
