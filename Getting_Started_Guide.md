This includes demonstration tasks for the following AWS services:
* [AWS IoT Jobs](https://docs.aws.amazon.com/iot/latest/developerguide/iot-jobs.html)
* [MQTT File Delivery](https://docs.aws.amazon.com/iot/latest/developerguide/mqtt-based-file-delivery.html)
* [AWS IoT OTA Update](https://docs.aws.amazon.com/freertos/latest/userguide/freertos-ota-dev.html)

The demo projects both connect to AWS IoT core via the included Ethernet or Cellular and use the [CoreMQTT-Agent](https://github.com/FreeRTOS/coreMQTT-Agent) library to share a single MQTT connection among multiple tasks. These tasks publish environemnt and motion sensor data from a subset of the sensor available on the development board, and demonstrate use of the AWS IoT Device Shadow and Device Defender services.

## AWS IoT Core Demo Tasks
* MQTT Agent
* OTA Update

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

## Step 4: Provision Device via CLI
### Setting up Tera-term
```
Receive : AUTO
Transmit: CR+LF
Port : Your COM
Baudrate : 115200
Data : 8 bit
Parity : none
Stop: 1 bit
Flow control : none

```
#### Use RSA key pair and certificate

#### Generate ECDSA key pair and certificate
1. Create a CA private key
```
$ openssl ecparam -genkey -name secp256r1 -out ca.key

```
2. Create a CA certificate from the created CA private key
```
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
3. Generate key pairs for elliptic curve cryptography
```
$ openssl ecparam -genkey -name secp256r1 -out secp256r1.keypair
using curve name prime256v1 instead of secp256r1
```

4. Create certificate signing request for key pair

```
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

5. Create a ***certificate*** from the certificate signing request/CA certificate/CA private key 

```
$ openssl x509 -req -sha256 -days 3650 -in secp256r1.csr -CA ca.crt -CAkey ca.key -CAcreateserial -out secp256r1.crt
Signature ok
subject=C = JP, ST = Tokyo, L = Kodaira, O = Renesas Electronics, OU = Software Development Division, CN = Renesas Tarou, emailAddress = Tarou.Renesas@sample.com
Getting CA Private Key
```
> Note: Note certificate

6. Extract the ***private key*** from the ECDSA key pair

```
$ openssl ec -in secp256r1.keypair -outform PEM -out secp256r1.privatekey
read EC key
writing EC key
```
> Note: Note private key

7. Extract the ***public key*** from the ECDSA key pair

```
$ openssl ec -in secp256r1.keypair -outform PEM -pubout -out secp256r1.publickey
read EC key
writing EC key
```
> Note: Note public key

#### EOF Conversion

To copy to tera-term without any issues, you need to change the EOF (Unix LF) of these key pair and certificate by uisng NotePad++.

#### Switch to CLI mode
```
> CLI
Going to FreeRTOS-CLI !

```

#### Upload Thing name
First, configure the desired thing name / mqtt device identifier:
```
> conf set thingname thing_name
OK
```
#### Upload MQTT Endpoint
Next, set the mqtt endpoint to the endpoint for your account:
```
> conf set mqtt_endpoint xxxxxxxxxxxxxx-ats.iot.us-west-2.amazonaws.com
OK
```
> Note: You can determine the endpoint for your AWS account with the ```aws iot describe-endpoint``` command or on the *Settings* page of the AWS IoT Core console.

#### Upload Certificate
Next, set the certificate 

```
> conf set cert xxxxxxxxxxxxxxxxxxxxxxxxxxxxx
OK
```

#### Upload private key
Next, set the certificate 

```
> conf set key xxxxxxxxxxxxxxxxxxxxxxxxxxxxx
OK
```

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


