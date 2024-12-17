# Getting Started Guide

* This document explains demo and how to run them.  
* This demo supports the FreeRTOS 202406.01 LTS libraries.  
* This demo only works with CK-RX65N v2.

## Summary of demo

If demo does not work properly, please refer to [Troubleshooting](#troubleshooting).

The following table indicates the character of demo.

|Demo Name|Based AWS IoT contents|Description|
| ---- | ---- | ---- |
|PubSub|[coreMQTT demo](https://docs.aws.amazon.com/freertos/latest/userguide/mqtt-demo.html)|It demonstrates simple MQTT communication between device and AWS server.|

The demo connect to AWS IoT core via the Ethernet with MQTT protocol by using the [CoreMQTT-Agent](https://github.com/FreeRTOS/coreMQTT-Agent) library to share a single MQTT connection among multiple tasks.These tasks establish a server and client-authenticated TLS connection, and demonstrates the subscribe-publish workflow of MQTT.  

**Note** : This demo does not support OTA and Fleet Provisioning.

## How to run demo

This chapter explains step by step instructions for running demo by importing project into e2 studio.  
When you create as a new project according to the FAQ below, you can skip step1 and step3.  
<https://en-support.renesas.com/knowledgeBase/21115016>  

### Prerequisites

#### Hardware requirements

* [CK-RX65 v2](https://www.renesas.com/products/microcontrollers-microprocessors/rx-32-bit-performance-efficiency-mcus/ck-rx65n-cloud-kit-based-rx65n-mcu-group)  
  * Ethernet  

#### Software requirements

* IDE: [e2 studio](https://www.renesas.com/software-tool/e-studio#download) 2024-10
  * A user account of Renesas is required to download.
* Compiler:
  * [CC-RX](https://www.renesas.com/software-tool/cc-compiler-package-rx-family) V3.06.00
  * [GCC](https://llvm-gcc-renesas.com/rx-download-toolchains/) for Renesas RX v8.3.0.202405  
* Code generator: [RX Smart Configurator](https://www.renesas.com/software-tool/rx-smart-configurator)
  * It is installed with e2 studio.
* Serial terminal application: such as Tera Term
  * Recommend using [Tera Term v4.106](https://github.com/TeraTermProject/teraterm/releases/tag/teraterm-4_106)

#### Choose project to use demo

The <project_name> term means one of the following folder name according to used communication way:

| RX MCU and Board | Connectivity | Project name | Compiler |Remark|
|:----------------:|:------------:|:------------:|:-----------:|:-----------:|
| [CK-RX65N v2](https://www.renesas.com/products/microcontrollers-microprocessors/rx-32-bit-performance-efficiency-mcus/ck-rx65n-cloud-kit-based-rx65n-mcu-group) | Ethernet | aws_ether_ck_rx65n_v2 | CC-RX/GCC |  |

The following are combinations of demo that can be tried for each procedure.

|Operating Procedure|Macro Settings|PubSub|Fleet Provisioning|OTA|
|---|---|---|---|---|
|[Step 4-1:<BR>Run PubSub/MQTT sample project](#step-4-1-run-pubsubmqtt-sample-project)|`ENABLE_FLEET_PROVISIONING_DEMO (0)`<BR>`ENABLE_OTA_UPDATE_DEMO (0)`|✓|-|-|

* The macro configuration is done in the file \src\frtos_config\demo_config.h.  
* Be sure to complete Step 1 to 3 before performing Step 4-1 in the table above.

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
    git clone https://github.com/renesas/iot-reference-rx.git --recurse-submodules
    ```  

  * To clone using  SSH:

    ```text
    git clone git@github.com:renesas/iot-reference-rx --recurse-submodules
    ```  

### Step 2: Hardware setup

#### To connect for communication

##### Hardware setup in case of Ethernet

Connect between device board and router/switch.

1. Connect an internet-connected Ethernet cable to the Ethernet port attached on device board.

#### To connect for power supply, debugging, and logging

For more details, refer to the [manual of CK-RX65N v2](https://www.renesas.com/document/mat/ck-rx65n-v2-users-manual?r=1611756).

1. Set up jumpers of device board as follows:

    * J2 Short for invalidating MCU current measurement point
    * J11 Open for single-chip mode as MCU boot option
    * J16 1-2 Short for debugging

2. For power supply and debugging, connect an USB cable between the debug connector (J14) on the CK-RX65N v2 and your PC with installed e2 studio.
    This connector has a role of both of power supply and debugging.
3. For receiving debug logs, connect an USB cable between the USB-serial connector (J10) on the CK-RX65N v2 and your PC with serial terminal application to receive logs.
    Note that debug logs mean serial output data which is coded in the demo program and Renesas driver software.
    It is not directly related to debugging on e2 studio.

Board settings image for ether :  
![2](https://github.com/renesas/iot-reference-rx/wiki/getting_started_guide_image/step2_v2_ether.png?raw=true)  

### Step 3: Import project into e2 studio

Import demo project into IDE; e2 studio.

1. Open e2 studio.
1. Choose workspace and click **Launch**.
1. **File** -> **Import...** -> **Existing Project into WorkSpace**.
1. Click **Browse...** and choose **aws_ether_ck_rx65n_v2** (for Ethernet) demo.  
In the project folder, there is a folder corresponding to your compiler. Select the **e2studio_ccrx** folder.  
   * The **e2studio_gcc** folder contains the GCC compiler project.  

![3-4](https://github.com/renesas/iot-reference-rx/wiki/getting_started_guide_image/step3_4_project_import_l3_1.png?raw=true)

**Note:** Ensure that **copy project into workspace** is not selected.  
![3-4](https://github.com/renesas/iot-reference-rx/wiki/getting_started_guide_image/step3_4_project_import2_l3_1.png?raw=true)  

1. Click **Finish** to import the project.

### Step 4: Run demo

#### Step 4-1: Run PubSub/MQTT sample project

See this chapter when running *PubSub/MQTT sample project*.  
In this chapter, the "PubSub/MQTT sample project" will be referred to as the "PubSub Demo".

##### Step 4-1-1: Software setup for PubSub demo

Configure settings to run PubSub demo.

###### Configure combination of demo for PubSub demo

Configure settings to select combination of demo.
In "Projects\\<project_name>\\e2studio_ccrx\\src\\frtos_config\\demo_config.h", configure both of `ENABLE_FLEET_PROVISIONING_DEMO` and `ENABLE_OTA_UPDATE_DEMO`, which allow to select the different supported demo combinations:

* `ENABLE_FLEET_PROVISIONING_DEMO`: (0)
* `ENABLE_OTA_UPDATE_DEMO`: (0)  
![4-1-1](https://github.com/renesas/iot-reference-rx/wiki/getting_started_guide_image/step4_1_1config_l3_1.png?raw=true)

###### Settings of RX Smart Configurator

Configure the Renesas code generator (RX Smart Configurator*).  
Please open RX Smart Configurator* and configure settings according to the image below.  
Select **aws_ether_ck_rx65n_v2.scfg** --> **Components** --> **Icon for adding components** --> **Configure general settings...** --> **Component** --> **code generation behavior column** --> **Update configuration files**.

![4-1-1](https://github.com/renesas/iot-reference-rx/wiki/getting_started_guide_image/step4_1_1_sc_generate_setting_l3_1.png?raw=true)

*See the [manual of RX Smart Configurator](https://www.renesas.com/document/mat/rx-smart-configurator-users-guide-e-studio) for details on how to use it. The Config files generated after code generation by RX Smart Configurator are exist in the following folder:

* "Projects\\<project_name>\\e2 studio_ccrx\\src\\smc_gen"

###### Download the FreeRTOS kernel, LittleFS and FIT module

Download FreeRTOS Kernel and LittleFS.  
If FreeRTOS kernel and LittleFS is not installed, the icon will be displayed as a gray square as shown in the image below.  
If you have already installed them, please skip the download step.  
FreeRTOS kernels can be downloaded using RX Smart Configurator.  
Click on **FreeRTOS_Kernel** and then select **downloading it**.  
![4-1-1](https://github.com/renesas/iot-reference-rx/wiki/getting_started_guide_image/Step4_1_1_kernel_littlefs_l3_1.png?raw=true)  

FIT modules can be downloaded in the same way.  
Select the FIT module you wish to download, and then download it from **downloading it**.  
If you have also installed FIT, please skip this step.  
If there are other grey icons in the component tree, the corresponding components are not present in your environment and you should download the required components using the steps above.

##### Step 4-1-2: Building for PubSub demo

Build firmware image by the builder with e2 studio.  
In the **Project Explorer** pane of e2 studio, Right click on the project and select **Build Project**. Wait for completing building.

**Note:** Or choose **Project**->**Build Project** menu to build.

![4-1-1](https://github.com/renesas/iot-reference-rx/wiki/getting_started_guide_image/4_1_3_build_l3_1.png?raw=true)

##### Step 4-1-3: Create RSA key pair and device certificate for PubSub demo

RSA key pair and device certificate can be generated with AWS Console when creating new iot-thing.
For more information about create iot-thing with **Auto-generate a new certificate**, please refer to the following webpage:
<https://github.com/renesas/iot-reference-rx/wiki/Register-device-to-AWS-IoT>  

If you follow the steps in the URL, the following three files will be generated.

* xxxx-certificate.pem.crt
* xxxx-public.pem.key
* xxxx-private.pem.key

##### Step 4-1-4: Set up serial terminal application for PubSub demo

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

You can run the PubSub demo to see the communication messages in AWS.  
To do this, you must first set up the **MQTT test client**.

* Open the **MQTT test client** website.  
  <https://us-east-2.console.aws.amazon.com/iot/home?region=us-east-2#/test>  
  Note: The above URL is an example, change it to the AWS site that matches your region.
* Click **Subscribe to a topic**, enter "#" in the **Topic filter**, and click the "Subscribe" button.  
  ![4-1-7](https://github.com/renesas/iot-reference-rx/wiki/getting_started_guide_image/step4_1_7_2.PNG?raw=true)
* You can check the communication messages by running the demo.  
  For instructions on how to check the messages, see [Step 4-1-9](#step-4-1-9-check-messages-with-the-aws-mqtt-test-client).

##### Step 4-1-6: Flashing for PubSub demo

Flash firmware image to your target device by the either of following ways.

In the **Project Explorer** panel of e2 studio, right click on the project and select **Debug As** --> **Renesas GDB Hardware Debugging**.
 ![4-1-5](https://github.com/renesas/iot-reference-rx/wiki/getting_started_guide_image/step4_1_5debug_start_l3_1.png?raw=true)  

If the window below appears, press "Switch".  
 ![4-1-5](https://github.com/renesas/iot-reference-rx/wiki/getting_started_guide_image/step4_1_5debug_message.PNG?raw=true)

Press the following button to start debugging.  
The first command of step 4-1-7 must be entered on Tera Term within 10 seconds after the start of debugging.
It is recommended that you have Tera Term ready for easy input.
 ![4-1-5](https://github.com/renesas/iot-reference-rx/wiki/getting_started_guide_image/step4_1_5_press_start_l3_1.png?raw=true)

##### Step 4-1-7: Input via CLI for PubSub demo

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

 ![4-1-6](https://github.com/renesas/iot-reference-rx/wiki/getting_started_guide_image/step4_1_6_1.PNG?raw=true)  

**Note:** CLI will timeout after some seconds. If timeout occurs, core process of demo runs.

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

 ![4-1-6](https://github.com/renesas/iot-reference-rx/wiki/getting_started_guide_image/step4_1_6_4endpoint.PNG?raw=true)  

**Note:** You can determine the endpoint for your AWS account with the ```aws iot describe-endpoint``` command or on the *Domain configurations* page of the AWS IoT Core console.

###### Set security information of target device

Next, set the device certificate:  
Drag and drop "xxxx-certificate.pem.crt" generated in [Step 4-1-3: Create RSA key pair and device certificate for PubSub demo](#step-4-1-3-create-rsa-key-pair-and-device-certificate-for-pubsub-demo) to Tera Term.

```text
> conf set cert <Drag and drop crt file>
```

 ![4-1-6](https://github.com/renesas/iot-reference-rx/wiki/getting_started_guide_image/step4_1_6_5cert.PNG?raw=true)  

**Note:** About \<device certificate\>, if you use Tera Term, drag and drop the certificate file onto the Terminal screen after entered `conf set cert` , then select `Send File (Paste content of file)` on the screen that appears, and press `OK`. The contents of the certificate file will be entered.

Next, set the device private key:  
Drag and drop "xxxx-private.pem.key" generated in [Step 4-1-3: Create RSA key pair and device certificate for PubSub demo](#step-4-1-3-create-rsa-key-pair-and-device-certificate-for-pubsub-demo) to Tera Term.

```text
> conf set key <Drag and drop key file>
```

 ![4-1-6](https://github.com/renesas/iot-reference-rx/wiki/getting_started_guide_image/step4_1_6_6.PNG?raw=true) 

**Note:** About \<private key\>, if you use Tera Term, drag and drop the certificate file onto the Terminal screen after entered `conf set key` , then select `Send File (Paste content of file)` on the screen that appears, and press `OK`. The contents of the certificate file will be entered.

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

![4-1-6](https://github.com/renesas/iot-reference-rx/wiki/getting_started_guide_image/step4_1_6_8.png?raw=true)

##### Step 4-1-8: Expected output for PubSub demo

After restart demo, wait a seconds with input nothing via CLI. Then demo is performed with the following output.

* **PubSub Demo Task 0** and **PubSub Demo Task 1** can complete successfully.  
![4-1-7](https://github.com/renesas/iot-reference-rx/wiki/getting_started_guide_image/step4_1_7_1_l3_1.png?raw=true)

##### Step 4-1-9: Check messages with the AWS MQTT test client

Once you configure the MQTT test client in [Step 4-1-5](#step-4-1-5-aws-mqtt-test-client-settings), you will be able to see the messages sent from the pubsub demo on AWS.

* Display the MQTT test client website you set in [Step 4-1-5](#step-4-1-5-aws-mqtt-test-client-settings).
* You can get message "*Task x publishing message x*" published to **MQTT test client** console.  
If successful, it will be displayed on AWS console as shown below.![4-1-7](https://github.com/renesas/iot-reference-rx/wiki/getting_started_guide_image/step4_1_7_3.PNG?raw=true)  

---

## Troubleshooting

### Improve timeout when connection error happens

If an error occurs during communication due to the communication environment, changing each definition to the following "correction value" may improve the problem.  

|Definition Name|Description|default value|correction value|path|
| ---- | ---- | ---- | ---- | ---- |
|mqttexampleTRANSPORT_SEND_RECV_TIMEOUT_MS| Timeout in milliseconds to send and receive in the transport layer. You try to change the value to more bigger when you met TLS handshake error. Changes in this value affect the time of MQTT communication. So you should adjust the value of time according to your communication environment. |450|`750`|Demos\mqtt_agent\mqtt_agent_task.c|
|MQTT_AGENT_MAX_EVENT_QUEUE_WAIT_TIME|Time in milliseconds that the MQTT agent task will wait in the Blocked state (so not using any CPU time) for a command to arrive in its command queue before exiting the blocked state so it can call MQTT_ProcessLoop().|50U|`1000U`|src\frtos_config\core_mqtt_agent_config.h|

However, each definition affects the execution time of the demo.  
If the above values are set, the execution time of the demo may increase.  

### How to change MAC Address

The MAC address used in the sample project can be changed using the Smart Configurator.  
The MAC address assigned to your board can be found on a sticker on the board's Ethernet connector.  
If the sticker is not attached, please check [Tool News](https://www.renesas.com/document/tnn/notification-information-about-ck-rx65n-mac-address?r=1611756).  
![TroubleShooting_0_1_MAC_Address_Board](https://github.com/renesas/iot-reference-rx/wiki/getting_started_guide_image/TroubleShooting_0_1_MAC_Address_Board_v2.png)  
MAC Address can be changed from FreeRTOS Kernel of the Smart Configurator.  
Change Components-> FreeRTOS Kernel-> MAC address X (X = 0 to 5).  
After changing the settings, be sure to **Generate Code** and **Build**.  
![TroubleShooting_0_2_MAC_Address](https://github.com/renesas/iot-reference-rx/wiki/getting_started_guide_image/TroubleShooting_0_2_MAC_Address.png)  
