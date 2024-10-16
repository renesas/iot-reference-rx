/***********************************************************************************************************************
 * File Name    : net_thread_entry.c
 * Description  : This file contains the User Application code for the Ethernet + TCP/IP
 ***********************************************************************************************************************/
/***********************************************************************************************************************
 * DISCLAIMER
 * This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products. No
 * other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all
 * applicable laws, including copyright laws.
 * THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
 * THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED. TO THE MAXIMUM
 * EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES
 * SHALL BE LIABLE FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO THIS
 * SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
 * Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability of
 * this software. By using this software, you agree to the additional terms and conditions found by accessing the
 * following link:
 * http://www.renesas.com/disclaimer
 *
 * Copyright (C) 2020 Renesas Electronics Corporation. All rights reserved.
 ***********************************************************************************************************************/
#include "net_thread.h"
#include "FreeRTOS_IP.h"
#include "FreeRTOS_DHCP.h"
#include "FreeRTOS.h"
#include "FreeRTOS_IP_Private.h"
#include "FreeRTOS_Sockets.h"
#include "user_app.h"

#define LinkProcess_REVISED
#ifdef LinkProcess_REVISED
#include "r_ether_rx_if.h"
extern volatile uint8_t link_detect[ ETHER_CHANNEL_MAX ];
#endif /* #ifdef LinkProcess_REVISED */

#include "platform.h"

uint8_t *  gp_domain_name = (uint8_t *) USR_TEST_DOMAIN_NAME; //Cast to uint8_t

/* New Thread entry function */
uint8_t *  gp_remote_ip_address = (uint8_t *) USR_TEST_PING_IP; //Cast to uint8_t

#if( ipconfigUSE_DHCP != 0 )
extern NetworkAddressingParameters_t xNetworkAddressing;
NetworkAddressingParameters_t xNd =
{ RESET_VALUE };
#endif

uint32_t    g_dhcp_in_use    = RESET_VALUE;
uint32_t    g_usr_ping_count = RESET_VALUE;
ping_data_t g_ping_data      = {RESET_VALUE,RESET_VALUE ,RESET_VALUE };
#if( ipconfigUSE_DHCP != 0 )
/* DHCP populates these IP address, Sub net mask and Gateway Address. So start with this is zeroed out values
 * The MAC address is Test MAC address.
 */
static uint8_t ucMACAddress[6]      = DEFAULT_MAC_ADDRESS;
static uint8_t ucIPAddress[4]       = DHCP_DEFAULT_VALUE;
static uint8_t ucNetMask[4]         = DHCP_NET_MASK;
static uint8_t ucGatewayAddress[4]  = DHCP_DEFAULT_VALUE;
static uint8_t ucDNSServerAddress[4]= DHCP_DEFAULT_VALUE;
#else
/* Static IP configuration, when DHCP mode is not used for the Example Project.
 * This needs to be populated by the user according to the Network Settings of your LAN.
 * This sample address taken from the LAN where it is tested. This is different for different LAN.
 * get the Address using the PC IPconfig details.
 */
static  uint8_t ucMACAddress[ 6 ]       = {configMAC_ADDR0, configMAC_ADDR1, configMAC_ADDR2, configMAC_ADDR3, configMAC_ADDR4, configMAC_ADDR5};
static  uint8_t ucIPAddress[ 4 ]        = {configIP_ADDR0, configIP_ADDR1, configIP_ADDR2, configIP_ADDR3};
static  uint8_t ucNetMask[ 4 ]          = {configNET_MASK0, configNET_MASK1, configNET_MASK2, configNET_MASK3};
static  uint8_t ucGatewayAddress[ 4 ]   = {configGATEWAY_ADDR0, configGATEWAY_ADDR1, configGATEWAY_ADDR2, configGATEWAY_ADDR3};
static  uint8_t ucDNSServerAddress[ 4 ] = {configDNS_SERVER_ADDR0, configDNS_SERVER_ADDR1, configDNS_SERVER_ADDR2, configDNS_SERVER_ADDR3};
#endif

static uint32_t s_usr_print_ability = RESET_VALUE;

/**********************************************************************************************************************
 * Function Name: ulRand
 * Description  : Generates 32 bit Random number
 * Argument     : void
 * Return Value : Random Number
 *********************************************************************************************************************/
