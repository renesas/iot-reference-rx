/*
 * FreeRTOS V202211.00
 * Copyright (C) 2017 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 * Modifications Copyright (C) 2023-2025 Renesas Electronics Corporation or its affiliates.
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

/*
 * NOTE:  This file uses a third party USB CDC driver.
 */

/* Standard includes. */
#include "string.h"
#include "stdio.h"

/* FreeRTOS includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

/* Example includes. */
#include "FreeRTOS_CLI.h"

/* Demo application includes. */
#include "serial.h"

/* Demo Config */
#include "demo_config.h"

/* Dimensions the buffer into which input characters are placed. */
#define cmdMAX_INPUT_SIZE        4096

/* Dimentions a buffer to be used by the UART driver, if the UART driver uses a
buffer at all. */
#define cmdQUEUE_LENGTH			2048

/* DEL acts as a backspace. */
#define cmdASCII_DEL        ( 0x7F )

/* The maximum time to wait for the mutex that guards the UART to become
available. */
#define cmdMAX_MUTEX_WAIT        pdMS_TO_TICKS( 300 )

#ifndef configCLI_BAUD_RATE
    #define configCLI_BAUD_RATE    115200
#endif

/*-----------------------------------------------------------*/

/*
 * The task that implements the command console processing.
 */
static void prvUARTCommandConsoleTask( void *pvParameters );
void vUARTCommandConsoleStart( uint16_t usStackSize, UBaseType_t uxPriority );

/*-----------------------------------------------------------*/
/* Const messages output by the command console. */
static const char * const pcWelcomeMessage1 = "FreeRTOS command server.\r\n"\
        "Type Help to view a list of registered commands.\r\n\r\n"\
        "\tStandard procedure:\r\n";

#if (ENABLE_FLEET_PROVISIONING_DEMO == 1)
#if (ENABLE_OTA_UPDATE_DEMO == 1)
static const char * const pcWelcomeMessage2 =
        "\t\t1. Set value for rootca(optional)/endpoint/claimcert/claimkey/template/codesigncert.\r\n";;

#else
static const char * const pcWelcomeMessage2 =
        "\t\t1. Set value for rootca(optional)/endpoint/claimcert/claimkey/template.\r\n";;
#endif
#else
#if (ENABLE_OTA_UPDATE_DEMO == 1)
static const char * const pcWelcomeMessage2 =
        "\t\t1. Set value for endpoint/thingname/certificate/key/codesigncert.\r\n";
#else
static const char * const pcWelcomeMessage2 =
        "\t\t1. Set value for endpoint/thingname/certificate/key.\r\n";
#endif
#endif

static const char *const pcWelcomeMessage3 =
        "\t\t2. Write the key value to Internal Data Flash Memory with 'commit' command.\r\n"
        "\t\t3. Reset the program to start the demo.\r\n\r\n>";
static const char * const pcWaitMessage = "Press CLI and enter to switch to CLI mode or wait 10secs to run demo! \r\n";
static const char * const pcEndOfOutputMessage = "\r\n>";
static const char * const pcNewLine = "\r\n";

/* Used to guard access to the UART in case messages are sent to the UART from
more than one task. */
static SemaphoreHandle_t xTxMutex = NULL;
TaskHandle_t xCLIHandle = NULL;
/* The handle to the UART port, which is not used by all ports. */
static xComPortHandle xPort = 0;
signed char cRxedChar;

/*-----------------------------------------------------------*/

void vUARTCommandConsoleStart( uint16_t usStackSize, UBaseType_t uxPriority )
{
    /* Create the semaphore used to access the UART Tx. */
    xTxMutex = xSemaphoreCreateMutex();
    configASSERT( xTxMutex );

    /* Create that task that handles the console itself. */
    xTaskCreate(     prvUARTCommandConsoleTask,    /* The task that implements the command console. */
                    "CLI",                        /* Text name assigned to the task.  This is just to assist debugging.  The kernel does not use this name itself. */
                    usStackSize,                /* The size of the stack allocated to the task. */
                    NULL,                        /* The parameter is not used, so NULL is passed. */
                    uxPriority,                    /* The priority allocated to the task. */
                    &xCLIHandle );                /* A CLI handle */
}
/*-----------------------------------------------------------*/

