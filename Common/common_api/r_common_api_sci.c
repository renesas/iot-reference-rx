/*
 * r_common_api_sci.c v1.0.0
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

/**********************************************************************************************************************
 Includes   <System Includes> , "Project Includes"
 *********************************************************************************************************************/
#include <stdio.h>

#include "FreeRTOS.h"
#include "queue.h"
#include "stddef.h"
#include "serial.h"
#include "semphr.h"
#include "r_common_api_sci.h"
#include "r_sci_rx_pinset.h"

/**********************************************************************************************************************
 * Macro definitions
 *********************************************************************************************************************/
#define cmdQUEUE_LENGTH         2048                                /* Originally described in UARTCommandConsole.c */

/**********************************************************************************************************************
 Exported global variables
 *********************************************************************************************************************/
sci_hdl_t xSerialSciHandle; /* Originally described in serial.c */
/* Characters received from the UART are stored in this queue, ready to be
 received by the application.  ***NOTE*** Using a queue in this way is very
 convenient, but also very inefficient.  It can be used here because characters
 will only arrive slowly.  In a higher bandwidth system a circular RAM buffer or
 DMA should be used in place of this queue. */
QueueHandle_t xRxQueue = NULL; /* Originally described in serial.c */

/**********************************************************************************************************************
 Private (static) variables
 *********************************************************************************************************************/
volatile static CommonAPI_Status_t s_SCI5_COM_Status = COMAPI_STATE_CLOSE;/* Variables that manage commonAPI status */

/* Function Name: SCI5_COM_init */
/******************************************************************************************************************//**
 * @brief Function that executes SCI5 open processing.
 * @param[in] void
 * @return e_commonapi_err_t
 * @retval COMMONAPI_SUCCESS        success
 * @retval COMMONAPI_ERR            fail
 *********************************************************************************************************************/
static e_commonapi_err_t SCI5_COM_init(void)
{
    sci_err_t sci_err_status = SCI_SUCCESS;

    /* Perform pinset processing before executing R_SCI_Open */
    U_SCI_UART_CLI_PINSET();
    sci_cfg_t xSerialSciConfig;
    xSerialSciConfig.async.baud_rate = BSP_CFG_SCI_UART_TERMINAL_BITRATE;
    xSerialSciConfig.async.clk_src = SCI_CLK_INT;
    xSerialSciConfig.async.data_size = SCI_DATA_8BIT;
    xSerialSciConfig.async.parity_en = SCI_PARITY_OFF;
    xSerialSciConfig.async.parity_type = SCI_EVEN_PARITY;
    xSerialSciConfig.async.stop_bits = SCI_STOPBITS_1;
    xSerialSciConfig.async.int_priority = 1; // lowest at first.

    /* Execution of R_SCI_Open */
    sci_err_status = R_SCI_Open (U_SCI_UART_CLI_SCI_CH, SCI_MODE_ASYNC, &xSerialSciConfig, vSerialSciCallback,
                                 &xSerialSciHandle);
    if (sci_err_status != SCI_SUCCESS)
    {
        return COMMONAPI_ERR;
    }

    return COMMONAPI_SUCCESS;
}
/**********************************************************************************************************************
 End of function SCI5_COM_init
 *********************************************************************************************************************/

/* Function Name: SCI5_COM_terminate */
/******************************************************************************************************************//**
 * @brief Function to close the process opened by SCI5_COM_init.
 * @param[in] void
 * @return e_commonapi_err_t
 * @retval COMMONAPI_SUCCESS        success
 * @retval COMMONAPI_ERR            fail
 *********************************************************************************************************************/
static e_commonapi_err_t SCI5_COM_terminate(void)
{
    sci_err_t err = R_SCI_Close (xSerialSciHandle);

    if (err != SCI_SUCCESS)
    {
        return COMMONAPI_ERR;
    }

    return COMMONAPI_SUCCESS;
}
/**********************************************************************************************************************
 End of function SCI5_COM_terminate
 *********************************************************************************************************************/

