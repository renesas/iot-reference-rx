/*
 * Copyright (c) 2025 Renesas Electronics Corporation and/or its affiliates
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/**********************************************************************************************************************
 * File Name    : sqnedrx.c
 * Description  : Function to execute the AT command (SQNEDRX).
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Includes   <System Includes> , "Project Includes"
 *********************************************************************************************************************/
#include "at_command.h"
#include "cellular_private_api.h"

/**********************************************************************************************************************
 * Macro definitions
 *********************************************************************************************************************/
#define CHAR_0  (0x30u)
#define BIT_1   (0x01u << 0)
#define BIT_2   (0x01u << 1)
#define BIT_4   (0x01u << 2)
#define BIT_8   (0x01u << 3)

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
 * Function Name  @fn            atc_sqnedrx
 ************************************************************************************************/
e_cellular_err_t atc_sqnedrx(st_cellular_ctrl_t * const p_ctrl, const st_cellular_edrx_config_t * const p_config)
{
    uint8_t          value;
    uint8_t          str[3][5]                             = {0};
    const uint8_t *  p_command_arg[CELLULAR_MAX_ARG_COUNT] = {0};
    e_cellular_err_t ret                                   = CELLULAR_SUCCESS;

    str[0][0] = (uint8_t)((uint8_t)p_config->edrx_mode + (uint8_t)CHAR_0);  //cast

    value     = (uint8_t)(p_config->edrx_cycle);            //cast
    str[1][0] = (uint8_t)(((value & BIT_8) >> 3) + CHAR_0); //cast
    str[1][1] = (uint8_t)(((value & BIT_4) >> 2) + CHAR_0); //cast
    str[1][2] = (uint8_t)(((value & BIT_2) >> 1) + CHAR_0); //cast
    str[1][3] = (uint8_t)((value & BIT_1) + CHAR_0);        //cast

    value     = (uint8_t)(p_config->ptw_cycle);             //cast
    str[2][0] = (uint8_t)(((value & BIT_8) >> 3) + CHAR_0); //cast
    str[2][1] = (uint8_t)(((value & BIT_4) >> 2) + CHAR_0); //cast
    str[2][2] = (uint8_t)(((value & BIT_2) >> 1) + CHAR_0); //cast
    str[2][3] = (uint8_t)((value & BIT_1) + CHAR_0);        //cast

    p_command_arg[0] = str[0];
    p_command_arg[1] = str[1];
    p_command_arg[2] = str[2];

    atc_generate(p_ctrl->sci_ctrl.atc_buff, gp_at_command[ATC_SET_EDRXS], p_command_arg);

    ret = cellular_execute_at_command(p_ctrl, p_ctrl->sci_ctrl.atc_timeout, ATC_RETURN_OK, ATC_SET_EDRXS);

    return ret;
}
/**********************************************************************************************************************
 * End of function atc_sqnedrx
 *********************************************************************************************************************/

/*************************************************************************************************
 * Function Name  @fn            atc_sqnedrx_check
 ************************************************************************************************/
e_cellular_err_t atc_sqnedrx_check(st_cellular_ctrl_t * const p_ctrl)
{
    e_cellular_err_t ret = CELLULAR_SUCCESS;

    atc_generate(p_ctrl->sci_ctrl.atc_buff, gp_at_command[ATC_GET_EDRXS], NULL);

    ret = cellular_execute_at_command(p_ctrl, p_ctrl->sci_ctrl.atc_timeout, ATC_RETURN_OK, ATC_GET_EDRXS);

    return ret;
}
/**********************************************************************************************************************
 * End of function atc_sqnedrx_check
 *********************************************************************************************************************/
