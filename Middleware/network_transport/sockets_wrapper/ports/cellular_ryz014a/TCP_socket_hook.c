/*
* Copyright (c) 2023-2025 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/

/**********************************************************************************************************************
 * File Name    : TCP_socket_hook.c
* Description   : TCP socket error handling and connection management hooks for cellular modules.
 *********************************************************************************************************************/
/**********************************************************************************************************************
 * History : DD.MM.YYYY Version Description
 *         : 29.12.2019 1.00 First Release
 *********************************************************************************************************************/

/******************************************************************************
 Includes   <System Includes> , "Project Includes"
 ******************************************************************************/

/* for using C standard library */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* for using FIT Module */
#include "platform.h"
#include "r_cellular_if.h"
#include "user_tcp_hook_config.h"
/* for using Amazon FreeRTOS */
/* Logging related header files are required to be included in the following order:
 * 1. Include the header file "logging_levels.h".
 * 2. Define LIBRARY_LOG_NAME and  LIBRARY_LOG_LEVEL.
 * 3. Include the header file "logging_stack.h".
 */

/* Include header that defines log levels. */
#include "logging_levels.h"

/* Logging configuration for the Sockets. */
#ifndef LIBRARY_LOG_NAME
    #define LIBRARY_LOG_NAME     "Sockets_Hook"
#endif
#ifndef LIBRARY_LOG_LEVEL
    #define LIBRARY_LOG_LEVEL    LOG_INFO
#endif

extern void vLoggingPrintf( const char * pcFormatString,... );

#include "logging_stack.h"

extern st_cellular_ctrl_t cellular_ctrl;
bool Connect2AP(void);
void CloseSocket(uint32_t socket_number);
e_cellular_err_t SocketErrorHook( e_cellular_err_t error, bool force_reset );

/**
 * @brief Band Select for Cellular connecting.
 * This is used for setting the band for cellular.
 * You can select below bands. Bands is related on Cellular carrier,
 * please must set bands your region and SIM acceptable.
 * ------------------------------------------------------
 * North America 2,4,5,12,13,25
 * EMEA 1,3,8,20,28
 * Japan 1,8,18,19,26
 * Australia 1,3,8,28
 * ------------------------------------------------------
 */
static const uint8_t * const CELLULAR_BAND_CONFIG = "1,2,4,5,8,12,13,14,17,18,19,20,25,26,28,66";

#if 0 == USER_TCP_HOOK_ENABLED

e_cellular_err_t SocketErrorHook( e_cellular_err_t error, bool force_reset )
{
	(void) force_reset;
	return error;
}

#else

extern volatile uint32_t count_module_comm;

/* This function resets cellular hardware when error and closes cellular module to reconnect to AP */
e_cellular_err_t SocketErrorHook( e_cellular_err_t error, bool force_reset )
{
	if((CELLULAR_ERR_MODULE_TIMEOUT == error) || (CELLULAR_ERR_MODULE_COM == error))
	{
		if (1 == force_reset)
		{
			LogInfo(("Start force reset Cellular Hardware due to error = %d \r\n", error));
			LogInfo(("Resetting Cellular Hardware \r\n"));
			R_CELLULAR_HardwareReset(&cellular_ctrl);
			LogInfo(("Cellular Hardware Reset Done!\r\n"));
			R_CELLULAR_Close(&cellular_ctrl);
			Connect2AP();
			return error;
		}
		else
		{
			if (CELLULAR_ERR_MODULE_COM == error)
			{
				count_module_comm++;
				LogInfo(("Cellular has error = %d in %d times \r\n ", CELLULAR_ERR_MODULE_COM, count_module_comm));
				if ( USER_COMM_ERROR_TRIES > count_module_comm )
				{
					return 0;
				}
			}

			if (( USER_COMM_ERROR_TRIES <= count_module_comm ) || (CELLULAR_ERR_MODULE_COM != error))
			{

				if ( USER_COMM_ERROR_TRIES <= count_module_comm )
				{
					LogInfo(("Start no force reset Cellular Hardware due to the continuation of %d error \r\n", error));
				}

				if ( CELLULAR_ERR_MODULE_COM != error )
				{
					LogInfo(("Start no force reset Cellular Hardware due to error = %d \r\n", error));
				}
				LogInfo(("Resetting Cellular Hardware \r\n"));

				count_module_comm = 0;
				R_CELLULAR_HardwareReset(&cellular_ctrl);
				LogInfo(("Cellular Hardware Reset Done!\r\n"));
				R_CELLULAR_Close(&cellular_ctrl);
				Connect2AP();
			}
			return error;
		}
	}
	else
	{
		count_module_comm = 0;
		return error;
	}

}
#endif

