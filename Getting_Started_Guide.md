# Getting Started Guide

This document explains demo and how to run them.

## Summary of demos

The following table indicates the character of demos.
|Demo Name|Based AWS IoT contents|Description|
| ---- | ---- | ---- |
|PubSub|[coreMQTT demos](https://docs.aws.amazon.com/freertos/latest/userguide/mqtt-demo.html)|It demonstrates simple MQTT communication between device and AWS server.|
|Fleet Provisioning|[AWS IoT fleet provisioning](https://docs.aws.amazon.com/iot/latest/developerguide/provision-wo-cert.html)|It provide the some secure provisioning operation to device at first connection to AWS server.|

Each demo is independent as a FreeRTOS's task. It means multiple demos can be run at the same time.

The demos connect to AWS IoT core via the Ethernet or Cellular with MQTT protocol by using the [CoreMQTT-Agent](https://github.com/FreeRTOS/coreMQTT-Agent) library to share a single MQTT connection among multiple tasks.These tasks establish a server and client-authenticated TLS connection, and demonstrates the subscribe-publish workflow of MQTT.

## How to run demos

This chapter explains step by step instructions for running demos.

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
* Flash tool: [Renesas Flash Programmer](https://www.renesas.com/software-tool/renesas-flash-programmer-programming-gui)
  * It is optional usage when not using emulator
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
3. For receiving debug logs, connect an USB cable between the USB-serial connector (J20) on the CK-RX65N and your PC with serial terminal application to receive logs.
    Note that debug logs mean serial output data which is coded in the demo program and Renesas driver software.
    It is not directly related to debugging on e2 studio.

### Step 3: Import projects into e2 studio

Import demo projects into IDE; e2 studio.

1. Open e2 studio.
1. Choose workspace abd click **Launch**.
1. **File** -> **Import...** -> **Existing Projects into WorkSpace**.
1. Click **Browse...** and choose **aws_cellular_ck_rx65n** (for Cellular) or **aws_ether_ck_rx65n** (for Ethernet) demo.

    **Note:** Ensure that **copy projects into workspace** is not selected.
1. Click **Finish** to import the projects.

### Step 4: Run demos

***Choose the following chapter according to using demo.***

The <project_name> term means one of the following folder name according to used communication way:

* When using Ethernet: aws_ether_ck_rx65n
* When using Cellular: aws_cellular_ck_rx65n

---

#### Step 4-1: Run PubSub demo without Fleet Provisioning

See this chapter when running PubSub demo without Fleet Provisioning.

##### Step 4-1-1: Software setup for PubSub demo

Configure settings to run PubSub demo.

###### Configure combination of demos for PubSub demo

Configure settings to select combination of demos.
In "Projects\\<project_name>\\e2 studio_ccrx\\src\\frtos_config\\demo_config.h", configure both of `ENABLE_FLEET_PROVISIONING_DEMO` and `ENABLE_OTA_UPDATE_DEMO`, which allow to select the different supported demo combinations:

* `ENABLE_FLEET_PROVISIONING_DEMO`: (0)
* `ENABLE_OTA_UPDATE_DEMO`: (0)

**Note:** This version does not support OTA Update demo. When `ENABLE_OTA_UPDATE_DEMO` is set to (1), compilation will fail.

###### Settings of reset hook (In case of using Cellular)

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

###### Settings of access point (In case of using Cellular)

Configure 4 macros related to access point in "Projects\\aws_cellular_ck_rx65n\\src\\smc_gen\\r_config\\r_cellular_config.h".
The settings will vary depending on the SIM card that you are using, so please check the SIM information.

* `CELLULAR_CFG_AP_NAME`: Set the access point name of the SIM
* `CELLULAR_CFG_AP_USERID`: Set the user name of the SIM
* `CELLULAR_CFG_AP_PASSWORD`: Set SIM card password
* `CELLULAR_CFG_AP_PIN_CODE`: Set SIM card PIN code

**Note:** You can also set them by RX Smart Configurator on e2 studio. See the chapter [Configure FIT modules in RX Driver Package](#configure-fit-modules-in-rx-driver-package) for more details.

###### Settings of bands (In case of using Cellular)

Configure settings related to the bands supported by your cellular module.
In case of RYZ014A, configure the following macro in "Projects\\aws_ryz014a_ck_rx65n\\e2studio_ccrx\\src\\application_code\\main.c"

* `CELLULAR_BAND_CONFIG`: Set `"1,2,4,5,8,12,13,14,17,18,19,20,25,26,28,66"`

###### Configure middleware

Configure settings of middleware.
Although that, you can use these settings as-is if you only run the demos. Change them if you want to do.
Config files are exist in the following folder:

* "Projects\\<project_name>\\e2 studio_ccrx\\src\\frtos_config"

###### Configure FIT modules in RX Driver Package

Configure some settings of [FIT modules](https://www.renesas.com/software-tool/fit), which is middleware or drivers provided by Renesas via [RX Driver Package](https://www.renesas.com/software-tool/rx-driver-package), by using RX Smart Configurator.
Although that, you can use these settings as-is if you only run the demos. Change them if you want to do.

See the [manual of RX Smart Configurator](https://www.renesas.com/document/mat/rx-smart-configurator-users-guide-e-studio) for details on how to use it. The Config files generated after code generation by RX Smart Configurator are exist in the following folder:

* "Projects\\<project_name>\\e2 studio_ccrx\\src\\smc_gen"

##### Step 4-1-2: Create RSA key pair and device certificate for PubSub demo

RSA key pair and device certificate can be generated with AWS Console when creating new iot-thing.
For more information about create iot-thing with **Auto-generate a new certificate**, please refer to the following webpage:
<https://docs.aws.amazon.com/iot/latest/developerguide/create-iot-resources.html>

##### Step 4-1-3: Building for PubSub demo

Build firmware image by the builder with e2 studio.

In the **Project Explorer** pane of e2 studio, Right click on the project and select **Build Project**. Wait for completing building.

**Note:** Or choose **Project**->**Build Project** menu to build.

##### Step 4-1-4: Flashing for PubSub demo

Flash firmware image to your target device by the either of following ways.

###### Flashing in case of using e2 studio

In the **Project Explorer** pane of e2 studio, right click on the project and select **Debug As** --> **Renesas GDB Hardware Debugging**.

###### Flashing in case of using Renesas Flash Programmer

Go to **Projects** -> **aws_cellular_ck_rx65n** or **aws_ether_ck_rx65n** -> **flash_project** -> **flash_project.rpj** and set the location of mot file.

**Note:** When using **Renesas Flash Programmer** method, please remember enabling "Motorola S format file" in **Properties**->**Settings**->**Converter**->**Output** to generate MOT file after compilation

##### Step 4-1-5: Set up serial terminal application for PubSub demo

Configure Tera Term settings as following if you use it:

* New line configuration: Setup->Terminal->New-line
  * Receive : AUTO
  * Transmit: CR+LF
* Serial port settings: Setup->Serial port
  * Port : Your COM
* Baudrate : 115200
* Data : 8 bit
* Parity : none
* Stop: 1 bit
* Flow control : none

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

**Note:** CLI will timeout after some seconds. If timeout occurs, core process of demo runs.

###### Set IoT information

Next, format the Data Flash:

```text
> format
Format OK !
```

Next, configure the desired thing name / mqtt device identifier:

```text
> conf set thingname thing_name
OK
```

Next, set the mqtt endpoint to the endpoint for your account:

```text
> conf set endpoint xxxxxxxxxxxxxx-ats.iot.us-west-2.amazonaws.com
OK
```

**Note:** You can determine the endpoint for your AWS account with the ```aws iot describe-endpoint``` command or on the *Settings* page of the AWS IoT Core console.

###### Set security information of target device

Next, set the device certificate:

```text
> conf set cert <device certificate>
OK
```

**Note:** About \<device certificate\>, if you use TeraTerm, drag and drop the certificate file onto the Terminal screen, then select `Send File (Paste content of file)` on the screen that appears, and press `OK`. The contents of the certificate file will be entered.

Next, set the device private key:

```text
> conf set key <private key>
OK
```

**Note:** About \<private key\>, if you use TeraTerm, drag and drop the private key file onto the Terminal screen, then select `Send File (Paste content of file)` on the screen that appears, and press `OK`. The contents of the private key file will be entered.

###### Commit configuration changes

Next, commit the staged configuration changes to Data Flash:

```text
> conf commit
Configuration saved to Data Flash.
```

###### Reset the target device and restart demo

Finally, reset the target device once by the following command to restart demo:

```text
> reset
OK
```

##### Step 4-1-7: Expected output for PubSub demo

After restart demo, wait a seconds with input nothing via CLI. Then demo is performed with the following output.

* **PubSub Demo Task 0** and **PubSub Demo Task 1** can complete successfully.
* You can get message "*Hello World*" published to **MQTT test client** console (<https://us-east-2.console.aws.amazon.com/iot/home?region=us-east-2#/test>).

---

#### Step 4-2: Run PubSub demo with Fleet Provisioning

Details for PubSub demo with Fleet Provisioning are provided in the special application note (document number: R20AN0674). You can search it by visiting the following webpage:

* <https://www.renesas.com/jp/ja/search?keywords=r20an0674>

About how to run this demo, see the chapter *4. Running the Fleet Provisioning Demo* in preceding application note.

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
