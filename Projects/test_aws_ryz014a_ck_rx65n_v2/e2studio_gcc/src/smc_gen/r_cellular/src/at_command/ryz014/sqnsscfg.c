/*
 * Copyright (c) 2025 Renesas Electronics Corporation and/or its affiliates
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/**********************************************************************************************************************
 * File Name    : sqnsscfg.c
 * Description  : Function to execute the AT command (SQNSSCFG).
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
#if (CELLULAR_IMPLEMENT_TYPE == 'B')
/*************************************************************************************************
 * Function Name  @fn            atc_sqnsscfg
 ************************************************************************************************/
e_cellular_err_t atc_sqnsscfg(st_cellular_ctrl_t * const p_ctrl,
                                    const uint8_t socket_no,
                                    const e_cellular_ssl_active_t active,
                                    const uint8_t security_profile_id)
{
    uint8_t          str[3][3]                             = {0};
    const uint8_t *  p_command_arg[CELLULAR_MAX_ARG_COUNT] = {0};
    e_cellular_err_t ret                                   = CELLULAR_SUCCESS;

    sprintf((char *)str[0], "%d", socket_no);    // (uint8_t *)->(char *)
    sprintf((char *)str[1], "%d", active);       // (uint8_t *)->(char *)
    sprintf((char *)str[2], "%d", security_profile_id);        // (uint8_t *)->(char *)

    p_command_arg[0] = str[0];
    p_command_arg[1] = str[1];
    p_command_arg[2] = str[2];

    atc_generate(p_ctrl->sci_ctrl.atc_buff, gp_at_command[ATC_CONFIG_SSL_SOCKET], p_command_arg);

    ret = cellular_execute_at_command(p_ctrl, p_ctrl->sci_ctrl.atc_timeout, ATC_RETURN_OK, ATC_CONFIG_SSL_SOCKET);

    return ret;
}
/**********************************************************************************************************************
 * End of function atc_sqnsscfg
 *********************************************************************************************************************/
#endif /* CELLULAR_IMPLEMENT_TYPE == 'B' */
