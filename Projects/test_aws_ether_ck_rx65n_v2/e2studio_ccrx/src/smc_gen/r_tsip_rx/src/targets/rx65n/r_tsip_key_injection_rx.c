/*
 * Copyright (c) 2015 Renesas Electronics Corporation and/or its affiliates
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/**********************************************************************************************************************
 * File Name    : r_tsip_key_injection.c
 * Version      : 1.22
 * Description  : Interface definition for the r_tsip_key_injection_rx TSIP module.
 *********************************************************************************************************************/
/**********************************************************************************************************************
 * History : DD.MM.YYYY Version  Description
 *         : 30.09.2017 1.03     First Release
 *         : 28.02.2018 1.04     Change Init/Update/Final API for RX231, add TLS function and 
 *         :                     return values change FIT rules.
 *         : 30.04.2018 1.05     Add TDES, MD5 and RSAES-PKCS1-v1_5 API
 *         : 28.09.2018 1.06     Add RSA Key Generation, AES, TDES, RSA Key update features, RX66T support
 *         : 28.12.2018 1.07     Add RX72T support
 *         : 30.09.2019 1.08     Added support for GCC and IAR compiler, ECC API, RX23W and RX72M
 *         : 31.03.2020 1.09     Added support for AES-CCM, HMAC key generation, ECDH, Key Wrap API, RX66N and RX72N
 *         : 30.06.2020 1.10     Added support for ARC4, ECC(P-384) API
 *         : 30.09.2020 1.11     Added support for DH, ECDHE P-512r1 API, and generalization of KDF.
 *         :                     Added support for Key wrap API with TSIP-Lite.
 *         : 30.06.2021 1.12     Added support for RX23W chip version D
 *         : 31.08.2021 1.13     Added support for RX671
 *         : 22.10.2021 1.14     Added support for TLS1.3
 *         : 31.03.2022 1.15     Added support for TLS1.3(RX72M_RX72N_RX66N)
 *         : 15.09.2022 1.16     Added support for RSA 3k/4k and updated support for TLS1.3
 *         : 20.01.2023 1.17     Added support for TLS1.3 server
 *         : 24.05.2023 1.18     Added support for RX26T
 *         : 30.11.2023 1.19     Update example of Secure Bootloader / Firmware Update
 *         : 28.02.2024 1.20     Applied software workaround of AES-CCM decryption
 *         : 28.06.2024 1.21     Added support for TLS1.2 server
 *         : 10.04.2025 1.22     Added support for RSAES-OAEP, SSH
 *         :                     Updated Firmware Update API
 *********************************************************************************************************************/

/**********************************************************************************************************************
 Includes   <System Includes> , "Project Includes"
 *********************************************************************************************************************/
#include <string.h>
#include "r_tsip_rx_private.h"

/**********************************************************************************************************************
 Macro definitions
 *********************************************************************************************************************/

/***********************************************************************************************************************
* Function Name: R_TSIP_GenerateAes128KeyIndex
*******************************************************************************************************************/ /**
* @details       The API for outputting User Key Generation Information of AES128 bit.
* @param[in]     encrypted_provisioning_key Input the provisioning key includes encrypted CBC/CBC-MAC key for user key
* @param[in]     iv Input the IV for user key CBC encrypt
* @param[in]     encrypted_key Input the user key encrypted with AES128-ECB mode
* @param[out]    key_index Output the user Key Generation Information (13 words) of AES128 bit
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_RESOURCE_CONFLICT: resource conflict
* @retval        TSIP_ERR_FAIL: Internal error occurred.
* @see           R_TSIP_GenerateAes128KeyIndexSub()
* @note          None
*/
e_tsip_err_t R_TSIP_GenerateAes128KeyIndex(uint8_t *encrypted_provisioning_key, uint8_t *iv, uint8_t *encrypted_key,
        tsip_aes_key_index_t *key_index)
{
    e_tsip_err_t error_code = TSIP_SUCCESS;
    uint32_t install_key_ring_index = TSIP_INSTALL_KEY_RING_INDEX;
    error_code = R_TSIP_GenerateAes128KeyIndexSub(&install_key_ring_index,
        /* Casting uint32_t pointer is used for address. */
        (uint32_t*)encrypted_provisioning_key, (uint32_t*)iv, (uint32_t*)encrypted_key, key_index->value);
    if (TSIP_SUCCESS == error_code)
    {
        key_index->type = TSIP_KEY_INDEX_TYPE_AES128;
    }
    else
    {
        key_index->type = TSIP_KEY_INDEX_TYPE_INVALID;
    }
    return error_code;
}
/******************************************
 End of function R_TSIP_GenerateAes128KeyIndex
 ******************************************/

/***********************************************************************************************************************
* Function Name: R_TSIP_GenerateAes256KeyIndex
*******************************************************************************************************************/ /**
* @details       The API for outputting User Key Generation Information of AES128 bit.
* @param[in]     encrypted_provisioning_key Input the provisioning key includes encrypted CBC/CBC-MAC key for user key
* @param[in]     iv Input the IV for user key CBC encrypt
* @param[in]     encrypted_key Input the user key encrypted with AES128-ECB mode
* @param[out]    key_index Output the user Key Generation Information (17 words) of AES256 bit
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_RESOURCE_CONFLICT: resource conflict
* @retval        TSIP_ERR_FAIL: Internal error occurred.
* @see           R_TSIP_GenerateAes256KeyIndexSub()
* @note          None
*/
e_tsip_err_t R_TSIP_GenerateAes256KeyIndex(uint8_t *encrypted_provisioning_key, uint8_t *iv, uint8_t *encrypted_key,
        tsip_aes_key_index_t *key_index)
{
    e_tsip_err_t error_code = TSIP_SUCCESS;
    uint32_t install_key_ring_index = TSIP_INSTALL_KEY_RING_INDEX;
    error_code = R_TSIP_GenerateAes256KeyIndexSub(&install_key_ring_index,
        /* Casting uint32_t pointer is used for address. */
        (uint32_t*)encrypted_provisioning_key, (uint32_t*)iv, (uint32_t*)encrypted_key, key_index->value);
    if (TSIP_SUCCESS == error_code)
    {
        key_index->type = TSIP_KEY_INDEX_TYPE_AES256;
    }
    else
    {
        key_index->type = TSIP_KEY_INDEX_TYPE_INVALID;
    }
    return error_code;
}
/******************************************
 End of function R_TSIP_GenerateAes256KeyIndex
 ******************************************/

