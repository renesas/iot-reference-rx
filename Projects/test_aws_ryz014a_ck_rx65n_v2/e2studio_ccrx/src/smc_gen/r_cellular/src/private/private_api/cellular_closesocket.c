/*
 * Copyright (c) 2025 Renesas Electronics Corporation and/or its affiliates
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/**********************************************************************************************************************
 * File Name    : cellular_closesocket.c
 * Description  : Function to close the socket.
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Includes   <System Includes> , "Project Includes"
 *********************************************************************************************************************/
#include "cellular_private_api.h"
#include "cellular_freertos.h"
#include "at_command.h"

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

/************************************************************************
 * Function Name  @fn            cellular_closesocket
 ***********************************************************************/
e_cellular_err_t cellular_closesocket(st_cellular_ctrl_t * const p_ctrl, const uint8_t socket_no)
{
    uint32_t         preemption = 0;
    e_cellular_err_t ret        = CELLULAR_SUCCESS;

    preemption = cellular_interrupt_disable();

    p_ctrl->p_socket_ctrl[socket_no - CELLULAR_START_SOCKET_NUMBER].ipversion                = 0;
    p_ctrl->p_socket_ctrl[socket_no - CELLULAR_START_SOCKET_NUMBER].protocol                 = 0;
    p_ctrl->p_socket_ctrl[socket_no - CELLULAR_START_SOCKET_NUMBER].receive_count            = 0;
    p_ctrl->p_socket_ctrl[socket_no - CELLULAR_START_SOCKET_NUMBER].receive_num              = 0;
    p_ctrl->p_socket_ctrl[socket_no - CELLULAR_START_SOCKET_NUMBER].receive_unprocessed_size = 0;
    p_ctrl->p_socket_ctrl[socket_no - CELLULAR_START_SOCKET_NUMBER].port                     = 0;

    p_ctrl->p_socket_ctrl[socket_no - CELLULAR_START_SOCKET_NUMBER].socket_status = CELLULAR_SOCKET_STATUS_CLOSED;
    p_ctrl->p_socket_ctrl[socket_no - CELLULAR_START_SOCKET_NUMBER].receive_flg   = CELLULAR_RECEIVE_FLAG_OFF;
    memset(&p_ctrl->p_socket_ctrl[socket_no - CELLULAR_START_SOCKET_NUMBER].ip_addr,
            0x00, sizeof(st_cellular_ipaddr_t));

    cellular_interrupt_enable(preemption);

    return ret;
}
/**********************************************************************************************************************
 * End of function cellular_closesocket
 *********************************************************************************************************************/
