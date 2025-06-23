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
 * File Name    : cclk.c
 * Description  : Function to execute the AT command (CCLK).
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
 * Function Name  @fn            atc_cclk
 ************************************************************************************************/
e_cellular_err_t atc_cclk(st_cellular_ctrl_t * const p_ctrl, const st_cellular_datetime_t * const p_time)
{
    uint8_t          str[CELLULAR_MAX_ARG_COUNT][10]       = {0};
    const uint8_t *  p_command_arg[CELLULAR_MAX_ARG_COUNT] = {0};
    e_cellular_err_t ret                                   = CELLULAR_SUCCESS;

    sprintf((char *)str[0], "%02d", p_time->year);      // (uint8_t *)->(char *)
    sprintf((char *)str[1], "%02d", p_time->month);     // (uint8_t *)->(char *)
    sprintf((char *)str[2], "%02d", p_time->day);       // (uint8_t *)->(char *)
    sprintf((char *)str[3], "%02d", p_time->hour);      // (uint8_t *)->(char *)
    sprintf((char *)str[4], "%02d", p_time->min);       // (uint8_t *)->(char *)
    sprintf((char *)str[5], "%02d", p_time->sec);       // (uint8_t *)->(char *)
    sprintf((char *)str[6], "%+03d", p_time->timezone); // (uint8_t *)->(char *)

    p_command_arg[0] = str[0];
    p_command_arg[1] = str[1];
    p_command_arg[2] = str[2];
    p_command_arg[3] = str[3];
    p_command_arg[4] = str[4];
    p_command_arg[5] = str[5];
    p_command_arg[6] = str[6];

    atc_generate(p_ctrl->sci_ctrl.atc_buff, gp_at_command[ATC_SET_TIME], p_command_arg);

    ret = cellular_execute_at_command(p_ctrl, p_ctrl->sci_ctrl.atc_timeout, ATC_RETURN_OK, ATC_SET_TIME);

    return ret;
}
/**********************************************************************************************************************
 * End of function atc_cclk
 *********************************************************************************************************************/

/*************************************************************************************************
 * Function Name  @fn            atc_cclk_check
 ************************************************************************************************/
e_cellular_err_t atc_cclk_check(st_cellular_ctrl_t * const p_ctrl)
{
    e_cellular_err_t ret = CELLULAR_SUCCESS;

    atc_generate(p_ctrl->sci_ctrl.atc_buff, gp_at_command[ATC_GET_TIME], NULL);

    ret = cellular_execute_at_command(p_ctrl, p_ctrl->sci_ctrl.atc_timeout, ATC_RETURN_OK, ATC_GET_TIME);

    return ret;
}
/**********************************************************************************************************************
 * End of function atc_cclk_check
 *********************************************************************************************************************/
