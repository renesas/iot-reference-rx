/*
 * r_common_api_flash.c v1.0.0
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
#include "r_common_api_flash.h"

/**********************************************************************************************************************
 * Macro definitions
 *********************************************************************************************************************/

/**********************************************************************************************************************
 Exported global variables
 *********************************************************************************************************************/
xSemaphoreHandle xSemaphoreFlashAccess;

volatile UPDATA_DATA_FLASH_CONTROL_BLOCK update_data_flash_control_block = {.status = DATA_FLASH_UPDATE_STATE_UNINITIALIZE};

volatile flash_res_t g_blank_check_result;

/**********************************************************************************************************************
 Private (static) variables
 *********************************************************************************************************************/
static volatile CommonAPI_Status_t s_Flash_COM_Status = COMAPI_STATE_CLOSE;/* Variables that manage commonAPI status */

/* Function Name: Flash_COM_init */
/******************************************************************************************************************//**
 * @brief Function that executes Flash open processing.
 * @param[in] void
 * @return e_commonapi_err_t
 * @retval COMMONAPI_SUCCESS        success
 * @retval COMMONAPI_ERR            fail
 *********************************************************************************************************************/
static e_commonapi_err_t Flash_COM_init(void)
{
    flash_err_t err = R_FLASH_Open ();

    if (FLASH_SUCCESS != err)
    {
        return COMMONAPI_ERR;
    }

    flash_interrupt_config_t cb_func_info;

    cb_func_info.pcallback = flashing_callback;
    cb_func_info.int_priority = 3;
    R_FLASH_Control (FLASH_CMD_SET_BGO_CALLBACK, (void*) &cb_func_info);

    update_data_flash_control_block.status = DATA_FLASH_UPDATE_STATE_INITIALIZE;

    return COMMONAPI_SUCCESS;
}
/**********************************************************************************************************************
 End of function Flash_COM_init
 *********************************************************************************************************************/

/* Function Name: Flash_COM_terminate */
/******************************************************************************************************************//**
 * @brief Function to close the process opened by Flash_COM_init.
 * @param[in] void
 * @return e_commonapi_err_t
 * @retval COMMONAPI_SUCCESS        success
 * @retval COMMONAPI_ERR            fail
 *********************************************************************************************************************/
static e_commonapi_err_t Flash_COM_terminate(void)
{
    flash_err_t err = R_FLASH_Close ();

    if (FLASH_SUCCESS != err)
    {
        return COMMONAPI_ERR;
    }

    return COMMONAPI_SUCCESS;
}
/**********************************************************************************************************************
 End of function Flash_COM_terminate
 *********************************************************************************************************************/

/* Function Name: Flash_COM_Resource_Create */
/******************************************************************************************************************//**
 * @brief Functions for creating OS resources.
 * @param[in] void
 * @return e_commonapi_err_t
 * @retval COMMONAPI_SUCCESS        success
 * @retval COMMONAPI_ERR            fail
 *********************************************************************************************************************/
static e_commonapi_err_t Flash_COM_Resource_Create(void)
{
    xSemaphoreFlashAccess = xSemaphoreCreateBinary();
    configASSERT(xSemaphoreFlashAccess);

    /* Return an error if xSemaphoreFlashAccess is NULL */
    if (NULL == xSemaphoreFlashAccess)
    {
        return COMMONAPI_ERR;
    }

    xSemaphoreGive(xSemaphoreFlashAccess);

    return COMMONAPI_SUCCESS;
}
/**********************************************************************************************************************
 End of function Flash_COM_Resource_Create
 *********************************************************************************************************************/

/* Function Name: Flash_COM_Resource_Release */
/******************************************************************************************************************//**
 * @brief Function to release OS resources.
 * @param[in] void
 * @return e_commonapi_err_t
 * @retval COMMONAPI_SUCCESS        success
 * @retval COMMONAPI_ERR            fail
 *********************************************************************************************************************/
static e_commonapi_err_t Flash_COM_Resource_Release(void)
{
    if (NULL != xSemaphoreFlashAccess)
    {
        vSemaphoreDelete(xSemaphoreFlashAccess);
        xSemaphoreFlashAccess = NULL;
    }

    return COMMONAPI_SUCCESS;
}
/**********************************************************************************************************************
 End of function Flash_COM_Resource_Release
 *********************************************************************************************************************/

/* Function Name: R_Demo_Common_API_Flash_Open */
/******************************************************************************************************************//**
 * @brief CommonAPI open function for Flash.
 * @param[in] void
 * @return e_commonapi_err_t
 * @retval COMMONAPI_SUCCESS        success
 * @retval COMMONAPI_ERR            fail
 *********************************************************************************************************************/
