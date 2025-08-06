---------------
1. Usage Notes
---------------

1.1. This project demonstrates the use of FreeRTOS-Plus-TCP Module with Ethernet. The application demonstrates the usage of FreeRTOS DHCP Client, DNS Client and ICMP code.  
This project supports 2 different modes:
  1) DHCP Mode: IP credentials are obtained from the DHCP Server on the Network.
  2) Static IP Mode: Static IP address is configured for the Ethernet based on the availability of free address in the LAN.

Note1: The project is configured as `DHCP Mode` by default.  
Note2: User is expected to enter the static IP address according to the network settings of the LAN.
      In case of DHCP, the application will communicate with the DHCP server to get a dynamic IP address.

1.2. When running the application using DHCP or Static IP mode, the following settings needs to be configured in the Smart Configurator.  
The same project can be used for both mode/configuration.  
DHCP Mode:
|    SC Component    |       Macro config       |       Value       |
|--------------------|--------------------------|-------------------|
|FreeRTOS_IP         |     ipconfigUSE_DHCP     |       Enable      |
|FreeRTOS_Kernel     |     configIP_ADDR0-3     |      Not used     |
|FreeRTOS_Kernel     |    configNET_MASK0-3     |      Not used     |
|FreeRTOS_Kernel     |  configGATEWAY_ADDR0-3   |      Not used     |
|FreeRTOS_Kernel     | configDNS_SERVER_ADDR0-3 |      Not used     |
  
Static IP Mode:  
|    SC Component    |       Macro config       |       Value       |
|--------------------|--------------------------|-------------------|
|FreeRTOS_IP         |     ipconfigUSE_DHCP     |      Disable      |
|FreeRTOS_Kernel     |     configIP_ADDR0-3     |        Used       |
|FreeRTOS_Kernel     |    configNET_MASK0-3     |        Used       |
|FreeRTOS_Kernel     |  configGATEWAY_ADDR0-3   |        Used       |
|FreeRTOS_Kernel     | configDNS_SERVER_ADDR0-3 |        Used       |


1.3. The steps to configure and run the project are described below:  
  1. In `user_app.h`, configure `USR_TEST_PING_IP` macro to a valid public IP address (e.g: google.com, 142.251.42.142).  
  2. Make sure Ethernet cable is connected to your device (e.g: https://github.com/renesas/iot-reference-rx/wiki/getting_started_guide_image/step2_ether.png?raw=true)
  3. Build and run the project.  
  4. The following log will be output to the `Renesas Debug Virtual Console`:  
     ```
        Network is Up
        Ethernet adapter for Renesas XXXX:
                Description . . . . . . . . . . . : XXXXX
                Physical Address. . . . . . . . . : XX-XX-XX-XX-XX-XX
                DHCP Enabled. . . . . . . . . . . : (Variable -- DHCP:Yes, StaticIP:No)
                IPv4 Address. . . . . . . . . . . : (Variable -- DHCP:Dynamic IP, StaticIP:ipconfigIP_ADDR0-3)
                Subnet Mask . . . . . . . . . . . : 255.255.255.0
                Default Gateway . . . . . . . . . : XXX.XX.XX.XX
                DNS Servers . . . . . . . . . . . : XXX.XX.XX.XX
        FreeRTOS_ProcessDNSCache: add: 'www.freertos.org' @ 12ac344dip (TTL 58)
        DNS[0x53DC]: The answer to 'www.freertos.org' (XX.XXX.XX.XX) will be stored

        DNS Lookup for www.freertos.org is      : XX.XXX.XX.XX  

        Pinging 142.251.42.142:
        ... 
        Ping Statistics for 142.251.42.142 :

        Packets: Sent  = 100, Received = XX, Lost = 00 
     ```

1.4. Since this is a minimal project, you can easily convert it to a `FreeRTOS_Kernel` only project by removing the `FreeRTOS_IP` component in Smart Configurator.  
    Note 1: In e2studio_2025-07 or older, after removing the `FreeRTOS_IP` component in Smart Configurator, please make sure to copy the `FreeRTOSConfig.h` from `/src/patch` folder to the `src/frtos_config` folder again.
      
    Note 2: After removing the `FreeRTOS_IP` component in Smart Configurator, please make sure the source code in `../sample_tcp/` folder does not invoke FreeRTOS-Plus-TCP related API. Otherwise, build error may happen.   
