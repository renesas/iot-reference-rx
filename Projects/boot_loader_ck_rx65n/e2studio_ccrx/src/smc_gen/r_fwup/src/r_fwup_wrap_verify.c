/**********************************************************************************************************************
 * DISCLAIMER
 * This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products. No
 * other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all
 * applicable laws, including copyright laws.
 * THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
 * THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED. TO THE MAXIMUM
 * EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES
 * SHALL BE LIABLE FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO
 * THIS SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
 * Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability of
 * this software. By using this software, you agree to the additional terms and conditions found by accessing the
 * following link:
 * http://www.renesas.com/disclaimer
 *
 * Copyright (C) 2023 Renesas Electronics Corporation. All rights reserved.
 *********************************************************************************************************************/
/**********************************************************************************************************************
 * File Name    : r_fwup_wrap_verify.c
 * Version      : 2.0
 * Description  : .
 *********************************************************************************************************************/

/**********************************************************************************************************************
 Includes   <System Includes> , "Project Includes"
 *********************************************************************************************************************/
#include "r_fwup_if.h"
#include "r_fwup_wrap_verify.h"

/**** Start user code ****/
/**** End user code   ****/

/**********************************************************************************************************************
 Macro definitions
 *********************************************************************************************************************/
/**** Start user code ****/
#define FWUP_HASH_BYTES        (TC_SHA256_DIGEST_SIZE)
#define FWUP_HASH_BITS         (FWUP_HASH_BYTES * 8)
#define FWUP_CERT_PEM           g_code_signer_public_key
/**** End user code   ****/

/**********************************************************************************************************************
 Local Typedef definitions
 *********************************************************************************************************************/
/**** Start user code ****/
/**** End user code   ****/

/**********************************************************************************************************************
 Exported global variables
 *********************************************************************************************************************/
/**** Start user code ****/
/**** End user code   ****/

S_C_CH_FAR VERIFICATION_SCHEME_ECDSA[]  = "sig-sha256-ecdsa";
S_C_CH_FAR VERIFICATION_SCHEME_SHA[]  = "hash-sha256";

/**********************************************************************************************************************
 Private (static) variables and functions
 *********************************************************************************************************************/
#if (FWUP_CFG_SIGNATURE_VERIFICATION == 0)
static int32_t wrap_extract_pubkey (uint8_t *p_buf);
static C_CH_FAR s_keyheader[] = "-----BEGIN PUBLIC KEY-----";
static C_CH_FAR s_keyfooter[] = "-----END PUBLIC KEY-----";
const uint8_t g_code_signer_public_key[] = CODE_SIGNER_PUBLIC_KEY_PEM;
#endif /* (FWUP_CFG_SIGNATURE_VERIFICATION == 0) */

/**********************************************************************************************************************
 * Function Name: r_fwup_wrap_sha256_init
 * Description  : wrapper function for sha256.
 * Arguments    : vp_ctx
 * Return Value : result
 **********************************************************************************************************************/
int32_t r_fwup_wrap_sha256_init(void * vp_ctx)
{
    /**** Start user code ****/
    return tc_sha256_init((TCSha256State_t)vp_ctx);
    /**** End user code   ****/
}
/**********************************************************************************************************************
 End of function r_fwup_wrap_sha256_init
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: r_fwup_wrap_sha256_update
 * Description  : wrapper function for sha256.
 * Arguments    : vp_ctx
 *                p_data
 *                data_len
 * Return Value : result
 **********************************************************************************************************************/
int32_t r_fwup_wrap_sha256_update(void * vp_ctx, C_U8_FAR *p_data, uint32_t datalen)
{
    /**** Start user code ****/
    return tc_sha256_update((TCSha256State_t)vp_ctx, p_data, datalen);
    /**** End user code   ****/
}
/**********************************************************************************************************************
 End of function r_fwup_wrap_sha256_update
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: r_fwup_wrap_sha256_final
 * Description  : wrapper function for sha256.
 * Arguments    : p_hash
 *                vp_ctx
 * Return Value : result
 **********************************************************************************************************************/
int32_t r_fwup_wrap_sha256_final(uint8_t *p_hash, void * vp_ctx)
{
    /**** Start user code ****/
    return tc_sha256_final(p_hash, (TCSha256State_t)vp_ctx);
    /**** End user code   ****/
}
/**********************************************************************************************************************
 End of function r_fwup_wrap_sha256_final
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: r_fwup_wrap_verify_ecdsa
 * Description  : wrapper function for ECDSA verification.
 * Arguments    : p_hash
 *                p_sig_type
 *                p_sig
 *                sig_size
 * Return Value : 0       verify OK
 *                other   verify NG
 **********************************************************************************************************************/
