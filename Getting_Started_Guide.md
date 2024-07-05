# Getting Started Guide

This document explains demo and how to run them.

## Summary of demos

If demo does not work properly, please refer to [Troubleshooting](#troubleshooting).

The following table indicates the character of demos.
|Demo Name|Based AWS IoT contents|Description|
| ---- | ---- | ---- |
|PubSub|[coreMQTT demos](https://docs.aws.amazon.com/freertos/latest/userguide/mqtt-demo.html)|It demonstrates simple MQTT communication between device and AWS server.|
|Fleet Provisioning|[AWS IoT fleet provisioning](https://docs.aws.amazon.com/iot/latest/developerguide/provision-wo-cert.html)|It provides the some secure provisioning operation to device at first connection to AWS server.|
|OTA| [OTA tutorial](https://docs.aws.amazon.com/freertos/latest/userguide/dev-guide-ota-workflow.html) | It provides the steps to update the firmware on your device.|

Each demo is independent as a FreeRTOS's task. It means multiple demos can be run at the same time.

The demos connect to AWS IoT core via the Ethernet, Cellular, or Wi-Fi with MQTT protocol by using the [CoreMQTT-Agent](https://github.com/FreeRTOS/coreMQTT-Agent) library to share a single MQTT connection among multiple tasks.These tasks establish a server and client-authenticated TLS connection, and demonstrates the subscribe-publish workflow of MQTT.

It is also possible to demonstrate MQTT connection using TLS communication with TSIP.

## How to run demos

This chapter explains step by step instructions for running demos by importing projects into e2 studio.  
When you create as a new projects according to the FAQ below, you can skip step1 and step3.  
https://en-support.renesas.com/knowledgeBase/21115016  

### Prerequisites

#### Hardware requirements

* [CK-RX65N](https://www.renesas.com/products/microcontrollers-microprocessors/rx-32-bit-performance-efficiency-mcus/ck-rx65n-cloud-kit-based-rx65n-mcu-group)
* [RYZ014A ](https://www.renesas.com/br/en/products/wireless-connectivity/cellular-iot-modules/ryz014a-lte-cat-m1-cellular-iot-module)[(Obsolete)](https://www.renesas.com/document/eln/plc-240004-end-life-eol-process-select-part-numbers?r=1503996) as Cellular (CAT M1) module if you use Cellular communication

  * [This product](https://www.renesas.com/products/wireless-connectivity/cellular-iot-modules/ryz014a-lte-cat-m1-cellular-iot-module) is no longer in production and technical support may be limited. ryz014a-lte-cat-m1-cellular-iot-module)

  * [End Of Life Notice: PLC# : 240004 End-of-Life (EOL) process on select part numbers](https://www.renesas.com/document/eln/plc-240004-end-life-eol-process-select-part-numbers?r=1503996)

* [DA16600](https://www.renesas.com/us/en/products/wireless-connectivity/wi-fi/low-power-wi-fi/da16600mod-ultra-low-power-wi-fi-bluetooth-low-energy-combo-modules-battery-powered-iot-devices) as Wi-Fi module if you use Wi-Fi communication

#### Software requirements

* IDE: [e2 studio](https://www.renesas.com/us/en/software-tool/e-studio#download) 2024-04
  * A user account of Renesas is required to download.
* Compiler:
  * [CC-RX](https://www.renesas.com/software-tool/cc-compiler-package-rx-family) V3.06.00
  * [GCC](https://llvm-gcc-renesas.com/rx-download-toolchains/) for Renesas RX v8.3.0.202311  
   **Note** : Demo with TSIP only supports CC-RX.
* Code generator: [RX Smart Configurator](https://www.renesas.com/software-tool/rx-smart-configurator)
  * It is installed with e2 studio.
* Serial terminal application: such as **TeraTerm**
  * Recommend using [TeraTerm v4.106](https://github.com/TeraTermProject/teraterm/releases/tag/teraterm-4_106)

#### Choose project to use demo

The <project_name> term means one of the following folder name according to used communication way:

| RX MCU and Board | Connectivity | Project name | Boot loader | Compiler |Remark|
|:----------------:|:------------:|:------------:|:-----------:|:-----------:|:-----------:|
| [CK-RX65N v1](https://www.renesas.com/products/microcontrollers-microprocessors/rx-32-bit-performance-efficiency-mcus/ck-rx65n-cloud-kit-based-rx65n-mcu-group) | Ethernet | aws_ether_ck_rx65n | boot_loader_ck_rx65n |CC-RX/GCC|See Note 1|
| [CK-RX65N v1](https://www.renesas.com/products/microcontrollers-microprocessors/rx-32-bit-performance-efficiency-mcus/ck-rx65n-cloud-kit-based-rx65n-mcu-group) | Cellular(CAT M1) [RYZ014A](https://www.renesas.com/br/en/products/wireless-connectivity/cellular-iot-modules/ryz014a-lte-cat-m1-cellular-iot-module)[(Obsolete)](https://www.renesas.com/document/eln/plc-240004-end-life-eol-process-select-part-numbers?r=1503996) | aws_ryz014a_ck_rx65n | boot_loader_ck_rx65n |CC-RX/GCC||
| [CK-RX65N v2](https://www.renesas.com/products/microcontrollers-microprocessors/rx-32-bit-performance-efficiency-mcus/ck-rx65n-cloud-kit-based-rx65n-mcu-group) | Wi-Fi [DA16600](https://www.renesas.com/us/en/products/wireless-connectivity/wi-fi/low-power-wi-fi/da16600mod-ultra-low-power-wi-fi-bluetooth-low-energy-combo-modules-battery-powered-iot-devices) | aws_da16600_ck_rx65n | boot_loader_ck_rx65n_v2 |CC-RX/GCC||
| [CK-RX65N v1](https://www.renesas.com/products/microcontrollers-microprocessors/rx-32-bit-performance-efficiency-mcus/ck-rx65n-cloud-kit-based-rx65n-mcu-group)|Ethernet<BR>(with TSIP)|aws_ether_tsip_ck_rx65n|boot_loader_ck_rx65n|CC-RX|See Note 1<BR>See Note 2|
| [CK-RX65N v1](https://www.renesas.com/products/microcontrollers-microprocessors/rx-32-bit-performance-efficiency-mcus/ck-rx65n-cloud-kit-based-rx65n-mcu-group)|Cellular(CAT M1) [RYZ014A](https://www.renesas.com/br/en/products/wireless-connectivity/cellular-iot-modules/ryz014a-lte-cat-m1-cellular-iot-module)[(Obsolete)](https://www.renesas.com/document/eln/plc-240004-end-life-eol-process-select-part-numbers?r=1503996)<BR>(with TSIP)|aws_ryz014a_tsip_ck_rx65n|boot_loader_ck_rx65n|CC-RX|See Note 2|

> **Note 1**: If you want to communicate **Ethernet** with **CK-RX65Nv2** , please refer [How to use Ethernet communication with CK-RX65Nv2](#how-to-use-ethernet-communication-with-ck-rx65n-v2).  
>
> **Note 2**: This demo using TSIP is intended for evaluation during the **development phase**, as the program that installs the private key remains in the code flash.  
Therefore, if you use it in production development, please handle the key with care.

The following are combinations of demos that can be tried for each procedure.

|Operating Procedure|Macro Settings|PubSub|Fleet Provisioning|OTA|
|---|---|---|---|---|
|[Step 4-1:<BR>Run PubSub/MQTT sample project](#step-4-1-run-pubsubmqtt-sample-project)|`ENABLE_FLEET_PROVISIONING_DEMO (0)`<BR>`ENABLE_OTA_UPDATE_DEMO (0)`|✓|-|-|
|[Step 4-2:<BR>Run PubSub/MQTT with Fleet Provisioning sample project](#step-4-2-run-pubsubmqtt-with-fleet-provisioning-sample-project)|`ENABLE_FLEET_PROVISIONING_DEMO (1)`<BR>`ENABLE_OTA_UPDATE_DEMO (0)`|✓|✓|-|
|[Step 4-3:<BR>Run PubSub/MQTT with Over-the-air(OTA) update sample project](#step-4-3-run-pubsubmqtt-with-over-the-airota-update-sample-project)|`ENABLE_FLEET_PROVISIONING_DEMO (0)`<BR>`ENABLE_OTA_UPDATE_DEMO (1)`|✓|-|✓|
|[Step 4-4:<BR>PubSub/MQTT with Over-the-air(OTA) update sample project (TLS with TSIP)](#step-4-4-run-pubsubmqtt-with-over-the-airota-update-sample-project-tls-with-tsip)|`ENABLE_FLEET_PROVISIONING_DEMO (0)`<BR>`ENABLE_OTA_UPDATE_DEMO (1)`|✓|-|✓|

* The macro configuration is done in the file \src\frtos_config\demo_config.h.  
* Be sure to complete Step 1 to 3 before performing Step 4-1 to 4-4 in the table above.

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

* If you have downloaded the repo without using the `--recurse-submodules` argument, you should run:

  ```text
  git submodule update --init --recursive
  ```

### Step 2: Hardware setup

#### To connect for communication

##### Hardware setup in case of Cellular

Connect a RYZ014A with LTE antenna and SIM card to device board.

1. Connect the LTE antenna to the RYZ014A. The antenna is packaged with the purchase of RYZ014A.
2. **The SIM card is required to be activated in advance** (refer to your SIM card's guide), then insert it to the RYZ014A.
3. Connect the RYZ014A to device board. In case of CK-RX65N, connected to the PMOD1 of it.
4. Connect a USB cable to the RYZ014A for supplemental power supply.

**Note:**

* Depending on the country you live in or provider of SIM card, you may not use RYZ014A due to the supported bands. Refer to manual of RYZ014A about supported bands for details.  
Also see [Settings of Bands](#settings-of-bands-only-using-cellular) for band settings.

##### Hardware setup in case of Wi-Fi

Connect a DA16600 to device board.

1. Connect the DA16600 to device board. In case of CK-RX65N, connected to the PMOD1 of it.

**Note:**

* Depending on the country you live in, you may not use DA16600 due to wrong country code or timezone.  
Refer to [Settings of Country code and GMT timezone](#settings-of-country-code-and-gmt-timezone-only-using-wi-fi) for settings.

##### Hardware setup in case of Ethernet

Connect between device board and router/switch.

1. Connect an internet-connected Ethernet cable to the Ethernet port attached on device board.

#### To connect for power supply, debugging, and logging

For more details, refer to the [manual of CK-RX65N](https://www.renesas.com/us/en/document/mat/ck-rx65n-v1-users-manual).

1. Set up jumpers of device board as follows:

    * J2 Short for invalidating MCU current measurement point
    * J11 Open for single-chip mode as MCU boot option
    * J16 1-2 Short for debugging

2. For power supply and debugging, connect an USB cable between the debug connector (J14) on the CK-RX65N and your PC with installed e2 studio.
    This connector has a role of both of power supply and debugging.
3. For receiving debug logs, connect an USB cable between the USB-serial connector (J10) on the CK-RX65N and your PC with serial terminal application to receive logs.
    Note that debug logs mean serial output data which is coded in the demo program and Renesas driver software.
    It is not directly related to debugging on e2 studio.

Board settings image for cellular :  
![2](https://github.com/renesas/iot-reference-rx/wiki/getting_started_guide_image/step2_cell.png?raw=true)  

Board settings image for Wi-Fi :  
![2](https://github.com/renesas/iot-reference-rx/wiki/getting_started_guide_image/step2_wifi.png?raw=true)  

Board settings image for ether :  
![2](https://github.com/renesas/iot-reference-rx/wiki/getting_started_guide_image/step2_ether.png?raw=true)  

### Step 3: Import projects into e2 studio

Import demo projects into IDE; e2 studio.

1. Open e2 studio.
1. Choose workspace and click **Launch**.
1. **File** -> **Import...** -> **Existing Projects into WorkSpace**.
1. Click **Browse...** and choose **aws_ryz014a_ck_rx65n** (for Cellular), **aws_da16600_ck_rx65n** (for Wi-Fi) or **aws_ether_ck_rx65n** (for Ethernet) demo.  
In the project folder, there is a folder corresponding to your compiler. Select the **e2studio_ccrx** folder.  
   * The **e2studio_gcc** folder contains the GCC compiler project.  
   * If you want to demo with TSIP, choose either **aws_ether_tsip_ck_rx65n** (for Ethernet) / **aws_ryz014a_tsip_ck_rx65n** (for Cellular).  

![3-4](https://github.com/renesas/iot-reference-rx/wiki/getting_started_guide_image/step3_4_project_import.PNG?raw=true)

**Note:** Ensure that **copy projects into workspace** is not selected.  
![3-4](https://github.com/renesas/iot-reference-rx/wiki/getting_started_guide_image/step3_4_project_import2.PNG?raw=true)  

1. Click **Finish** to import the projects.

### Step 4: Run demos

#### Step 4-1: Run PubSub/MQTT sample project

See this chapter when running *PubSub/MQTT sample project*.  
In this chapter, the "PubSub/MQTT sample project" will be referred to as the "PubSub Demo".

If you want to communicate **Ethernet** with **CK-RX65Nv2** , please refer [How to use Ethernet communication with CK-RX65Nv2](#how-to-use-ethernet-communication-with-ck-rx65n-v2) (CC-RX/GCC).  

##### Step 4-1-1: Software setup for PubSub demo

Configure settings to run PubSub demo.

###### Configure combination of demos for PubSub demo

Configure settings to select combination of demos.
In "Projects\\<project_name>\\e2 studio_ccrx\\src\\frtos_config\\demo_config.h", configure both of `ENABLE_FLEET_PROVISIONING_DEMO` and `ENABLE_OTA_UPDATE_DEMO`, which allow to select the different supported demo combinations:

* `ENABLE_FLEET_PROVISIONING_DEMO`: (0)
* `ENABLE_OTA_UPDATE_DEMO`: (0)  
![4-1-1](https://github.com/renesas/iot-reference-rx/wiki/getting_started_guide_image/step4_1_1config.PNG?raw=true)

###### Settings of the hook function (Only using Cellular or Wi-Fi)

The socket wrapper layer provides a hook function to reset the cellular or wi-fi module when an error occurs.
It is recommended to reset the module if the FIT module API "R_Cellular_xxx"/"R_WIFI_xxx" returns errors `CELLULAR_ERR_MODULE_TIMEOUT`, `CELLULAR_ERR_MODULE_COM` in case of Cellular or `WIFI_ERR_MODULE_COM` in case of Wi-Fi. Otherwise, the module may not communicate.
You can change `USER_TCP_HOOK_ENABLED` in "Projects\\<project_name>\\e2 studio_ccrx\\src\\frtos_config\\user_tcp_hook_config.h" to enable or disable this hook.

You can use hook as-is if you only run the demos.
In our current implementation, cellular or wi-fi module is reset in this hook when the following conditions:

* On TCP_Sockets_Connect,
  * When both errors occur 1 time.
* On TCP_Sockets_Recv and TCP_Sockets_Send,
  * In case of Cellular:
    * When `CELLULAR_ERR_MODULE_TIMEOUT` occurs 1 time.
    * When `CELLULAR_ERR_MODULE_COM` occurs continuously 3 times.
      * You can change retry numbers by configuring `USER_COMM_ERROR_TRIES`.
  * In case of Wi-Fi:
    * When `WIFI_ERR_MODULE_COM` occurs 1 time.

###### Settings of RX Smart Configurator

Configure the Renesas code generator (RX Smart Configurator*).  
Please open RX Smart Configurator* and configure settings according to the image below.  
Select **aws_ryz014a_ck_rx65n.scfg** --> **Components** --> **Icon for adding components**　--> **Configure general settings...** --> **Component** --> **code generation behavior column** --> **Update configuration files**.

![4-1-1](https://github.com/renesas/iot-reference-rx/wiki/getting_started_guide_image/step4_1_1_sc_genetate_setting.PNG?raw=true)

*See the [manual of RX Smart Configurator](https://www.renesas.com/document/mat/rx-smart-configurator-users-guide-e-studio) for details on how to use it. The Config files generated after code generation by RX Smart Configurator are exist in the following folder:

* "Projects\\<project_name>\\e2 studio_ccrx\\src\\smc_gen"

###### Download the FreeRTOS kernel, FreeRTOS LittleFS and FIT module

Download FreeRTOS Kernel and FreeRTOS LittleFS.  
If FreeRTOS kernel and FreeRTOS LittleFS is not installed, the icon will be displayed as a gray square as shown in the image below.  
If you have already installed the FreeRTOS kernel and FreeRTOS LittleFS, please skip the download step.  
FreeRTOS kernels can be downloaded using RX Smart Configurator.  
Click on **FreeRTOS_Kernel** and then select **downloading it**.  
![4-1-1](https://github.com/renesas/iot-reference-rx/wiki/getting_started_guide_image/Step4_1_1_kernel_littlefs.png?raw=true)    

FIT modules can be downloaded in the same way.  
Select the FIT module you wish to download, and then download it from **downloading it**.  
If you have also installed FIT, please skip this step.  
If there are other grey icons in the component tree, the corresponding components are not present in your environment and you should download the required components using the steps above.

###### Settings of access point (Only using Cellular)

Use the RX Smart Configurator to configure the SIM card settings.  
Open the RX Smart Configurator as shown in the image below and set the four parameters `"Access point name"`, `"Access point login ID"`, `"Access point password"`, and `"SIM card PIN card PIN code"`.  
The settings will vary depending on the SIM card that you are using, so please check the SIM information.

* `"Access point name"` : Set the access point name of the SIM
* `"Access point login ID"` : Set the user name of the SIM
* `"Access point password"` : Set SIM card password
* `"SIM  card PIN code"` : Set SIM card PIN code

If you are using the truephone SIM that comes with CK-RX65N v2, it will work if you only enter the Access point name.

![4-1-1](https://github.com/renesas/iot-reference-rx/wiki/getting_started_guide_image/step4_1_1_Input_APN.PNG?raw=true)

###### Settings of bands (Only using Cellular)

Configure settings related to the bands supported by your cellular module.
In case of RYZ014A, configure the following macro in "Middleware\\network_transport\\sockets_wrapper\\ports\\cellular_ryz014a\\TCP_socket_hook.c"

* `CELLULAR_BAND_CONFIG`: Set `"1,2,4,5,8,12,13,14,17,18,19,20,25,26,28,66"`

###### Settings of Country code and GMT timezone (Only using Wi-Fi)

Use the RX Smart Configurator to configure the Country code and GMT timezone.  
Open the RX Smart Configurator as shown in the image below and set the 2 parameters `"WIFI_CFG_COUNTRY_CODE"` and `"WIFI_CFG_SNTP_UTC_OFFSET"`.

* `"WIFI_CFG_COUNTRY_CODE"` : Country code defined in ISO 3166-1 alpha-2 standard. Such as KR, US, JP, and CH.
* `"WIFI_CFG_SNTP_UTC_OFFSET"` : GMT timezone offset in hours (-12 ~ 12).

![4-1-1](https://github.com/renesas/iot-reference-rx/wiki/getting_started_guide_image/step4_1_1_Input_WFCC_TZONE.PNG?raw=true)

###### Settings of Wi-Fi network (Only using Wi-Fi)

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

/*
 * @brief Password needed to join Wi-Fi network.
 * @todo If you are using WPA, set this to your network password.
 */
#define clientcredentialWIFI_PASSWORD                ""
```

##### Step 4-1-2: Building for PubSub demo

Build firmware image by the builder with e2 studio.  
In the **Project Explorer** pane of e2 studio, Right click on the project and select **Build Project**. Wait for completing building.

**Note:** Or choose **Project**->**Build Project** menu to build.

![4-1-1](https://github.com/renesas/iot-reference-rx/wiki/getting_started_guide_image/4_1_3_build.png?raw=true)

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
* Baudrate : 115200
* Data : 8 bit
* Parity : none
* Stop: 1 bit
* Flow control : none  
  ![4-1-4](https://github.com/renesas/iot-reference-rx/wiki/getting_started_guide_image/step4_1_5teraterm_setting2.PNG?raw=true)

##### Step 4-1-5: AWS MQTT test client settings  

You can run the PubSub demo to see the communication messages in AWS.  
To do this, you must first set up the **MQTT test client**.

* Open the **MQTT test client** website.  
  <https://us-east-2.console.aws.amazon.com/iot/home?region=us-east-2#/test>  
  Note: The above URL is an example, change it to the AWS site that matches your region.
* Click **Subscribe to a topic**, enter "#" in the **topic filter**, and click the "Subscribe" button.  
  ![4-1-7](https://github.com/renesas/iot-reference-rx/wiki/getting_started_guide_image/step4_1_7_2.PNG?raw=true)
* You can check the communication messages by running the demo.  
  For instructions on how to check the messages, see [Step 4-1-9](#step-4-1-9-check-messages-with-the-aws-mqtt-test-client).

##### Step 4-1-6: Flashing for PubSub demo

Flash firmware image to your target device by the either of following ways.

In the **Project Explorer** panel of e2 studio, right click on the project and select **Debug As** --> **Renesas GDB Hardware Debugging**.
 ![4-1-5](https://github.com/renesas/iot-reference-rx/wiki/getting_started_guide_image/step4_1_5debug_start.png?raw=true)  

If the window below appears, press "Switch".  
 ![4-1-5](https://github.com/renesas/iot-reference-rx/wiki/getting_started_guide_image/step4_1_5debug_message.PNG?raw=true)

Press the following button to start debugging.  
The first command of step 4-1-6 must be entered on teraterm within 10 seconds after the start of debugging.
It is recommended that you have teraterm ready for easy input.
 ![4-1-5](https://github.com/renesas/iot-reference-rx/wiki/getting_started_guide_image/step4_1_5_press_start.PNG?raw=true)

##### Step 4-1-7: Input via CLI for PubSub demo

CLI runs on the serial terminal application when starting demo.
Input the following commands to the serial terminal application to start communication to AWS.

The summarized flow of the command input is as follows:

1. Switch CLI mode.
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
> conf set thingname github_thing_en
OK
```
 ![4-1-6](https://github.com/renesas/iot-reference-rx/wiki/getting_started_guide_image/step4_1_6_3thingname.PNG?raw=true) 

Next, set the mqtt endpoint to the endpoint for your account:
```text
> conf set endpoint xxxxxxxxxxxxxx-ats.iot.us-west-2.amazonaws.com
OK
```
 ![4-1-6](https://github.com/renesas/iot-reference-rx/wiki/getting_started_guide_image/step4_1_6_4endpoint.PNG?raw=true) 

**Note:** You can determine the endpoint for your AWS account with the ```aws iot describe-endpoint``` command or on the *Settings* page of the AWS IoT Core console.

###### Set security information of target device

Next, set the device certificate:  
Drag and drop "xxxx-certificate.pem.crt" generated in [Step 4-1-3: Create RSA key pair and device certificate for PubSub demo](#step-4-1-3-create-rsa-key-pair-and-device-certificate-for-pubsub-demo) to teraterm.

 ![4-1-6](https://github.com/renesas/iot-reference-rx/wiki/getting_started_guide_image/step4_1_6_5cert.PNG?raw=true) 

**Note:** About \<device certificate\>, if you use TeraTerm, drag and drop the certificate file onto the Terminal screen after entered `conf set cert` , then select `Send File (Paste content of file)` on the screen that appears, and press `OK`. The contents of the certificate file will be entered.

Next, set the device private key:  
Drag and drop "xxxx-private.pem.key" generated in [Step 4-1-3: Create RSA key pair and device certificate for PubSub demo](#step-4-1-3-create-rsa-key-pair-and-device-certificate-for-pubsub-demo) to teraterm.


 ![4-1-6](https://github.com/renesas/iot-reference-rx/wiki/getting_started_guide_image/step4_1_6_6.PNG?raw=true) 

**Note:** About \<private key\>, if you use TeraTerm, drag and drop the certificate file onto the Terminal screen after entered `conf set key` , then select `Send File (Paste content of file)` on the screen that appears, and press `OK`. The contents of the certificate file will be entered.

###### Commit configuration changes

Next, commit the staged configuration changes to Data Flash:

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
![4-1-7](https://github.com/renesas/iot-reference-rx/wiki/getting_started_guide_image/step4_1_7_1.PNG?raw=true)

##### Step 4-1-9: Check messages with the AWS MQTT test client

Once you configure the MQTT test client in [Step 4-1-5](#step-4-1-5-aws-mqtt-test-client-settings), you will be able to see the messages sent from the pubsub demo on AWS.

* Display the MQTT test client website you set in [Step 4-1-5](#step-4-1-5-aws-mqtt-test-client-settings).
* You can get message "*Task x publishing messege x*" published to **MQTT test client** console.  
If successful, it will be displayed on AWS conclose as shown below.![4-1-7](https://github.com/renesas/iot-reference-rx/wiki/getting_started_guide_image/step4_1_7_3.PNG?raw=true)  

---

#### Step 4-2: Run PubSub/MQTT with Fleet Provisioning sample project

Details for *PubSub/MQTT with Fleet Provisioning sample project* are provided in the special application note (document number: R20AN0674). You can search it by visiting the following webpage:  
In this chapter, the "PubSub/MQTT with Fleet Provisioning sample project" will be referred to as the "PubSub demo with Fleet Provisioning".

* <https://www.renesas.com/search?keywords=r20an0674>

About how to run this demo, see the chapter *4. Running the Fleet Provisioning Demo* in preceding application note. Or watching the following video:

* <https://www.renesas.com/us/en/video/amazon-freertos-fleet-provisioning-tutorial-ck-rx65n-12-iot-devices-cloud-operation>

Though this application note is for both Ethernet and Cellular projects, Wi-Fi project will also work as described in application note.

When running this demo, please enable [**Settings of RX Smart Configurator**](#settings-of-rx-smart-configurator) before generating code with RX Smart Configurator.  
Also, if there is a gray icon in the component tree of Smart Configurator, the corresponding component does not exist in your environment.  
Refer to the "[**Download the FreeRTOS kernel, FreeRTOS LittleFS and FIT module**](#download-the-freertos-kernel-freertos-littlefs-and-fit-module)" section to download the modules you need.

If you want to communicate **Ethernet** with **CK-RX65Nv2** , please refer [How to use Ethernet communication with CK-RX65Nv2](#how-to-use-ethernet-communication-with-ck-rx65n-v2) (CC-RX/GCC).  

##### Precaution for PubSub demo with Fleet Provisioning

* Make sure to follow the steps described in the preceding application note.
* If you deviate from the *normal flow*, the Data Flash usage is not guaranteed.
* The following is an example of Fleet Provisioning with *abnormal flow*:
  1. When demo program enters CLI mode first time,
     set and commit claim credentials, endpoint, template and rootCA information as usual.
  1. After Fleet Provisioning and PubSub demo completes, reset the device.
  1. When demo program enters CLI mode again after reset,
      set and commit a new set of device credentials.
* With the *normal flow*, Data Flash usage is ~7808 bytes after Fleet Provisioning.
* With the *abnormal flow*, new device credentials will overwrite existing credentials provisioned by Fleet Provisioning.
  * In this case, an additional 1408 bytes will be used.
* Currently the maximum response latency between AWS and the device is set at 5000 ms.  
The latency is set to be long enough to allow for a margin of error.  
If you want to shorten the latency, please adjust it according to your environment.  
Demos\common\Mqtt_Demo_Helpers\mqtt_pkcs11_demo_helper.c  
`mqttexamplePROCESS_LOOP_TIMEOUT_MS　(5000U)`

---

#### Step 4-3: Run PubSub/MQTT with Over-the-air(OTA) update sample project

Details for *PubSub/MQTT with Over-the-air(OTA) update sample project* are provided in the special application note (document number: R01AN7037). You can search it by visiting the following webpage:

* <https://www.renesas.com/search?keywords=r01an7037>  

About how to run this demo, see the chapter *2. Prerequisites* in preceding application note.

Though this application note is for both Ethernet and Cellular projects, Wi-Fi project will also work as described in application note.

The "boot_loader_ck_rx65n_v2" project should be used when running this demo on CK-RX65N v2.

When running this demo, please enable [**Settings of RX Smart Configurator**](#settings-of-rx-smart-configurator) before generating code with RX Smart Configurator.  
Also, if there is a gray icon in the component tree of Smart Configurator, the corresponding component does not exist in your environment.  
Refer to the "[**Download the FreeRTOS kernel, FreeRTOS LittleFS and FIT module**](#download-the-freertos-kernel-freertos-littlefs-and-fit-module)" section to download the modules you need.

If you want to communicate **Ethernet** with **CK-RX65Nv2** , please refer [How to use Ethernet communication with CK-RX65Nv2](#how-to-use-ethernet-communication-with-ck-rx65n-v2) (CC-RX/GCC).  

* **Note** : On creating GCC version OTA projects  
  When creating a GCC version OTA project, the method for setting sections is different from the CC-RX version.  
  Before building, configure the section as follows:
  * Open "\src\linker_script.ld" from the e2 studio project tree.  
    ![step4_3_1_ld_file_open](https://github.com/renesas/iot-reference-rx/wiki/getting_started_guide_image/step4_3_1_ld_file_open.png)
  * Change the values ​​in the following two sections:
    | name | value |
    | :----: | :----: |
    | .exvectors | 0xFFFEFF80 |
    | .fvectors | 0xFFFEFFFC |  

    ![step4_3_1_section_setting](https://github.com/renesas/iot-reference-rx/wiki/getting_started_guide_image/step4_3_2_section_setting.png)
  * Save "linker_script.ld" overwriting it.
  
  ---

#### Step 4-4: Run PubSub/MQTT with Over-the-air(OTA) update sample project (TLS with TSIP)

Details for *PubSub/MQTT with Over-the-air(OTA) update sample project (TLS with TSIP)* are provided in the special application note (document number: R20AN0749). You can search it by visiting the following webpage:  

* <https://www.renesas.com/search?keywords=r20an0749>  

About how to run this demo, see the chapter *2. Prerequisites* in preceding application note.  

This application note is intended for both Ethernet and cellular projects.(Wi-Fi project not supported)  

The basic operation method is the same as `Step4-3`, but the setting method is different.  
To use TSIP, you must obtain or generate a certificate and key for TSIP. For more information, see Chapter 5 of the aforementioned application note.

When running this demo, please enable [**Settings of RX Smart Configurator**](#settings-of-rx-smart-configurator) before generating code with RX Smart Configurator.  
Also, if there is a gray icon in the component tree of Smart Configurator, the corresponding component does not exist in your environment.  
Refer to the "[**Download the FreeRTOS kernel, FreeRTOS LittleFS and FIT module**](#download-the-freertos-kernel-freertos-littlefs-and-fit-module)" section to download the modules you need.

If you want to communicate **Ethernet** with **CK-RX65Nv2** , please refer [How to use Ethernet communication with CK-RX65Nv2](#how-to-use-ethernet-communication-with-ck-rx65n-v2) (CC-RX).  

* **Note** :  
  This demo using TSIP is intended for evaluation during the **development phase**, as the program that installs the private key remains in the code flash.  
  Therefore, if you use it in production development, please handle the key with care.

---

## How to use Ethernet communication with CK-RX65N v2

### Preparing the Project

To use **Ethernet** communication with CK-RX65N **v2**, the settings of the Ethernet sample project for CK-RX65N **v1** (**aws_ether_ck_rx65n**) must be changed for CK-RX65N **v2**.  
When using OTA, use the **boot_loader_ck_rx65n_v2** for the bootloader.

| RX MCU and Board | Connectivity | Project name | Boot loader | Compiler |
|------------------|--------------|--------------------|----------------------|----------------------|
| CK-RX65N v2 | Ethernet | aws_ether_ck_rx65n | boot_loader_ck_rx65n_v2 | CC-RX/GCC |
| CK-RX65N v2 |Ethernet<BR>(with TSIP)|aws_ether_tsip_ck_rx65n| boot_loader_ck_rx65n_v2 | CC-RX |

There are two ways to obtain a project. See below for a description of each way.  
<details><summary>Import Project - click to learn more</summary>  

Please refer to [**Step 1: Downloading this product**](#step-1-downloading-this-product) and [**Step 3: Import projects into e2 studio**](#step-3-import-projects-into-e2-studio) chapter for instructions on how to import the sample projects.  

Note: Please import "boot_loader_ck_rx65n_v2" as the bootloader.

</details>  

<details><summary>Generate Project with e2 studio - click to learn more</summary>  

To generate a FreeRTOS project using the projsect generation function of e2 studio, generate the project according to the following settings.  

* Generate the OTA demo project  
  * Select **CK-RX65N** in Target Board and **Dual Bank** in Bank Mode.  
![v2_eth_0_1_PG_setting](https://github.com/renesas/iot-reference-rx/wiki/getting_started_guide_image/v2_eth_0_1_PG_setting.png)  
  * Select the application of the function you want to use from the applications marked with **(Ethernet)**.  
  ![v2_eth_0_2_PG_select_project](https://github.com/renesas/iot-reference-rx/wiki/getting_started_guide_image/v2_eth_0_2_PG_select_project.png)  
  * Please refer the following video for how to generate FreeRTOS projects in e2 studio.  
  [![how to generate FreeRTOS projects](https://github.com/renesas/iot-reference-rx/wiki/getting_started_guide_image/v2_eth_0_5_PG_movie.png)](https://www.renesas.com/us/en/video/freertos-ota-tutorial-ck-rx65n-23-program-build)  
* Generate the bootloader project (For CK-RX65N v2)  
  Create a bootloader project using the same procedure as for the demo project.  
  The bootloader generated this time is for **CK-RX65N v2**.  
  The bootloader for **CK-RX65N** v1 cannot be used.  

  The settings for generating the bootloader are as follows.  
  * Select **CK-RX65N v2** in Target Board and Dual Bank in Bank Mode.  
    ![v2_eth_0_3_PG_bootloder_setting](https://github.com/renesas/iot-reference-rx/wiki/getting_started_guide_image/v2_eth_0_3_PG_bootloder_setting.png)  
  * Select the **Secure Bootloader sample project**.  
  ![v2_eth_0_4_PG_select_bootloader_project](https://github.com/renesas/iot-reference-rx/wiki/getting_started_guide_image/v2_eth_0_4_PG_select_bootloder_project.png)

</details>  

> **Note:** Settings related to the project, board, and demo operation are common to other projects and are omitted in this section.  
Be sure to refer to the [**How to run demos**](#how-to-run-demos) and the necessary Steps thereafter.  
The components used in the demo project may not have been downloaded (gray icon displayed on the Smart Configurator).
Please refer to [**Download the FreeRTOS kernel, FreeRTOS LittleFS and FIT module**](https://github.com/renesas/iot-reference-rx/blob/main/Getting_Started_Guide.md#download-the-freertos-kernel-freertos-littlefs-and-fit-module) to download the necessary components.  

### Saving section settings

Changing device will overwrite section settings. If section settings are overwritten, build errors may occur.  
If the file for section re-setting does not exist in the folder below, the section settings must be saved before the device is changed.  
**Note:** If the file in the table below exists, it is not necessary to save the section settings.  

* CC-RX
   | Project | Pass | File name |
   | ------ | ------ | ------ |
   | Import | C:\\<work space\>\iot-reference-rx\Configuration\samples\pubsub\ck-rx65n\ether | linker_section_sample.esi |
   | Project Generation with e2 studio | C:\\<work space\>\\<project_name\>\src | linker_section_sample.esi |
* GCC  
The GCC project does not include a file for section re-setting.  

**The steps for saving section settings are as follows:**  
<details><summary>For CC-RX users - click to learn more</summary>

* Right-click on the **project name** in the Project Explorer -> Click on **Properties** -> Click **C/C++ Build**-> **Settings** -> Tool Settings-> Linker-> Section-> `...` -> Export…  
  Save anywhere with any name(ex.linker_section_sample).  
  ![01_CCRX_Export_esiFile](https://github.com/renesas/iot-reference-rx/wiki/getting_started_guide_image/01_CCRX_Export_esiFile.png)

</details>  

<details><summary>For GCC users - click to learn more</summary>

* Project Explorer -> project name -> src -> Right-click on **linker_sctipt.ld** -> Copy -> right-click on **linker_sctipt.ld** -> Paste -> OK  
  ![01_GCC_Cpy_ldFile](https://github.com/renesas/iot-reference-rx/wiki/getting_started_guide_image/01_GCC_Cpy_ldFile.png)

</details>  

### Changing the device

This section describes how to change CK-RX65N v1 project's Board Settings to CK-RX65N v2.  

1. Double-click **\<project name\>.scfg** in Project Explorer, and then click **Board**.  
![v2_eth_1_1_Open_scfg_and_BoardTab](https://github.com/renesas/iot-reference-rx/wiki/getting_started_guide_image/v2_eth_1_1_Open_scfg_and_BoardTab.png)  
1. Select Board-> **CK-RX65N-V2**, select **Dual Bank** for Bank Mode and press **Next**  
![v2_eth_1_2_select_ckv2](https://github.com/renesas/iot-reference-rx/wiki/getting_started_guide_image/v2_eth_1_2_select_ckv2.png)  
1. Click **Next**  
![v2_eth_1_3_select_ckv2_found_prpblems](https://github.com/renesas/iot-reference-rx/wiki/getting_started_guide_image/v2_eth_1_3_select_ckv2_found_prpblems.png)  
1. Click **Finish**  
![v2_eth_1_4_device_cng_finish](https://github.com/renesas/iot-reference-rx/wiki/getting_started_guide_image/v2_eth_1_4_device_cng_finish.png)  
1. When the following Question dialog box is displayed, press **No**.  
![v2_eth_1_5_Question](https://github.com/renesas/iot-reference-rx/wiki/getting_started_guide_image/v2_eth_1_5_Question.png)  
1. Confirm that the Board is changed to **CK-RX65N-V2** and click **Generate Code**.  
![v2_eth_1_6_code_gene](https://github.com/renesas/iot-reference-rx/wiki/getting_started_guide_image/v2_eth_1_6_code_gene.png)  
1. When the following Code Generating dialog box is displayed, press **Proceed**.  
![v2_eth_1_7_proceed](https://github.com/renesas/iot-reference-rx/wiki/getting_started_guide_image/v2_eth_1_7_proceed.png)  

### Set section

The section setting method differs depending on the compiler.  
Please configure the following settings according to the Compiler you are using.  

<details><summary>For CC-RX users - click to learn more</summary>

#### CC-RX

1. Right-click the project name in the Project Explorer to open **Properties**.  
![v2_eth_1_8_Property](https://github.com/renesas/iot-reference-rx/wiki/getting_started_guide_image/v2_eth_1_8_Property.png)  
1. Click C/C++ Build-> Settings -> Tool Settings-> Linker-> Section-> `...` -> Import...  
![v2_eth_1_9_section_import](https://github.com/renesas/iot-reference-rx/wiki/getting_started_guide_image/v2_eth_1_9_section_import.png)  
1. Import the section settings.
The section configuration file (**linker_section_sample.esi**) is stored in the following path of the project:  

   | Project | Pass |
   | ------ | ------ |
   | Import | C:\\<work space\>\iot-reference-rx\Configuration\samples\pubsub\ck-rx65n\ether |
   | Project Generation with e2 studio | C:\\<work space\>\\<project_name\>\src |

   **Note:**
If no configuration file exists above, use the file saved in [Saving section settings](#saving-section-settings).  

   ![v2_eth_1_10_section_file_locate](https://github.com/renesas/iot-reference-rx/wiki/getting_started_guide_image/v2_eth_1_10_section_file_locate.png)  

1. Click **OK**.  
![v2_eth_1_11_section_ok](https://github.com/renesas/iot-reference-rx/wiki/getting_started_guide_image/v2_eth_1_11_section_ok.png)  
1. Click **Apply and Close**.  
![v2_eth_1_12_apply_and_close](https://github.com/renesas/iot-reference-rx/wiki/getting_started_guide_image/v2_eth_1_12_apply_and_close.png)  
1. When the following Settings dialog box appears, press **Rebuild Index**.  
![v2_eth_1_13_rebuild](https://github.com/renesas/iot-reference-rx/wiki/getting_started_guide_image/v2_eth_1_13_rebuild.png)  

</details>

<details><summary>For GCC users - click to learn more</summary>

#### GCC

Use the file saved in section [Saving section settings](#saving-section-settings).  
In the following procedure, the pre-saved file is **linker_script2.ld**.  
  
1. Project Explorer -> project name -> src -> Double-click **linker_sctipt.ld** and **linker_script2.ld**  
![02_GCC_ldFile_Open](https://github.com/renesas/iot-reference-rx/wiki/getting_started_guide_image/02_GCC_ldFile_Open.png)  
1. Select all text in linker_sctipt2.ld -> Right click -> Copy  
![03_GCC_ldFile_Copy](https://github.com/renesas/iot-reference-rx/wiki/getting_started_guide_image/03_GCC_ldFile_Copy.png)
1. Select all text in linker_sctipt.ld -> Right click -> Paste  
![04_GCC_ldFile_Update](https://github.com/renesas/iot-reference-rx/wiki/getting_started_guide_image/04_GCC_ldFile_Update.png)
1. Project Explorer -> project name -> src -> Right click **linker_script2.ld** > Delete > OK
![05_GCC_ldFile_Delete](https://github.com/renesas/iot-reference-rx/wiki/getting_started_guide_image/05_GCC_ldFile_Delete.png)

</details>
  
After changing the section settings by either of the above methods, build the project according to the following.

1. Right-click the project name in Project Explorer -> click **Build Project**.  
![v2_eth_1_14_build](https://github.com/renesas/iot-reference-rx/wiki/getting_started_guide_image/v2_eth_1_14_build.png)  
  
### Next Step

With the above settings, CK-RX65N v2 can perform Ethernet communication.  

Please refer to the following for the settings of various sample operations.
|Operating Procedure|Macro Settings|PubSub|Fleet Provisioning|OTA|
|---|---|---|---|---|
|[Step 4-1:<BR>Run PubSub/MQTT sample project](#step-4-1-run-pubsubmqtt-sample-project)|`ENABLE_FLEET_PROVISIONING_DEMO (0)`<BR>`ENABLE_OTA_UPDATE_DEMO (0)`|✓|-|-|
|[Step 4-2:<BR>Run PubSub/MQTT with Fleet Provisioning sample project](#step-4-2-run-pubsubmqtt-with-fleet-provisioning-sample-project)|`ENABLE_FLEET_PROVISIONING_DEMO (1)`<BR>`ENABLE_OTA_UPDATE_DEMO (0)`|✓|✓|-|
|[Step 4-3:<BR>Run PubSub/MQTT with Over-the-air(OTA) update sample project](#step-4-3-run-pubsubmqtt-with-over-the-airota-update-sample-project)|`ENABLE_FLEET_PROVISIONING_DEMO (0)`<BR>`ENABLE_OTA_UPDATE_DEMO (1)`|✓|-|✓|
|[Step 4-4:<BR>PubSub/MQTT with Over-the-air(OTA) update sample project (TLS with TSIP)](#step-4-4-run-pubsubmqtt-with-over-the-airota-update-sample-project-tls-with-tsip)|`ENABLE_FLEET_PROVISIONING_DEMO (0)`<BR>`ENABLE_OTA_UPDATE_DEMO (1)`|✓|-|✓|

* The macro configuration is done in the file \src\frtos_config\demo_config.h.
* As you perform each step, review the [Additional information for sample operations](#additional-information-for-sample-operations).

### Additional information for sample operations

Also, when referring to the application notes for each sample operation, please refer to the following precautions.

* Application note ([R01AN7037](https://www.renesas.com/search?keywords=r01an7037))  
  The supplementary information for the application note ([R01AN7037](https://www.renesas.com/search?keywords=r01an7037)) is as follows:

  * 2.5 Connecting the CK-RX65N.  
    In the Ethernet version project, the connection procedure for the **CK-RX65N v2** differs from that in the application note, so the procedure is explained below.
    * Connect a LAN cable that can connect to the Internet to the LAN connector(J5) on the base board.
    * On the base board, position the jumper on pins 1-2 of J16 to enable debugging mode.
    * Connect J10 on the base board to a PC using a USB cable (USB serial connection).  
    * Connect J14 on the base board to a PC using a USB cable (debugger connection).
    ![CK-RX65Nv2](https://github.com/renesas/iot-reference-rx/wiki/getting_started_guide_image/step_4_3_ck-rx65n-baseboard_v2.png)
  * 4.2.2 (2) Motorola S format file output settings.  
    For GCC projects, the steps to set up file output in Motorola S format are different from those for CC-RX projects.  
    Follow the steps below to set it up.
    * From the Project menu, select Properties, expand the **C/C++ Build** menu, click **Settings**, and then click the **Tool Settings** tab.
    * In the **Tool Settings** tree, click **objcopy** and then click **General**.
    * Select `Motorola S-record(srec)` in the **OutFormat** settings.  
    ![s-record_setting](https://github.com/renesas/iot-reference-rx/wiki/getting_started_guide_image/step4_3_gcc_sformat_setting.PNG)  
  * 4.2.4 Creating the Initial Firmware, (3).  
    Replace the Python command when creating the Initial Firmware with the following:  
    `python image-gen.py -iup aws_ether_ck_rx65n.mot -ip RX65N_DualBank_ImageGenerator_PRM.csv -o userprog -ibp boot_loader_ck_rx65n_v2.mot -key secp256r1.privatekey -vt ecdsa -ff RTOS`
  * 5.1 Creating the Updated Firmware, 5.1.1 (2).  
    `python image-gen.py -iup aws_ether_ck_rx65n.mot -ip RX65N_DualBank_ImageGenerator_PRM.csv -o user_093 -key secp256r1.privatekey -vt ecdsa -ff RTOS`  

* Application note ([R20AN0749](https://www.renesas.com/search?keywords=r20an0749))  
The supplementary information for the application note ([R20AN0749](https://www.renesas.com/search?keywords=r20an0749)) is as follows:
  
  * 6.1.3 Creating the Initial Firmware, (1).  
    Replace the Python command when creating the Initial Firmware with the following:  
    `python image-gen.py -iup aws_ether_tsip_ck_rx65n.mot -ip RX65N_DualBank_ImageGenerator_PRM.csv -o userprog -ibp boot_loader_ck_rx65n_v2.mot -key secp256r1.privatekey -vt ecdsa -ff RTOS`
  * 6.2.1 Creating Update Firmware, (2).  
    Replace the Python command when creating the Updated farmware with the following:  
    `python image-gen.py -iup aws_ether_tsip_ck_rx65n.mot -ip RX65N_DualBank_ImageGenerator_PRM.csv -o user_093 -key secp256r1.privatekey -vt ecdsa -ff RTOS`  

## Troubleshooting

### Improve tranfer speed for OTA Update

* `mqttexampleTRANSPORT_SEND_RECV_TIMEOUT_MS` can be set to 150 or higher  to improve OTA execution time.
* Set the default value to 150 or higher after setting the sending and receiving timeout values separately for sending and receiving.

### Improve timeout when connection error happens

If an error occurs during cellular or wi-fi communication due to the communication environment, changing each definition to the following "correction value" may improve the problem. 

|Definition Name|Description|defalt value|correction value|path|
| ---- | ---- | ---- | ---- | ---- |
|mqttexampleTRANSPORT_SEND_RECV_TIMEOUT_MS| Timeout in milliseconds to send and receive in the transport layer. You try to change the value to more bigger when you met TLS handshake error. Changes in this value affect the time of MQTT communication. A change in the MQTT communication time may increase or decrease the OTA process time. So you should adjust the value of time according to your communication environment. |450|`750`|Demos\mqtt_agent\mqtt_agent_task.c|
|MQTT_AGENT_MAX_EVENT_QUEUE_WAIT_TIME|Time in milliseconds that the MQTT agent task will wait in the Blocked state (so not using any CPU time) for a command to arrive in its command queue before exiting the blocked state so it can call MQTT_ProcessLoop().|50U|`1000U`|src\frtos_config\core_mqtt_agent_config.h|

However, each definition affects the execution time of the demo.  
If the above values are set, the execution time of the demo may increase.  

### How to change MAC Address

The MAC address used in the sample project can be changed using the Smart Configurator.  
The MAC address assigned to your board can be found on a sticker on the board's Ethernet connector.  
If the sticker is not attached, please check [Tool News](https://www.renesas.com/document/tnn/notification-information-about-ck-rx65n-mac-address?r=1611756).  
![TroubleShooting_0_1_MAC_Address_Board](https://github.com/renesas/iot-reference-rx/wiki/getting_started_guide_image/TroubleShooting_0_1_MAC_Address_Board.png)  
MAC Address can be changed from FreeRTOS Kernel of the Smart Configurator.  
Change Components-> FreeRTOS Kernel-> MAC address X (X = 0 to 5).  
After changing the settings, be sure to **Generate Code** and **Build**.  
![TroubleShooting_0_2_MAC_Address](https://github.com/renesas/iot-reference-rx/wiki/getting_started_guide_image/TroubleShooting_0_2_MAC_Address.png)  

### Improve build error "cannot find -lr_tsip_rx65n_little"

If the error "cannot find -lr_tsip_rx65n_little" occurs, please do the following:  

* Add **r_tsip_rx** component in the Smart Configurator  
  1. Double-click **{project name}.scfg** in Project Explorer -> Components -> r_tsip_rx -> downloading it -> OK  
  ![00_download_tsip](https://github.com/renesas/iot-reference-rx/wiki/getting_started_guide_image/00_download_tsip.png)  

  1. Confirm that the component icon has turned blue and click "Generate Code"
  ![01_GenerateCode](https://github.com/renesas/iot-reference-rx/wiki/getting_started_guide_image/01_GenerateCode.png)
* Check the path setting
  1. Right-click on the **project name** in the Project Explorer -> Click on **Properties** -> **C/C++ Build**-> **Settings** -> Tool Settings-> Archives
  1. Check if the path to **"\$\{workspace_loc:\/\$\{ProjName\}\/src\/smc_gen\/r_tsip_rx\/lib\/gcc\/\}"** is set in the area enclosed by the dotted line  
      * If the path is set, no operation is required
    ![00_Chk_TSIPlib_pass](https://github.com/renesas/iot-reference-rx/wiki/getting_started_guide_image/00_Chk_TSIPlib_pass.png)
      * If the path is not set, please add it
        1. Click "Add..." icon -> Set **"\$\{workspace_loc:\/\$\{ProjName\}\/src\/smc_gen\/r_tsip_rx\/lib\/gcc\/\}"** -> OK
          ![00_Set_TSIPlib_pass](https://github.com/renesas/iot-reference-rx/wiki/getting_started_guide_image/00_Set_TSIPlib_pass.png)  
        1. Click **Apply and Close**
          ![01_Set_TSIPlib_pass_Apply](https://github.com/renesas/iot-reference-rx/wiki/getting_started_guide_image/01_Set_TSIPlib_pass_Apply.png)  
        1. Click **Rebuild Index**
          ![02_Set_TSIPlib_pass_rebuild](https://github.com/renesas/iot-reference-rx/wiki/getting_started_guide_image/02_Set_TSIPlib_pass_rebuild.png)  

After changing the settings, please build the project again.