#if TSIP_PRV_USE_ARC4
/***********************************************************************************************************************
* Function Name: R_TSIP_GenerateArc4KeyIndex
*******************************************************************************************************************/ /**
* @details       The API for outputting User Key Generation Information of Arc4.
* @param[in]     encrypted_provisioning_key Input the provisioning key includes encrypted CBC/CBC-MAC key for user key
* @param[in]     iv Input the IV for user key CBC encrypt
* @param[in]     encrypted_key Input the user key encrypted with AES128-ECB mode
* @param[out]    key_index Output the user Key Generation Information (73 words) of ARC4.
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_RESOURCE_CONFLICT: resource conflict
* @retval        TSIP_ERR_FAIL: Internal error occurred.
* @see           R_TSIP_GenerateArc4KeyIndexSub()
* @note          None
*/
e_tsip_err_t R_TSIP_GenerateArc4KeyIndex(uint8_t *encrypted_provisioning_key, uint8_t *iv, uint8_t *encrypted_key,
        tsip_arc4_key_index_t *key_index)
{
    e_tsip_err_t error_code = TSIP_SUCCESS;
    uint32_t install_key_ring_index = TSIP_INSTALL_KEY_RING_INDEX;
    error_code = R_TSIP_GenerateArc4KeyIndexSub(&install_key_ring_index,
        /* Casting uint32_t pointer is used for address. */
        (uint32_t*)encrypted_provisioning_key, (uint32_t*)iv, (uint32_t*)encrypted_key, key_index->value);
    if (TSIP_SUCCESS == error_code)
    {
        key_index->type = TSIP_KEY_INDEX_TYPE_ARC4;
    }
    else
    {
        key_index->type = TSIP_KEY_INDEX_TYPE_INVALID;
    }
    return error_code;
}
/*******************************
 End of function R_TSIP_GenerateArc4KeyIndex
 *******************************/
#endif /* TSIP_PRV_USE_ARC4 */

#if TSIP_ECDSA_P192
/***********************************************************************************************************************
* Function Name: R_TSIP_GenerateEccP192PublicKeyIndex
*******************************************************************************************************************/ /**
* @details       Generate ECC P-192 public user key index.
* @param[in]     encrypted_provisioning_key Provisioning key wrapped in DLM.
* @param[in]     iv The initial vector used when encrypted_key was generated.
* @param[in]     encrypted_key ECC P-192 public key encrypted and MAC.
* @param[out]    key_index ECC P-192 public key user key generation information (25 words).
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_RESOURCE_CONFLICT: resource conflict
* @retval        TSIP_ERR_FAIL: Internal error occurred.
* @see           R_TSIP_GenerateEccPublicKeyIndexSub()
* @note          None
*/
e_tsip_err_t R_TSIP_GenerateEccP192PublicKeyIndex(uint8_t *encrypted_provisioning_key, uint8_t *iv,
        uint8_t *encrypted_key, tsip_ecc_public_key_index_t *key_index)
{
    uint32_t indata_cmd = 0;
    e_tsip_err_t error_code = TSIP_SUCCESS;
    uint32_t install_key_ring_index = TSIP_INSTALL_KEY_RING_INDEX;

    indata_cmd = change_endian_long(2); /* P-192 */
    /* Casting uint32_t pointer is used for address. */
    error_code = R_TSIP_GenerateEccPublicKeyIndexSub(&install_key_ring_index, (uint32_t*)encrypted_provisioning_key,
        /* Casting uint32_t pointer is used for address. */
        &indata_cmd, (uint32_t*)iv, (uint32_t*)encrypted_key, (uint32_t*)&key_index->value);
    if (TSIP_SUCCESS == error_code)
    {
        key_index->type = TSIP_KEY_INDEX_TYPE_ECC_P192_PUBLIC;
    }
    else
    {
        key_index->type = TSIP_KEY_INDEX_TYPE_INVALID;
    }

    return error_code;
}
/*******************************
 End of function R_TSIP_GenerateEccP192PublicKeyIndex
 *******************************/
#endif /* TSIP_ECDSA_P192 */

#if TSIP_ECDSA_P224
/***********************************************************************************************************************
* Function Name: R_TSIP_GenerateEccP224PublicKeyIndex
*******************************************************************************************************************/ /**
* @details       Generate ECC P-224 public user key index.
* @param[in]     encrypted_provisioning_key Provisioning key wrapped in DLM.
* @param[in]     iv The initial vector used when encrypted_key was generated.
* @param[in]     encrypted_key ECC P-224 public key encrypted and MAC.
* @param[out]    key_index ECC P-224 public key user key generation information (25 words).
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_RESOURCE_CONFLICT: resource conflict
* @retval        TSIP_ERR_FAIL: Internal error occurred.
* @see           R_TSIP_GenerateEccPublicKeyIndexSub()
* @note          None
*/
e_tsip_err_t R_TSIP_GenerateEccP224PublicKeyIndex(uint8_t *encrypted_provisioning_key, uint8_t *iv,
        uint8_t *encrypted_key, tsip_ecc_public_key_index_t *key_index)
{
    uint32_t indata_cmd = 0;
    e_tsip_err_t error_code = TSIP_SUCCESS;
    uint32_t install_key_ring_index = TSIP_INSTALL_KEY_RING_INDEX;

    indata_cmd = change_endian_long(1); /* P-224 */
    /* Casting uint32_t pointer is used for address. */
    error_code = R_TSIP_GenerateEccPublicKeyIndexSub(&install_key_ring_index, (uint32_t*)encrypted_provisioning_key,
        /* Casting uint32_t pointer is used for address. */
        &indata_cmd, (uint32_t*)iv, (uint32_t*)encrypted_key, (uint32_t*)&key_index->value);
    if (TSIP_SUCCESS == error_code)
    {
        key_index->type = TSIP_KEY_INDEX_TYPE_ECC_P224_PUBLIC;
    }
    else
    {
        key_index->type = TSIP_KEY_INDEX_TYPE_INVALID;
    }

    return error_code;
}
/*******************************
 End of function R_TSIP_GenerateEccP224PublicKeyIndex
 *******************************/
