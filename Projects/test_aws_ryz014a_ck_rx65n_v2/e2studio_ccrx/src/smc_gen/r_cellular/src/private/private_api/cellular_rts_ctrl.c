/*
 * Copyright (c) 2025 Renesas Electronics Corporation and/or its affiliates
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/**********************************************************************************************************************
 * File Name    : cellular_rts_ctrl.c
 * Description  : Function to control the RTS pin.
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Includes   <System Includes> , "Project Includes"
 *********************************************************************************************************************/
#include "cellular_private_api.h"
#include "at_command.h"
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

/************************************************************************
 * Function Name  @fn            cellular_rts_ctrl
 ***********************************************************************/
void cellular_rts_ctrl(const uint8_t lowhigh)
{
    CELLULAR_LOG_DEBUG(("RTS output %d\n", lowhigh));
    CELLULAR_SET_PODR(CELLULAR_CFG_RTS_PORT, CELLULAR_CFG_RTS_PIN) = lowhigh;
}
/**********************************************************************************************************************
 * End of function cellular_rts_ctrl
 *********************************************************************************************************************/

#if CELLULAR_CFG_CTS_SW_CTRL == 1
/************************************************************************
 * Function Name  @fn            cellular_rts_hw_flow_enable
 ***********************************************************************/
void cellular_rts_hw_flow_enable(void)
{
    CELLULAR_LOG_DEBUG(("RTS HW flow enable\n"));

    R_BSP_RegisterProtectDisable(BSP_REG_PROTECT_MPC);
    CELLULAR_SET_PODR(CELLULAR_CFG_RTS_PORT, CELLULAR_CFG_RTS_PIN) = 0;
    CELLULAR_SET_PDR(CELLULAR_CFG_RTS_PORT, CELLULAR_CFG_RTS_PIN)  = 0;
    CELLULAR_SET_PFS(CELLULAR_CFG_RTS_PORT, CELLULAR_CFG_RTS_PIN)  = CELLULAR_CFG_PFS_SET_VALUE;
    CELLULAR_SET_PMR(CELLULAR_CFG_RTS_PORT, CELLULAR_CFG_RTS_PIN)  = 1;
    R_BSP_RegisterProtectEnable(BSP_REG_PROTECT_MPC);
}
/**********************************************************************************************************************
 * End of function cellular_rts_hw_flow_enable
 *********************************************************************************************************************/

/************************************************************************
 * Function Name  @fn            cellular_rts_hw_flow_disable
 ***********************************************************************/
void cellular_rts_hw_flow_disable(void)
{
    CELLULAR_LOG_DEBUG(("RTS HW flow disable\n"));

    R_BSP_RegisterProtectDisable(BSP_REG_PROTECT_MPC);
    CELLULAR_SET_PFS(CELLULAR_CFG_RTS_PORT, CELLULAR_CFG_RTS_PIN) = 0x00U;
    CELLULAR_SET_PMR(CELLULAR_CFG_RTS_PORT, CELLULAR_CFG_RTS_PIN) = 0;
    CELLULAR_SET_PDR(CELLULAR_CFG_RTS_PORT, CELLULAR_CFG_RTS_PIN) = 1;
    R_BSP_RegisterProtectEnable(BSP_REG_PROTECT_MPC);
}
/**********************************************************************************************************************
 * End of function cellular_rts_hw_flow_disable
 *********************************************************************************************************************/
#endif  /* CELLULAR_CFG_CTS_SW_CTRL == 1 */
