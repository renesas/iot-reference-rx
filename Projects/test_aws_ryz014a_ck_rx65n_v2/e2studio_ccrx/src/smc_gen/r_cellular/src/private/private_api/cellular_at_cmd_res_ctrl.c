/**********************************************************************************************************************
 * DISCLAIMER
 * This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products. No
 * other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all
 * applicable laws, including copyright laws.
 * THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
 * THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED. TO THE MAXIMUM
 * EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES
 * SHALL BE LIABLE FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO
 * THIS SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
 * Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability of
 * this software. By using this software, you agree to the additional terms and conditions found by accessing the
 * following link:
 * http://www.renesas.com/disclaimer
 *
 * Copyright (C) 2024 Renesas Electronics Corporation. All rights reserved.
 *********************************************************************************************************************/
/**********************************************************************************************************************
 * File Name    : cellular_at_cmd_res_ctrl.c
 * Description  : Function to manage AT command responses.
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Includes   <System Includes> , "Project Includes"
 *********************************************************************************************************************/
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

/****************************************************************************
 * Function Name  @fn            cellular_set_atc_number
 ***************************************************************************/
void cellular_set_atc_number(st_cellular_ctrl_t * const p_ctrl, const e_atc_list_t command)
{
    p_ctrl->sci_ctrl.at_command = command;
    p_ctrl->sci_ctrl.atc_flg    = CELLULAR_ATC_RESPONSE_UNCONFIRMED;
    return;
}
/**********************************************************************************************************************
 * End of function cellular_set_atc_number
 *********************************************************************************************************************/

/****************************************************************************
 * Function Name  @fn            cellular_get_atc_response
 *************************************************************************/
e_cellular_atc_return_t cellular_get_atc_response(st_cellular_ctrl_t * const p_ctrl)
{
    e_cellular_atc_return_t res = ATC_RETURN_NONE;

    if (CELLULAR_ATC_RESPONSE_CONFIRMED == p_ctrl->sci_ctrl.atc_flg)
    {
        res = p_ctrl->sci_ctrl.atc_res;
    }

    return res;
}
/**********************************************************************************************************************
 * End of function cellular_get_atc_response
 *********************************************************************************************************************/