#endif /* TSIP_ECDSA_P224 */

#if TSIP_ECDSA_P256 || TSIP_ECDH_P256
/***********************************************************************************************************************
* Function Name: R_TSIP_GenerateEccP256PublicKeyIndex
*******************************************************************************************************************/ /**
* @details       Generate ECC P-256 public user key index.
* @param[in]     encrypted_provisioning_key Provisioning key wrapped in DLM.
* @param[in]     iv The initial vector used when encrypted_key was generated.
* @param[in]     encrypted_key ECC P-256 public key encrypted and MAC.
* @param[out]    key_index ECC P-256 public key user key generation information (25 words).
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_RESOURCE_CONFLICT: resource conflict
* @retval        TSIP_ERR_FAIL: Internal error occurred.
* @see           R_TSIP_GenerateEccPublicKeyIndexSub()
* @note          None
*/
e_tsip_err_t R_TSIP_GenerateEccP256PublicKeyIndex(uint8_t *encrypted_provisioning_key, uint8_t *iv,
        uint8_t *encrypted_key, tsip_ecc_public_key_index_t *key_index)
{
    uint32_t indata_cmd = 0;
    e_tsip_err_t error_code = TSIP_SUCCESS;
    uint32_t install_key_ring_index = TSIP_INSTALL_KEY_RING_INDEX;

    indata_cmd = change_endian_long(0); /* P-256 */
    /* Casting uint32_t pointer is used for address. */
    error_code = R_TSIP_GenerateEccPublicKeyIndexSub(&install_key_ring_index, (uint32_t*)encrypted_provisioning_key,
        /* Casting uint32_t pointer is used for address. */
        &indata_cmd, (uint32_t*)iv, (uint32_t*)encrypted_key, (uint32_t*)&key_index->value);
    if (TSIP_SUCCESS == error_code)
    {
        key_index->type = TSIP_KEY_INDEX_TYPE_ECC_P256_PUBLIC;
    }
    else
    {
        key_index->type = TSIP_KEY_INDEX_TYPE_INVALID;
    }

    return error_code;
}
/*******************************
 End of function R_TSIP_GenerateEccP256PublicKeyIndex
 *******************************/
#endif /* TSIP_ECDSA_P256 || TSIP_ECDH_P256 */

#if TSIP_ECDSA_P384
/***********************************************************************************************************************
* Function Name: R_TSIP_GenerateEccP384PublicKeyIndex
*******************************************************************************************************************/ /**
* @details       Generate ECC P-384 public user key index.
* @param[in]     encrypted_provisioning_key Provisioning key wrapped in DLM.
* @param[in]     iv The initial vector used when encrypted_key was generated.
* @param[in]     encrypted_key ECC P-384 public key encrypted and MAC.
* @param[out]    key_index ECC P-384 public key user key generation information (33 words).
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_RESOURCE_CONFLICT: resource conflict
* @retval        TSIP_ERR_FAIL: Internal error occurred.
* @see           R_TSIP_GenerateEccP384PublicKeyIndexSub()
* @note          None
*/
e_tsip_err_t R_TSIP_GenerateEccP384PublicKeyIndex(uint8_t *encrypted_provisioning_key, uint8_t *iv,
        uint8_t *encrypted_key, tsip_ecc_public_key_index_t *key_index)
{
    e_tsip_err_t error_code = TSIP_SUCCESS;
    uint32_t install_key_ring_index = TSIP_INSTALL_KEY_RING_INDEX;

    error_code = R_TSIP_GenerateEccP384PublicKeyIndexSub(&install_key_ring_index,
        /* Casting uint32_t pointer is used for address. */
        (uint32_t*)encrypted_provisioning_key, (uint32_t*)iv, (uint32_t*)encrypted_key, (uint32_t*)&key_index->value);
    if (TSIP_SUCCESS == error_code)
    {
        key_index->type = TSIP_KEY_INDEX_TYPE_ECC_P384_PUBLIC;
    }
    else
    {
        key_index->type = TSIP_KEY_INDEX_TYPE_INVALID;
    }

    return error_code;
}
/*******************************
 End of function R_TSIP_GenerateEccP384PublicKeyIndex
 *******************************/
#endif /* TSIP_ECDSA_P384 */

#if TSIP_ECDSA_P192
/***********************************************************************************************************************
* Function Name: R_TSIP_GenerateEccP192PrivateKeyIndex
*******************************************************************************************************************/ /**
* @details       Generate ECC P-192 private user key index.
* @param[in]     encrypted_provisioning_key Provisioning key wrapped in DLM.
* @param[in]     iv The initial vector used when encrypted_key was generated.
* @param[in]     encrypted_key ECC P-192 private key encrypted and MAC.
* @param[out]    key_index ECC P-192 private key user key generation information (17 words).
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_RESOURCE_CONFLICT: resource conflict
* @retval        TSIP_ERR_FAIL: Internal error occurred.
* @see           R_TSIP_GenerateEccPrivateKeyIndexSub()
* @note          None
*/
e_tsip_err_t R_TSIP_GenerateEccP192PrivateKeyIndex(uint8_t *encrypted_provisioning_key, uint8_t *iv,
        uint8_t *encrypted_key, tsip_ecc_private_key_index_t *key_index)
{
    uint32_t indata_cmd = 0;
    e_tsip_err_t error_code = TSIP_SUCCESS;
    uint32_t install_key_ring_index = TSIP_INSTALL_KEY_RING_INDEX;

    indata_cmd = change_endian_long(2); /* P-192 */
    /* Casting uint32_t pointer is used for address. */
    error_code = R_TSIP_GenerateEccPrivateKeyIndexSub(&install_key_ring_index, (uint32_t*)encrypted_provisioning_key,
        /* Casting uint32_t pointer is used for address. */
        &indata_cmd, (uint32_t*)iv, (uint32_t*)encrypted_key, (uint32_t*)&key_index->value);
    if (TSIP_SUCCESS == error_code)
    {
        key_index->type = TSIP_KEY_INDEX_TYPE_ECC_P192_PRIVATE;
    }
    else
    {
        key_index->type = TSIP_KEY_INDEX_TYPE_INVALID;
    }

    return error_code;
}
/*******************************
 End of function R_TSIP_GenerateEccP192PrivateKeyIndex
 *******************************/
