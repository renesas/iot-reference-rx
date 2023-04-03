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

## Step 1 Prerequisites

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


## Step 2 Hardware and Software Setup

1. Plug in the Cellular RYZ014A to the PMOD1 on the  CK-RX65N board for using cellular
2. Plug in the Ethernet cab to the CK-RX65N board for using ethernet


## Step 3  Import Projects into e2studio
1. Open e2studio
2. Choose workspace abd click **Lauch**
3. **File** -> **Import...** -> **Existing Projects into WorkSpace**
4. Click **Browse...** and choose **Ethernet project** or **Cellular project** demo
> Ensure that *copy projects into workspace* is not selected
7. Click **Finish** to import the projects.

### Step 4 Build Firmware image and Flash your development board
### Building

In the **Project Explorer** pane of e2studio, Right click on the project and select **Build Project**
> Note: Or choose **Project**->**Build Project** menu to build

### Flashing
1. Using e2studio

In the **Project Explorer** pane of e2studio, Right click on the project and select **Debug As** --> **Renesas GDB Hardware Debugging**

2. Using **Rensas Flash Programmer**

Go to **Projects** -> **aws_cellular_ck_rx65n** or **aws_ether_ck_rx65n** -> **flash_project** -> **flash_project.rpj** and set the location of mot file

## Step 8: Provision Device via CLI
```
Setting up Tera-term

Receive : AUTO
Transmit: CR+LF
Port : Your COM
Baudrate : 115200
Data : 8 bit
Parity : none
Stop: 1 bit
Flow control : none

```

#### Switch to CLI mode
```
> CLI
OK
```


#### Thing name
First, configure the desired thing name / mqtt device identifier:
```
> conf set thingname thing_name
OK
```
#### MQTT Endpoint
Next, set the mqtt endpoint to the endpoint for your account:
```
> conf set mqtt_endpoint xxxxxxxxxxxxxx-ats.iot.us-west-2.amazonaws.com
OK
```
> Note: You can determine the endpoint for your AWS account with the ```aws iot describe-endpoint``` command or on the *Settings* page of the AWS IoT Core console.

#### Commit Configuration Changes
Finally, commit the staged configuration changes to Data Flash.
```
> conf commit
Configuration saved to Data Flash.
```
Save the resulting certificate to a new file.

#### Reset the target device to start demo
```
> reset
OK
```


