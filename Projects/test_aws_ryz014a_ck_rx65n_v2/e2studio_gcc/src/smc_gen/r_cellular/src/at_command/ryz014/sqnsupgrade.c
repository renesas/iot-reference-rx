/*
 * Copyright (c) 2025 Renesas Electronics Corporation and/or its affiliates
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/**********************************************************************************************************************
 * File Name    : sqnsupgrade.c
 * Description  : Function to execute the AT command (SQNSUPGRADE).
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Includes   <System Includes> , "Project Includes"
 *********************************************************************************************************************/
#include "at_command.h"
#include "cellular_private_api.h"
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

/*************************************************************************************************
 * Function Name  @fn            atc_sqnsupgrade
 ************************************************************************************************/
e_cellular_err_t atc_sqnsupgrade(st_cellular_ctrl_t * const p_ctrl, const uint8_t * const p_url,
                                    const uint8_t mode, const e_cellular_firmupgrade_t command, const uint8_t spid)
{
    uint8_t          str[3][2]                             = {0};
    const uint8_t *  p_command_arg[CELLULAR_MAX_ARG_COUNT] = {0};
    e_cellular_err_t ret                                   = CELLULAR_SUCCESS;

    sprintf((char *)str[0], "%d", mode);    // (uint8_t *)->(char *)
    sprintf((char *)str[1], "%d", command); // (uint8_t *)->(char *)

    p_command_arg[0] = (uint8_t *)p_url;    // (const uint8_t *)->(uint8_t *)
    p_command_arg[1] = str[0];
    p_command_arg[2] = str[1];

    if (0 == spid)
    {
        atc_generate(p_ctrl->sci_ctrl.atc_buff, gp_at_command[ATC_FIRMUPGRADE], p_command_arg);

        if (0 == mode)
        {
            ret = cellular_execute_at_command(p_ctrl, p_ctrl->sci_ctrl.atc_timeout, ATC_RETURN_OK, ATC_FIRMUPGRADE);
        }
        else
        {
            ret = cellular_execute_at_command(p_ctrl, CELLULAR_CFG_UPGRADE_TIME * 1000 * 60,
                                                ATC_RETURN_OK, ATC_FIRMUPGRADE);
        }
    }
    else
    {
        sprintf((char *)str[2], "%d", spid); // (uint8_t *)->(char *)

        p_command_arg[3] = str[2];

        atc_generate(p_ctrl->sci_ctrl.atc_buff, gp_at_command[ATC_FIRMUPGRADE_SSL], p_command_arg);

        if (0 == mode)
        {
            ret = cellular_execute_at_command(p_ctrl, p_ctrl->sci_ctrl.atc_timeout,
                                                ATC_RETURN_OK, ATC_FIRMUPGRADE_SSL);
        }
        else
        {
            ret = cellular_execute_at_command(p_ctrl, CELLULAR_CFG_UPGRADE_TIME * 1000 * 60,
                                                ATC_RETURN_OK, ATC_FIRMUPGRADE_SSL);
        }
    }

    return ret;
}
/**********************************************************************************************************************
 * End of function atc_sqnsupgrade
 *********************************************************************************************************************/

/*************************************************************************************************
 * Function Name  @fn            atc_sqnsupgrade_check
 ************************************************************************************************/
e_cellular_err_t atc_sqnsupgrade_check(st_cellular_ctrl_t * const p_ctrl)
{
    e_cellular_err_t ret = CELLULAR_SUCCESS;

    atc_generate(p_ctrl->sci_ctrl.atc_buff, gp_at_command[ATC_FIRMUPGRADE_CHECK], NULL);

    ret = cellular_execute_at_command(p_ctrl, p_ctrl->sci_ctrl.atc_timeout, ATC_RETURN_OK, ATC_FIRMUPGRADE_CHECK);

    return ret;
}
/**********************************************************************************************************************
 * End of function atc_sqnsupgrade_check
 *********************************************************************************************************************/
