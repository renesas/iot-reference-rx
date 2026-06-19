/*
 * Copyright (c) 2025 Renesas Electronics Corporation and/or its affiliates
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/**********************************************************************************************************************
 * File Name    : cgatt.c
 * Description  : Function to execute the AT command (CGATT).
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Includes   <System Includes> , "Project Includes"
 *********************************************************************************************************************/
#include "at_command.h"
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

/*************************************************************************************************
 * Function Name  @fn            atc_cgatt
 ************************************************************************************************/
e_cellular_err_t atc_cgatt(st_cellular_ctrl_t * const p_ctrl, const e_atc_ap_status_t mode)
{
    uint8_t          str[2]                                = {0};
    const uint8_t *  p_command_arg[CELLULAR_MAX_ARG_COUNT] = {0};
    e_cellular_err_t ret                                   = CELLULAR_SUCCESS;

    sprintf((char *)str, "%d", mode); // (uint8_t *)->(char *)

    p_command_arg[0] = str;

    atc_generate(p_ctrl->sci_ctrl.atc_buff, gp_at_command[ATC_SET_CONNECT_STATUS], p_command_arg);

    if (ATC_AP_CONNECT == mode)
    {
        ret = cellular_execute_at_command(p_ctrl, p_ctrl->sci_ctrl.atc_timeout, ATC_RETURN_OK, ATC_SET_CONNECT_STATUS);
    }
    else
    {
        ret = cellular_execute_at_command(p_ctrl, p_ctrl->sci_ctrl.atc_timeout, ATC_RETURN_OK, ATC_SET_CONNECT_STATUS);
    }

    return ret;
}
/**********************************************************************************************************************
 * End of function atc_cgatt
 *********************************************************************************************************************/


/*************************************************************************************************
 * Function Name  @fn            atc_cgatt_check
 ************************************************************************************************/
e_cellular_err_t atc_cgatt_check(st_cellular_ctrl_t * const p_ctrl)
{
    e_cellular_err_t ret = CELLULAR_SUCCESS;

    atc_generate(p_ctrl->sci_ctrl.atc_buff, gp_at_command[ATC_CONNECT_CHECK], NULL);

    ret = cellular_execute_at_command(p_ctrl, p_ctrl->sci_ctrl.atc_timeout, ATC_RETURN_OK, ATC_CONNECT_CHECK);

    return ret;
}
/**********************************************************************************************************************
 * End of function atc_cgatt_check
 *********************************************************************************************************************/
