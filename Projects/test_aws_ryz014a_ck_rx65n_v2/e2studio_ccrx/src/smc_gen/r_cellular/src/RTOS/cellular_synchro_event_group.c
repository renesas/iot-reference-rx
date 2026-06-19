/*
 * Copyright (c) 2025 Renesas Electronics Corporation and/or its affiliates
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/**********************************************************************************************************************
 * File Name    : cellular_synchro_event_group.c
 * Description  : Function to synchronize between multiple tasks.
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

/*****************************************************************************************
 * Function Name  @fn            cellular_synchro_event_group
 ****************************************************************************************/
uint32_t cellular_synchro_event_group(void * const xEventGroup,
                                        const uint32_t uxBitsToSet,
                                        const uint32_t uxBitsToWaitFor,
                                        const uint32_t xTicksToWait)
{
    uint32_t ret = 0;

    if (NULL != xEventGroup)
    {
#if BSP_CFG_RTOS_USED == (1)
        if (CELLULAR_TIME_OUT_MAX_DELAY == xTicksToWait)
        {
            ret = xEventGroupSync((EventGroupHandle_t)xEventGroup,
                                    (EventBits_t)uxBitsToSet,
                                    (EventBits_t)uxBitsToWaitFor,
                                    (TickType_t)portMAX_DELAY);
        }
        else
        {
            ret = xEventGroupSync((EventGroupHandle_t)xEventGroup,
                                    (EventBits_t)uxBitsToSet,
                                    (EventBits_t)uxBitsToWaitFor,
                                    (TickType_t)xTicksToWait);
        }
#elif BSP_CFG_RTOS_USED == (5)
        UINT rtos_ret;

        rtos_ret = tx_event_flags_set((TX_EVENT_FLAGS_GROUP *)xEventGroup,
                                        (ULONG)uxBitsToSet,
                                        (UINT)TX_OR);
        if (TX_SUCCESS == rtos_ret)
        {
            if (CELLULAR_TIME_OUT_MAX_DELAY == xTicksToWait)
            {
                rtos_ret = tx_event_flags_get((TX_EVENT_FLAGS_GROUP *)xEventGroup,
                                                (ULONG)uxBitsToWaitFor,
                                                (UINT)TX_AND,
                                                (ULONG *)&ret,
                                                (ULONG)TX_WAIT_FOREVER);
            }
            else
            {
                rtos_ret = tx_event_flags_get((TX_EVENT_FLAGS_GROUP *)xEventGroup,
                                                (ULONG)uxBitsToWaitFor,
                                                (UINT)TX_AND,
                                                (ULONG *)&ret,
                                                (ULONG)xTicksToWait);
            }
        }
#endif
    }

    return ret;
}
/**********************************************************************************************************************
 End of function cellular_synchro_event_group
 *********************************************************************************************************************/
