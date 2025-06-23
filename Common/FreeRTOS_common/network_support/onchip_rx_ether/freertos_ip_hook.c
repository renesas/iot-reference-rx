/*
* Copyright (c) 2023-2025 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/

/***********************************************************************************************************************
 * File Name    : freertos_ip_hook.c
 * Description  : This file provides the implementation of network event hook functions for FreeRTOS+TCP.
 *                It handles network up/down events and prints network configuration details.
 **********************************************************************************************************************/

/**********************************************************************************************************************
 Includes   <System Includes> , "Project Includes"
 *********************************************************************************************************************/
#include <stdio.h>
#include "FreeRTOS.h"
#include "task.h"
#include "FreeRTOS_IP.h"
#include "FreeRTOS_Sockets.h"

/******************************************************************************
* Function Name: vApplicationIPNetworkEventHook
* Description  : This function will be called on each network up/down event.
* Arguments    : eNetworkEvent -
*                    Type of network up/down event
* Return Value : None.
******************************************************************************/
void vApplicationIPNetworkEventHook(eIPCallbackEvent_t eNetworkEvent)
{
    uint32_t ulIPAddress;
    uint32_t ulNetMask;
    uint32_t ulGatewayAddress;
    uint32_t ulDNSServerAddress;
    char cBuffer[16];

    /* If the network has just come up...*/
    if (eNetworkUp == eNetworkEvent)
    {
        /* Print out the network configuration, which may have come from a DHCP
         * server. */
        FreeRTOS_GetAddressConfiguration(
            &ulIPAddress,
            &ulNetMask,
            &ulGatewayAddress,
            &ulDNSServerAddress );

        FreeRTOS_inet_ntoa(ulIPAddress, cBuffer);
        FreeRTOS_printf(("IP Address: %s\n", cBuffer));

        FreeRTOS_inet_ntoa(ulNetMask, cBuffer);
        FreeRTOS_printf(("Subnet Mask: %s\n", cBuffer));

        FreeRTOS_inet_ntoa(ulGatewayAddress, cBuffer);
        FreeRTOS_printf(("Gateway Address: %s\n", cBuffer));

        FreeRTOS_inet_ntoa(ulDNSServerAddress, cBuffer);
        FreeRTOS_printf(("DNS Server Address: %s\n", cBuffer));
    }
}
/*****************************************************************************************
End of function vApplicationIPNetworkEventHook
****************************************************************************************/