int32_t r_fwup_wrap_verify_ecdsa(uint8_t *p_hash, uint8_t *p_sig_type, uint8_t *p_sig, uint32_t sig_size)
{
    /**** Start user code ****/
    int32_t x_result = -1;
#if (FWUP_CFG_SIGNATURE_VERIFICATION == 0)
    uint8_t public_key[64];
#endif /* (FWUP_CFG_SIGNATURE_VERIFICATION == 0) */

    /* SHA256 */
    if (0 == STRCMP((const char FWUP_FAR *)p_sig_type, VERIFICATION_SCHEME_SHA))
    {
        return MEMCMP(p_sig, p_hash, sig_size);
    }

#if (FWUP_CFG_SIGNATURE_VERIFICATION == 0)
    /* ECDSA */
    if (0 == STRCMP((const char FWUP_FAR *)p_sig_type, VERIFICATION_SCHEME_ECDSA))
    {
        /* Extract public key */
        if (0 == wrap_extract_pubkey(public_key))
        {
            /* ECC verify */
            if (uECC_verify(public_key, p_hash, FWUP_HASH_BITS, p_sig, uECC_secp256r1()))
            {
                x_result = 0;
            }
        }
    }
#endif /* (FWUP_CFG_SIGNATURE_VERIFICATION == 0) */

    return x_result;

    /**** End user code   ****/
}
/**********************************************************************************************************************
 End of function r_fwup_wrap_verify_ecdsa
 *********************************************************************************************************************/

/**** Start user code ****/

#if (FWUP_CFG_SIGNATURE_VERIFICATION == 0)
/**********************************************************************************************************************
 * Function Name: wrap_extract_pubkey
 * Description  : wrapper module for extracting public key.
 * Arguments    : p_buf
 * Return Value : result
 **********************************************************************************************************************/
static int32_t wrap_extract_pubkey(uint8_t *p_buf)
{
    int32_t result = -1;
    uint8_t binary[128] = {0};
    uint8_t data_length;
    uint8_t FWUP_FAR * p_head;
    uint8_t FWUP_FAR * p_current;
    uint8_t FWUP_FAR * p_tail;

    /* Base64 decode */
    p_head = (uint8_t FWUP_FAR *)g_code_signer_public_key + STRLEN(s_keyheader);
    p_tail =  (uint8_t FWUP_FAR *)STRSTR((char FWUP_FAR *)g_code_signer_public_key, s_keyfooter);
    base64_decode(p_head, binary, p_tail - p_head);

    /* Extract Public key */
    /*
      --- Format ---
      RFC 5480 - Elliptic Curve Cryptography Subject Public Key Information
      SEQUENCE (2 elem)
          SEQUENCE (2 elem)
              OBJECT IDENTIFIER 1.2.840.10045.2.1 ecPublicKey (ANSI X9.62 public key type)
              OBJECT IDENTIFIER 1.2.840.10045.3.1.7 prime256v1 (ANSI X9.62 named elliptic curve)
          BIT STRING (520 bit) 0000010010000001011001110101110110000001011111111011011000000101000000… <- public key
      Example:
        30 : Tag=SEQUENCE
        59 : Length
        30 : Tag=SEQUENCE
        13 : Length
        06 : OBJECT IDENTIFIER (ecPubkey)
        07 : Length
        2A 86 48 CE 3D 02 01
        06 : OBJECT IDENTIFIER (prime256v1)
        08 : Length
        2A 86 48 CE 3D 03 01 07
        03 : Tag=BITSTRING
        42 : Length
        00 : Number of not using bits
        04 : 04 = Not compressed , 02 = Compressed
        -- Public Key Data (256bit) --
        81 67 5D 81 7F B6 05 03 21 7D D6 FE 2F 24 CF 79
        05 5C 81 A6 4E 72 F2 D2 0E EC 56 CC 4E 82 5D 4E
        DB E7 E1 CD 54 8C 58 49 3C 9F 08 A4 E1 54 D5 32
        B3 C1 56 F7 D4 D8 00 4D 4B EF 8B 6F 23 FB 3D AC
     */
    p_current = binary;
    data_length = *(++p_current);
    while (1)
    {
        /* found BIT STRING (maybe public key) */
        if ((0x03 == *p_current) && (0x42 == *(p_current + 1)))
        {
            /* Extract Public key */
            p_current++; /* Move pointer to LENGTH */
            MEMCPY(p_buf, p_current + 3, *p_current -2);
            result = 0;
            break;
        }
        if ((p_current - binary) > data_length)
        {
            break;  /* parsing error */
        }
        p_current++;
    }
    return result;
}
/**********************************************************************************************************************
 End of function wrap_extract_pubkey
 *********************************************************************************************************************/
#endif  /* FWUP_CFG_SIGNATURE_VERIFICATION == 0 */

/**** End user code   ****/

