/**********************************************************************************************************************
 * Copyright (C) 2024 Renesas Electronics Corporation. All rights reserved.
 *
 * DISCLAIMER
 * This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products. No
 * other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all
 * applicable laws, including copyright laws.
 * THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
 * THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED. TO THE MAXIMUM
 * EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES
 * SHALL BE LIABLE FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO THIS
 * SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
 * Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability of
 * this software. By using this software, you agree to the additional terms and conditions found by accessing the
 * following link:
 * http://www.renesas.com/disclaimer
 *********************************************************************************************************************/
/**********************************************************************************************************************
 * File Name    : key_write.h
 * Description  : Write the key used in TSIP to data flash Header
 *********************************************************************************************************************/
/**********************************************************************************************************************
 * History : DD.MM.YYYY Version Description
 *         : 29.02.2024 1.00 First Release
 *********************************************************************************************************************/

/* Standard includes. */
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
