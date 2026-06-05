/*
* Copyright (c) 2023-2025 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/

/**********************************************************************************************************************
 * File Name    : user_init.c
 * Description  : User initialization
 *********************************************************************************************************************/
/**********************************************************************************************************************
 * History : DD.MM.YYYY Version Description
 *         : 29.12.2019 1.00 First Release
 *********************************************************************************************************************/

/******************************************************************************
 Includes   <System Includes> , "Project Includes"
 ******************************************************************************/

/* for using C standard library */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* for using FIT Module */
#include "platform.h"
#include "r_pinset.h"
#include "r_flash_rx_if.h"
#include "r_sci_rx_if.h"
#include "Pin.h"

/**********************************************************************************************************************
Typedef definitions
**********************************************************************************************************************/

/******************************************************************************
 External variables
 ******************************************************************************/

/******************************************************************************
 Private global variables
 ******************************************************************************/
char* txBuffer = NULL;

/******************************************************************************

 External functions
 ******************************************************************************/


void UserInitialization (void);

/******************************************************************************
 Function Name   : UserInitialization
 Description     : Initialize Smart Configurator pins
 Arguments       : none
 Return value    : none
 ******************************************************************************/
void UserInitialization(void)
{
    /* Buffer to contain the whole string of printf before sending to SCI */
    txBuffer = pvPortMalloc((size_t)SCI_CFG_CH5_TX_BUFSIZ * 5);
    if (NULL != txBuffer)
    {
        memset(txBuffer, '\0', SCI_CFG_CH5_TX_BUFSIZ * 5);
    }
    else
    {
        configPRINTF( ( "pvPortMalloc in UserInitialization failed to allocate txBuffer" ) );
    }

    /* enable MCU pins */
    R_Pins_Create();
}
/******************************************************************************
 End of function UserInitialization()
 ******************************************************************************/


/******************************************************************************
 End  Of File
 ******************************************************************************/