#endif /* TSIP_ECDSA_P192 */

#if TSIP_ECDSA_P224
/***********************************************************************************************************************
* Function Name: R_TSIP_GenerateEccP224PrivateKeyIndex
*******************************************************************************************************************/ /**
* @details       Generate ECC P-224 private user key index.
* @param[in]     encrypted_provisioning_key Provisioning key wrapped in DLM.
* @param[in]     iv The initial vector used when encrypted_key was generated.
* @param[in]     encrypted_key ECC P-224 private key encrypted and MAC.
* @param[out]    key_index ECC P-224 private key user key generation information (17 words).
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_RESOURCE_CONFLICT: resource conflict
* @retval        TSIP_ERR_FAIL: Internal error occurred.
* @see           R_TSIP_GenerateEccPrivateKeyIndexSub()
* @note          None
*/
e_tsip_err_t R_TSIP_GenerateEccP224PrivateKeyIndex(uint8_t *encrypted_provisioning_key, uint8_t *iv,
        uint8_t *encrypted_key, tsip_ecc_private_key_index_t *key_index)
{
    uint32_t indata_cmd = 0;
    e_tsip_err_t error_code = TSIP_SUCCESS;
    uint32_t install_key_ring_index = TSIP_INSTALL_KEY_RING_INDEX;

    indata_cmd = change_endian_long(1); /* P-224 */
    /* Casting uint32_t pointer is used for address. */
    error_code = R_TSIP_GenerateEccPrivateKeyIndexSub(&install_key_ring_index, (uint32_t*)encrypted_provisioning_key,
        /* Casting uint32_t pointer is used for address. */
        &indata_cmd, (uint32_t*)iv, (uint32_t*)encrypted_key, (uint32_t*)&key_index->value);
    if (TSIP_SUCCESS == error_code)
    {
        key_index->type = TSIP_KEY_INDEX_TYPE_ECC_P224_PRIVATE;
    }
    else
    {
        key_index->type = TSIP_KEY_INDEX_TYPE_INVALID;
    }

    return error_code;
}
/*******************************
 End of function R_TSIP_GenerateEccP224PrivateKeyIndex
 *******************************/
#endif /* TSIP_ECDSA_P224 */

#if TSIP_ECDSA_P256 || TSIP_ECDH_P256 || TSIP_TLS
/***********************************************************************************************************************
* Function Name: R_TSIP_GenerateEccP256PrivateKeyIndex
*******************************************************************************************************************/ /**
* @details       Generate ECC P-256 private user key index.
* @param[in]     encrypted_provisioning_key Provisioning key wrapped in DLM.
* @param[in]     iv The initial vector used when encrypted_key was generated.
* @param[in]     encrypted_key ECC P-256 private key encrypted and MAC.
* @param[out]    key_index ECC P-256 private key user key generation information (17 words).
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_RESOURCE_CONFLICT: resource conflict
* @retval        TSIP_ERR_FAIL: Internal error occurred.
* @see           R_TSIP_GenerateEccPrivateKeyIndexSub()
* @note          None
*/
e_tsip_err_t R_TSIP_GenerateEccP256PrivateKeyIndex(uint8_t *encrypted_provisioning_key, uint8_t *iv,
        uint8_t *encrypted_key, tsip_ecc_private_key_index_t *key_index)
{
    uint32_t indata_cmd = 0;
    e_tsip_err_t error_code = TSIP_SUCCESS;
    uint32_t install_key_ring_index = TSIP_INSTALL_KEY_RING_INDEX;

    indata_cmd = change_endian_long(0); /* P-256 */
    /* Casting uint32_t pointer is used for address. */
    error_code = R_TSIP_GenerateEccPrivateKeyIndexSub(&install_key_ring_index, (uint32_t*)encrypted_provisioning_key,
        /* Casting uint32_t pointer is used for address. */
        &indata_cmd, (uint32_t*)iv, (uint32_t*)encrypted_key, (uint32_t*)&key_index->value);
    if (TSIP_SUCCESS == error_code)
    {
        key_index->type = TSIP_KEY_INDEX_TYPE_ECC_P256_PRIVATE;
    }
    else
    {
        key_index->type = TSIP_KEY_INDEX_TYPE_INVALID;
    }

    return error_code;
}
/*******************************
 End of function R_TSIP_GenerateEccP256PrivateKeyIndex
 *******************************/
#endif /* TSIP_ECDSA_P256 || TSIP_ECDH_P256 || TSIP_TLS */

#if TSIP_ECDSA_P384
/***********************************************************************************************************************
* Function Name: R_TSIP_GenerateEccP384PrivateKeyIndex
*******************************************************************************************************************/ /**
* @details       Generate ECC P-384 private user key index.
* @param[in]     encrypted_provisioning_key Provisioning key wrapped in DLM.
* @param[in]     iv The initial vector used when encrypted_key was generated.
* @param[in]     encrypted_key ECC P-384 private key encrypted and MAC.
* @param[out]    key_index ECC P-384 private key user key generation information (21 words).
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_RESOURCE_CONFLICT: resource conflict
* @retval        TSIP_ERR_FAIL: Internal error occurred.
* @see           R_TSIP_GenerateEccP384PrivateKeyIndexSub()
* @note          None
*/
e_tsip_err_t R_TSIP_GenerateEccP384PrivateKeyIndex(uint8_t *encrypted_provisioning_key, uint8_t *iv,
        uint8_t *encrypted_key, tsip_ecc_private_key_index_t *key_index)
{
    e_tsip_err_t error_code = TSIP_SUCCESS;
    uint32_t install_key_ring_index = TSIP_INSTALL_KEY_RING_INDEX;

    error_code = R_TSIP_GenerateEccP384PrivateKeyIndexSub(&install_key_ring_index,
        /* Casting uint32_t pointer is used for address. */
        (uint32_t*)encrypted_provisioning_key, (uint32_t*)iv, (uint32_t*)encrypted_key, (uint32_t*)&key_index->value);
    if (TSIP_SUCCESS == error_code)
    {
        key_index->type = TSIP_KEY_INDEX_TYPE_ECC_P384_PRIVATE;
    }
    else
    {
        key_index->type = TSIP_KEY_INDEX_TYPE_INVALID;
    }

    return error_code;
}
/*******************************
 End of function R_TSIP_GenerateEccP384PrivateKeyIndex
 *******************************/
