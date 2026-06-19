/*
* Copyright (C) 2016 Renesas Electronics Corporation and/or its affiliates  
*
* SPDX-License-Identifier: BSD-3-Clause
*/
/***********************************************************************************************************************
* File Name    : r_ether_setting_rx65n.c
* Version      : 1.24
* Device       : RX65N
* Description  : Ethernet module device driver
***********************************************************************************************************************/
/**********************************************************************************************************************
* History : DD.MM.YYYY Version  Description
*         : 01.10.2016 1.00     First Release
*         : 01.10.2017 1.13     Removed ether_clear_icu_source function.
*         :                     Added RX65N-2MB support.
*         : 22.11.2019 1.20     Added support for atomic control. 
*         : 20.03.2025 1.24     Changed the disclaimer in program sources
***********************************************************************************************************************/

/***********************************************************************************************************************
Includes   <System Includes> , "Project Includes"
***********************************************************************************************************************/
#include "platform.h"

#include "r_ether_rx_if.h"
#include "src/r_ether_rx_private.h"

#if defined(BSP_MCU_RX65N)
/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/

/***********************************************************************************************************************
Typedef definitions
***********************************************************************************************************************/

/***********************************************************************************************************************
Imported global variables and functions (from other files)
***********************************************************************************************************************/

/***********************************************************************************************************************
Exported global variables (to be accessed by other files)
***********************************************************************************************************************/

/***********************************************************************************************************************
Private global variables and functions
***********************************************************************************************************************/

/* Check the setting values is valid. Please review the setting values
 * in r_ether_rx_config.h if error message is output */
    #if !((ETHER_CFG_AL1_INT_PRIORTY >= 1) && (ETHER_CFG_AL1_INT_PRIORTY <= 15))
        #error "ERROR- ETHER_CFG_AL1_INT_PRIORTY - Interrupt priority level is out of range defined in r_ether_rx_config.h."
    #endif

/***********************************************************************************************************************
* Function Name: ether_enable_icu
* Description  :
* Arguments    : channel -
*                    Ethernet channel number
* Return Value : none
***********************************************************************************************************************/
void ether_enable_icu(uint32_t channel)
{
    volatile uint32_t   dummy;
    bsp_int_ctrl_t int_ctrl;

#if ((R_BSP_VERSION_MAJOR == 5) && (R_BSP_VERSION_MINOR >= 30)) || (R_BSP_VERSION_MAJOR >= 6)
    bsp_int_ctrl_t    ipl_value;
#endif

#if ((R_BSP_VERSION_MAJOR == 5) && (R_BSP_VERSION_MINOR >= 30)) || (R_BSP_VERSION_MAJOR >= 6)
    R_BSP_InterruptControl(BSP_INT_SRC_EMPTY, BSP_INT_CMD_FIT_INTERRUPT_DISABLE, &ipl_value);
#endif
    ICU.GENAL1.BIT.EN4 = 1;
#if ((R_BSP_VERSION_MAJOR == 5) && (R_BSP_VERSION_MINOR >= 30)) || (R_BSP_VERSION_MAJOR >= 6)
    R_BSP_InterruptControl(BSP_INT_SRC_EMPTY, BSP_INT_CMD_FIT_INTERRUPT_ENABLE, &ipl_value);
#endif
    int_ctrl.ipl = ETHER_CFG_AL1_INT_PRIORTY;
    R_BSP_InterruptControl(BSP_INT_SRC_AL1_EDMAC0_EINT0, BSP_INT_CMD_GROUP_INTERRUPT_ENABLE, &int_ctrl);

    dummy = channel;
} /* End of function ether_enable_icu() */

/***********************************************************************************************************************
* Function Name: ether_disable_icu
* Description  :
* Arguments    : channel -
*                    Ethernet channel number
* Return Value : none
***********************************************************************************************************************/
void ether_disable_icu(uint32_t channel)
{
    volatile uint32_t   dummy;

#if ((R_BSP_VERSION_MAJOR == 5) && (R_BSP_VERSION_MINOR >= 30)) || (R_BSP_VERSION_MAJOR >= 6)
    bsp_int_ctrl_t    int_ctrl;
#endif

#if ((R_BSP_VERSION_MAJOR == 5) && (R_BSP_VERSION_MINOR >= 30)) || (R_BSP_VERSION_MAJOR >= 6)
    R_BSP_InterruptControl(BSP_INT_SRC_EMPTY, BSP_INT_CMD_FIT_INTERRUPT_DISABLE, &int_ctrl);
#endif
    ICU.GENAL1.BIT.EN4 = 0;
#if ((R_BSP_VERSION_MAJOR == 5) && (R_BSP_VERSION_MINOR >= 30)) || (R_BSP_VERSION_MAJOR >= 6)
    R_BSP_InterruptControl(BSP_INT_SRC_EMPTY, BSP_INT_CMD_FIT_INTERRUPT_ENABLE, &int_ctrl);
#endif
    R_BSP_InterruptControl(BSP_INT_SRC_AL1_EDMAC0_EINT0, BSP_INT_CMD_GROUP_INTERRUPT_DISABLE, FIT_NO_PTR);

    dummy = channel;
} /* End of function ether_disable_icu() */

/***********************************************************************************************************************
* Function Name: ether_set_phy_mode
* Description  :
* Arguments    : connect -
*                    Ethernet channel number
* Return Value : none
***********************************************************************************************************************/
void ether_set_phy_mode(uint8_t connect)
{
    if (PORT_CONNECT_ET0 == (connect & PORT_CONNECT_ET0))
    {
    #if (ETHER_CFG_MODE_SEL == 0)
        /* MII */
        MPC.PFENET.BIT.PHYMODE0 = 1;
    #elif (ETHER_CFG_MODE_SEL == 1)
        /* RMII */
        MPC.PFENET.BIT.PHYMODE0 = 0;
    #endif
    }

} /* End of function ether_set_phy_mode() */

#endif  /* defined(BSP_MCU_RX65N) */

/* End of File */
