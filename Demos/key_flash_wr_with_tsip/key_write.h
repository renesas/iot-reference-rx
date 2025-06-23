/*
* Copyright (c) 2024-2025 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/

/**********************************************************************************************************************
 * File Name    : key_write.h
 * Description  : Write the key used in TSIP to data flash Header
 *********************************************************************************************************************/
/**********************************************************************************************************************
 * History : DD.MM.YYYY Version Description
 *         : 29.02.2024 1.00 First Release
 *********************************************************************************************************************/
/**********************************************************************************************************************
 Includes   <System Includes> , "Project Includes"
 *********************************************************************************************************************/
#include <stdio.h>
#include <string.h>

#include "store.h"

#ifndef KEY_WRITE_H_
#define KEY_WRITE_H_

/* Function Name: r_tsip_key_read */
/**********************************************************************************************************************
 * @brief         Data flash writing function for key data used in TSIP.
 * @retval        read data length.
 *********************************************************************************************************************/
uint32_t r_tsip_key_read (KVStoreKey_t key, char * rdKey);

/* Function Name: r_tsip_key_write */
/**********************************************************************************************************************
 * @brief         Data flash reading function for key data used in TSIP.
 * @retval        result(pdTRUE/pdFALSE)
 *********************************************************************************************************************/
BaseType_t r_tsip_key_write (KVStoreKey_t key, char * wrKey, uint32_t keySize);

#endif /* KEY_WRITE_H_ */
