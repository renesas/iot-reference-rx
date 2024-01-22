/*
 * FreeRTOS Wi-Fi V1.0.0
 * Copyright (C) 2020 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
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
 *
 * http://aws.amazon.com/freertos
 * http://www.FreeRTOS.org
 */

/**
 * @file iot_wifi.c
 * @brief Wi-Fi Interface.
 */
#include <stdio.h>
#include <string.h>
/* FreeRTOS includes. */
#include "FreeRTOS.h"
#include "iot_wifi.h"

/* Wi-Fi configuration includes. */
#include "iot_wifi_config.h"

/* WiFi configuration includes. */
#include "platform.h"
#include "r_sci_rx_if.h"
#include "r_wifi_da16xxx_if.h"

/**
 * @brief Wi-Fi initialization status.
 */
static uint32_t prvConvertSecurityFromDaAT( WIFISecurity_t xSecurity );
static uint32_t prvConvertEncryptionFromDaAT( WIFIEncryption_t xEncryption );

static uint32_t prvConvertSecurityFromDaAT( WIFISecurity_t xSecurity )
{
    uint32_t xConvertedSecurityType = WIFI_SECURITY_UNDEFINED;

    switch( xSecurity )
    {
        case eWiFiSecurityOpen:
            xConvertedSecurityType = WIFI_SECURITY_OPEN;
            break;

        case eWiFiSecurityWEP:
            xConvertedSecurityType = WIFI_SECURITY_WEP;
            break;

        case eWiFiSecurityWPA:
            xConvertedSecurityType = WIFI_SECURITY_WPA;
            break;

        case eWiFiSecurityWPA2:
            xConvertedSecurityType = WIFI_SECURITY_WPA2;
            break;

        case eWiFiSecurityNotSupported:
            xConvertedSecurityType = WIFI_SECURITY_UNDEFINED;
            break;
    }

    return xConvertedSecurityType;
}
/*-----------------------------------------------------------*/

static uint32_t prvConvertEncryptionFromDaAT( WIFIEncryption_t xEncryption )
{
    uint32_t xConvertedEncryptionType = WIFI_ENCRYPTION_UNDEFINED;

    switch( xEncryption )
    {
        case eWiFiEncryptionTKIP:
        	xConvertedEncryptionType = WIFI_ENCRYPTION_TKIP;
            break;

        case eWiFiEncryptionAES:
        	xConvertedEncryptionType = WIFI_ENCRYPTION_AES;
            break;

        case eWiFiEncryptionTKIP_AES:
        	xConvertedEncryptionType = WIFI_ENCRYPTION_TKIP_AES;
            break;

        case eWiFiEncryptionUndefined:
        	xConvertedEncryptionType = WIFI_ENCRYPTION_UNDEFINED;
            break;
    }

    return xConvertedEncryptionType;
}
/*-----------------------------------------------------------*/

WIFIReturnCode_t WIFI_On( void )
{
    /* FIX ME. */
    WIFIReturnCode_t xRetVal = eWiFiFailure;

    if(WIFI_SUCCESS == R_WIFI_DA16XXX_Open())
    {
    	xRetVal = eWiFiSuccess;
    }

	return xRetVal;
}
/*-----------------------------------------------------------*/

WIFIReturnCode_t WIFI_Off( void )
{
    /* FIX ME. */
	R_WIFI_DA16XXX_Close();

    return eWiFiSuccess;
}
/*-----------------------------------------------------------*/

WIFIReturnCode_t WIFI_ConnectAP( const WIFINetworkParams_t * const pxNetworkParams )
{
	wifi_err_t ret;
	wifi_ip_configuration_t ipconfig;
	uint8_t dhcp_enable;
	wifi_security_t convert_security;
	uint32_t convert_encryption;

	WIFIScanResult_t * pxBuffer_scan;

	convert_security = (wifi_security_t) prvConvertSecurityFromDaAT(pxNetworkParams->xSecurity);

	if( (pxNetworkParams == NULL) || (pxNetworkParams->ucSSIDLength == 0 ) || ( pxNetworkParams->ucSSIDLength > wificonfigMAX_SSID_LEN ) )
	{
		return eWiFiFailure;
	}

	if( (pxNetworkParams->xSecurity != eWiFiSecurityOpen) && (pxNetworkParams->xPassword.xWPA.ucLength == 0) )
	{
		return eWiFiFailure;
	}

    if ( pxNetworkParams->xPassword.xWPA.ucLength > wificonfigMAX_PASSPHRASE_LEN )
    {
        return eWiFiFailure;
    }

    if(pdTRUE == WIFI_IsConnected(pxNetworkParams))
    {
    	WIFI_Disconnect();
    }

#if	(ipconfigUSE_DHCP == 0)
	dhcp_enable = 0;
	ipconfig.ipaddress[0] = configIP_ADDR0;
	ipconfig.ipaddress[1] = configIP_ADDR1;
	ipconfig.ipaddress[2] = configIP_ADDR2;
	ipconfig.ipaddress[3] = configIP_ADDR3;
	ipconfig.subnetmask[0] = configNET_MASK0;
	ipconfig.subnetmask[1] = configNET_MASK1;
	ipconfig.subnetmask[2] = configNET_MASK2;
	ipconfig.subnetmask[3] = configNET_MASK3;
	ipconfig.gateway[0] = configGATEWAY_ADDR0;
	ipconfig.gateway[1] = configGATEWAY_ADDR1;
	ipconfig.gateway[2] = configGATEWAY_ADDR2;
	ipconfig.gateway[3] = configGATEWAY_ADDR3;
#else
	dhcp_enable = 1;
#endif

	//work around to fix the bug when memcopy ssid and passphrase
	uint8_t ssid[ wificonfigMAX_SSID_LEN ] = { 0 };
	uint8_t password[ wificonfigMAX_PASSPHRASE_LEN ] = { 0 };
	memcpy( ssid, pxNetworkParams->ucSSID, pxNetworkParams->ucSSIDLength );
	memcpy( password, pxNetworkParams->xPassword.xWPA.cPassphrase, pxNetworkParams->xPassword.xWPA.ucLength );

	/* FIX ME. */
	ret = R_WIFI_DA16XXX_Connect (
			ssid,
			password,
			convert_security,
			WIFI_ENCRYPTION_AES
	);

	if(WIFI_SUCCESS != ret)
	{
		return eWiFiFailure;
	}


	return eWiFiSuccess;
}
/*-----------------------------------------------------------*/