/* Function Name: SCI5_COM_Resource_Create */
/******************************************************************************************************************//**
 * @brief Functions for creating OS resources.
 * @param[in] void
 * @return e_commonapi_err_t
 * @retval COMMONAPI_SUCCESS        success
 * @retval COMMONAPI_ERR            fail
 *********************************************************************************************************************/
static e_commonapi_err_t SCI5_COM_Resource_Create(void)
{
    /* Processing for the CLI library, originally defined in Demos\cli\serial.c */
    xRxQueue = xQueueCreate(cmdQUEUE_LENGTH, sizeof(char ));
    configASSERT(xRxQueue);

    /* Return an error if xRxQueue is NULL */
    if (xRxQueue == NULL)
    {
        return COMMONAPI_ERR;
    }

    return COMMONAPI_SUCCESS;
}
/**********************************************************************************************************************
 End of function SCI5_COM_Resource_Create
 *********************************************************************************************************************/

/* Function Name: SCI5_COM_Resource_Release */
/******************************************************************************************************************//**
 * @brief Function to release OS resources.
 * @param[in] void
 * @return e_commonapi_err_t
 * @retval COMMONAPI_SUCCESS        success
 * @retval COMMONAPI_ERR            fail
 *********************************************************************************************************************/
static e_commonapi_err_t SCI5_COM_Resource_Release(void)
{
    if (xRxQueue != NULL)
    {
        vQueueDelete (xRxQueue);
        xRxQueue = NULL;
    }

    return COMMONAPI_SUCCESS;
}
/**********************************************************************************************************************
 End of function SCI5_COM_Resource_Release
 *********************************************************************************************************************/

/* Function Name: R_Demo_Common_API_SCI5_Open */
/******************************************************************************************************************//**
 * @brief CommonAPI open function for sci5.
 * @param[in] void
 * @return e_commonapi_err_t
 * @retval COMMONAPI_SUCCESS        success
 * @retval COMMONAPI_ERR            fail
 *********************************************************************************************************************/
e_commonapi_err_t R_Demo_Common_API_SCI5_Open(void)
{
    e_commonapi_err_t return_err_state = COMMONAPI_SUCCESS;

    /* Enter critical section */
    taskENTER_CRITICAL();

    if (s_SCI5_COM_Status == COMAPI_STATE_CLOSE)
    {
        /* Status change */
        s_SCI5_COM_Status = COMAPI_STATE_RUNNING;

        /* Exit critical section */
        taskEXIT_CRITICAL();

        /* SCI5 init */
        return_err_state = SCI5_COM_init ();
        if (return_err_state == COMMONAPI_ERR)
        {
            s_SCI5_COM_Status = COMAPI_STATE_CLOSE;

            return COMMONAPI_ERR;
        }

        /* Resource Create */
        return_err_state = SCI5_COM_Resource_Create ();
        if (return_err_state == COMMONAPI_ERR)
        {
            s_SCI5_COM_Status = COMAPI_STATE_CLOSE;

            return COMMONAPI_ERR;
        }

        /* Status change */
        s_SCI5_COM_Status = COMAPI_STATE_OPEN;

    }
    else if (s_SCI5_COM_Status == COMAPI_STATE_RUNNING)
    {
        /* Exit critical section */
        taskEXIT_CRITICAL();

        /* Execute delay until running state ends */
        while (s_SCI5_COM_Status == COMAPI_STATE_RUNNING)
        {
            vTaskDelay (10);
        }

        /* Returns an error if status is not open */
        if (s_SCI5_COM_Status != COMAPI_STATE_OPEN)
        {
            return COMMONAPI_ERR;
        }
    }
    else if (s_SCI5_COM_Status == COMAPI_STATE_OPEN)
    {
        /* Exit critical section */
        taskEXIT_CRITICAL();
    }
    else
    {
        /* Exit critical section */
        taskEXIT_CRITICAL();

        /* Returns an error because it contains an unexpected status */
        return COMMONAPI_ERR;
    }

    return return_err_state;
}
/**********************************************************************************************************************
 End of function R_Demo_Common_API_SCI5_Open
 *********************************************************************************************************************/

