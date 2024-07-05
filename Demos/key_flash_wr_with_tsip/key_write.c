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
 * File Name    : key_write.c
 * Description  : Write the key used in TSIP to data flash
 *********************************************************************************************************************/
/**********************************************************************************************************************
 * History : DD.MM.YYYY Version Description
 *         : 29.02.2024 1.00 First Release
 *********************************************************************************************************************/

#include "key_write.h"


/* Function Name: r_tsip_key_write */
/******************************************************************************************************************//**
 * @brief Writing TSIP key index information
 * @param[in] key KVS Key type
 * @param[in] wrKey Pointer to the key data to be copied into the cache.
 * @param[in] keySize Length of the key data.
 * @return result.
 *********************************************************************************************************************/
BaseType_t r_tsip_key_write(KVStoreKey_t key, char * wrKey, uint32_t keySize)
{
    BaseType_t xSuccess = pdFALSE;

    if ((KVS_TSIP_ROOTCA_PUBKEY_ID == key) ||
        (KVS_TSIP_CLIENT_PUBKEY_ID == key) ||
        (KVS_TSIP_CLIENT_PRIKEY_ID == key))
    {
        if (0 < keySize)
        {
            xSuccess = xprvWriteValueToImpl(key, wrKey, keySize);
        }
    }
    return (xSuccess);
}
/**********************************************************************************************************************
* End of function r_tsip_key_write()
**********************************************************************************************************************/

/* Function Name: r_tsip_key_read */
/******************************************************************************************************************//**
 * @brief Reading TSIP key index information
 * @param[in] key KVS Key type
 * @param[in] rdKey Pointer to the key data to be copied into the cache.
 * @return keySize Length of the Read key data.
 *********************************************************************************************************************/
uint32_t r_tsip_key_read(KVStoreKey_t key, char * rdKey)
{
    uint32_t xNvLength = 0;
    uint32_t keysize = 0;

    if ((KVS_TSIP_ROOTCA_PUBKEY_ID == key) ||
        (KVS_TSIP_CLIENT_PUBKEY_ID == key) ||
        (KVS_TSIP_CLIENT_PRIKEY_ID == key))
    {
        xNvLength = xprvGetValueLengthFromImpl(key);
        char * p_xValue = NULL;
        if (0 < xNvLength)
        {
            uint32_t * p_valueLength = &(xNvLength);
            (void)xprvReadValueFromImpl(key, &p_xValue, p_valueLength, *p_valueLength);
            keysize = *p_valueLength;
            if ((NULL != p_xValue) && (0 < keysize))
            {
                memcpy(rdKey, p_xValue, keysize);
                vPortFree(&p_xValue);
                p_xValue = NULL;
            }
            else
            {
                keysize = 0;
            }
        }
    }
    return (keysize);
}
/**********************************************************************************************************************
* End of function r_tsip_key_read()
**********************************************************************************************************************/
