/*
 * FreeRTOS V202211.00
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
 * https://www.FreeRTOS.org
 * https://github.com/FreeRTOS
 *
 */

/* Kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

/* Demo program includes. */
#include "serial.h"

/* Renesas includes. */
#include "platform.h"
#include "r_dtc_rx_if.h"
#include "r_sci_rx_if.h"
#include "r_byteq_if.h"



/* FreeRTOS CLI Command Console */
#define U_SCI_UART_CLI_PINSET()	R_SCI_PinSet_SCI5()
#define U_SCI_UART_CLI_SCI_CH	(SCI_CH5)
#define U_DTC_UART_CLI_TX_ACT	((dtc_activation_source_t)VECT(SCI5,TXI5))
#define U_DTC_UART_CLI_TX_DR	(SCI5.TDR)
/* Characters received from the UART are stored in this queue, ready to be
received by the application.  ***NOTE*** Using a queue in this way is very
convenient, but also very inefficient.  It can be used here because characters
will only arrive slowly.  In a higher bandwidth system a circular RAM buffer or
DMA should be used in place of this queue. */
QueueHandle_t xRxQueue = NULL;

/* When a task calls vSerialPutString() its handle is stored in xSendingTask,
before being placed into the Blocked state (so does not use any CPU time) to
wait for the transmission to end.  The task handle is then used from the UART
transmit end interrupt to remove the task from the Blocked state. */
static TaskHandle_t xSendingTask = NULL;

/* Board Support Data Structures. */
sci_hdl_t xSerialSciHandle;
dtc_transfer_data_t xSerialTxDtcInfo;
void CLI_Support_Settings(void);
void vSerialSciCallback( void *pvArgs );
void CLI_Close(void);

void CLI_Support_Settings(void)
{

    /* FreeRTOS CLI Command Console */
    U_SCI_UART_CLI_PINSET();
    sci_cfg_t xSerialSciConfig;
    xSerialSciConfig.async.baud_rate    = 115200;
    xSerialSciConfig.async.clk_src      = SCI_CLK_INT;
    xSerialSciConfig.async.data_size    = SCI_DATA_8BIT;
    xSerialSciConfig.async.parity_en    = SCI_PARITY_OFF;
    xSerialSciConfig.async.parity_type  = SCI_EVEN_PARITY;
    xSerialSciConfig.async.stop_bits    = SCI_STOPBITS_1;
    xSerialSciConfig.async.int_priority = 1; /* lowest at first. */
    R_SCI_Open(U_SCI_UART_CLI_SCI_CH, SCI_MODE_ASYNC, &xSerialSciConfig, vSerialSciCallback, &xSerialSciHandle);
    R_DTC_Open();
    R_DTC_Control(DTC_CMD_DTC_START, NULL, NULL);
}

void CLI_Close(void)
{
	R_DTC_Close();
	R_SCI_Close(xSerialSciHandle);
}