uint32_t ulRand()
{
    /* example of a 32-bit random number generator.
     * Here rand() returns a 15-bit number. so create 32 bit Random number using 15 bit rand()
     */
    uint32_t ulResult = (((uint32_t) rand() /*Casting 15-bit value to 32-bit*/) &0x7fffuL)
                    | (((((uint32_t) rand()) /*Casting 15-bit value to 32-bit*/) &0x7fffuL) << 15)
                    | (((((uint32_t)rand())/*Casting 15-bit value to 32-bit*/)&0x0003uL) << 30);
    return ulResult;
}
/**********************************************************************************************************************
 End of function ulRand
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: ulApplicationGetNextSequenceNumber
 * Description  : Generates 32 sequence number
 * Arguments    : ulSourceAddress
 *              : usSourcePort
 *              : ulDestinationAddress
 *              : usDestinationPort
 * Return Value : Sequence Number
 *********************************************************************************************************************/
uint32_t ulApplicationGetNextSequenceNumber(uint32_t ulSourceAddress, uint16_t usSourcePort,
        uint32_t ulDestinationAddress, uint16_t usDestinationPort)
{
    /* Here we need to get random number for the sequence number.
     * This is just for testing purpose, so software rand() is okay.
     * This can also be tied to the TRNG.
     */

    return ((ulSourceAddress + ulDestinationAddress + usSourcePort + usDestinationPort) && ulRand ());
}
/**********************************************************************************************************************
 End of function ulApplicationGetNextSequenceNumber
 *********************************************************************************************************************/


/**********************************************************************************************************************
 * Function Name: send_ping
 * Description  : Send ICMP Ping request  based on the user input IP Address.
 * Argument     : IP address to Ping
 * Return Value : Sequence Number
 *********************************************************************************************************************/
BaseType_t send_ping(const char *pcIPAddress)
{
    uint32_t ulIPAddress = RESET_VALUE;

    /*
     * The pcIPAddress parameter holds the destination IP address as a string in
     * decimal dot notation (for example, 192.168.0.200). Convert the string into
     * the required 32-bit format.
     */
    ulIPAddress = FreeRTOS_inet_addr (pcIPAddress);

    /*
     * Send a ping request containing 8 data bytes.  Wait (in the Blocked state) a
     * maximum of 100ms for a network buffer into which the generated ping request
     * can be written and sent.
     */
    return (FreeRTOS_SendPingRequest (ulIPAddress, 8, 100 / portTICK_PERIOD_MS));
}
/**********************************************************************************************************************
 End of function send_ping
 *********************************************************************************************************************/


/**********************************************************************************************************************
 * Function Name: vApplicationPingReplyHook
 * Description  : User Hook for the Ping Reply. vApplicationPingReplyHook() is called by the TCP/IP
 *                stack when the stack receives a ping reply.
 * Arguments    : Ping reply status
 *              : usIdentifier
 * Return Value : None
 *********************************************************************************************************************/
void vApplicationPingReplyHook(ePingReplyStatus_t eStatus, uint16_t usIdentifier)
{
    (void) usIdentifier;

    switch (eStatus)
    {
        /* A valid ping reply has been received */
        case eSuccess:
            g_ping_data.received++;
            break;
        case eInvalidData:
        default:
            g_ping_data.lost++;
            break;
    }
}
/**********************************************************************************************************************
 End of function vApplicationPingReplyHook
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: net_thread_entry
 * Description  : main source code
 * Argument     : pvParameters
 * Return Value : None
 *********************************************************************************************************************/
