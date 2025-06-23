/*
* Copyright (c) 2024-2025 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/
/**********************************************************************************************************************
 * File Name    : key_write.c
 * Description  : Write the key used in TSIP to data flash
 *********************************************************************************************************************/
/**********************************************************************************************************************
 * History : DD.MM.YYYY Version Description
 *         : 29.02.2024 1.00 First Release
 *********************************************************************************************************************/
/**********************************************************************************************************************
 Includes   <System Includes> , "Project Includes"
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
