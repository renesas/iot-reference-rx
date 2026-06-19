/*
 * Copyright (c) 2025 Renesas Electronics Corporation and/or its affiliates
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/**********************************************************************************************************************
 * File Name    : sqnsnvw.c
 * Description  : Function to execute the AT command (SQNSNVW).
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
 * Function Name  @fn            atc_sqnsnvw
 ************************************************************************************************/
e_cellular_err_t atc_sqnsnvw(st_cellular_ctrl_t * const p_ctrl, const e_cellular_nvm_type_t data_type, 
                                    const uint8_t index, const uint32_t size)
{
    uint8_t          str[3][12]                            = {0};
    const uint8_t *  p_command_arg[CELLULAR_MAX_ARG_COUNT] = {0};
    e_cellular_err_t ret                                   = CELLULAR_SUCCESS;

    if (CELLULAR_NVM_TYPE_CERTIFICATE == data_type)
    {
        strcpy((char *)&str[0][0], "certificate"); //(uint8_t[])->(char *)
    }
    else
    {
        strcpy((char *)&str[0][0], "privatekey"); //(uint8_t[])->(char *)
    }
    sprintf((char *)str[1], "%d", index);       // (uint8_t *)->(char *)
    sprintf((char *)str[2], "%d", size);        // (uint8_t *)->(char *)

    p_command_arg[0] = str[0];
    p_command_arg[1] = str[1];
    p_command_arg[2] = str[2];

    atc_generate(p_ctrl->sci_ctrl.atc_buff, gp_at_command[ATC_WRITE_CERTIFICATE], p_command_arg);

    ret = cellular_execute_at_command(p_ctrl, p_ctrl->sci_ctrl.atc_timeout,
            ATC_RETURN_OK_GO_SEND, ATC_WRITE_CERTIFICATE);

    return ret;
}
/**********************************************************************************************************************
 * End of function atc_sqnsnvw
 *********************************************************************************************************************/

/*************************************************************************************************
 * Function Name  @fn            atc_sqnsnvw_erase
 ************************************************************************************************/
e_cellular_err_t atc_sqnsnvw_erase(st_cellular_ctrl_t * const p_ctrl, const e_cellular_nvm_type_t data_type,
                                    const uint8_t index)
{
    uint8_t          str[2][12]                            = {0};
    const uint8_t *  p_command_arg[CELLULAR_MAX_ARG_COUNT] = {0};
    e_cellular_err_t ret                                   = CELLULAR_SUCCESS;

    if (CELLULAR_NVM_TYPE_CERTIFICATE == data_type)
    {
        strcpy((char *)&str[0][0], "certificate");  // (uint8_t[])->(char *)
    }
    else
    {
        strcpy((char *)&str[0][0], "privatekey");   // (uint8_t[])->(char *)
    }
    sprintf((char *)str[1], "%d", index);           // (uint8_t *)->(char *)

    p_command_arg[0] = str[0];
    p_command_arg[1] = str[1];

    atc_generate(p_ctrl->sci_ctrl.atc_buff, gp_at_command[ATC_ERASE_CERTIFICATE], p_command_arg);

    ret = cellular_execute_at_command(p_ctrl, p_ctrl->sci_ctrl.atc_timeout, ATC_RETURN_OK, ATC_ERASE_CERTIFICATE);

    return ret;
}
/**********************************************************************************************************************
 * End of function atc_sqnsnvw_erase
 *********************************************************************************************************************/