WIFIReturnCode_t WIFI_Disconnect( void )
{
    /* FIX ME. */
	R_WIFI_DA16XXX_Disconnect();
    return eWiFiSuccess;
}
/*-----------------------------------------------------------*/

WIFIReturnCode_t WIFI_Reset( void )
{
    /* FIX ME. */
	WIFIReturnCode_t ret;

    WIFI_Off();
    ret = WIFI_On();
    return ret;
}
/*-----------------------------------------------------------*/

WIFIReturnCode_t WIFI_Scan( WIFIScanResult_t * pxBuffer,
                            uint8_t ucNumNetworks )
{
    WIFIReturnCode_t result = eWiFiFailure;
    wifi_err_t ret;

	ret = R_WIFI_DA16XXX_Scan(pxBuffer, ucNumNetworks);
	if(WIFI_SUCCESS == ret)
	{
		result = eWiFiSuccess;
	}
    return result;
}
/*-----------------------------------------------------------*/

WIFIReturnCode_t WIFI_SetMode( WIFIDeviceMode_t xDeviceMode )
{
    /* FIX ME. */
    return eWiFiNotSupported;
}
/*-----------------------------------------------------------*/

WIFIReturnCode_t WIFI_GetMode( WIFIDeviceMode_t * pxDeviceMode )
{
    /* FIX ME. */
    return eWiFiNotSupported;
}
/*-----------------------------------------------------------*/

WIFIReturnCode_t WIFI_NetworkAdd( const WIFINetworkProfile_t * const pxNetworkProfile,
                                  uint16_t * pusIndex )
{
    /* FIX ME. */
    return eWiFiNotSupported;
}
/*-----------------------------------------------------------*/

WIFIReturnCode_t WIFI_NetworkGet( WIFINetworkProfile_t * pxNetworkProfile,
                                  uint16_t usIndex )
{
    /* FIX ME. */
    return eWiFiNotSupported;
}
/*-----------------------------------------------------------*/

WIFIReturnCode_t WIFI_NetworkDelete( uint16_t usIndex )
{
    /* FIX ME. */
    return eWiFiNotSupported;
}
/*-----------------------------------------------------------*/

WIFIReturnCode_t WIFI_Ping( uint8_t * pucIPAddr,
                            uint16_t usCount )
{
    /* FIX ME. */
    WIFIReturnCode_t result = eWiFiFailure;
    wifi_err_t ret;

	ret = R_WIFI_DA16XXX_Ping(pucIPAddr, usCount);
	if(WIFI_SUCCESS == ret)
	{
		result = eWiFiSuccess;
	}
    return result;
}
/*-----------------------------------------------------------*/

WIFIReturnCode_t WIFI_GetIPInfo( WIFIIPConfiguration_t * pxIPConfig )
{
    /* FIX ME. */
	wifi_ip_configuration_t ipconfig;

	if(NULL == pxIPConfig)
	{
		return eWiFiFailure;
	}
	if(WIFI_SUCCESS != R_WIFI_DA16XXX_GetIpAddress(&ipconfig))
	{
		return eWiFiFailure;
	}

	memset( pxIPConfig, 0x00, sizeof( WIFIIPConfiguration_t ) );

	pxIPConfig->xIPAddress.ulAddress[0] = (uint32_t)ipconfig.ipaddress[0];
	pxIPConfig->xIPAddress.ulAddress[1] = (uint32_t)ipconfig.ipaddress[1];
	pxIPConfig->xIPAddress.ulAddress[2] = (uint32_t)ipconfig.ipaddress[2];
	pxIPConfig->xIPAddress.ulAddress[3] = (uint32_t)ipconfig.ipaddress[3];
	pxIPConfig->xGateway.ulAddress[0] = (uint32_t)ipconfig.gateway[0];
	pxIPConfig->xGateway.ulAddress[1] = (uint32_t)ipconfig.gateway[1];
	pxIPConfig->xGateway.ulAddress[2] = (uint32_t)ipconfig.gateway[2];
	pxIPConfig->xGateway.ulAddress[3] = (uint32_t)ipconfig.gateway[3];
	pxIPConfig->xNetMask.ulAddress[0] = (uint32_t)ipconfig.subnetmask[0];
	pxIPConfig->xNetMask.ulAddress[1] = (uint32_t)ipconfig.subnetmask[1];
	pxIPConfig->xNetMask.ulAddress[2] = (uint32_t)ipconfig.subnetmask[2];
	pxIPConfig->xNetMask.ulAddress[3] = (uint32_t)ipconfig.subnetmask[3];
	return eWiFiSuccess;
}
/*-----------------------------------------------------------*/

