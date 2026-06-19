/* Generated configuration header file - do not edit */
/***********************************************************************************************************************
* Copyright (c) 2013 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
***********************************************************************************************************************/
/***********************************************************************************************************************
* File Name     : r_s12ad_rx_config.h
* Description   : Configures the 12-bit A/D driver
************************************************************************************************************************
* History : DD.MM.YYYY Version Description
*           22.07.2013 1.00    Initial Release.
*           21.04.2014 1.20    Updated for RX210 advanced features; RX110/63x support.
*           05.04.2019 4.00    Deleted the macro definition of ADC_CFG_PGA_GAIN.
*           20.03.2025 5.41    Changed the disclaimer in program sources.
***********************************************************************************************************************/
#ifndef S12AD_CONFIG_H
#define S12AD_CONFIG_H

/***********************************************************************************************************************
Configuration Options
***********************************************************************************************************************/

/*
 * SPECIFY WHETHER TO INCLUDE CODE FOR API PARAMETER CHECKING
 * Setting to BSP_CFG_PARAM_CHECKING_ENABLE utilizes the system default setting.
 * Setting to 1 includes parameter checking; 0 compiles out parameter checking.
 */
#define ADC_CFG_PARAM_CHECKING_ENABLE   BSP_CFG_PARAM_CHECKING_ENABLE

#endif /* S12AD_CONFIG_H */
