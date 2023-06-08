/*
 * FreeRTOS V202211.00
 * Copyright (C) 2017 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 * Modifications Copyright (C) 2023 Renesas Electronics Corporation. or its affiliates.
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
 * https://www.FreeRTOS.org
 * https://aws.amazon.com/freertos
 *
 */


 /******************************************************************************
 *
 * https://www.FreeRTOS.org/cli
 *
 ******************************************************************************/


/* FreeRTOS includes. */
#include "FreeRTOS.h"
#include "task.h"

/* Standard includes. */
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* FreeRTOS+CLI includes. */
#include "FreeRTOS_CLI.h"

#include "platform.h"
#include "store.h"
#include "common_data.h"
/* Key provisioning include. */
#ifdef __TEST__
#include "dev_mode_key_provisioning.h"
#else
#include "aws_dev_mode_key_provisioning.h"
#endif
extern volatile uint32_t pvwrite;
#ifndef  configINCLUDE_TRACE_RELATED_CLI_COMMANDS
	#define configINCLUDE_TRACE_RELATED_CLI_COMMANDS 0
#endif

#ifndef configINCLUDE_QUERY_HEAP_COMMAND
	#define configINCLUDE_QUERY_HEAP_COMMAND 0
#endif


static BaseType_t prvWaitMessage( char * pcWriteBuffer,
										   size_t xWriteBufferLen,
										   const char * pcCommandString );

static BaseType_t prvFormat( char * pcWriteBuffer,
										   size_t xWriteBufferLen,
										   const char * pcCommandString );
/*
 * The function that registers the commands that are defined within this file.
 */
void vRegisterSampleCLICommands( void );

/*
 * Implements the task-stats command.
 */
static BaseType_t prvTaskStatsCommand( char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString );
static BaseType_t prvReset( char * pcWriteBuffer,
										   size_t xWriteBufferLen,
										   const char * pcCommandString );
/*
 * Implements the "query heap" command.
 */
#if( configINCLUDE_QUERY_HEAP_COMMAND == 1 )
	static BaseType_t prvQueryHeapCommand( char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString );
#endif

/*
 * Implements the "trace start" and "trace stop" commands;
 */
#if( configINCLUDE_TRACE_RELATED_CLI_COMMANDS == 1 )
	static BaseType_t prvStartStopTraceCommand( char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString );
#endif

/* Structure that defines the "task-stats" command line command.  This generates
a table that gives information on each task in the system. */
static const CLI_Command_Definition_t xTaskStats =
{
	"task-stats", /* The command string to type. */
	"\r\ntask-stats:\r\n Displays a table showing the state of each FreeRTOS task\r\n",
	prvTaskStatsCommand, /* The function to run. */
	0 /* No parameters are expected. */
};


#if( configINCLUDE_QUERY_HEAP_COMMAND == 1 )
	/* Structure that defines the "query_heap" command line command. */
	static const CLI_Command_Definition_t xQueryHeap =
	{
		"query-heap",
		"\r\nquery-heap:\r\n Displays the free heap space, and minimum ever free heap space.\r\n",
		prvQueryHeapCommand, /* The function to run. */
		0 /* The user can enter any number of commands. */
	};
#endif /* configQUERY_HEAP_COMMAND */

static BaseType_t prvParameterEchoCommand( char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString );
/* Structure that defines the "echo_parameters" command line command.  This
takes a variable number of parameters that the command simply echos back one at
a time. */
static const CLI_Command_Definition_t xParameterEcho =
{
	"test",
	"\r\necho-parameters <...>:\r\n Take variable number of parameters, echos each in turn\r\n",
	prvParameterEchoCommand, /* The function to run. */
	-1 /* The user can enter any number of commands. */
};

static BaseType_t prvConfigCommandHandler( char * pcWriteBuffer,
										   size_t xWriteBufferLen,
										   const char * pcCommandString );