/* Callback function which is called from Renesas API's interrupt service routine. */
void vSerialSciCallback( void *pvArgs )
{
sci_cb_args_t *pxArgs = (sci_cb_args_t *)pvArgs;

    /* Renesas API has a built-in queue but we will ignore it.  If the queue is not
    full, a received character is passed with SCI_EVT_RX_CHAR event.  If the queue
    is full, a received character is passed with SCI_EVT_RXBUF_OVFL event. */
    if( SCI_EVT_RX_CHAR == pxArgs->event || SCI_EVT_RXBUF_OVFL == pxArgs->event )
    {
        BaseType_t xHigherPriorityTaskWoken = pdFALSE;

        configASSERT( xRxQueue );

        /* Characters received from the UART are stored in this queue, ready to be
        received by the application.  ***NOTE*** Using a queue in this way is very
        convenient, but also very inefficient.  It can be used here because
        characters will only arrive slowly.  In a higher bandwidth system a circular
        RAM buffer or DMA should be used in place of this queue. */
        xQueueSendFromISR( xRxQueue, &pxArgs->byte, &xHigherPriorityTaskWoken );

        /* See http://www.freertos.org/xQueueOverwriteFromISR.html for information
        on the semantics of this ISR. */
        portYIELD_FROM_ISR( xHigherPriorityTaskWoken );
    }
    /* Renesas API notifies the completion of transmission by SCI_EVT_TEI event. */
    else if( SCI_EVT_TEI == pxArgs->event )
    {
        BaseType_t xHigherPriorityTaskWoken = pdFALSE;

        if( xSendingTask != NULL )
        {
            /* A task is waiting for the end of the Tx, unblock it now.
            http://www.freertos.org/vTaskNotifyGiveFromISR.html */
            vTaskNotifyGiveFromISR( xSendingTask, &xHigherPriorityTaskWoken );
            xSendingTask = NULL;

            portYIELD_FROM_ISR( xHigherPriorityTaskWoken );
        }
    }
}

/* Function required in order to link UARTCommandConsole.c - which is used by
multiple different demo application. */
xComPortHandle xSerialPortInitMinimal( unsigned long ulWantedBaud, unsigned portBASE_TYPE uxQueueLength )
{
    ( void ) ulWantedBaud;
    ( void ) uxQueueLength;

    /* Characters received from the UART are stored in this queue, ready to be
    received by the application.  ***NOTE*** Using a queue in this way is very
    convenient, but also very inefficient.  It can be used here because
    characters will only arrive slowly.  In a higher bandwidth system a circular
    RAM buffer or DMA should be used in place of this queue. */
    xRxQueue = xQueueCreate( uxQueueLength, sizeof( char ) );
    configASSERT( xRxQueue );

    /* Set interrupt priority. (Other UART settings had been initialized in the
    src/smc_gen/general/r_cg_hardware_setup.c.) */
    uint8_t ucInterruptPriority = configMAX_SYSCALL_INTERRUPT_PRIORITY - 1;
    R_SCI_Control( xSerialSciHandle, SCI_CMD_SET_RXI_PRIORITY, ( void * ) &ucInterruptPriority );
    R_SCI_Control( xSerialSciHandle, SCI_CMD_SET_TXI_PRIORITY, ( void * ) &ucInterruptPriority );

    /* Only one UART is supported, so it doesn't matter what is returned
    here. */
    return 0;
}

