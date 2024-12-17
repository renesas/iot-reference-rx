/*
 * freertos_ip_hook.c v1.0.0
 * Copyright (C) Renesas Electronics Corporation and/or its affiliates.
 * All Rights Reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

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