static CLI_Command_Definition_t xCommandConfig =
{
	.pcCommand                   = "conf",
	.pcHelpString                = "\r\n"
								   "conf:\r\n"
								   "    Command to change or retrieve configuration for the device.\r\n"
								   "    Usage: conf [get|set|commit] [cert|key|thingname|endpoint] [value]\r\n"
                                   "           [commit]   : to write the set value to Internal Data Flash Memory\r\n"
			                       "           [cert]     : to get/set the certificate\r\n"
			                       "           [key]      : to get/set the private key\r\n"
                                   "           [thingname]: to get/set the AWS thing name\r\n"
                                   "           [endpoint] : to get/set the AWS MQTT endpoint\r\n",
	.pxCommandInterpreter        = prvConfigCommandHandler,
	.cExpectedNumberOfParameters = -1
};
static CLI_Command_Definition_t xReset =
{
		.pcCommand                   = "reset",
		.pcHelpString                = "\r\n"
									   "reset:\r\n"
									   "    Command to reset the device.\r\n"
									   "    Usage: reset\r\n",
		.pxCommandInterpreter        = prvReset,
		.cExpectedNumberOfParameters = -1
};

static CLI_Command_Definition_t xFormat =
{
		.pcCommand                   = "format",
		.pcHelpString                = "\r\n"
									   "format:\r\n"
									   "    Command to format the device.\r\n"
									   "    Usage: format\r\n",
		.pxCommandInterpreter        = prvFormat,
		.cExpectedNumberOfParameters = -1
};

static CLI_Command_Definition_t xWait =
{
		.pcCommand                   = "CLI",
		.pcHelpString                = "\r\n"
									   "CLI:\r\n"
									   "    Command to enter CLI mode and wait for user input.\r\n"
									   "    Usage: wait for input\r\n",
		.pxCommandInterpreter        = prvWaitMessage,
		.cExpectedNumberOfParameters = -1
};
/*-----------------------------------------------------------*/

void vRegisterSampleCLICommands( void )
{
	/* Register all the command line commands defined immediately above. */
	FreeRTOS_CLIRegisterCommand( &xTaskStats );
	FreeRTOS_CLIRegisterCommand( &xCommandConfig );
	FreeRTOS_CLIRegisterCommand( &xParameterEcho );
	FreeRTOS_CLIRegisterCommand( &xReset );
	FreeRTOS_CLIRegisterCommand( &xFormat );
	FreeRTOS_CLIRegisterCommand( &xWait );

	#if( configGENERATE_RUN_TIME_STATS == 1 )
	{
		FreeRTOS_CLIRegisterCommand( &xRunTimeStats );
	}
	#endif
	
	#if( configINCLUDE_QUERY_HEAP_COMMAND == 1 )
	{
		FreeRTOS_CLIRegisterCommand( &xQueryHeap );
	}
	#endif

	#if( configINCLUDE_TRACE_RELATED_CLI_COMMANDS == 1 )
	{
		FreeRTOS_CLIRegisterCommand( &xStartStopTrace );
	}
	#endif
}
/*-----------------------------------------------------------*/

static BaseType_t prvTaskStatsCommand( char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString )
{
const char *const pcHeader = "     State   Priority  Stack    #\r\n************************************************\r\n";
BaseType_t xSpacePadding;

	/* Remove compile time warnings about unused parameters, and check the
	write buffer is not NULL.  NOTE - for simplicity, this example assumes the
	write buffer length is adequate, so does not check for buffer overflows. */
	( void ) pcCommandString;
	( void ) xWriteBufferLen;
	configASSERT( pcWriteBuffer );

	/* Generate a table of task stats. */
	strcpy( pcWriteBuffer, " Task" );
	pcWriteBuffer += strlen( pcWriteBuffer );

	/* Minus three for the null terminator and half the number of characters in
	"Task" so the column lines up with the centre of the heading. */
	configASSERT( configMAX_TASK_NAME_LEN > 3 );
	for( xSpacePadding = strlen( "Task" ); xSpacePadding < ( configMAX_TASK_NAME_LEN - 3 ); xSpacePadding++ )
	{
		/* Add a space to align columns after the task's name. */
		*pcWriteBuffer = ' ';
		pcWriteBuffer++;

		/* Ensure always terminated. */
		*pcWriteBuffer = 0x00;
	}
	strcpy( pcWriteBuffer, pcHeader );
	vTaskList( pcWriteBuffer + strlen( pcHeader ) );

	/* There is no more data to return after this single string, so return
	pdFALSE. */
	return pdFALSE;
}
/*-----------------------------------------------------------*/