#endif /* TSIP_ECDSA_P384 */

#if TSIP_SHA_1_HMAC
/***********************************************************************************************************************
* Function Name: R_TSIP_GenerateSha1HmacKeyIndex
*******************************************************************************************************************/ /**
* @details       Generate SHA-1 HMAC user key index.
* @param[in]     encrypted_provisioning_key Provisioning key wrapped in DLM.
* @param[in]     iv The initial vector used when encrypted_key was generated.
* @param[in]     encrypted_key SHA-1 HMAC key encrypted and MAC.
* @param[out]    key_index SHA-1 HMAC key user key generation information (17 words).
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_RESOURCE_CONFLICT: resource conflict
* @retval        TSIP_ERR_FAIL: Internal error occurred.
* @see           R_TSIP_GenerateShaHmacKeyIndexSub()
* @note          None
*/
e_tsip_err_t R_TSIP_GenerateSha1HmacKeyIndex(uint8_t *encrypted_provisioning_key, uint8_t *iv,
        uint8_t *encrypted_key, tsip_hmac_sha_key_index_t *key_index)
{
    uint32_t indata_cmd = 0;
    e_tsip_err_t error_code = TSIP_SUCCESS;
    uint32_t install_key_ring_index = TSIP_INSTALL_KEY_RING_INDEX;

    indata_cmd = change_endian_long(0); /* SHA-1 */
    /* Casting uint32_t pointer is used for address. */
    error_code = R_TSIP_GenerateShaHmacKeyIndexSub(&install_key_ring_index, (uint32_t*)encrypted_provisioning_key,
        /* Casting uint32_t pointer is used for address. */
        &indata_cmd, (uint32_t*)iv, (uint32_t*)encrypted_key, key_index->value);
    if (TSIP_SUCCESS == error_code)
    {
        key_index->type = TSIP_KEY_INDEX_TYPE_HMAC_SHA1;
    }
    else
    {
        key_index->type = TSIP_KEY_INDEX_TYPE_INVALID;
    }

    return error_code;
}
/*******************************
 End of function R_TSIP_GenerateSha1HmacKeyIndex
 *******************************/
#endif /* TSIP_SHA_1_HMAC */

#if TSIP_SHA_256_HMAC
/***********************************************************************************************************************
* Function Name: R_TSIP_GenerateSha256HmacKeyIndex
*******************************************************************************************************************/ /**
* @details       Generate SHA-256 HMAC user key index.
* @param[in]     encrypted_provisioning_key Provisioning key wrapped in DLM.
* @param[in]     iv The initial vector used when encrypted_key was generated.
* @param[in]     encrypted_key SHA-256 HMAC key encrypted and MAC.
* @param[out]    key_index SHA-256 HMAC key user key generation information (17 words).
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_RESOURCE_CONFLICT: resource conflict
* @retval        TSIP_ERR_FAIL: Internal error occurred.
* @see           R_TSIP_GenerateShaHmacKeyIndexSub()
* @note          None
*/
e_tsip_err_t R_TSIP_GenerateSha256HmacKeyIndex(uint8_t *encrypted_provisioning_key, uint8_t *iv,
        uint8_t *encrypted_key, tsip_hmac_sha_key_index_t *key_index)
{
    uint32_t indata_cmd = 0;
    e_tsip_err_t error_code = TSIP_SUCCESS;
    uint32_t install_key_ring_index = TSIP_INSTALL_KEY_RING_INDEX;

    indata_cmd = change_endian_long(2); /* SHA-256 */
    /* Casting uint32_t pointer is used for address. */
    error_code = R_TSIP_GenerateShaHmacKeyIndexSub(&install_key_ring_index, (uint32_t*)encrypted_provisioning_key,
        /* Casting uint32_t pointer is used for address. */
        &indata_cmd, (uint32_t*)iv, (uint32_t*)encrypted_key, key_index->value);
    if (TSIP_SUCCESS == error_code)
    {
        key_index->type = TSIP_KEY_INDEX_TYPE_HMAC_SHA256;
    }
    else
    {
        key_index->type = TSIP_KEY_INDEX_TYPE_INVALID;
    }

    return error_code;
}
/*******************************
 End of function R_TSIP_GenerateSha256HmacKeyIndex
 *******************************/
#endif /* TSIP_SHA_256_HMAC */

#if TSIP_PRV_USE_RSA_1024
/***********************************************************************************************************************
* Function Name: R_TSIP_GenerateRsa1024PublicKeyIndex
*******************************************************************************************************************/ /**
* @details       Generate RSA Public key index data
* @param[in]     encrypted_provisioning_key Input the provisioning key includes encrypted CBC/CBC-MAC key for user key
* @param[in]     iv Input the IV for user key CBC encrypt
* @param[in]     encrypted_key Input the user key encrypted with AES128-ECB mode
* @param[out]    key_index Output the user Key Generation Information of RSA1024 bit
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_RESOURCE_CONFLICT: resource conflict
* @retval        TSIP_ERR_FAIL: Internal error occurred.
* @see           R_TSIP_GenerateRsa1024PublicKeyIndexSub()
* @note          None
*/
e_tsip_err_t R_TSIP_GenerateRsa1024PublicKeyIndex(uint8_t *encrypted_provisioning_key, uint8_t *iv,
        uint8_t *encrypted_key, tsip_rsa1024_public_key_index_t *key_index)
{
    e_tsip_err_t error_code = TSIP_SUCCESS;
    uint32_t install_key_ring_index = TSIP_INSTALL_KEY_RING_INDEX;
    error_code = R_TSIP_GenerateRsa1024PublicKeyIndexSub(&install_key_ring_index,
        /* Casting uint32_t pointer is used for address. */
        (uint32_t*)encrypted_provisioning_key, (uint32_t*)iv, (uint32_t*)encrypted_key, (uint32_t*)&key_index->value);
    if (TSIP_SUCCESS == error_code)
    {
        key_index->type = TSIP_KEY_INDEX_TYPE_RSA1024_PUBLIC;
    }
    else
    {
        key_index->type = TSIP_KEY_INDEX_TYPE_INVALID;
    }
    return error_code;
}
/*******************************
 End of function R_TSIP_GenerateRsa1024PublicKeyIndex
 *******************************/
