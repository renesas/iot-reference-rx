/*
 * Copyright (c) 2025 Renesas Electronics Corporation and/or its affiliates
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/**********************************************************************************************************************
 * File Name    : cellular_set_event_flg.c
 * Description  : Function to set event flags using RTOS features.
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
 * Function Name  @fn            cellular_set_event_flg
 ***************************************************************************/
e_cellular_err_t cellular_set_event_flg(void * const xEventGroup, const uint32_t uxBitsToSet,
                                        int32_t * pxHigherPriorityTaskWoken)
{
    e_cellular_err_t ret = CELLULAR_ERR_EVENT_GROUP_INIT;
#if BSP_CFG_RTOS_USED == (1)
    BaseType_t rtos_ret;

    rtos_ret = xEventGroupSetBitsFromISR((EventGroupHandle_t)xEventGroup,
                                            (EventBits_t)uxBitsToSet,
                                            (BaseType_t *)pxHigherPriorityTaskWoken);

    if (pdFAIL != rtos_ret)
    {
        ret = CELLULAR_SUCCESS;
    }
#elif BSP_CFG_RTOS_USED == (5)
    (void *)pxHigherPriorityTaskWoken;
    UINT rtos_ret;

    rtos_ret = tx_event_flags_set((TX_EVENT_FLAGS_GROUP *)xEventGroup,
                                    (ULONG)uxBitsToSet,
                                    (UINT)TX_OR);

    if (TX_SUCCESS == rtos_ret)
    {
        ret = CELLULAR_SUCCESS;
    }
#endif
    return ret;
}
/**********************************************************************************************************************
 * End of function cellular_set_event_flg
 *********************************************************************************************************************/