WIFIReturnCode_t WIFI_GetMAC( uint8_t * pucMac )
{
    /* FIX ME. */
	if(WIFI_SUCCESS != R_WIFI_DA16XXX_GetMacAddress(pucMac))
	{
		return eWiFiFailure;
	}
    return eWiFiSuccess;
}
/*-----------------------------------------------------------*/

WIFIReturnCode_t WIFI_GetHostIP( char * pcHost,
                                 uint8_t * pucIPAddr )
{
    /* FIX ME. */
	uint8_t ipAddress[4] = { 0, 0, 0, 0 };

	if(WIFI_SUCCESS != R_WIFI_DA16XXX_DnsQuery((uint8_t*) pcHost, ipAddress))
	{
		return eWiFiFailure;
	}
	pucIPAddr[0] = ipAddress[0];
	pucIPAddr[1] = ipAddress[1];
	pucIPAddr[2] = ipAddress[2];
	pucIPAddr[3] = ipAddress[3];

    return eWiFiSuccess;
}
/*-----------------------------------------------------------*/

WIFIReturnCode_t WIFI_StartAP( void )
{
    /* FIX ME. */
    return eWiFiNotSupported;
}
/*-----------------------------------------------------------*/

WIFIReturnCode_t WIFI_StopAP( void )
{
    /* FIX ME. */
    return eWiFiNotSupported;
}
/*-----------------------------------------------------------*/

WIFIReturnCode_t WIFI_ConfigureAP( const WIFINetworkParams_t * const pxNetworkParams )
{
    /* FIX ME. */
    return eWiFiNotSupported;
}
/*-----------------------------------------------------------*/

WIFIReturnCode_t WIFI_SetPMMode( WIFIPMMode_t xPMModeType,
                                 const void * pvOptionValue )
{
    /* FIX ME. */
    return eWiFiNotSupported;
}
/*-----------------------------------------------------------*/

WIFIReturnCode_t WIFI_GetPMMode( WIFIPMMode_t * pxPMModeType,
                                 void * pvOptionValue )
{
    /* FIX ME. */
    return eWiFiNotSupported;
}
/*-----------------------------------------------------------*/

BaseType_t WIFI_IsConnected( const WIFINetworkParams_t * pxNetworkParams )
{
	BaseType_t ret = pdFALSE;
	/* FIX ME. */
	if(0 ==  R_WIFI_DA16XXX_IsConnected())
	{
		ret = pdTRUE;
	}
	return ret;
}

WIFIReturnCode_t WIFI_RegisterEvent( WIFIEventType_t xEventType, WIFIEventHandler_t xHandler )
{
    /** Needs to implement dispatching network state change events **/
    return eWiFiNotSupported;
}

WIFIReturnCode_t WIFI_GetCapability( WIFICapabilityInfo_t * pxCaps )
{
	return eWiFiNotSupported;
}

WIFIReturnCode_t WIFI_GetCountryCode( char * pcCountryCode )
{
	return eWiFiNotSupported;
}

WIFIReturnCode_t WIFI_GetRSSI( int8_t * pcRSSI )
{
	return eWiFiNotSupported;
}

WIFIReturnCode_t WIFI_GetScanResults( const WIFIScanResult_t ** pxBuffer,
                                      uint16_t * ucNumNetworks )
{
	return eWiFiNotSupported;
}

WIFIReturnCode_t WIFI_GetStationList( WIFIStationInfo_t * pxStationList,
                                      uint8_t * pcStationListSize )
{
	return eWiFiNotSupported;
}

WIFIReturnCode_t WIFI_GetStatistic( WIFIStatisticInfo_t * pxStats )
{
	return eWiFiNotSupported;
}

WIFIReturnCode_t WIFI_SetCountryCode( const char * pcCountryCode )
{
	return eWiFiNotSupported;
}

WIFIReturnCode_t WIFI_StartConnectAP( const WIFINetworkParams_t * pxNetworkParams )
{
	return eWiFiNotSupported;
}

WIFIReturnCode_t WIFI_StartDisconnect( void )
{
	return eWiFiNotSupported;
}

WIFIReturnCode_t WIFI_StartScan( WIFIScanConfig_t * pxScanConfig )
{
	return eWiFiNotSupported;
}

