/*
 * Copyright (c) 2025 Renesas Electronics Corporation and/or its affiliates
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/**********************************************************************************************************************
 * File Name    : cellular_create_task.c
 * Description  : Function to create a task using RTOS features.
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Includes   <System Includes> , "Project Includes"
 *********************************************************************************************************************/
#include "cellular_freertos.h"
#include "cellular_private_api.h"

/**********************************************************************************************************************
 * Macro definitions
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Typedef definitions
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Exported global variables
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Private (static) variables and functions
 *********************************************************************************************************************/

/****************************************************************************
 * Function Name  @fn            cellular_create_task
 ***************************************************************************/
#if BSP_CFG_RTOS_USED == (1)
e_cellular_err_t cellular_create_task(void (*pxTaskCode)(void *),
#elif BSP_CFG_RTOS_USED == (5)
e_cellular_err_t cellular_create_task(void (*pxTaskCode)(ULONG),
#endif
                        const char * const pcName,
                        const uint16_t usStackDepth,
                        void * const pvParameters,
                        const uint32_t uxPriority,
                        void * const pxCreatedTask)
{
    e_cellular_err_t ret = CELLULAR_SUCCESS;
#if BSP_CFG_RTOS_USED == (1)
    int32_t rtos_ret;

    rtos_ret = xTaskCreate((TaskFunction_t)pxTaskCode,
                            pcName,
                            (configSTACK_DEPTH_TYPE)usStackDepth,
                            pvParameters,
                            tskIDLE_PRIORITY + (UBaseType_t)uxPriority,
                            (TaskHandle_t *)pxCreatedTask);

    if (pdPASS != rtos_ret)
    {
        ret = CELLULAR_ERR_CREATE_TASK;
    }
#elif BSP_CFG_RTOS_USED == (5)
    UINT tx_ret = 0;
    void ** p_ret = pxCreatedTask;

    *p_ret = cellular_malloc(sizeof(TX_THREAD));
    if (NULL != pxCreatedTask)
    {
        if (0 == strcmp(pcName, CELLULAR_RECV_TASK_NAME))
        {
            tx_ret = tx_thread_create((TX_THREAD *) * p_ret,    //cast
                                        (CHAR *)pcName,         //cast
                                        pxTaskCode,
                                        0ul,
                                        (void *)g_recv_thread,  //cast
                                        usStackDepth,
                                        uxPriority,
                                        uxPriority,
                                        TX_NO_TIME_SLICE,
                                        TX_AUTO_START);
        }
        else if (0 == strcmp(pcName, CELLULAR_RING_TASK_NAME))
        {
            tx_ret = tx_thread_create((TX_THREAD *) * p_ret,    //cast
                                        (CHAR *)pcName,         //cast
                                        pxTaskCode,
                                        0ul,
                                        (void *)g_ring_thread,  //cast
                                        usStackDepth,
                                        uxPriority,
                                        uxPriority,
                                        TX_NO_TIME_SLICE,
                                        TX_AUTO_START);
        }
        else
        {
            R_BSP_NOP();
        }

        if (TX_SUCCESS != tx_ret)
        {
            ret = CELLULAR_ERR_CREATE_TASK;
            cellular_free(*p_ret);
        }
    }
#endif  /* BSP_CFG_RTOS_USED == 1 */

    return ret;
}
/**********************************************************************************************************************
 End of function cellular_create_task
 *********************************************************************************************************************/
