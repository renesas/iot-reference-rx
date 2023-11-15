# Getting Started Guide

This document explains demo and how to run them.

## Summary of demos

The following table indicates the character of demos.
|Demo Name|Based AWS IoT contents|Description|
| ---- | ---- | ---- |
|PubSub demo|[coreMQTT demos](https://docs.aws.amazon.com/freertos/latest/userguide/mqtt-demo.html)|It demonstrates simple MQTT communication between device and AWS server.|
|PubSub demo with Fleet Provisioning|[AWS IoT fleet provisioning](https://docs.aws.amazon.com/iot/latest/developerguide/provision-wo-cert.html)|It provides the some secure provisioning operation to device at first connection to AWS server.|
| PubSub demo with OTA | [OTA tutorial](https://docs.aws.amazon.com/freertos/latest/userguide/dev-guide-ota-workflow.html) | It provides the steps to update the firmware on your device.|

Each demo is independent as a FreeRTOS's task. It means multiple demos can be run at the same time.

The demos connect to AWS IoT core via the Ethernet or Cellular with MQTT protocol by using the [CoreMQTT-Agent](https://github.com/FreeRTOS/coreMQTT-Agent) library to share a single MQTT connection among multiple tasks.These tasks establish a server and client-authenticated TLS connection, and demonstrates the subscribe-publish workflow of MQTT.

## How to run demos

This chapter explains step by step instructions for running demos by importing projects into e2 studio.  
When you create as a new projects according to the FAQ below, you can skip step1 and step3.  
https://en-support.renesas.com/knowledgeBase/21115016  

### Prerequisites

#### Hardware requirements

* [CK-RX65N](https://www.renesas.com/products/microcontrollers-microprocessors/rx-32-bit-performance-efficiency-mcus/ck-rx65n-cloud-kit-based-rx65n-mcu-group)
* [RYZ014A](https://www.renesas.com/br/en/products/wireless-connectivity/cellular-iot-modules/ryz014a-lte-cat-m1-cellular-iot-module) as Cellular (CAT M1) module if you use Cellular communication

#### Software requirements

* IDE: [e2 studio](https://www.renesas.com/us/en/software-tool/e-studio#download) 2023-07 or later
  * A user account of Renesas is required to download.
* Compiler: [CC-RX](https://www.renesas.com/software-tool/cc-compiler-package-rx-family) V3.05.00 or later
* Code generator: [RX Smart Configurator](https://www.renesas.com/software-tool/rx-smart-configurator)
  * It is installed with e2 studio.
* Serial terminal application: such as [Tera Term](https://ttssh2.osdn.jp/index.html.en).

### Step 1: Downloading this product

At first step, prepare this product on your local environment.
Clone this repository by the following commands.

For long path name:

```text
git config --global core.longpaths true
```

To clone using HTTPS:

```text
git clone https://github.com/renesas/iot-reference-rx.git --recurse-submodules
```

Using SSH:

```text
git clone git@github.com:renesas/iot-reference-rx --recurse-submodules
```

If you have downloaded the repo without using the `--recurse-submodules` argument, you should run:

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
Also see [Settings of Bands](#settings-of-bands-in-case-of-using-cellular) for band settings.


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

Board settings image for ether :  
![2](https://github.com/renesas/iot-reference-rx/wiki/getting_started_guide_image/step2_ether.png?raw=true)  

### Step 3: Import projects into e2 studio

Import demo projects into IDE; e2 studio.

1. Open e2 studio.
1. Choose workspace and click **Launch**.
1. **File** -> **Import...** -> **Existing Projects into WorkSpace**.
1. Click **Browse...** and choose **aws_ryz014a_ck_rx65n** (for Cellular) or **aws_ether_ck_rx65n** (for Ethernet) demo.  
![3-4](https://github.com/renesas/iot-reference-rx/wiki/getting_started_guide_image/step3_4_project_import.PNG?raw=true)

    **Note:** Ensure that **copy projects into workspace** is not selected.  
![3-4](https://github.com/renesas/iot-reference-rx/wiki/getting_started_guide_image/step3_4_project_import2.PNG?raw=true)  
1. Click **Finish** to import the projects.

### Step 4: Run demos

***Choose the following chapter according to using demo.***

The <project_name> term means one of the following folder name according to used communication way:

* When using Ethernet: aws_ether_ck_rx65n
* When using Cellular: aws_cellular_ck_rx65n

The correspondence between each demo and explanation is shown below.
|demo|operating procedure|
|---|---|
|Pubsub demo| [Step 4-1: Run Pubsub demo](#step-4-1-run-pubsub-demo)  |
|Pubsub demo with Fleet Provisioning|[Step 4-2: Run Pubsub demo with Fleet provisioning](#step-4-2-run-pubsub-demo-with-fleet-provisioning)|
|Pubsub demo with OTA|[Step 4-3: Run Pubsub demo with OTA](#step-4-3-run-pubsub-demo-with-ota)|

---

#### Step 4-1: Run PubSub demo

See this chapter when running PubSub demo.

##### Step 4-1-1: Software setup for PubSub demo

Configure settings to run PubSub demo.

###### Configure combination of demos for PubSub demo

Configure settings to select combination of demos.
In "Projects\\<project_name>\\e2 studio_ccrx\\src\\frtos_config\\demo_config.h", configure both of `ENABLE_FLEET_PROVISIONING_DEMO` and `ENABLE_OTA_UPDATE_DEMO`, which allow to select the different supported demo combinations:

* `ENABLE_FLEET_PROVISIONING_DEMO`: (0)
* `ENABLE_OTA_UPDATE_DEMO`: (0)  
![4-1-1](https://github.com/renesas/iot-reference-rx/wiki/getting_started_guide_image/step4_1_1config.PNG?raw=true)

###### Settings of reset hook (Only using Cellular)

The socket wrapper layer provides a hook function to reset the cellular module when an error occurs.
It is recommended to reset the cellular module if the cellular FIT module API "R_Cellular_xxx" returns errors `CELLULAR_ERR_MODULE_TIMEOUT` or `CELLULAR_ERR_MODULE_COM`. Otherwise, cellular module may not communicate.
You can change `USER_TCP_HOOK_ENABLED` in "Projects\\<project_name>\\e2 studio_ccrx\\src\\frtos_config\\user_tcp_hook_config.h" to enable or disable this hook.

You can use hook as-is if you only run the demos.
In our current implementation, cellular module is reset in this hook when the following conditions:

* On TCP_Sockets_Connect,
  * When both errors occur 1 time.
* On TCP_Sockets_Recv and TCP_Sockets_Send,
  * When `CELLULAR_ERR_MODULE_TIMEOUT` occurs 1 time.
  * When `CELLULAR_ERR_MODULE_COM` occurs continuously 3 times.
    * You can change retry numbers by configuring `USER_COMM_ERROR_TRIES`.

###### Settings of RX Smart Configurator

Configure the Renesas code generator (RX Smart Configurator*).  
Please open RX Smart Configurator* and configure settings according to the image below.  
Select **aws_ryz014a_ck_rx65n.scfg** --> **Components** --> **Icon for adding components**　--> **Configure general settings...** --> **Component** --> **code generation behavior column** --> **Update configuration files**.

![4-1-1](https://github.com/renesas/iot-reference-rx/wiki/getting_started_guide_image/step4_1_1_sc_genetate_setting.PNG?raw=true)

*See the [manual of RX Smart Configurator](https://www.renesas.com/document/mat/rx-smart-configurator-users-guide-e-studio) for details on how to use it. The Config files generated after code generation by RX Smart Configurator are exist in the following folder:

* "Projects\\<project_name>\\e2 studio_ccrx\\src\\smc_gen"

###### Download the FreeRTOS kernel and FIT module

Download the FreeRTOS kernel to run the sample project.  
If FreeRTOS kernel is not installed, the icon will be displayed as a gray square as shown in the image below.  
If you have already installed the FreeRTOS kernel, please skip the download step.  
FreeRTOS kernels can be downloaded using RX Smart Configurator.  
Right-click FreeRTOS_Kernel and select **Change version...**.  
![4-1-1](https://github.com/renesas/iot-reference-rx/wiki/getting_started_guide_image/step4_1_1download_freertos1.png?raw=true)  

Click **Manage RTOS Versions...** and download the latest version of FreeRTOS Kernel.  
![4-1-1](https://github.com/renesas/iot-reference-rx/wiki/getting_started_guide_image/step4_1_1_download_freertos2.PNG?raw=true)

To install the FIT module, click **downlowding it** as shown in the image below to install the latest version.  
If you have also installed FIT, please skip this step.  
![4-1-1](https://github.com/renesas/iot-reference-rx/wiki/getting_started_guide_image/step4_1_1_download_FIT.PNG?raw=true)

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


##### Step 4-1-5: Flashing for PubSub demo

Flash firmware image to your target device by the either of following ways.

In the **Project Explorer** panel of e2 studio, right click on the project and select **Debug As** --> **Renesas GDB Hardware Debugging**.
 ![4-1-5](https://github.com/renesas/iot-reference-rx/wiki/getting_started_guide_image/step4_1_5debug_start.png?raw=true)  

If the window below appears, press "Switch".  
 ![4-1-5](https://github.com/renesas/iot-reference-rx/wiki/getting_started_guide_image/step4_1_5debug_message.PNG?raw=true) 

Press the following button to start debugging.  
The first command of step 4-1-6 must be entered on teraterm within 10 seconds after the start of debugging.
It is recommended that you have teraterm ready for easy input.
 ![4-1-5](https://github.com/renesas/iot-reference-rx/wiki/getting_started_guide_image/step4_1_5_press_start.PNG?raw=true) 

##### Step 4-1-6: Input via CLI for PubSub demo

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
Drag and drop "xxxx-certificate.pem.crt" generated in [Step 4-1-2 Create RSA key pair and device certificate for PubSub demo](#step-4-1-2-create-rsa-key-pair-and-device-certificate-for-pubsub-demo) to teraterm.

 ![4-1-6](https://github.com/renesas/iot-reference-rx/wiki/getting_started_guide_image/step4_1_6_5cert.PNG?raw=true) 

**Note:** About \<device certificate\>, if you use TeraTerm, drag and drop the certificate file onto the Terminal screen after entered `conf set cert` , then select `Send File (Paste content of file)` on the screen that appears, and press `OK`. The contents of the certificate file will be entered.

Next, set the device private key:  
Drag and drop "xxxx-private.pem.key" generated in [Step 4-1-2 Create RSA key pair and device certificate for PubSub demo](#step-4-1-2-create-rsa-key-pair-and-device-certificate-for-pubsub-demo) to teraterm.


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

##### Step 4-1-7: Expected output for PubSub demo

After restart demo, wait a seconds with input nothing via CLI. Then demo is performed with the following output.

* **PubSub Demo Task 0** and **PubSub Demo Task 1** can complete successfully.  
![4-1-7](https://github.com/renesas/iot-reference-rx/wiki/getting_started_guide_image/step4_1_7_1.PNG?raw=true) 


* You can get message "*Hello World*" published to **MQTT test client** console  (<https://us-east-2.console.aws.amazon.com/iot/home?region=us-east-2#/test>).  
Enter "#" on the AWS console and press "Subscribe".![4-1-7](https://github.com/renesas/iot-reference-rx/wiki/getting_started_guide_image/step4_1_7_2.PNG?raw=true) 
If successful, it will be displayed on AWS conclose as shown below.![4-1-7](https://github.com/renesas/iot-reference-rx/wiki/getting_started_guide_image/step4_1_7_3.PNG?raw=true) 

---

#### Step 4-2: Run PubSub demo with Fleet Provisioning

Details for PubSub demo with Fleet Provisioning are provided in the special application note (document number: R20AN0674). You can search it by visiting the following webpage:

* <https://www.renesas.com/search?keywords=r20an0674>

About how to run this demo, see the chapter *4. Running the Fleet Provisioning Demo* in preceding application note.

When running this demo, please enable [Settings of RX Smart Configurator](#settings-of-rx-smart-configrater) before generating code with RX Smart Configurator.

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

#### Step 4-3: Run Pubsub demo with OTA
Details for Pubsub demo with OTA are provided in the special application note (document number: R20AN7037). You can search it by visiting the following webpage:

* <https://www.renesas.com/search?keywords=r20an7037>  

About how to run this demo, see the chapter *2. Prerequisites* in preceding application note.

When running this demo, please enable [Settings of RX Smart Configurator](#settings-of-rx-smart-configrater) before generating code with RX Smart Configurator.

---

## Troubleshooting
If an error occurs during cellular communication due to the communication environment, changing each definition to the following "correction value" may improve the problem.    

|Definition Name|Description|defalt value|correction value|path|
| ---- | ---- | ---- | ---- | ---- |
|mqttexampleTRANSPORT_SEND_RECV_TIMEOUT_MS|Transport timeout in milliseconds for transport send and receive. You try to change the value to more big when you met TLS handshake error. Please be careful the value is effected the total time of OTA process. So you should adjust the value of time as your module.|450|`750`|Demos\mqtt_agent\mqtt_agent_task.c|
|MQTT_AGENT_MAX_EVENT_QUEUE_WAIT_TIME|Time in milliseconds that the MQTT agent task will wait in the Blocked state (so not using any CPU time) for a command to arrive in its command queue before exiting the blocked state so it can call MQTT_ProcessLoop().|50U|`1000U`|src\frtos_config\core_mqtt_agent_config.h|


However, each definition affects the execution time of the demo.  
If the above values are set, the execution time of the demo may increase.  
