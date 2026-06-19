/*
 * Copyright (c) 2025 Renesas Electronics Corporation and/or its affiliates
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/**********************************************************************************************************************
 * File Name    : cellular_semaphore_ctrl.c
 * Description  : Function to manage semaphore functions.
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Includes   <System Includes> , "Project Includes"
 *********************************************************************************************************************/
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

/****************************************************************************
 * Function Name  @fn            cellular_semaphore_init
 ***************************************************************************/
e_cellular_err_t cellular_semaphore_init(st_cellular_ctrl_t * const p_ctrl)
{
    uint8_t          semaphore_name[20] = {0};
    uint8_t          cnt                = 0;
    e_cellular_err_t ret                = CELLULAR_SUCCESS;

    p_ctrl->at_semaphore = cellular_create_semaphore("atc_semaphore");
    if (NULL == p_ctrl->at_semaphore)
    {
        ret = CELLULAR_ERR_SEMAPHORE_INIT;
    }
    else
    {
        /* WAIT_LOOP */
        for (cnt = 0; cnt < p_ctrl->creatable_socket; cnt++)
        {
            sprintf((char *)semaphore_name,                                 //(uint8_t * ) -> (char *)
                    "socket-%d_semaphore", cnt + CELLULAR_START_SOCKET_NUMBER);
            p_ctrl->p_socket_ctrl[cnt].rx_semaphore
                = cellular_create_semaphore((const char *)semaphore_name);  //(uint8_t * ) -> (const char *)
            if (NULL == p_ctrl->p_socket_ctrl[cnt].rx_semaphore)
            {
                ret = CELLULAR_ERR_SEMAPHORE_INIT;
            }
        }
    }

    return ret;
}
/**********************************************************************************************************************
 * End of function cellular_semaphore_init
 *********************************************************************************************************************/