/* This function helps cellular module connect to AP */
bool Connect2AP( void )
{
	uint32_t countAP = 0;
	e_cellular_err_t ret = CELLULAR_SUCCESS;
	do
	{
		countAP++;
		LogInfo(("Tried to connect %d times \r\n",countAP ));


		ret = R_CELLULAR_Open(&cellular_ctrl, NULL);

		if (CELLULAR_SUCCESS == ret)
		{
			ret = R_CELLULAR_SetOperator(&cellular_ctrl, "standard");

		}
		else
		{
			LogInfo(("Cellular Open Failed with %d \r\n", ret ));
			return  (CELLULAR_SUCCESS == ret);
		}
		if(CELLULAR_SUCCESS == ret )
		{
			/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!  */
			/* !! Please must set your cellular band not to connect band that not support your region  !!  */
			/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!  */
			/* Show band setting for cellular */
			LogInfo(("Set the band of Cellular. Set bands %s \r\n", CELLULAR_BAND_CONFIG ));
			/* Set cellular bands */
			ret = R_CELLULAR_SetBand(&cellular_ctrl, CELLULAR_BAND_CONFIG);

		}
		else
		{
			LogInfo(("Set SetOperator Failed with %d \r\n", ret ));
			return  (CELLULAR_SUCCESS == ret);
		}

		if(CELLULAR_SUCCESS == ret )
		{
			LogInfo(("Connecting to AccessPoint... \r\n"));
			LogInfo(("It takes around 3 or 5 minute when you connect to it for the first time. \r\n "));

			ret = R_CELLULAR_APConnect(&cellular_ctrl, NULL);
		}
		else
		{
			LogInfo(("Set band Failed with %d \r\n", ret ));
			return  (CELLULAR_SUCCESS == ret);
		}

		if(CELLULAR_SUCCESS == ret)
		{
			LogInfo(("Connected to AccessPoint \r\n "));
			return  (CELLULAR_SUCCESS == ret);
		}
		else
		{
			LogInfo(("It has error = %d when connecting \r\n",ret));

/***********************************************************************************************/
/* If you do not need the reset process, comment out the following */
			if (CELLULAR_ERR_AP_CONNECT_FAILED == ret)
			{
				LogInfo(("Start resetting Cellular Hardware due to error = %d in Connect2AP\r\n", ret));
				LogInfo(("Resetting Cellular Hardware \r\n"));
				R_CELLULAR_HardwareReset(&cellular_ctrl);
				R_CELLULAR_Close(&cellular_ctrl);
				/*Wait 10ms before going to R_CELLULAR_OPEN */
				vTaskDelay(10);
			}
			else
			{
				return  (CELLULAR_SUCCESS == ret);
			}
/* If you do not need the reset process, comment out the above. */
/***********************************************************************************************/
		}

	}
	while( USER_RECONNECT_TRIES > countAP);

	if (USER_RECONNECT_TRIES <= countAP)
	{
		LogInfo(("Tried to connect %d times, but FAILED \r\n",countAP ));
	}

	return  (CELLULAR_SUCCESS == ret);
}

void CloseSocket(uint32_t socket_number)
{
	int32_t count = 0;
	e_cellular_err_t ret = CELLULAR_SUCCESS;
	while (USER_CLOSE_SOCKET_TRIES > count)
	{
		ret = R_CELLULAR_CloseSocket(&cellular_ctrl,socket_number);

		if ((CELLULAR_SUCCESS == ret) || (CELLULAR_ERR_NOT_OPEN == ret) || (CELLULAR_ERR_SOCKET_NOT_READY == ret))
		{
			break;
		}
		count++;
		LogInfo( ( "Try to close in %d times.",count ) );
	}

	if ((CELLULAR_SUCCESS == ret) || (CELLULAR_ERR_NOT_OPEN == ret) || (CELLULAR_ERR_SOCKET_NOT_READY == ret))
	{
		LogInfo( ( "Closed Socket: Socket Number = %d.",socket_number ) );
	}
	else
	{
		LogInfo( ( "Try to close but failed to close Socket: Socket Number = %d with %d.",socket_number,ret ) );

		/**************************
		 * Support resetting the cellular module if multiple attempts to close the socket fail.
		 * Please remove the comment out to enable this feature.
		 ****************************/
		/*
		LogInfo(("Start resetting Cellular Hardware due to error = %d in CloseSocket\r\n", ret));
		(void)R_CELLULAR_HardwareReset(&cellular_ctrl);
		(void)R_CELLULAR_Close(&cellular_ctrl);
		vTaskDelay(10);
		Connect2AP();
		*/
	}
}

/******************************************************************************
 End  Of File
 ******************************************************************************/

