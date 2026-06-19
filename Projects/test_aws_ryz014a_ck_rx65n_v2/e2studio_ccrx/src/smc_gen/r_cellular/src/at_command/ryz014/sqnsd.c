/*
 * Copyright (c) 2025 Renesas Electronics Corporation and/or its affiliates
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/**********************************************************************************************************************
 * File Name    : sqnsd.c
 * Description  : Function to execute the AT command (SQNSD).
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
 * Function Name  @fn            atc_sqnsd
 ************************************************************************************************/
e_cellular_err_t atc_sqnsd(st_cellular_ctrl_t * const p_ctrl, const uint8_t socket_no,
                            const uint8_t * const p_ip_addr, const uint16_t port)
{
    uint8_t          str[3][10]                            = {0};
    const uint8_t *  p_command_arg[CELLULAR_MAX_ARG_COUNT] = {0};
    e_cellular_err_t ret                                   = CELLULAR_SUCCESS;

    sprintf((char *)str[0], "%d", socket_no);   // (uint8_t *)->(char *)
    if (CELLULAR_PROTOCOL_TCP == p_ctrl->p_socket_ctrl[socket_no - CELLULAR_START_SOCKET_NUMBER].protocol)
    {
        sprintf((char *)str[1], "%d", 0);   // (uint8_t *)->(char *)
    }
    else
    {
        sprintf((char *)str[1], "%d", 1);   // (uint8_t *)->(char *)
    }
    sprintf((char *)str[2], "%u", port);    // (uint8_t *)->(char *)

    p_command_arg[0] = str[0];
    p_command_arg[1] = str[1];
    p_command_arg[2] = str[2];
    p_command_arg[3] = (uint8_t *)p_ip_addr;    //(const uint8_t *)->(uint8_t *)
    p_command_arg[4] = str[2];

    atc_generate(p_ctrl->sci_ctrl.atc_buff, gp_at_command[ATC_CONNECT_SOCKET], p_command_arg);

    if (p_ctrl->sci_ctrl.atc_timeout >
        ((p_ctrl->p_socket_ctrl[socket_no - CELLULAR_START_SOCKET_NUMBER].connect_timeout * 100)
                + CELLULAR_SOCKETCONNECT_DELAY))
    {
        ret = cellular_execute_at_command(p_ctrl, p_ctrl->sci_ctrl.atc_timeout, ATC_RETURN_OK, ATC_CONNECT_SOCKET);
    }
    else
    {
        ret = cellular_execute_at_command(p_ctrl,
                ((uint32_t)p_ctrl->p_socket_ctrl[socket_no - CELLULAR_START_SOCKET_NUMBER].connect_timeout * 100)//cast
                    + CELLULAR_SOCKETCONNECT_DELAY, ATC_RETURN_OK, ATC_CONNECT_SOCKET);
    }

    return ret;
}
/**********************************************************************************************************************
 * End of function atc_sqnsd
 *********************************************************************************************************************/