/* Function required in order to link UARTCommandConsole.c - which is used by
multiple different demo application. */
void vSerialPutString(const signed char * pcString, unsigned short usStringLength )
{
const TickType_t xMaxBlockTime = pdMS_TO_TICKS( 5000 );

    /* Only one port is supported. */


    /* Don't send the string unless the previous string has been sent. */
    if( ( xSendingTask == NULL ) && ( usStringLength > 0 ) )
    {
        /* Ensure the calling task's notification state is not already
        pending. */
        xTaskNotifyStateClear( NULL );

        /* Store the handle of the transmitting task.  This is used to unblock
        the task when the transmission has completed. */
        xSendingTask = xTaskGetCurrentTaskHandle();
        uint16_t str_length = 0;
		uint16_t transmit_length = 0;
		sci_err_t sci_err;
		uint32_t retry = 0xFFFF;

		str_length = usStringLength;

		while ((retry > 0) && (str_length > 0))
		{

			R_SCI_Control(xSerialSciHandle, SCI_CMD_TX_Q_BYTES_FREE, &transmit_length);

			if(transmit_length > str_length)
			{
				transmit_length = str_length;
			}

			sci_err = R_SCI_Send(xSerialSciHandle, (uint8_t *) pcString,
								 transmit_length);

			if ((sci_err == SCI_ERR_XCVR_BUSY) || (sci_err == SCI_ERR_INSUFFICIENT_SPACE))
			{
				retry--; // retry if previous transmission still in progress or tx buffer is insufficient.
				continue;
			}

			str_length -= transmit_length;
			pcString += transmit_length;

		}

		if (SCI_SUCCESS != sci_err)
		{
			R_BSP_NOP(); //TODO error handling code
		}
        /* Send the string using the Renesas API with a workaround. */
//        if( usStringLength > 1 )
//        {
//            /* Set up Data Transfer Control. */
//            dtc_cmd_arg_t xSerialTxDtcArg;
//            dtc_transfer_data_cfg_t xSerialTxDtcConfig;
//
//            xSerialTxDtcArg.act_src                   = U_DTC_UART_CLI_TX_ACT;
//            xSerialTxDtcConfig.transfer_mode          = DTC_TRANSFER_MODE_NORMAL;
//            xSerialTxDtcConfig.data_size              = DTC_DATA_SIZE_BYTE;
//            xSerialTxDtcConfig.src_addr_mode          = DTC_SRC_ADDR_INCR;
//            xSerialTxDtcConfig.dest_addr_mode         = DTC_DES_ADDR_FIXED;
//            xSerialTxDtcConfig.response_interrupt     = DTC_INTERRUPT_AFTER_ALL_COMPLETE;
//            xSerialTxDtcConfig.repeat_block_side      = DTC_REPEAT_BLOCK_SOURCE;
//            xSerialTxDtcConfig.chain_transfer_enable  = DTC_CHAIN_TRANSFER_DISABLE;
//            xSerialTxDtcConfig.chain_transfer_mode    = (dtc_chain_transfer_mode_t)0;
//            xSerialTxDtcConfig.source_addr            = ( uint32_t ) pcString;
//            xSerialTxDtcConfig.dest_addr              = ( uint32_t ) &U_DTC_UART_CLI_TX_DR;
//            xSerialTxDtcConfig.transfer_count         = ( uint32_t ) usStringLength;
//            xSerialTxDtcArg.chain_transfer_nr         = 0;
//            xSerialTxDtcArg.p_transfer_data           = &xSerialTxDtcInfo;
//            xSerialTxDtcArg.p_data_cfg                = &xSerialTxDtcConfig;
//
//            R_DTC_Create( xSerialTxDtcArg.act_src, &xSerialTxDtcInfo, &xSerialTxDtcConfig, 0 );
//            R_DTC_Control( DTC_CMD_ACT_SRC_ENABLE, NULL, &xSerialTxDtcArg );
//            R_SCI_Send( xSerialSciHandle, ( uint8_t * ) (pcString + usStringLength - 1), 1 );
//        }
//        else
//        {
//            R_SCI_Send( xSerialSciHandle, ( uint8_t * ) pcString, 1 );
//        }

        /* Wait in the Blocked state (so not using any CPU time) until the
        transmission has completed. */
        ulTaskNotifyTake( pdTRUE, xMaxBlockTime );

        /* A breakpoint can be set here for debugging. */
        nop();
    }
}

/* Function required in order to link UARTCommandConsole.c - which is used by
multiple different demo application. */
signed portBASE_TYPE xSerialGetChar( xComPortHandle pxPort, signed char *pcRxedChar, TickType_t xBlockTime )
{
    /* Only one UART is supported. */
    ( void ) pxPort;

    /* Return a received character, if any are available.  Otherwise block to
    wait for a character. */
    return xQueueReceive( xRxQueue, pcRxedChar, xBlockTime );
}

/* Function required in order to link UARTCommandConsole.c - which is used by
multiple different demo application. */
signed portBASE_TYPE xSerialPutChar( xComPortHandle pxPort, signed char cOutChar, TickType_t xBlockTime )
{
    /* Just mapped to vSerialPutString() so the block time is not used. */
    ( void ) xBlockTime;
    ( void ) pxPort;

    vSerialPutString( &cOutChar, sizeof( cOutChar ) );
    return pdPASS;
}