static void prvUARTCommandConsoleTask( void *pvParameters )
{
signed char cPrevChar = NULL;
uint16_t ucInputIndex = 0;
char *pcOutputString = NULL;
static char cInputString[ cmdMAX_INPUT_SIZE ], cLastInputString[ cmdMAX_INPUT_SIZE ];
BaseType_t xReturned;

    ( void ) pvParameters;

    /* Obtain the address of the output buffer.  Note there is no mutual
    exclusion on this buffer as it is assumed only one command console interface
    will be used at any one time. */
    pcOutputString = FreeRTOS_CLIGetOutputBuffer();

	/* Initialise the UART. */
	xPort = xSerialPortInitMinimal( configCLI_BAUD_RATE, cmdQUEUE_LENGTH );

    /* Send the welcome message. */
    vSerialPutString( ( signed char * ) pcWelcomeMessage1, ( unsigned short ) strlen( pcWelcomeMessage1 ) );
    vSerialPutString( ( signed char * ) pcWelcomeMessage2, ( unsigned short ) strlen( pcWelcomeMessage2 ) );
    vSerialPutString( ( signed char * ) pcWelcomeMessage3, ( unsigned short ) strlen( pcWelcomeMessage3 ) );
    vSerialPutString( ( signed char * ) pcWaitMessage, ( unsigned short ) strlen( pcWaitMessage ) );
    vSerialPutString( ( signed char * ) pcEndOfOutputMessage, ( unsigned short ) strlen( pcEndOfOutputMessage ) );
    for( ;; )
    {
        /* Wait for the next character.  The while loop is used in case
        INCLUDE_vTaskSuspend is not set to 1 - in which case portMAX_DELAY will
        be a genuine block time rather than an infinite block time. */
        while( xSerialGetChar( xPort, &cRxedChar, portMAX_DELAY ) != pdPASS );

        /* Ensure exclusive access to the UART Tx. */
        if( xSemaphoreTake( xTxMutex, cmdMAX_MUTEX_WAIT ) == pdPASS )
        {
            /* Echo the character back. */
            xSerialPutChar( xPort, cRxedChar, portMAX_DELAY );

            /* Was it the end of the line? */
            if( cRxedChar == '\n' && cPrevChar == '\r')
            {
                /* Just to space the output from the input. */
                vSerialPutString( ( signed char * ) pcNewLine, ( unsigned short ) strlen( pcNewLine ) );

                /* See if the command is empty, indicating that the last command
                is to be executed again. */
                if( ucInputIndex == 0 )
                {
                    /* Copy the last command back into the input string. */
                    strcpy( cInputString, cLastInputString );
                }

                /* Pass the received command to the command interpreter.  The
                command interpreter is called repeatedly until it returns
                pdFALSE    (indicating there is no more output) as it might
                generate more than one string. */
                do
                {
                    /* Get the next output string from the command interpreter. */
                    xReturned = FreeRTOS_CLIProcessCommand( cInputString, pcOutputString, configCOMMAND_INT_MAX_OUTPUT_SIZE );

                    /* Write the generated string to the UART. */
                    vSerialPutString( ( signed char * ) pcOutputString, ( unsigned short ) strlen( pcOutputString ) );
                    memset(pcOutputString, '\0', configCOMMAND_INT_MAX_OUTPUT_SIZE);

                } while( xReturned != pdFALSE );

                /* All the strings generated by the input command have been
                sent.  Clear the input string ready to receive the next command.
                Remember the command that was just processed first in case it is
                to be processed again. */
                strcpy( cLastInputString, cInputString );
                ucInputIndex = 0;
                memset( cInputString, 0x00, cmdMAX_INPUT_SIZE );

                vSerialPutString( ( signed char * ) pcEndOfOutputMessage, ( unsigned short ) strlen( pcEndOfOutputMessage ) );
            }
            else
            {
                if( cRxedChar == '\r' )
                {
                    /* Ignore the character. */
                }
                else if( ( cRxedChar == '\b' ) || ( cRxedChar == cmdASCII_DEL ) )
                {
                    /* Backspace was pressed.  Erase the last character in the
                    string - if any. */
                    if( ucInputIndex > 0 )
                    {
                        ucInputIndex--;
                        cInputString[ ucInputIndex ] = '\0';
                    }
                }
                else
                {
                    /* A character was entered.  Add it to the string entered so
                    far.  When a \n is entered the complete    string will be
                    passed to the command interpreter. */
                        if( ucInputIndex < cmdMAX_INPUT_SIZE )
                        {
                            cInputString[ ucInputIndex ] = cRxedChar;
                            ucInputIndex++;
                        }
                }
            }

            /* Must ensure to give the mutex back. */
            xSemaphoreGive( xTxMutex );
        }
        cPrevChar = cRxedChar;
    }

    /* Delete the mutex in the end of CLI task */
    vSemaphoreDelete( xTxMutex );

}
/*-----------------------------------------------------------*/

void vOutputString( const char * pcMessage )
{
    {
        vSerialPutString( ( signed char * ) pcMessage, ( unsigned short ) strlen( pcMessage ) );
    }
}
/*-----------------------------------------------------------*/

/******************************************************************************
 * Function Name: vOutputChar
 * Description  : Outputs a single character to the UART interface (custom BSP charput function).
 * Argument     : cOutChar
 * Return Value : None
 *****************************************************************************/
void vOutputChar(const char cOutChar)
{
    {
        vSerialPutString((signed char *) &cOutChar, (unsigned short)sizeof(cOutChar)); //Cast to match input arg of vSerialPutString
    }
}
/******************************************************************************
 End of function vOutputChar
 *****************************************************************************/
