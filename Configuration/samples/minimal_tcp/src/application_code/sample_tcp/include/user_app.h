/***********************************************************************************************************************
 * File Name    : user_app.h
 * Description  : Contains macros, data structures and functions defined for user application.
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
 * Copyright (C) 2023 Renesas Electronics Corporation. All rights reserved.
 ***********************************************************************************************************************/

#ifndef SRC_APPLICATION_CODE_INCLUDE_USER_APP_H_
#define SRC_APPLICATION_CODE_INCLUDE_USER_APP_H_

/* generic headers */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* Initialization values */
#define RESET_VALUE  (0x00)

/* Board and version details */
#define KIT_NAME     "CK-RX65N"
#define MODULE_NAME  "Ethernet using FreeRTOS+TCP"

#define ETH_PREINIT  "\r\n \r\n--------------------------------------------------------------------------------"\
    "\r\nEthernet adapter Configuration for Renesas "KIT_NAME": Pre IP Init       "\
    "\r\n--------------------------------------------------------------------------------\r\n\r\n"

#define ETH_POSTINIT "\r\n \r\n--------------------------------------------------------------------------------"\
    "\r\nEthernet adapter Configuration for Renesas "KIT_NAME": Post IP Init       "\
    "\r\n--------------------------------------------------------------------------------\r\n\r\n"

#if ( ipconfigUSE_DHCP != 0)
#include "FreeRTOS_DHCP.h"
#endif

#define USR_TEST_DOMAIN_NAME         "www.freertos.org"
#define USR_TEST_PING_IP             "142.251.42.142"
#define USR_PING_COUNT               (100)

#define SUCCESS                      (0)
#define PRINT_UP_MSG_DISABLE         (0x01)
#define PRINT_DOWN_MSG_DISABLE       (0x02)
#define PRINT_NWK_USR_MSG_DISABLE    (0x04)

#define ETHERNET_LINK_DOWN           (0x01)
#define ETHERNET_LINK_UP             (0x00)
#define IP_LINK_DOWN                 (0x02)
#define IP_LINK_UP                   (0x00)

#define RESET_VALUE                  (0x00)
#define ETHERNET_RESET_VALUE         (0)
#define DEFAULT_MAC_ADDRESS          {configMAC_ADDR0, configMAC_ADDR1, configMAC_ADDR2, configMAC_ADDR3, configMAC_ADDR4, configMAC_ADDR5}

#define DHCP_DEFAULT_VALUE           {ETHERNET_RESET_VALUE}
#define DHCP_NET_MASK                {configNET_MASK0, configNET_MASK1, configNET_MASK2, configNET_MASK3}

typedef struct st_ping_data
{
    uint32_t sent;     // Ping Request
    uint32_t received; // Ping Response
    uint32_t lost;     // Ping failure
} ping_data_t;

/**********************************************************************************************************************
 * Function Name: is_network_up
 * Description  : Checks if the network is up.
 * Return Value : Returns a non-zero value if the network is up, otherwise returns zero.
 *********************************************************************************************************************/
uint32_t is_network_up (void);

/**********************************************************************************************************************
 * Function Name: send_ping
 * Description  : Sends a ping to the given IP address.
 * Arguments    : pcIPAddress - IP address to ping.
 * Return Value : pdPASS if successful, pdFAIL otherwise.
 *********************************************************************************************************************/
BaseType_t send_ping (const char *  pcIPAddress);


/**********************************************************************************************************************
 * Function Name: print_ipconfig
 * Description  : Prints the IP configuration.
 * Return Value : None.
 *********************************************************************************************************************/
void print_ipconfig (void);

/**********************************************************************************************************************
 * Function Name: print_ping_result
 * Description  : Prints the result of the last ping operation.
 * Return Value : None.
 *********************************************************************************************************************/
void print_ping_result (void);

/**********************************************************************************************************************
 * Function Name: dns_querry_func
 * Description  : Queries the DNS for the IP address associated with the given domain name.
 * Arguments    : gp_domain_name - Domain name to query.
 * Return Value : None.
 *********************************************************************************************************************/
void dns_querry_func (char *  gp_domain_name);

#if( ipconfigUSE_DHCP != 0 )
/**********************************************************************************************************************
 * Function Name: update_dhcp_response_to_usr
 * Description  : Update the DHCP info to the User data structure.
 * Return Value : None
 *********************************************************************************************************************/
void update_dhcp_response_to_usr (void);

#endif


#endif /* SRC_APPLICATION_CODE_INCLUDE_USER_APP_H_ */
