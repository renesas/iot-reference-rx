/*
 * Copyright (c) 2025 Renesas Electronics Corporation and/or its affiliates
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/**********************************************************************************************************************
 * File Name    : cellular_power_down.c
 * Description  : Function to shut down the module.
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Includes   <System Includes> , "Project Includes"
 *********************************************************************************************************************/
#include "cellular_private_api.h"
#include "at_command.h"
#include "cellular_freertos.h"

/**********************************************************************************************************************
 * Macro definitions
 *********************************************************************************************************************/
#define CELLULAR_SHUTDOWN_LIMIT     (50)

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
 * Function Name  @fn            cellular_power_down
 ***************************************************************************/
e_cellular_err_t cellular_power_down(st_cellular_ctrl_t * const p_ctrl)
{
    volatile uint8_t           flg           = CELLULAR_FLG_OFF;
    uint8_t                    cnt           = 0;
    e_cellular_err_t           ret           = CELLULAR_SUCCESS;
    e_cellular_err_semaphore_t semaphore_ret = CELLULAR_SEMAPHORE_ERR_TAKE;

    semaphore_ret = cellular_take_semaphore(p_ctrl->at_semaphore);
    if (CELLULAR_SEMAPHORE_SUCCESS == semaphore_ret)
    {
        p_ctrl->recv_data = (void *) &flg; //(&uint8_t)->(void *)
        ret               = atc_sqnsshdn(p_ctrl);
        if (CELLULAR_SUCCESS == ret)
        {
            ret = CELLULAR_ERR_MODULE_COM;

            /* WAIT_LOOP */
            do
            {
                if (CELLULAR_FLG_SHUTDOWN == flg)
                {
                    cnt = CELLULAR_SHUTDOWN_LIMIT;
                    ret = CELLULAR_SUCCESS;
                }
                else
                {
                    cellular_delay_task(1000);
                }
                cnt++;
            } while (cnt < CELLULAR_SHUTDOWN_LIMIT);
        }
        p_ctrl->recv_data = NULL;
        cellular_give_semaphore(p_ctrl->at_semaphore);
    }
    else
    {
        ret = CELLULAR_ERR_OTHER_ATCOMMAND_RUNNING;
    }

    return ret;
}
/**********************************************************************************************************************
 End of function cellular_power_down
 *********************************************************************************************************************/
