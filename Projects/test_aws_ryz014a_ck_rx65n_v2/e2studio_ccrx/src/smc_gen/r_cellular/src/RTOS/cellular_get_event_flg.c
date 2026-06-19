/*
 * Copyright (c) 2025 Renesas Electronics Corporation and/or its affiliates
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/**********************************************************************************************************************
 * File Name    : cellular_get_event_flg.c
 * Description  : Function to retrieve event flags using RTOS features.
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Includes   <System Includes> , "Project Includes"
 *********************************************************************************************************************/
#include "cellular_freertos.h"

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
 * Function Name  @fn            cellular_get_event_flg
 ***************************************************************************/
e_cellular_err_t cellular_get_event_flg(void * const xEventGroup, const uint32_t uxBitsToGet,
                                            const uint32_t xTicksToWait)
{
    e_cellular_err_t ret = CELLULAR_ERR_EVENT_GROUP_INIT;
#if BSP_CFG_RTOS_USED == (1)
    EventBits_t rtos_ret;

    if (CELLULAR_TIME_OUT_MAX_DELAY == xTicksToWait)
    {
        rtos_ret = xEventGroupWaitBits((EventGroupHandle_t)xEventGroup,
                                                (EventBits_t)uxBitsToGet,
                                                pdTRUE,
                                                pdTRUE,
                                                portMAX_DELAY);
    }
    else
    {
        rtos_ret = xEventGroupWaitBits((EventGroupHandle_t)xEventGroup,
                                                (EventBits_t)uxBitsToGet,
                                                pdTRUE,
                                                pdTRUE,
                                                xTicksToWait);
    }

    if (pdFAIL != rtos_ret)
    {
        ret = CELLULAR_SUCCESS;
    }
#elif BSP_CFG_RTOS_USED == (5)
    UINT        rtos_ret;
    uint32_t    flg_ret = 0;

    if (CELLULAR_TIME_OUT_MAX_DELAY == xTicksToWait)
    {
        rtos_ret = tx_event_flags_get((TX_EVENT_FLAGS_GROUP *)xEventGroup,
                                        (ULONG)uxBitsToGet,
                                        (UINT)TX_AND_CLEAR,
                                        (ULONG *)&flg_ret,
                                        (ULONG)TX_WAIT_FOREVER);
    }
    else
    {
        rtos_ret = tx_event_flags_get((TX_EVENT_FLAGS_GROUP *)xEventGroup,
                                        (ULONG)uxBitsToGet,
                                        (UINT)TX_AND_CLEAR,
                                        (ULONG *)&flg_ret,
                                        (ULONG)xTicksToWait);
    }

    if (TX_SUCCESS == rtos_ret)
    {
        ret = CELLULAR_SUCCESS;
    }
#endif
    return ret;
}
/**********************************************************************************************************************
 * End of function cellular_get_event_flg
 *********************************************************************************************************************/
