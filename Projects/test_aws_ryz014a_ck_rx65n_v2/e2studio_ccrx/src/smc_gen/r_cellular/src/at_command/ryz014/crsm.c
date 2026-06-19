/*
 * Copyright (c) 2025 Renesas Electronics Corporation and/or its affiliates
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/**********************************************************************************************************************
 * File Name    : crsm.c
 * Description  : Function to execute the AT command (CRSM).
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
 * Function Name  @fn            atc_crsm
 ************************************************************************************************/
e_cellular_err_t atc_crsm(st_cellular_ctrl_t * const p_ctrl, const uint8_t command, const uint16_t fileid,
                            const uint8_t para_1, const uint8_t para_2, const uint8_t para_3,
                            const uint8_t * data, const uint8_t * pathid)
{
    uint8_t          str[5][10]                            = {0};
    const uint8_t *  p_command_arg[CELLULAR_MAX_ARG_COUNT] = {0};
    e_cellular_err_t ret                                   = CELLULAR_SUCCESS;

    sprintf((char *)str[0], "%d", command);     // (uint8_t *)->(char *)
    sprintf((char *)str[1], "%d", fileid);      // (uint8_t *)->(char *)
    sprintf((char *)str[2], "%d", para_1);      // (uint8_t *)->(char *)
    sprintf((char *)str[3], "%d", para_2);      // (uint8_t *)->(char *)
    sprintf((char *)str[4], "%d", para_3);      // (uint8_t *)->(char *)

    p_command_arg[0] = str[0];
    p_command_arg[1] = str[1];
    p_command_arg[2] = str[2];
    p_command_arg[3] = str[3];
    p_command_arg[4] = str[4];
    p_command_arg[5] = (uint8_t *)data;     //(const uint8_t *)->(uint8_t *)
    p_command_arg[6] = (uint8_t *)pathid;   //(const uint8_t *)->(uint8_t *)

    atc_generate(p_ctrl->sci_ctrl.atc_buff, gp_at_command[ATC_SEND_COMMAND_TO_SIM], p_command_arg);

    ret = cellular_execute_at_command(p_ctrl, p_ctrl->sci_ctrl.atc_timeout, ATC_RETURN_OK, ATC_SEND_COMMAND_TO_SIM);

    return ret;
}
/**********************************************************************************************************************
 * End of function atc_crsm
 *********************************************************************************************************************/