#if( configINCLUDE_QUERY_HEAP_COMMAND == 1 )

	static BaseType_t prvQueryHeapCommand( char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString )
	{
		/* Remove compile time warnings about unused parameters, and check the
		write buffer is not NULL.  NOTE - for simplicity, this example assumes the
		write buffer length is adequate, so does not check for buffer overflows. */
		( void ) pcCommandString;
		( void ) xWriteBufferLen;
		configASSERT( pcWriteBuffer );

		sprintf( pcWriteBuffer, "Current free heap %d bytes, minimum ever free heap %d bytes\r\n", ( int ) xPortGetFreeHeapSize(), ( int ) xPortGetMinimumEverFreeHeapSize() );

		/* There is no more data to return after this single string, so return
		pdFALSE. */
		return pdFALSE;
	}

#endif /* configINCLUDE_QUERY_HEAP */
/*-----------------------------------------------------------*/

static BaseType_t prvWaitMessage( char * pcWriteBuffer,
										   size_t xWriteBufferLen,
										   const char * pcCommandString )
{
	( void ) pcCommandString;
	( void ) xWriteBufferLen;
	configASSERT( pcWriteBuffer );
	extern signed char cRxedChar;
	if (cRxedChar != 0)
	{
			sprintf( pcWriteBuffer, "Going to FreeRTOS-CLI !\r\n");
	}

	return pdFALSE;
}
static BaseType_t prvReset( char * pcWriteBuffer,
										   size_t xWriteBufferLen,
										   const char * pcCommandString )
{
	/* Remove compile time warnings about unused parameters, and check the
	write buffer is not NULL.  NOTE - for simplicity, this example assumes the
	write buffer length is adequate, so does not check for buffer overflows. */
	( void ) pcCommandString;
	( void ) xWriteBufferLen;
	configASSERT( pcWriteBuffer );
	sprintf( pcWriteBuffer, "Resetting !\r\n");
	set_psw( 0 );
	R_BSP_InterruptsDisable();
	R_BSP_SoftwareReset();

	/* There is no more data to return after this single string, so return
	pdFALSE. */
	return pdFALSE;
}

