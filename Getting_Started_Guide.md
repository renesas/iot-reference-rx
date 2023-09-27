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
* Signing: [OpenSSL for Windows](https://www.openssl.org/)
  * To create the OTA signing key and certificate.
  * If you have git installed on your machine, you can also use the openssl.exe that comes with the git installation.
  * We run the demo with using OpenSSL [3.0.7](https://www.openssl.org/source/old/3.0/).
* Text editor: [NotePad++](https://notepad-plus-plus.org/)
  * It is required for EOL conversion.

### Step 1: downloading this product

At first step, prepare this product on your local environment.
Clone this repository by the following commands.

For long path name:

```batchfile
git config --global core.longpaths true
```

To clone using HTTPS:

```batchfile
git clone https://github.com/renesas/iot-reference-rx.git --recurse-submodules
```

Using SSH:

```batchfile
git clone git@github.com:renesas/iot-reference-rx --recurse-submodules
```

If you have downloaded the repo without using the `--recurse-submodules` argument, you should run:

```batchfile
git submodule update --init --recursive
```

### Step 2: Hardware setup

#### To Connect for communication

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

#### To Connect for power supply, debugging, and logging

For more details, refer to the [manual of CK-RX65N](https://www.renesas.com/us/en/document/mat/ck-rx65n-v1-users-manual).

1. Set up jumpers of device board as follows:

    * J2 Short for invalidating MCU current measurement point
    * J11 Open for single-chip mode as MCU boot option
    * J16 1-2 Short for debugging

2. For power supply and debugging, connect an USB cable between the debug connector (J14) on the CK-RX65N and your PC with installed e2 studio.
    This connector has a role of both of power supply and debugging.
3. For receiving debug logs, connect an USB cable between the USB-serial connector (J20) on the CK-RX65N and your PC to receive logs.
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

### Step 4: Software Setup

Configure settings to run demos.
The <project_name> term means the following folder name:

* aws_ether_ck_rx65n
* aws_cellular_ck_rx65n

#### Configure application

##### Configure combination of demos

Configure settings to select combination of demos run.
In "Projects\\<project_name>\\e2 studio_ccrx\\src\\frtos_config\\demo_config.h", configure `ENABLE_FLEET_PROVISIONING_DEMO` and `ENABLE_OTA_UPDATE_DEMO` to enable the different supported demo combinations:

* PubSub demo only : **ENABLE_FLEET_PROVISIONING_DEMO (0)** && **ENABLE_OTA_UPDATE_DEMO (0)**
* PubSub demo with Fleet Provisioning : **ENABLE_FLEET_PROVISIONING_DEMO (1)** && **ENABLE_OTA_UPDATE_DEMO (0)**

**Note:** This version does not support OTA Update demo. When `ENABLE_OTA_UPDATE_DEMO` is set to (1), compilation will fail.

##### Configure Cellular settings (In case of using Cellular)

Configure information of SIM card and cellular module to connect cellular network if you use it.

###### Settings of reset hook for Cellular

The socket wrapper layer provides a hook function to reset the cellular module when an error occurs.
It is recommended to reset the cellular module if the cellular FIT module API "R_Cellular_xxx" returns errors `CELLULAR_ERR_MODULE_TIMEOUT` and `CELLULAR_ERR_MODULE_COM`. Otherwise, cellular module may not communicate.
You can change `USER_TCP_HOOK_ENABLED` in "Projects\\<project_name>\\e2 studio_ccrx\\src\\frtos_config\\user_tcp_hook_config.h" to enable or disable this hook.

You can use hook as-is if you only run the demos.
In our current implementation, cellular module is reset in this hook when the following conditions:

* On TCP_Sockets_Connect,
  * When both errors occur 1 time.
* On TCP_Sockets_Recv and TCP_Sockets_Send,
  * When `CELLULAR_ERR_MODULE_TIMEOUT` occurs 1 time.
  * When `CELLULAR_ERR_MODULE_COM` occurs continuously 3 times.
    * You can change retry numbers by configuring `USER_COMM_ERROR_TRIES`.

###### Settings of access point for Cellular

Configure 4 macros related to access point in "Projects\\aws_cellular_ck_rx65n\\src\\smc_gen\\r_config\\r_cellular_config.h".
The settings will vary depending on the SIM card that you are using, so please check the SIM information.

* `CELLULAR_CFG_AP_NAME`: Set the access point name of the SIM
* `CELLULAR_CFG_AP_USERID`: Set the user name of the SIM
* `CELLULAR_CFG_AP_PASSWORD`: Set SIM card password
* `CELLULAR_CFG_AP_PIN_CODE`: Set SIM card PIN code

**Note:** You can also set them by RX Smart Configurator on e2 studio. See the chapter [Configure FIT modules in RX Driver Package](#configure-fit-modules-in-rx-driver-package) for more details.

###### Settings of bands for Cellular

Configure settings related to the bands supported by your cellular module.
In case of RYZ014A, configure the following macro in "Projects\aws_ryz014a_ck_rx65n\e2studio_ccrx\src\application_code.main.c"

* `CELLULAR_BAND_CONFIG`: Set `"1,2,4,5,8,12,13,14,17,18,19,20,25,26,28,66"`

#### Configure middleware

Configure settings of middleware.
Although that, you can use these settings as-is if you only run the demos. Change them if you want to do.
Config files are exist in the following folder:

* "Projects\\<project_name>\\e2 studio_ccrx\\src\\frtos_config"

#### Configure FIT modules in RX Driver Package

Configure some settings of [FIT modules](https://www.renesas.com/software-tool/fit), which is middleware or drivers provided by Renesas via [RX Driver Package](https://www.renesas.com/software-tool/rx-driver-package), by using RX Smart Configurator.
Although that, you can use these settings as-is if you only run the demos. Change them if you want to do.

See the [manual of RX Smart Configurator](https://www.renesas.com/document/mat/rx-smart-configurator-users-guide-e-studio) for details on how to use it. The Config files generated after code generation by RX Smart Configurator are exist in the following folder:

* "Projects\\<project_name>\\e2 studio_ccrx\\src\\smc_gen"

### Step 5: Creating iot-thing, device certificate, and device key pair

#### Create RSA key pair and certificate

RSA device certificate and key pair can be generated with AWS Console when creating new iot-thing.
For more information about create iot-thing with **Auto-generate a new certificate**, please refer to the following webpage:
<https://docs.aws.amazon.com/iot/latest/developerguide/create-iot-resources.html>

#### Generate ECDSA key pair and certificate by OpenSSL

1. Create a ***CA private key***.

    ```batchfile
    $ openssl ecparam -genkey -name secp256r1 -out ca.key
    ```

2. Create a ***CA certificate*** from the created CA private key.

    ```batchfile
    $ openssl req -x509 -sha256 -new -nodes -key ca.key -days 3650 -out ca.crt
    You are about to be asked to enter information that will be incorporated into your certificate request.
    What you are about to enter is what is called a Distinguished Name or a DN.
    There are quite a few fields but you can leave some blank For some fields there will be a default value, If you enter '.', the field will be left blank.
    -----
    Country Name (2 letter code) [AU]:JP
    State or Province Name (full name) [Some-State]:Tokyo
    Locality Name (eg, city) []:Kodaira
    Organization Name (eg, company) [Internet Widgits Pty Ltd]:Renesas Electronics
    Organizational Unit Name (eg, section) []:Software Development Division
    Common Name (e.g. server FQDN or YOUR name) []:Renesas Tarou
    Email Address []:Tarou.Renesas@sample.com
    ```

3. Generate ***key pairs*** for elliptic curve cryptography.

    ```batchfile
    $ openssl ecparam -genkey -name secp256r1 -out secp256r1.keypair
    using curve name prime256v1 instead of secp256r1
    ```

4. Create ***certificate signing request*** for key pair.

    ```batchfile
    $ openssl req -new -sha256 -key secp256r1.keypair > secp256r1.csr
    You are about to be asked to enter information that will be incorporated
    into your certificate request.
    What you are about to enter is what is called a Distinguished Name or a DN.
    There are quite a few fields but you can leave some blank
    For some fields there will be a default value,
    If you enter '.', the field will be left blank.
    -----
    Country Name (2 letter code) [AU]:JP
    State or Province Name (full name) [Some-State]:Tokyo
    Locality Name (eg, city) []:Kodaira
    Organization Name (eg, company) [Internet Widgits Pty Ltd]:Renesas Electronics
    Organizational Unit Name (eg, section) []:Software Development Division
    Common Name (e.g. server FQDN or YOUR name) []: Renesas Tarou
    Email Address []: Tarou.Renesas@sample.com
    Please enter the following 'extra' attributes
    to be sent with your certificate request
    A challenge password []:
    An optional company name []:
    ```

5. Create a ***certificate*** from the certificate signing request/CA certificate/CA private key.

    ```batchfile
    $ openssl x509 -req -sha256 -days 3650 -in secp256r1.csr -CA ca.crt -CAkey ca.key -CAcreateserial -out secp256r1.crt
    Signature ok
    subject=C = JP, ST = Tokyo, L = Kodaira, O = Renesas Electronics, OU = Software Development Division, CN = Renesas Tarou, emailAddress = Tarou.Renesas@sample.com
    Getting CA Private Key
    ```

6. Extract the ***private key*** from the ECDSA key pair.

    ```batchfile
    $ openssl ec -in secp256r1.keypair -outform PEM -out secp256r1.privatekey
    read EC key
    writing EC key
    ```

7. Extract the ***public key*** from the ECDSA key pair.

    ```batchfile
    $ openssl ec -in secp256r1.keypair -outform PEM -pubout -out secp256r1.publickey
    read EC key
    writing EC key
    ```

#### Attach generated certificate to AWS IoT thing

After generating your certificate and keypair in above section, you need to register the certificate to your AWS IoT thing.
This certificate will be used for authentication and granting permission to AWS resources (e.g: AWS IoT Core, S3 bucket, etc).
The following steps describe how to register your certificate.

1. Sign in to your ***AWS IoT console***.

2. In the navigation pane, under **Manage**, select **Security** > **Certificates**.

3. On the ***Certificates*** page, choose **Add certificate** > **Register certificate**.

4. On the ***Register certificates*** page:
    1. Choose **CA is registered with AWS IoT**.
    1. Under **Registered CA**, select **Register a new CA**.
    This will redirect to the ***CA certificate registration process*** page.

5. On the ***CA certificate registration process*** page:
    1. Select **Register CA in Multi-account mode** *(this option is recommended)*.
    1. Under **CA certificate registration**, click **Choose CA certificate** and upload your rootCA (ca.crt).
    1. Under **CA status**, choose **Active** to activate your CA immediately.
    1. Under **Automatic certificate registration**, choose **On**, so that future uploaded certificate will be registered automatically.
    1. Click **Register**.

6. Go back to the ***Register certificate*** page described in Step 4.
    1. Under **Registered CA**, you should be able to see your registered CA from Step 5.
    If it is not displayed, please refresh the page.
    1. Click **Upload** and select your generated certificate.
    1. Make sure to select your uploaded certificate and click **Activate** to activate it immediately.
    1. Click **Register**.

Your generated certificate is now successfully registered. Next, you need to attach your AWS IoT thing to the certificate.

[//]: # (Maybe this part is not necessary because GSG only uses Fleet Provisioning)

#### Claim credentials in case of PubSub demo with Fleet Provisioning

PubSub demo with Fleet Provisioning requires additional claim credentials. The following steps describe how to generate and configure claim credentials.

1. Generate RSA certificate and key pair using AWS IoT Console.

    For detail steps to generate device certificate and key pair, please refer to the following webpage:
    <https://docs.aws.amazon.com/iot/latest/developerguide/device-certs-create.html>
    Please remember to activate certificate after creating also.
    (<https://docs.aws.amazon.com/iot/latest/developerguide/activate-or-deactivate-device-cert.html>)

1. Create Fleet Provisioning policy.

    For detail steps to create new policy, please refer to the following webpage:
    <https://docs.aws.amazon.com/iot/latest/developerguide/create-iot-resources.html>

1. Attach Fleet Provisioning policy to created certificate.
    For detail steps to attach policy to certificate, please refer to the following webpage:
    <https://docs.aws.amazon.com/iot/latest/developerguide/attach-to-cert.html>

1. Create Fleet Provisioning Templates.
    For more information on Fleet Provisioning templates, please refer to the following webpage:
    <https://docs.aws.amazon.com/iot/latest/developerguide/provision-template.html#fleet-provision-template>

    **Note:** Note template name

### Step 6: Build and flash firmware image

#### Building

Build firmware image by the builder with e2 studio.

In the **Project Explorer** pane of e2 studio, Right click on the project and select **Build Project**. Wait for completing building.
**Note:** Or choose **Project**->**Build Project** menu to build.

#### Flashing

Flash firmware image to your target device by the either of following ways.

##### Flashing in case of using e2 studio

In the **Project Explorer** pane of e2 studio, right click on the project and select **Debug As** --> **Renesas GDB Hardware Debugging**.

##### Flashing in case of using Renesas Flash Programmer

Go to **Projects** -> **aws_cellular_ck_rx65n** or **aws_ether_ck_rx65n** -> **flash_project** -> **flash_project.rpj** and set the location of mot file.

**Note:** When using **Renesas Flash Programmer** method, please remember enabling "Motorola S format file" in **Properties**->**Settings**->**Converter**->**Output** to generate MOT file after compilation

### Step 7: Provision device via CLI

#### Set up serial terminal application

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

#### Input IoT information via CLI

To start communication to AWS, a device need some information. Input the following commands to CLI which runs with starting a demo.

The summarized flow of the command input is as follows:

1. Switch CLI mode.
1. Set IoT information (thing name and MQTT endpoint).
1. Set security information (certificate, key, etc.) required for each demo.
1. Commit preceding changes.
1. Reset and restart demo.

**Note:** In case of using Tera Term, you need **EOL Conversion** when inputting key pair and certificate to Tera Term. Change the EOL (Unix LF) of these key pair and certificate by copying from original files and pasting to [NotePad++](https://notepad-plus-plus.org/), then input them to Tera Term.

##### Switch to CLI mode

CLI automatically runs just after starting demo. Switch to CLI mode by inputting the following command:

```batchfile
> CLI
Going to FreeRTOS-CLI !
```

**Note:** CLI will timeout after some seconds. If timeout occurs, core process of demo runs.

##### Set IoT information

###### Upload thing name

First, configure the desired thing name / mqtt device identifier:

```batchfile
> conf set thingname thing_name
OK
```

###### Upload MQTT endpoint

Next, set the mqtt endpoint to the endpoint for your account:

```batchfile
> conf set mqtt_endpoint xxxxxxxxxxxxxx-ats.iot.us-west-2.amazonaws.com
OK
```

**Note:** You can determine the endpoint for your AWS account with the ```aws iot describe-endpoint``` command or on the *Settings* page of the AWS IoT Core console.

##### Set security information in case of PubSub demo only

###### Upload certificate

Next, set the device certificate.

```batchfile
> conf set cert xxxxxxxxxxxxxxxxxxxxxxxxxxxxx
OK
```

###### Upload private key

Next, set the device private key.

```batchfile
> conf set key xxxxxxxxxxxxxxxxxxxxxxxxxxxxx
OK
```

##### Set security information in case of PubSub demo with Fleet Provisioning

###### Upload rootCA

Next, set the rootCA.
**Note:** This step is optional. If no rootCA is set, the demo will use default root defined in source code

```batchfile
> conf set rootca xxxxxxxxxxxxxxxxxxxxxxxxxxxxx
OK
```

###### Upload claim certificate

Next, set the claim certificate.

```batchfile
> conf set claimcert xxxxxxxxxxxxxxxxxxxxxxxxxxxxx
OK
```

###### Upload claim key

Next, set the claim key.

```batchfile
> conf set claimkey xxxxxxxxxxxxxxxxxxxxxxxxxxxxx
OK
```

###### Upload provisioning template

Next, set the provisioning template.

```batchfile
> conf set template xxxxxxxxxxxxxxxxxxxxxxxxxxxxx
OK
```

##### Commit configuration changes

Finally, commit the staged configuration changes to Data Flash.

```batchfile
> conf commit
Configuration saved to Data Flash.
```

Save the resulting certificate to a new file.

##### Reset the target device and restart demo

Reset the target device once by the following command to restart demo.

```batchfile
> reset
OK
```

### Expected output

Each demo expects the following output.

#### Output in case of PubSub demo only

* **PubSub Demo Task 0** and **PubSub Demo Task 1** can complete successfully.
* You can get message "*Hello World*" published to **MQTT test client** console (<https://us-east-2.console.aws.amazon.com/iot/home?region=us-east-2#/test>).

#### Output in case of PubSub demo Fleet Provisioning

* Device is provisioned with a new iot-thing name and new device certificate (incl. attached policy) created on AWS IoT Console
* **PubSub Demo Task 0** and **PubSub Demo Task 1** can complete successfully.
* PubSub demo will run after Fleet Provisioning task completes (after "Fleet Provisioning Task Finished" in log).
* You can get message "*Hello World*" published to **MQTT test client** console (<https://us-east-2.console.aws.amazon.com/iot/home?region=us-east-2#/test>).