#endif /* TSIP_PRV_USE_RSA_1024 */

#if TSIP_PRV_USE_RSA_2048
/***********************************************************************************************************************
* Function Name: R_TSIP_GenerateRsa2048PublicKeyIndex
*******************************************************************************************************************/ /**
* @details       Generate RSA Public key index data
* @param[in]     encrypted_provisioning_key Input the provisioning key includes encrypted CBC/CBC-MAC key for user key
* @param[in]     iv Input the IV for user key CBC encrypt
* @param[in]     encrypted_key Input the user key encrypted with AES128-ECB mode
* @param[out]    key_index Output the user Key Generation Information of RSA2048 bit
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_RESOURCE_CONFLICT: resource conflict
* @retval        TSIP_ERR_FAIL: Internal error occurred.
* @see           R_TSIP_GenerateRsa2048PublicKeyIndexSub()
* @note          None
*/
e_tsip_err_t R_TSIP_GenerateRsa2048PublicKeyIndex(uint8_t *encrypted_provisioning_key, uint8_t *iv,
        uint8_t *encrypted_key, tsip_rsa2048_public_key_index_t *key_index)
{
    e_tsip_err_t error_code = TSIP_SUCCESS;
    uint32_t install_key_ring_index = TSIP_INSTALL_KEY_RING_INDEX;
    error_code = R_TSIP_GenerateRsa2048PublicKeyIndexSub(&install_key_ring_index,
        /* Casting uint32_t pointer is used for address. */
        (uint32_t*)encrypted_provisioning_key, (uint32_t*)iv, (uint32_t*)encrypted_key, (uint32_t*)&key_index->value);
    if (TSIP_SUCCESS == error_code)
    {
        key_index->type = TSIP_KEY_INDEX_TYPE_RSA2048_PUBLIC;
    }
    else
    {
        key_index->type = TSIP_KEY_INDEX_TYPE_INVALID;
    }
    return error_code;
}
/*******************************
 End of function R_TSIP_GenerateRsa2048PublicKeyIndex
 *******************************/
#endif /* TSIP_PRV_USE_RSA_2048 */

#if TSIP_PRV_USE_RSA_3072
/***********************************************************************************************************************
* Function Name: R_TSIP_GenerateRsa3072PublicKeyIndex
*******************************************************************************************************************/ /**
* @details       Generate RSA Public key index data
* @param[in]     encrypted_provisioning_key Input the provisioning key includes encrypted CBC/CBC-MAC key for user key
* @param[in]     iv Input the IV for user key CBC encrypt
* @param[in]     encrypted_key Input the user key encrypted with AES128-ECB mode
* @param[out]    key_index Output the user Key Generation Information of RSA3072 bit
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_RESOURCE_CONFLICT: resource conflict
* @retval        TSIP_ERR_FAIL: Internal error occurred.
* @see           R_TSIP_GenerateRsa3072PublicKeyIndexSub()
* @note          None
*/
e_tsip_err_t R_TSIP_GenerateRsa3072PublicKeyIndex(uint8_t *encrypted_provisioning_key, uint8_t *iv,
        uint8_t *encrypted_key, tsip_rsa3072_public_key_index_t *key_index)
{
    e_tsip_err_t error_code = TSIP_SUCCESS;
    uint32_t install_key_ring_index = TSIP_INSTALL_KEY_RING_INDEX;
    error_code = R_TSIP_GenerateRsa3072PublicKeyIndexSub(&install_key_ring_index,
        /* Casting uint32_t pointer is used for address. */
        (uint32_t*)encrypted_provisioning_key, (uint32_t*)iv, (uint32_t*)encrypted_key, (uint32_t*)&key_index->value);
    if (TSIP_SUCCESS == error_code)
    {
        key_index->type = TSIP_KEY_INDEX_TYPE_RSA3072_PUBLIC;
    }
    else
    {
        key_index->type = TSIP_KEY_INDEX_TYPE_INVALID;
    }
    return error_code;
}
/*******************************
 End of function R_TSIP_GenerateRsa3072PublicKeyIndex
 *******************************/
#endif /* TSIP_PRV_USE_RSA_3072 */

#if TSIP_PRV_USE_RSA_4096
/***********************************************************************************************************************
* Function Name: R_TSIP_GenerateRsa4096PublicKeyIndex
*******************************************************************************************************************/ /**
* @details       Generate RSA Public key index data
* @param[in]     encrypted_provisioning_key Input the provisioning key includes encrypted CBC/CBC-MAC key for user key
* @param[in]     iv Input the IV for user key CBC encrypt
* @param[in]     encrypted_key Input the user key encrypted with AES128-ECB mode
* @param[out]    key_index Output the user Key Generation Information of RSA4096 bit
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_RESOURCE_CONFLICT: resource conflict
* @retval        TSIP_ERR_FAIL: Internal error occurred.
* @see           R_TSIP_GenerateRsa4096PublicKeyIndexSub()
* @note          None
*/
e_tsip_err_t R_TSIP_GenerateRsa4096PublicKeyIndex(uint8_t *encrypted_provisioning_key, uint8_t *iv,
        uint8_t *encrypted_key, tsip_rsa4096_public_key_index_t *key_index)
{
    e_tsip_err_t error_code = TSIP_SUCCESS;
    uint32_t install_key_ring_index = TSIP_INSTALL_KEY_RING_INDEX;
    error_code = R_TSIP_GenerateRsa4096PublicKeyIndexSub(&install_key_ring_index,
        /* Casting uint32_t pointer is used for address. */
        (uint32_t*)encrypted_provisioning_key, (uint32_t*)iv, (uint32_t*)encrypted_key, (uint32_t*)&key_index->value);
    if (TSIP_SUCCESS == error_code)
    {
        key_index->type = TSIP_KEY_INDEX_TYPE_RSA4096_PUBLIC;
    }
    else
    {
        key_index->type = TSIP_KEY_INDEX_TYPE_INVALID;
    }
    return error_code;
}
/*******************************
 End of function R_TSIP_GenerateRsa4096PublicKeyIndex
 *******************************/