void net_thread_entry(void *pvParameters)
{
    BaseType_t         status   = pdFALSE;

    /* Prints the Ethernet Configuration prior to the IP Init*/
    FreeRTOS_printf( (ETH_PREINIT) );
    print_ipconfig ();

    /* FreeRTOS IP Initialization: This init initializes the IP stack  */
    status = FreeRTOS_IPInit (ucIPAddress, ucNetMask, ucGatewayAddress, ucDNSServerAddress, ucMACAddress);

    if (pdFALSE == status)
    {
        {
            FreeRTOS_printf( ("ERROR: FreeRTOS_IPInit Failed") );
        }
    }

    FreeRTOS_printf( (ETH_POSTINIT) );

    while (pdTRUE)
    {
        /* Check if Both the Ethernet Link and IP link are UP */
        if (is_network_up() == pdFALSE)
        {
            /* s_usr_print_ability is added to avoid multiple UP messages or Down Messages repeating*/
            if (!(PRINT_UP_MSG_DISABLE & s_usr_print_ability))
            {
                FreeRTOS_printf( ("\r\nNetwork is Up") );
                s_usr_print_ability |= PRINT_UP_MSG_DISABLE;
            }

            if (!(PRINT_NWK_USR_MSG_DISABLE & s_usr_print_ability))
            {
#if( ipconfigUSE_DHCP != 0 )
                /* Display the New IP credentials obtained from the DHCP server */
                update_dhcp_response_to_usr ();
#endif
                /* Updated IP credentials on to the RTT console */
                print_ipconfig ();

                /*DNS lookup for the Domain name requested. This is Synchronous Activity */
                dns_querry_func((char *)gp_domain_name);
            }

            if (!(PRINT_NWK_USR_MSG_DISABLE & s_usr_print_ability))
            {
                FreeRTOS_printf( ("\r\nPinging %s:\r\n\r\n", (char*)gp_remote_ip_address) );/* Casting gp_remote_ip_address */
            }

            while (g_usr_ping_count < USR_PING_COUNT)
            {
                /* Send a ICMP Ping request to the requested IP address
                 * USR_PING_COUNT 100 is used in this Example Project
                 * For Continuous testing the count can be increased to bigger number
                 */

                status = send_ping ((char*)gp_remote_ip_address/* Casting to match expected API format */);
                if (pdFALSE != status)
                {
                    g_ping_data.sent++;
                    FreeRTOS_printf( ("sent data \n") );
                }
                else
                {
                    g_ping_data.lost++;
                    FreeRTOS_printf( ("lost data \n") );
                }
                g_usr_ping_count++;

                /* Add some delay between Pings */
                vTaskDelay (10);
            }

            if (!(PRINT_NWK_USR_MSG_DISABLE & s_usr_print_ability))
            {
                print_ping_result ();
                s_usr_print_ability |= PRINT_NWK_USR_MSG_DISABLE;
            }
        }
        else
        {
            if (!(PRINT_DOWN_MSG_DISABLE & s_usr_print_ability))
            {
                FreeRTOS_printf( ("\r\nNetwork is Down\n") );
                s_usr_print_ability |= PRINT_DOWN_MSG_DISABLE;
            }
        }
        vTaskDelay (100);
    }
}
/**********************************************************************************************************************
 End of function net_thread_entry
 *********************************************************************************************************************/


#if( ipconfigUSE_DHCP != 0 )
/**********************************************************************************************************************
 * Function Name: xApplicationDHCPHook
 * Description  : This is the User Hook for the DHCP Response. xApplicationDHCPHook()
 *                is called by DHCP Client Code when DHCP.
 *                handshake messages are exchanged from the Server.
 * Arguments    : Different Phases of DHCP Phases and the Offered IP Address
 * Return Value : Returns DHCP Answers.
 *********************************************************************************************************************/
eDHCPCallbackAnswer_t xApplicationDHCPHook(eDHCPCallbackPhase_t eDHCPPhase, uint32_t ulIPAddress)
{
    eDHCPCallbackAnswer_t eReturn = eDHCPContinue;

    /*This hook is called in a couple of places during the DHCP process, as identified by the eDHCPPhase parameter.*/
    switch (eDHCPPhase)
    {
        case eDHCPPhasePreDiscover:

            /*
             *  A DHCP discovery is about to be sent out.  eDHCPContinue is returned to allow the discovery to go out.
             *  If eDHCPUseDefaults had been returned instead then the DHCP process would be stopped and the statically
             *  configured IP address would be used.
             *  If eDHCPStopNoChanges had been returned instead then the DHCP process would be stopped and whatever the
             *  current network configuration was would continue to be used.
             */
            break;

        case eDHCPPhasePreRequest:

            /* An offer has been received from the DHCP server, and the offered IP address is passed in the ulIPAddress
             * parameter.
             */

            /*
             * The sub-domains do not match, so continue with the DHCP process so the offered IP address is used.
             */
            /* Update the Structure, the DHCP state Machine is not updating this */
            xNetworkAddressing.ulDefaultIPAddress = ulIPAddress;
            g_dhcp_in_use = 1;
            break;

        default:

            /*Cannot be reached, but set eReturn to prevent compiler warnings.*/
            break;
    }

    return eReturn;
}
/**********************************************************************************************************************
 End of function xApplicationDHCPHook
 *********************************************************************************************************************/

#endif /*  ipconfigUSE_DHCP != 0  */

/**********************************************************************************************************************
 * Function Name: print_ping_result
 * Description  : Prints the Ping response on to the RTT console
 * Return Value : None
 *********************************************************************************************************************/
