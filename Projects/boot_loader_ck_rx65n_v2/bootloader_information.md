# Bootloader (Using in the "PubSub/MQTT with Over-the-air(OTA) update sample project")

## Overview

This document provides information about the bootloader used for "*PubSub/MQTT with Over-the-air(OTA) update sample project*".

> **Note:**  
> For more information about setting up and running the demo, see [**"Getting_Started_Guide.md"**](../../../Getting_Started_Guide.md).

## About the Bootloader

In the OTA demo, the bootloader runs first and verifies the firmware signature.  
If the signature verification passes, the firmware starts.

### Memory usage

| Compiler | RAM size | ROM size |
|----------|----------|----------|
| CC-RX    | 5KB      | 30KB    |
| GCC      | 11KB     | 42KB    |

### Confirmed Operation Environment

| Compiler | Version  | Details |
|----------|----------|---------|
| CC-RX    | V3.07.00 | Optimization: Level 2 |
| GCC      | GCC for Renesas RX v8.3.0.202411 | Optimization options:<br>&emsp;- Optimize for Debug<br>&emsp;- Function sections<br>&emsp;- Data sections<br>&emsp;- Enable garbage collection of unused input sections (-gc-sections) |

## Bootloader design policy

* The process executed by this bootloader is as follows:
  ![bootloader_mechanism](https://github.com/renesas/iot-reference-rx/wiki/getting_started_guide_image/bootloader_mechanism.png)  
   1. The Bootloader and User Program are written to the execute area , and the Bootloader is written to the temporary area.  
     The user program in the execute area (bank 0) is executed.
   2. The new user program is downloaded from AWS and written to the temporary area.
   3. A bank swap is performed and the execute area and temporary area are swapped.
   4. The old user program in the temporary area (bank 0) is deleted and the new user program in the execute area (bank 1) is executed.

## Explanation specific to Bootloader

### Software setup

The bootloader project needs to register a public key for firmware signature verification before building.

* The procedure for registering the public key are provided in the special application note (document number: R01AN7662).  
  You can search it by visiting the following webpage:
  * <https://www.renesas.com/search?keywords=r01an7662>  
* Refer to Section 4.2.2 of the application note to register the public key to the project.
* Also, For software setup instructions, see [**Getting Started Guide: step 4-2**](../../../Getting_Started_Guide.md#step-4-2-run-pubsubmqtt-with-over-the-airota-update-sample-project).