/* Function Name: R_Demo_Common_API_SCI5_Close */
/******************************************************************************************************************//**
 * @brief CommonAPI close function for sci5.
 * @param[in] void
 * @return e_commonapi_err_t
 * @retval COMMONAPI_SUCCESS        success
 * @retval COMMONAPI_ERR            fail
 *********************************************************************************************************************/
e_commonapi_err_t R_Demo_Common_API_SCI5_Close(void)
{
    e_commonapi_err_t return_err_state = COMMONAPI_SUCCESS;

    /* Enter critical section */
    taskENTER_CRITICAL();

    if (s_SCI5_COM_Status == COMAPI_STATE_OPEN)
    {
        /* Status change */
        s_SCI5_COM_Status = COMAPI_STATE_RUNNING;

        /* Exit critical section */
        taskEXIT_CRITICAL();

        /* SCI5 terminate */
        return_err_state = SCI5_COM_terminate ();
        if (return_err_state == COMMONAPI_ERR)
        {
            s_SCI5_COM_Status = COMAPI_STATE_CLOSE;

            return COMMONAPI_ERR;
        }

        /* Resource Release */
        return_err_state = SCI5_COM_Resource_Release ();
        if (return_err_state == COMMONAPI_ERR)
        {
            s_SCI5_COM_Status = COMAPI_STATE_CLOSE;

            return COMMONAPI_ERR;
        }

        /* Status change */
        s_SCI5_COM_Status = COMAPI_STATE_CLOSE;

    }
    else if (s_SCI5_COM_Status == COMAPI_STATE_RUNNING)
    {
        /* Exit critical section */
        taskEXIT_CRITICAL();

        /* Execute delay until running state ends */
        while (s_SCI5_COM_Status == COMAPI_STATE_RUNNING)
        {
            vTaskDelay (10);
        }

        /* Returns an error if status is not close */
        if (s_SCI5_COM_Status != COMAPI_STATE_CLOSE)
        {
            return COMMONAPI_ERR;
        }
    }
    else if (s_SCI5_COM_Status == COMAPI_STATE_CLOSE)
    {
        /* Exit critical section */
        taskEXIT_CRITICAL();
    }
    else
    {
        /* Exit critical section */
        taskEXIT_CRITICAL();

        /* Returns an error because it contains an unexpected status */
        return COMMONAPI_ERR;
    }

    return return_err_state;
}
/**********************************************************************************************************************
 End of function R_Demo_Common_API_SCI5_Close
 *********************************************************************************************************************/

/* Function Name: vSerialSciCallback */
/******************************************************************************************************************//**
 * @brief Callback function which is called from Renesas API's interrupt service routine.
 * @param[in] void *pvArgs
 * @return void
 *********************************************************************************************************************/
void vSerialSciCallback(void *pvArgs)
{
    sci_cb_args_t *pxArgs = (sci_cb_args_t*) pvArgs;

    configASSERT(pxArgs != NULL);

    /* Renesas API has a built-in queue but we will ignore it.  If the queue is not
     full, a received character is passed with SCI_EVT_RX_CHAR event.  If the queue
     is full, a received character is passed with SCI_EVT_RXBUF_OVFL event. */
    if (pxArgs->event == SCI_EVT_RX_CHAR || pxArgs->event == SCI_EVT_RXBUF_OVFL)
    {
        BaseType_t xHigherPriorityTaskWoken = pdFALSE;

        configASSERT(xRxQueue);

        /* Characters received from the UART are stored in this queue, ready to be
         received by the application.  ***NOTE*** Using a queue in this way is very
         convenient, but also very inefficient.  It can be used here because
         characters will only arrive slowly.  In a higher bandwidth system a circular
         RAM buffer or DMA should be used in place of this queue. */
        xQueueSendFromISR(xRxQueue, &pxArgs->byte, &xHigherPriorityTaskWoken);

        /* See http://www.freertos.org/xQueueOverwriteFromISR.html for information
         on the semantics of this ISR. */
        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
    }
}
/**********************************************************************************************************************
 End of function vSerialSciCallback
 *********************************************************************************************************************/