void print_ping_result(void)
{
    FreeRTOS_printf( ("\r\n \r\nPing Statistics for %s :\r\n", (char*)gp_remote_ip_address/* Cast gp_remote_ip_address */) );
    FreeRTOS_printf( ("\r\nPackets: Sent  = %02d, Received = %02d, Lost = %02d \r\n", g_ping_data.sent, g_ping_data.received,
            g_ping_data.lost) );
}
/**********************************************************************************************************************
 End of function print_ping_result
 *********************************************************************************************************************/


/**********************************************************************************************************************
 * Function Name: print_ipconfig
 * Description  : Creates and prints the the IP configuration to display on the RTT console
 * Return Value : None
 *********************************************************************************************************************/
void print_ipconfig(void)
{
#if( ipconfigUSE_DHCP != 0 )
    if (g_dhcp_in_use)
    {
        ucNetMask[3] = (uint8_t)((xNd.ulNetMask & 0xFF000000) >> 24); // Cast to uint8_t for byte extraction
        ucNetMask[2] = (uint8_t)((xNd.ulNetMask & 0x00FF0000) >> 16); // Cast to uint8_t for byte extraction
        ucNetMask[1] = (uint8_t)((xNd.ulNetMask & 0x0000FF00) >> 8);  // Cast to uint8_t for byte extraction
        ucNetMask[0] = (uint8_t)(xNd.ulNetMask & 0x000000FF);         // Cast to uint8_t for byte extraction

        ucGatewayAddress[3] = (uint8_t)((xNd.ulGatewayAddress & 0xFF000000) >> 24); // Cast to uint8_t
        ucGatewayAddress[2] = (uint8_t)((xNd.ulGatewayAddress & 0x00FF0000) >> 16); // Cast to uint8_t
        ucGatewayAddress[1] = (uint8_t)((xNd.ulGatewayAddress & 0x0000FF00) >> 8);  // Cast to uint8_t
        ucGatewayAddress[0] = (uint8_t)(xNd.ulGatewayAddress & 0x000000FF);         // Cast to uint8_t

        ucDNSServerAddress[3] = (uint8_t)((xNd.ulDNSServerAddress & 0xFF000000) >> 24); // Cast to uint8_t
        ucDNSServerAddress[2] = (uint8_t)((xNd.ulDNSServerAddress & 0x00FF0000) >> 16); // Cast to uint8_t
        ucDNSServerAddress[1] = (uint8_t)((xNd.ulDNSServerAddress & 0x0000FF00) >> 8);  // Cast to uint8_t
        ucDNSServerAddress[0] = (uint8_t)(xNd.ulDNSServerAddress & 0x000000FF);         // Cast to uint8_t

        ucIPAddress[3]        = (uint8_t)((xNd.ulDefaultIPAddress & 0xFF000000) >> 24); // Cast to uint8_t
        ucIPAddress[2]        = (uint8_t)((xNd.ulDefaultIPAddress & 0x00FF0000) >> 16); // Cast to uint8_t
        ucIPAddress[1]        = (uint8_t)((xNd.ulDefaultIPAddress & 0x0000FF00) >> 8);  // Cast to uint8_t
        ucIPAddress[0]        = (uint8_t)(xNd.ulDefaultIPAddress & 0x000000FF);         // Cast to uint8_t
    }
#endif
    FreeRTOS_printf( ("\r\nEthernet adapter for Renesas "KIT_NAME":\r\n") );

    FreeRTOS_printf( ("\tDescription . . . . . . . . . . . : Renesas "KIT_NAME" Ethernet\r\n") );
    FreeRTOS_printf( ("\tPhysical Address. . . . . . . . . : %02x-%02x-%02x-%02x-%02x-%02x\r\n", ucMACAddress[0],
            ucMACAddress[1], ucMACAddress[2], ucMACAddress[3], ucMACAddress[4], ucMACAddress[5]) );
    FreeRTOS_printf( ("\tDHCP Enabled. . . . . . . . . . . : %s\r\n", g_dhcp_in_use ? "Yes" : "No") );
    FreeRTOS_printf( ("\tIPv4 Address. . . . . . . . . . . : %d.%d.%d.%d\r\n",
        ucIPAddress[0], ucIPAddress[1], ucIPAddress[2], ucIPAddress[3]) );
    FreeRTOS_printf( ("\tSubnet Mask . . . . . . . . . . . : %d.%d.%d.%d\r\n", ucNetMask[0], ucNetMask[1], ucNetMask[2],
            ucNetMask[3]) );
    FreeRTOS_printf( ("\tDefault Gateway . . . . . . . . . : %d.%d.%d.%d\r\n", ucGatewayAddress[0], ucGatewayAddress[1],
            ucGatewayAddress[2], ucGatewayAddress[3]) );
    FreeRTOS_printf( ("\tDNS Servers . . . . . . . . . . . : %d.%d.%d.%d\r\n", ucDNSServerAddress[0], ucDNSServerAddress[1],
            ucDNSServerAddress[2], ucDNSServerAddress[3]) );
}
/**********************************************************************************************************************
 End of function print_ipconfig
 *********************************************************************************************************************/