static BaseType_t prvFormat( char * pcWriteBuffer,
										   size_t xWriteBufferLen,
										   const char * pcCommandString )
{
	/* Remove compile time warnings about unused parameters, and check the
	write buffer is not NULL.  NOTE - for simplicity, this example assumes the
	write buffer length is adequate, so does not check for buffer overflows. */
	( void ) pcCommandString;
	( void ) xWriteBufferLen;
	int32_t err ;
	RM_LITTLEFS_FLASH_Open(g_rm_littlefs0.p_ctrl, g_rm_littlefs0.p_cfg);


	err = lfs_format(&g_rm_littlefs0_lfs, &g_rm_littlefs0_lfs_cfg);
	if (LFS_ERR_OK == err)
	{
		lfs_mount(&g_rm_littlefs0_lfs, &g_rm_littlefs0_lfs_cfg);
		sprintf( pcWriteBuffer, "Format OK !\r\n");
	}
	else
	{
		sprintf( pcWriteBuffer, "Format NG !\r\n");
	}

	/* There is no more data to return after this single string, so return
	pdFALSE. */
	return pdFALSE;
}
static BaseType_t prvConfigCommandHandler( char * pcWriteBuffer,
										   size_t xWriteBufferLen,
										   const char * pcCommandString )
{
	(void) xWriteBufferLen;
    BaseType_t result = pdPASS;
    char * pRequest = NULL, * pKey = NULL, * pValue = NULL, * getValue = NULL;
    BaseType_t requestLength = 0, keyLength = 0, valueLength = 0;

    pRequest = FreeRTOS_CLIGetParameter( pcCommandString, 1U, &requestLength );
    * pcWriteBuffer = NULL;
	if( pRequest != NULL )
	{
		if( strncmp( pRequest, "get", requestLength ) == 0 )
		{
			pKey = FreeRTOS_CLIGetParameter( pcCommandString, 2U, &keyLength );
			getValue = xprvGetCacheEntry(pKey,keyLength);
			if (getValue == NULL)
			{
				sprintf(pcWriteBuffer,"No %s in Data Flash!\r\n",pKey);
			}
			else
			{
				sprintf(pcWriteBuffer,"%s\r\n",getValue);
			}

		}
		else if( strncmp( pRequest, "set", requestLength ) == 0 )
		{
			pKey = FreeRTOS_CLIGetParameter( pcCommandString, 2U, &keyLength );
			pValue = FreeRTOS_CLIGetParameter( pcCommandString, 3U, &valueLength );
			if (xprvWriteCacheEntry(keyLength,pKey,valueLength,pValue) < 0)
			{
				result = pdFALSE;
			}
			else
			{
				sprintf(pcWriteBuffer, "OK.\r\n" );
			}
		}
		else  if( strncmp( pRequest, "commit", requestLength ) == 0 )
		{

			BaseType_t xResult = KVStore_xCommitChanges();
			if( xResult == pdTRUE )
			{
				sprintf(pcWriteBuffer, "Configuration saved to Data Flash and used %d bytes.\r\n",( int )pvwrite );
			}
			else
			{
				sprintf(pcWriteBuffer, "Error: Could not save configuration to Data Flash or saved before.\r\n" );
			}

		}

		else
		{
			result = pdFALSE;
		}
	}

	if( result != pdPASS )
	{
		sprintf(pcWriteBuffer, "Error.\r\n\r\n" );
	}

	return pdFALSE;
}

static BaseType_t prvParameterEchoCommand( char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString )
{
const char *pcParameter;
BaseType_t xParameterStringLength, xReturn;
static UBaseType_t uxParameterNumber = 0;

	/* Remove compile time warnings about unused parameters, and check the
	write buffer is not NULL.  NOTE - for simplicity, this example assumes the
	write buffer length is adequate, so does not check for buffer overflows. */
	( void ) pcCommandString;
	( void ) xWriteBufferLen;
	configASSERT( pcWriteBuffer );

	if( uxParameterNumber == 0 )
	{
		/* The first time the function is called after the command has been
		entered just a header string is returned. */
		sprintf( pcWriteBuffer, "The parameters were:\r\n" );

		/* Next time the function is called the first parameter will be echoed
		back. */
		uxParameterNumber = 1U;

		/* There is more data to be returned as no parameters have been echoed
		back yet. */
		xReturn = pdPASS;
	}
	else
	{
		/* Obtain the parameter string. */
		pcParameter = FreeRTOS_CLIGetParameter
						(
							pcCommandString,		/* The command string itself. */
							uxParameterNumber,		/* Return the next parameter. */
							&xParameterStringLength	/* Store the parameter string length. */
						);

		if( pcParameter != NULL )
		{
			/* Return the parameter string. */
			memset( pcWriteBuffer, 0x00, xWriteBufferLen );
			sprintf( pcWriteBuffer, "%d: ", ( int ) uxParameterNumber );
			strncat( pcWriteBuffer, ( char * ) pcParameter, ( size_t ) xParameterStringLength );
			strncat( pcWriteBuffer, "\r\n", strlen( "\r\n" ) );

			/* There might be more parameters to return after this one. */
			xReturn = pdTRUE;
			uxParameterNumber++;
		}
		else
		{
			/* No more parameters were found.  Make sure the write buffer does
			not contain a valid string. */
			pcWriteBuffer[ 0 ] = 0x00;

			/* No more data to return. */
			xReturn = pdFALSE;

			/* Start over the next time this command is executed. */
			uxParameterNumber = 0;
		}
	}

	return xReturn;
}
/*-----------------------------------------------------------*/