#endif /* TSIP_PRV_USE_RSA_4096 */

#if TSIP_PRV_USE_RSA_1024
/***********************************************************************************************************************
* Function Name: R_TSIP_GenerateRsa1024PrivateKeyIndex
*******************************************************************************************************************/ /**
* @details       Generate RSA Private key index data
* @param[in]     encrypted_provisioning_key Input the provisioning key includes encrypted CBC/CBC-MAC key for user key
* @param[in]     iv Input the IV for user key CBC encrypt
* @param[in]     encrypted_key Input the user key encrypted with AES128-ECB mode
* @param[out]    key_index Output the user Key Generation Information of RSA1024 bit
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_RESOURCE_CONFLICT: resource conflict
* @retval        TSIP_ERR_FAIL: Internal error occurred.
* @see           R_TSIP_GenerateRsa1024PrivateKeyIndexSub()
* @note          None
*/
e_tsip_err_t R_TSIP_GenerateRsa1024PrivateKeyIndex(uint8_t *encrypted_provisioning_key, uint8_t *iv,
        uint8_t *encrypted_key, tsip_rsa1024_private_key_index_t *key_index)
{
    e_tsip_err_t error_code = TSIP_SUCCESS;
    uint32_t install_key_ring_index = TSIP_INSTALL_KEY_RING_INDEX;
    error_code = R_TSIP_GenerateRsa1024PrivateKeyIndexSub(&install_key_ring_index,
        /* Casting uint32_t pointer is used for address. */
        (uint32_t*)encrypted_provisioning_key, (uint32_t*)iv, (uint32_t*)encrypted_key, (uint32_t*)&key_index->value);
    if (TSIP_SUCCESS == error_code)
    {
        key_index->type = TSIP_KEY_INDEX_TYPE_RSA1024_PRIVATE;
    }
    else
    {
        key_index->type = TSIP_KEY_INDEX_TYPE_INVALID;
    }
    return error_code;
}
/*******************************
 End of function R_TSIP_GenerateRsa1024PrivateKeyIndex
 *******************************/
#endif /* TSIP_PRV_USE_RSA_1024 */

#if TSIP_PRV_USE_RSA_2048 || TSIP_TLS
/***********************************************************************************************************************
* Function Name: R_TSIP_GenerateRsa2048PrivateKeyIndex
*******************************************************************************************************************/ /**
* @details       Generate RSA Private key index data
* @param[in]     encrypted_provisioning_key Input the provisioning key includes encrypted CBC/CBC-MAC key for user key
* @param[in]     iv Input the IV for user key CBC encrypt
* @param[in]     encrypted_key Input the user key encrypted with AES128-ECB mode
* @param[out]    key_index Output the user Key Generation Information of RSA2048 bit
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_RESOURCE_CONFLICT: resource conflict
* @retval        TSIP_ERR_FAIL: Internal error occurred.
* @see           R_TSIP_GenerateRsa2048PrivateKeyIndexSub()
* @note          None
*/
e_tsip_err_t R_TSIP_GenerateRsa2048PrivateKeyIndex(uint8_t *encrypted_provisioning_key, uint8_t *iv,
        uint8_t *encrypted_key, tsip_rsa2048_private_key_index_t *key_index)
{
    e_tsip_err_t error_code = TSIP_SUCCESS;
    uint32_t install_key_ring_index = TSIP_INSTALL_KEY_RING_INDEX;
    error_code = R_TSIP_GenerateRsa2048PrivateKeyIndexSub(&install_key_ring_index,
        /* Casting uint32_t pointer is used for address. */
        (uint32_t*)encrypted_provisioning_key, (uint32_t*)iv, (uint32_t*)encrypted_key, (uint32_t*)&key_index->value);
    if (TSIP_SUCCESS == error_code)
    {
        key_index->type = TSIP_KEY_INDEX_TYPE_RSA2048_PRIVATE;
    }
    else
    {
        key_index->type = TSIP_KEY_INDEX_TYPE_INVALID;
    }
    return error_code;
}
/*******************************
 End of function R_TSIP_GenerateRsa2048PrivateKeyIndex
 *******************************/
#endif /* TSIP_PRV_USE_RSA_2048 || TSIP_TLS */

/***********************************************************************************************************************
* Function Name: R_TSIP_GenerateUpdateKeyRingKeyIndex
*******************************************************************************************************************/ /**
* @details       The API for outputting the key generation information of the key update key ring.
* @param[in]     encrypted_provisioning_key Input the provisioning key includes encrypted CBC/CBC-MAC key for user key
* @param[in]     iv Input the IV for user key CBC encrypt
* @param[in]     encrypted_key Input the key update key ring encrypted with AES128-ECB mode
* @param[out]    key_index Output the key update key ring Generation Information (17 words)
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_FAIL: Internal error occurred.
* @retval        TSIP_ERR_RESOURCE_CONFLICT: resource conflict
* @see           R_TSIP_GenerateUpdateKeyRingKeyIndexSub()
* @note          None
*/
e_tsip_err_t R_TSIP_GenerateUpdateKeyRingKeyIndex(uint8_t *encrypted_provisioning_key, uint8_t *iv,
        uint8_t *encrypted_key, tsip_update_key_ring_t *key_index)
{
    e_tsip_err_t error_code = TSIP_SUCCESS;
    uint32_t install_key_ring_index = TSIP_INSTALL_KEY_RING_INDEX;
    error_code = R_TSIP_GenerateUpdateKeyRingKeyIndexSub(&install_key_ring_index,
        /* Casting uint32_t pointer is used for address. */
        (uint32_t*)encrypted_provisioning_key, (uint32_t*)iv, (uint32_t*)encrypted_key, key_index->value);
    if (TSIP_SUCCESS == error_code)
    {
        key_index->type = TSIP_KEY_INDEX_TYPE_UPDATE_KEY_RING;
    }
    else
    {
        key_index->type = TSIP_KEY_INDEX_TYPE_INVALID;
    }
    return error_code;
}
/******************************************
 End of function R_TSIP_GenerateUpdateKeyRingKeyIndex
 ******************************************/

