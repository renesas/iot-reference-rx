/*
 * Copyright (c) 2025 Renesas Electronics Corporation and/or its affiliates
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/**********************************************************************************************************************
 * File Name    : cellular_smcwtx.c
 * Description  : Function to initiate downlink continuous wave services.
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
 * Function Name  @fn            cellular_smcwtx
 ***************************************************************************/
e_cellular_err_t cellular_smcwtx(st_cellular_ctrl_t * const p_ctrl, const uint8_t enable,
                                    const uint16_t earfcn, const int32_t level)
{
    e_cellular_err_t           ret           = CELLULAR_SUCCESS;
    e_cellular_err_semaphore_t semaphore_ret = CELLULAR_SEMAPHORE_ERR_TAKE;

    semaphore_ret = cellular_take_semaphore(p_ctrl->at_semaphore);

    if (CELLULAR_SEMAPHORE_SUCCESS == semaphore_ret)
    {
        ret = atc_cfun(p_ctrl, CELLULAR_MODULE_OPERATING_LEVEL5);
        if (CELLULAR_SUCCESS == ret)
        {
            ret = atc_smcwtx(p_ctrl, enable, earfcn, level);
        }
        cellular_give_semaphore(p_ctrl->at_semaphore);
    }
    else
    {
        ret = CELLULAR_ERR_OTHER_ATCOMMAND_RUNNING;
    }

    return ret;
}
/**********************************************************************************************************************
 End of function cellular_smcwtx
 *********************************************************************************************************************/