/**********************************************************************************************************************
 * Function Name: dns_querry_func
 * Description  : DNS Query for the requested Domain name.
 *                Uses the FreeRTOS Client API FreeRTOS_gethostbyname to get the IP address for the domain name.
 * Argument     : domain
 * Return Value : None
 *********************************************************************************************************************/
void dns_querry_func(char *domain)
{
    uint32_t ulIPAddress = RESET_VALUE;
    int8_t cBuffer[16]   = { RESET_VALUE };

    /* Lookup the IP address of the FreeRTOS.org website. */
    ulIPAddress = FreeRTOS_gethostbyname ((char*)domain);
    if (0 != ulIPAddress)
    {
        /* Convert the IP address to a string. */
        FreeRTOS_inet_ntoa (ulIPAddress, (char*)cBuffer);

        /* Print out the IP address obtained from the DNS lookup. */
        FreeRTOS_printf(("\r\nDNS Lookup for %s is      : %s  \r\n",(char*)gp_domain_name, cBuffer));
    }
    else
    {
        printf("\r\nDNS Lookup failed for %s \r\n", (char*)gp_domain_name);
    }
}
/**********************************************************************************************************************
 End of function dns_querry_func
 *********************************************************************************************************************/

/******************************************************************************
 * Function Name: is_network_up
 * Description  : This Function checks the Network status (Both Ethernet and IP Layer). If the Network is down,
 *                the Application will not send any data on the network.
 * Return Value : Network Status
 *****************************************************************************/
uint32_t is_network_up(void)
{
	BaseType_t  eth_link_status = pdTRUE;
    BaseType_t networkUp       = pdFALSE;
    uint32_t   network_status  = (IP_LINK_UP | ETHERNET_LINK_UP);

    networkUp       = FreeRTOS_IsNetworkUp();
#ifndef LinkProcess_REVISED
    eth_link_status = R_ETHER_LinkProcess((uint32_t)0);
#else
    R_ETHER_LinkProcess((uint32_t)0);
    if (link_detect[ETHER_CHANNEL_0] != ETHER_FLAG_ON_LINK_ON)
    {
    	eth_link_status = pdFALSE;
    }
#endif

    if (pdTRUE == networkUp)
    {
        return network_status;
    }
    else
    {
        if (pdTRUE != eth_link_status)
        {
            network_status |= ETHERNET_LINK_DOWN;
        }
        else if (pdTRUE == eth_link_status)
        {
            network_status |= ETHERNET_LINK_UP;
        }
        else
        {
            portNOP();
        }

        if (pdTRUE != networkUp)
        {
            network_status |= IP_LINK_DOWN;
        }
        else if (pdTRUE == networkUp)
        {
            network_status |= IP_LINK_UP;
        }
        else
        {
            portNOP();
        }
        return network_status;
    }
}
/**********************************************************************************************************************
 End of function is_network_up
 *********************************************************************************************************************/


#if( ipconfigUSE_DHCP != 0 )
/**********************************************************************************************************************
 * Function Name: update_dhcp_response_to_usr
 * Description  : Update the DHCP info to the User data structure.
 * Return Value : None
 *********************************************************************************************************************/
void update_dhcp_response_to_usr(void)
{
    if (g_dhcp_in_use)
    {
        memcpy (&xNd, &xNetworkAddressing, sizeof(xNd));
    }
}
/**********************************************************************************************************************
 End of function update_dhcp_response_to_usr
 *********************************************************************************************************************/
#endif

#if( ipconfigDHCP_REGISTER_HOSTNAME == 1 )
/******************************************************************************
 * Function Name: pcApplicationHostnameHook
 * Description  : DHCP Hook function to populate the user defined Host name for the Kit.
 * Return Value : Hostname
 *****************************************************************************/
const char* pcApplicationHostnameHook(void)
{
    return KIT_NAME;
}
/**********************************************************************************************************************
 End of function pcApplicationHostnameHook
 *********************************************************************************************************************/


#endif