#if TSIP_PRV_USE_TDES
/***********************************************************************************************************************
* Function Name: R_TSIP_GenerateTdesKeyIndex
*******************************************************************************************************************/ /**
* @details       The API for outputting User Key Generation Information
* @param[in]     encrypted_provisioning_key Input the provisioning key includes encrypted CBC/CBC-MAC key for user key
* @param[in]     iv Input the IV for user key CBC encrypt
* @param[in]     encrypted_key Input the user key encrypted with AES128-ECB mode
* @param[out]    key_index Output the user Key Generation Information (17 words) of TDES.
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_RESOURCE_CONFLICT: resource conflict
* @retval        TSIP_ERR_FAIL: Internal error occurred.
* @see           R_TSIP_GenerateTdesKeyIndexSub()
* @note          None
*/
e_tsip_err_t R_TSIP_GenerateTdesKeyIndex(uint8_t *encrypted_provisioning_key, uint8_t *iv, uint8_t *encrypted_key,
        tsip_tdes_key_index_t *key_index)
{
    e_tsip_err_t error_code = TSIP_SUCCESS;
    uint32_t install_key_ring_index = TSIP_INSTALL_KEY_RING_INDEX;
    error_code = R_TSIP_GenerateTdesKeyIndexSub(&install_key_ring_index,
        /* Casting uint32_t pointer is used for address. */
        (uint32_t*)encrypted_provisioning_key, (uint32_t*)iv, (uint32_t*)encrypted_key, key_index->value);
    if (TSIP_SUCCESS == error_code)
    {
        key_index->type = TSIP_KEY_INDEX_TYPE_TDES;
    }
    else
    {
        key_index->type = TSIP_KEY_INDEX_TYPE_INVALID;
    }
    return error_code;
}
/*******************************
 End of function R_TSIP_GenerateTdesKeyIndex
 *******************************/
#endif /* TSIP_PRV_USE_TDES */

#if TSIP_TLS == 1
/***********************************************************************************************************************
* Function Name: R_TSIP_GenerateTlsRsaPublicKeyIndex
*******************************************************************************************************************/ /**
* @details       Generate TLS RSA Public key index data
* @param[in]     encrypted_provisioning_key Input the provisioning key includes encrypted CBC/CBC-MAC key for user key
* @param[in]     iv Input the IV for user key CBC encrypt
* @param[in]     encrypted_key Input the user key encrypted with AES128-ECB mode
* @param[out]    key_index Output the user Key Generation Information (141 words) of RSA2048 bit
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_RESOURCE_CONFLICT: resource conflict
* @retval        TSIP_ERR_FAIL: Internal error occurred.
* @see           R_TSIP_GenerateTlsRsaInstallDataSub()
* @note          None
*/
e_tsip_err_t R_TSIP_GenerateTlsRsaPublicKeyIndex(uint8_t *encrypted_provisioning_key, uint8_t *iv,
        uint8_t *encrypted_key, tsip_tls_ca_certification_public_key_index_t *key_index)
{
    e_tsip_err_t error_code = TSIP_SUCCESS;
    uint32_t install_key_ring_index = TSIP_INSTALL_KEY_RING_INDEX;
    error_code = R_TSIP_GenerateTlsRsaInstallDataSub(&install_key_ring_index,
        /* Casting uint32_t pointer is used for address. */
        (uint32_t*)encrypted_provisioning_key, (uint32_t*)iv, (uint32_t*)encrypted_key, key_index->value);
    if (TSIP_SUCCESS == error_code)
    {
        key_index->type = TSIP_KEY_INDEX_TYPE_TLS_CA_CERTIFICATION_PUBLIC_KEY;
    }
    else
    {
        key_index->type = TSIP_KEY_INDEX_TYPE_INVALID;
    }
    return error_code;
}
/*******************************
 End of function R_TSIP_GenerateTlsRsaPublicKeyIndex
 *******************************/

/***********************************************************************************************************************
* Function Name: R_TSIP_GenerateTlsSVRsaPublicKeyIndex
*******************************************************************************************************************/ /**
* @details       Generate TLS RSA Public key index data for server.
* @param[in]     encrypted_provisioning_key Input the provisioning key includes encrypted CBC/CBC-MAC key for user key
* @param[in]     iv Input the IV for user key CBC encrypt
* @param[in]     encrypted_key Input the user key encrypted with AES128-ECB mode
* @param[out]    key_index Output the user Key Generation Information (141 words) of RSA2048 bit
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_RESOURCE_CONFLICT: resource conflict
* @retval        TSIP_ERR_FAIL: Internal error occurred.
* @see           R_TSIP_GenerateTlsSVRsaInstallDataSub()
* @note          None
*/
e_tsip_err_t R_TSIP_GenerateTlsSVRsaPublicKeyIndex(uint8_t *encrypted_provisioning_key, uint8_t *iv,
        uint8_t *encrypted_key, tsip_tls_ca_certification_public_key_index_t *key_index)
{
    e_tsip_err_t error_code = TSIP_SUCCESS;
    uint32_t install_key_ring_index = TSIP_INSTALL_KEY_RING_INDEX;
    error_code = R_TSIP_GenerateTlsSVRsaInstallDataSub(&install_key_ring_index,
        /* Casting uint32_t pointer is used for address. */
        (uint32_t*)encrypted_provisioning_key, (uint32_t*)iv, (uint32_t*)encrypted_key, key_index->value);
    if (TSIP_SUCCESS == error_code)
    {
        key_index->type = TSIP_KEY_INDEX_TYPE_TLS_CA_CERTIFICATION_PUBLIC_KEY;
    }
    else
    {
        key_index->type = TSIP_KEY_INDEX_TYPE_INVALID;
    }
    return error_code;
}
/*******************************
 End of function R_TSIP_GenerateTlsSVRsaPublicKeyIndex
 *******************************/
#endif /* TSIP_TLS == 1 */