e_commonapi_err_t R_Demo_Common_API_Flash_Open(void)
{
    e_commonapi_err_t return_err_state = COMMONAPI_SUCCESS;

    /* Enter critical section */
    taskENTER_CRITICAL();

    if (COMAPI_STATE_CLOSE == s_Flash_COM_Status)
    {
        /* Status change */
        s_Flash_COM_Status = COMAPI_STATE_RUNNING;

        /* Exit critical section */
        taskEXIT_CRITICAL();

        /* Flash init */
        return_err_state = Flash_COM_init ();
        if (COMMONAPI_ERR == return_err_state)
        {
            s_Flash_COM_Status = COMAPI_STATE_CLOSE;

            return COMMONAPI_ERR;
        }

        /* Resource Create */
        return_err_state = Flash_COM_Resource_Create ();
        if (COMMONAPI_ERR == return_err_state)
        {
            s_Flash_COM_Status = COMAPI_STATE_CLOSE;

            return COMMONAPI_ERR;
        }

        /* Status change */
        s_Flash_COM_Status = COMAPI_STATE_OPEN;

    }
    else if (COMAPI_STATE_RUNNING == s_Flash_COM_Status)
    {
        /* Exit critical section */
        taskEXIT_CRITICAL();

        /* Execute delay until running state ends */
        while (COMAPI_STATE_RUNNING == s_Flash_COM_Status)
        {
            vTaskDelay (10);
        }

        /* Returns an error if status is not open */
        if (COMAPI_STATE_OPEN != s_Flash_COM_Status)
        {
            return COMMONAPI_ERR;
        }
    }
    else if (COMAPI_STATE_OPEN == s_Flash_COM_Status)
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
 End of function R_Demo_Common_API_Flash_Open
 *********************************************************************************************************************/

/* Function Name: R_Demo_Common_API_Flash_Close */
/******************************************************************************************************************//**
 * @brief CommonAPI close function for Flash.
 * @param[in] void
 * @return e_commonapi_err_t
 * @retval COMMONAPI_SUCCESS        success
 * @retval COMMONAPI_ERR            fail
 *********************************************************************************************************************/
e_commonapi_err_t R_Demo_Common_API_Flash_Close(void)
{
    e_commonapi_err_t return_err_state = COMMONAPI_SUCCESS;

    /* Enter critical section */
    taskENTER_CRITICAL();

    if (COMAPI_STATE_OPEN == s_Flash_COM_Status)
    {
        /* Status change */
        s_Flash_COM_Status = COMAPI_STATE_RUNNING;

        /* Exit critical section */
        taskEXIT_CRITICAL();

        /* Flash terminate */
        return_err_state = Flash_COM_terminate ();
        if (return_err_state == COMMONAPI_ERR)
        {
            s_Flash_COM_Status = COMAPI_STATE_CLOSE;

            return COMMONAPI_ERR;
        }

        /* Resource Release */
        return_err_state = Flash_COM_Resource_Release ();
        if (COMMONAPI_ERR == return_err_state)
        {
            s_Flash_COM_Status = COMAPI_STATE_CLOSE;

            return COMMONAPI_ERR;
        }

        /* Status change */
        s_Flash_COM_Status = COMAPI_STATE_CLOSE;

    }
    else if (COMAPI_STATE_RUNNING == s_Flash_COM_Status)
    {
        /* Exit critical section */
        taskEXIT_CRITICAL();

        /* Execute delay until running state ends */
        while (COMAPI_STATE_RUNNING == s_Flash_COM_Status)
        {
            vTaskDelay (10);
        }

        /* Returns an error if status is not close */
        if (COMAPI_STATE_CLOSE != s_Flash_COM_Status)
        {
            return COMMONAPI_ERR;
        }
    }
    else if (COMAPI_STATE_CLOSE == s_Flash_COM_Status)
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
 End of function R_Demo_Common_API_Flash_Close
 *********************************************************************************************************************/

/* Function Name: flashing_callback */
/******************************************************************************************************************//**
 * @brief Callback function for Flash. This function is called from Renesas API's interrupt service routine.
 * @param[in] void* event
 * @return void
 *********************************************************************************************************************/
void flashing_callback(void *event)
{
    uint32_t event_code;
    event_code = *((uint32_t*) event);

    static portBASE_TYPE xHigherPriorityTaskWoken;

    switch (event_code)
    {
        case FLASH_INT_EVENT_ERASE_COMPLETE:
            if (DATA_FLASH_UPDATE_STATE_ERASE_WAIT_COMPLETE == update_data_flash_control_block.status)
            {
                update_data_flash_control_block.status = DATA_FLASH_UPDATE_STATE_FINALIZE;
                xSemaphoreGiveFromISR(xSemaphoreFlashAccess, &xHigherPriorityTaskWoken);
                portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
            }
            else
            {
                update_data_flash_control_block.status = DATA_FLASH_UPDATE_STATE_ERROR;
                xSemaphoreGiveFromISR(xSemaphoreFlashAccess, &xHigherPriorityTaskWoken);
            }
        break;
        case FLASH_INT_EVENT_WRITE_COMPLETE:
            if (DATA_FLASH_UPDATE_STATE_WRITE_WAIT_COMPLETE == update_data_flash_control_block.status)
            {
                update_data_flash_control_block.status = DATA_FLASH_UPDATE_STATE_FINALIZE;
                xSemaphoreGiveFromISR(xSemaphoreFlashAccess, &xHigherPriorityTaskWoken);
                portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
            }
            else
            {
                update_data_flash_control_block.status = DATA_FLASH_UPDATE_STATE_ERROR;
                xSemaphoreGiveFromISR(xSemaphoreFlashAccess, &xHigherPriorityTaskWoken);
            }
        break;
        case FLASH_INT_EVENT_BLANK:
            g_blank_check_result = FLASH_RES_BLANK;
            xSemaphoreGiveFromISR(xSemaphoreFlashAccess, &xHigherPriorityTaskWoken);
        break;
        case FLASH_INT_EVENT_NOT_BLANK:
            g_blank_check_result = FLASH_RES_NOT_BLANK;
            xSemaphoreGiveFromISR(xSemaphoreFlashAccess, &xHigherPriorityTaskWoken);
        break;
        default:
            update_data_flash_control_block.status = DATA_FLASH_UPDATE_STATE_ERROR;
            xSemaphoreGiveFromISR(xSemaphoreFlashAccess, &xHigherPriorityTaskWoken);
        break;
    }
}
/**********************************************************************************************************************
 End of function flashing_callback
 *********************************************************************************************************************/