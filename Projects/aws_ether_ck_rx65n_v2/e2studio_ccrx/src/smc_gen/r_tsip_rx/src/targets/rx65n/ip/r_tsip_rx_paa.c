/**********************************************************************************************************************
 * DISCLAIMER
 * This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products. No
 * other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all
 * applicable laws, including copyright laws.
 * THIS SOFTWARE IS PROVIDED  AND RENESAS MAKES NO WARRANTIES REGARDING
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
 * Copyright (C) 2015-2024 Renesas Electronics Corporation. All rights reserved.
 *********************************************************************************************************************/
/**********************************************************************************************************************
 * History : DD.MM.YYYY Version  Description
 *         : 27.06.2015 1.00     First Release
 *         : 15.05.2017 1.01     Add AES-CMAC
 *         : 30.09.2017 1.03     Add Init/Update/Final API and SHA, RSA
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
 *********************************************************************************************************************/

/**********************************************************************************************************************
 Includes   <System Includes> , "Project Includes"
 *********************************************************************************************************************/
#include "../r_tsip_rx_private.h"

#if TSIP_PRV_USE_RSA_3072 == 1
/**********************************************************************************************************************
 Macro definitions
 *********************************************************************************************************************/

/**********************************************************************************************************************
 Typedef definitions
 *********************************************************************************************************************/

/**********************************************************************************************************************
 Imported global variables and functions (from other files)
 *********************************************************************************************************************/

/**********************************************************************************************************************
 Exported global variables (to be accessed by other files)
 *********************************************************************************************************************/

/**********************************************************************************************************************
 Private global variables and functions
 *********************************************************************************************************************/

/***********************************************************************************************************************
* Function Name: R_TSIP_GenerateRsa3072PublicKeyIndexSub
*******************************************************************************************************************/ /**
* @details       RX65NHU RSA-3072 Public Key Install
* @param[in]     InData_SharedKeyIndex
* @param[in]     InData_SessionKey
* @param[in]     InData_IV
* @param[in]     InData_InstData
* @param[out]    OutData_KeyIndex
* @retval        TSIP_SUCCESS
* @retval        TSIP_ERR_FAIL
* @retval        TSIP_ERR_RESOURCE_CONFLICT
* @note          None
*/
e_tsip_err_t R_TSIP_GenerateRsa3072PublicKeyIndexSub(uint32_t *InData_SharedKeyIndex, uint32_t *InData_SessionKey, uint32_t *InData_IV, uint32_t *InData_InstData, uint32_t *OutData_KeyIndex)
{
    int32_t iLoop = 0u, jLoop = 0u, kLoop = 0u, oLoop1 = 0u, oLoop2 = 0u, iLoop2 = 0u;
    uint32_t KEY_ADR = 0u, OFS_ADR = 0u;
    (void)iLoop;
    (void)jLoop;
    (void)kLoop;
    (void)iLoop2;
    (void)oLoop1;
    (void)oLoop2;
    (void)KEY_ADR;
    (void)OFS_ADR;
    #if TSIP_MULTI_THREADING == 1
    TSIP_MULTI_THREADING_LOCK_FUNCTION();
    #endif /* TSIP_MULTI_THREADING == 1 */
    if (0x0u != (TSIP.REG_1B4H.WORD & 0x1du))
    {
        #if TSIP_MULTI_THREADING == 1
        TSIP_MULTI_THREADING_UNLOCK_FUNCTION();
        #endif /* TSIP_MULTI_THREADING == 1 */
        return TSIP_ERR_RESOURCE_CONFLICT;
    }
    TSIP.REG_84H.WORD = 0x0000aa01u;
    TSIP.REG_108H.WORD = 0x00000000u;
    TSIP.REG_104H.WORD = 0x00000068u;
    TSIP.REG_E0H.WORD = 0x80010000u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(InData_SharedKeyIndex[0]);
    TSIP.REG_ECH.WORD = 0x38008c00u;
    TSIP.REG_ECH.WORD = 0xfffffff0u;
    TSIP.REG_E0H.WORD = 0x00000080u;
    TSIP.REG_1CH.WORD = 0x00A60000u;
    RX65NHU_func100(change_endian_long(0x19a4cbd4u), change_endian_long(0x8b6db042u), change_endian_long(0x78828672u), change_endian_long(0x0fe55b70u));
    TSIP.REG_1CH.WORD = 0x00400000u;
    TSIP.REG_1D0H.WORD = 0x00000000u;
    if (1u == (TSIP.REG_1CH.BIT.B22))
    {
        RX65NHU_func102(change_endian_long(0x19c3170cu), change_endian_long(0x309539bcu), change_endian_long(0x4724e180u), change_endian_long(0x895b2cfcu));
        TSIP.REG_1B4H.WORD = 0x00000040u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_18H.BIT.B12)
        {
            /* waiting */
        }
        #if TSIP_MULTI_THREADING == 1
        TSIP_MULTI_THREADING_UNLOCK_FUNCTION();
        #endif /* TSIP_MULTI_THREADING == 1 */
        return TSIP_ERR_FAIL;
    }
    else
    {
        RX65NHU_func100(change_endian_long(0x934456cdu), change_endian_long(0x80491d10u), change_endian_long(0x7334cc68u), change_endian_long(0x5fe93038u));
        OFS_ADR = InData_SharedKeyIndex[0]*8;
        TSIP.REG_C4H.WORD = 0x000c3b0cu;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_104H.BIT.B31)
        {
            /* waiting */
        }
        TSIP.REG_100H.WORD = change_endian_long(0x3b74d08au);
        TSIP.REG_C4H.WORD = 0x000c0b0cu;
        TSIP.REG_E0H.WORD = 0x81010000u;
        TSIP.REG_00H.WORD = 0x00002807u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_00H.BIT.B25)
        {
            /* waiting */
        }
        TSIP.REG_1CH.WORD = 0x00001800u;
        TSIP.REG_104H.WORD = 0x00000361u;
        TSIP.REG_A4H.WORD = 0x02fb073du;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_104H.BIT.B31)
        {
            /* waiting */
        }
        TSIP.REG_100H.WORD = S_FLASH[OFS_ADR + 0];
        TSIP.REG_100H.WORD = S_FLASH[OFS_ADR + 1];
        TSIP.REG_100H.WORD = S_FLASH[OFS_ADR + 2];
        TSIP.REG_100H.WORD = S_FLASH[OFS_ADR + 3];
        RX65NHU_func100(change_endian_long(0x1622f4a8u), change_endian_long(0xceb35926u), change_endian_long(0xb9569ce7u), change_endian_long(0x61c1774du));
        TSIP.REG_104H.WORD = 0x00000361u;
        TSIP.REG_A4H.WORD = 0x000007bdu;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_104H.BIT.B31)
        {
            /* waiting */
        }
        TSIP.REG_100H.WORD = S_FLASH[OFS_ADR+4 + 0];
        TSIP.REG_100H.WORD = S_FLASH[OFS_ADR+4 + 1];
        TSIP.REG_100H.WORD = S_FLASH[OFS_ADR+4 + 2];
        TSIP.REG_100H.WORD = S_FLASH[OFS_ADR+4 + 3];
        TSIP.REG_A4H.WORD = 0x00800c45u;
        TSIP.REG_00H.WORD = 0x00001113u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_00H.BIT.B25)
        {
            /* waiting */
        }
        TSIP.REG_1CH.WORD = 0x00001800u;
        TSIP.REG_104H.WORD = 0x00000361u;
        TSIP.REG_A4H.WORD = 0x00087a05u;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_104H.BIT.B31)
        {
            /* waiting */
        }
        TSIP.REG_100H.WORD = InData_SessionKey[0];
        TSIP.REG_100H.WORD = InData_SessionKey[1];
        TSIP.REG_100H.WORD = InData_SessionKey[2];
        TSIP.REG_100H.WORD = InData_SessionKey[3];
        RX65NHU_func100(change_endian_long(0x5a3ad1ddu), change_endian_long(0xf91e9860u), change_endian_long(0xcaa45cbcu), change_endian_long(0x9babdf2eu));
        TSIP.REG_104H.WORD = 0x00000361u;
        TSIP.REG_A4H.WORD = 0x00097a05u;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_104H.BIT.B31)
        {
            /* waiting */
        }
        TSIP.REG_100H.WORD = InData_SessionKey[4];
        TSIP.REG_100H.WORD = InData_SessionKey[5];
        TSIP.REG_100H.WORD = InData_SessionKey[6];
        TSIP.REG_100H.WORD = InData_SessionKey[7];
        RX65NHU_func100(change_endian_long(0x221c36c9u), change_endian_long(0x15ea9c5au), change_endian_long(0x093dc2edu), change_endian_long(0x59198a34u));
        TSIP.REG_104H.WORD = 0x00000361u;
        TSIP.REG_A4H.WORD = 0x00040805u;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_104H.BIT.B31)
        {
            /* waiting */
        }
        TSIP.REG_100H.WORD = InData_IV[0];
        TSIP.REG_100H.WORD = InData_IV[1];
        TSIP.REG_100H.WORD = InData_IV[2];
        TSIP.REG_100H.WORD = InData_IV[3];
        TSIP.REG_104H.WORD = 0x00000051u;
        TSIP.REG_A4H.WORD = 0x00050804u;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_104H.BIT.B31)
        {
            /* waiting */
        }
        TSIP.REG_100H.WORD = change_endian_long(0x00000000u);
        RX65NHU_func103();
        RX65NHU_func100(change_endian_long(0xcdbb3c4du), change_endian_long(0xd5a56185u), change_endian_long(0xa9f4a667u), change_endian_long(0x553c22e5u));
        TSIP.REG_104H.WORD = 0x00000052u;
        TSIP.REG_C4H.WORD = 0x01000c84u;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_104H.BIT.B31)
        {
            /* waiting */
        }
        TSIP.REG_100H.WORD = change_endian_long(0x00000000u);
        TSIP.REG_E0H.WORD = 0x80040000u;
        TSIP.REG_00H.WORD = 0x00008213u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_00H.BIT.B25)
        {
            /* waiting */
        }
        TSIP.REG_1CH.WORD = 0x00001800u;
        TSIP.REG_A4H.WORD = 0x200c3b0du;
        TSIP.REG_E0H.WORD = 0x81040000u;
        TSIP.REG_00H.WORD = 0x00001813u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_00H.BIT.B25)
        {
            /* waiting */
        }
        TSIP.REG_1CH.WORD = 0x00001800u;
        TSIP.REG_A4H.WORD = 0x000c0b0cu;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_104H.BIT.B31)
        {
            /* waiting */
        }
        TSIP.REG_100H.WORD = change_endian_long(0x0819a788u);
        TSIP.REG_104H.WORD = 0x00000052u;
        TSIP.REG_C4H.WORD = 0x00040804u;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_104H.BIT.B31)
        {
            /* waiting */
        }
        TSIP.REG_100H.WORD = change_endian_long(0x00000000u);
        TSIP.REG_00H.WORD = 0x00012103u;
        TSIP.REG_104H.WORD = 0x00000031u;
        TSIP.REG_B0H.WORD = 0x00000f00u;
        TSIP.REG_A4H.WORD = 0x00d0c9a7u;
        TSIP.REG_D0H.WORD = 0x00000f00u;
        TSIP.REG_C4H.WORD = 0x02e0888fu;
        TSIP.REG_04H.WORD = 0x00000202u;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_104H.BIT.B31)
        {
            /* waiting */
        }
        TSIP.REG_100H.WORD = InData_InstData[0];
        TSIP.REG_100H.WORD = InData_InstData[1];
        TSIP.REG_100H.WORD = InData_InstData[2];
        TSIP.REG_100H.WORD = InData_InstData[3];
        for (iLoop = 4; iLoop < 64; iLoop = iLoop + 4)
        {
            /* WAIT_LOOP */
            while (1u != TSIP.REG_104H.BIT.B31)
            {
                /* waiting */
            }
            TSIP.REG_100H.WORD = InData_InstData[iLoop + 0];
            TSIP.REG_100H.WORD = InData_InstData[iLoop + 1];
            TSIP.REG_100H.WORD = InData_InstData[iLoop + 2];
            TSIP.REG_100H.WORD = InData_InstData[iLoop + 3];
            /* WAIT_LOOP */
            while (1u != TSIP.REG_04H.BIT.B30)
            {
                /* waiting */
            }
            OutData_KeyIndex[iLoop + 0] = TSIP.REG_100H.WORD;
            OutData_KeyIndex[iLoop + 1] = TSIP.REG_100H.WORD;
            OutData_KeyIndex[iLoop + 2] = TSIP.REG_100H.WORD;
            OutData_KeyIndex[iLoop + 3] = TSIP.REG_100H.WORD;
        }
        /* WAIT_LOOP */
        while (1u != TSIP.REG_04H.BIT.B30)
        {
            /* waiting */
        }
        OutData_KeyIndex[iLoop + 0] = TSIP.REG_100H.WORD;
        OutData_KeyIndex[iLoop + 1] = TSIP.REG_100H.WORD;
        OutData_KeyIndex[iLoop + 2] = TSIP.REG_100H.WORD;
        OutData_KeyIndex[iLoop + 3] = TSIP.REG_100H.WORD;
        TSIP.REG_104H.WORD = 0x00000000u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_00H.BIT.B25)
        {
            /* waiting */
        }
        TSIP.REG_1CH.WORD = 0x00001800u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_C8H.BIT.B6)
        {
            /* waiting */
        }
        RX65NHU_func100(change_endian_long(0xf3215410u), change_endian_long(0x0f56bcb7u), change_endian_long(0x982aae86u), change_endian_long(0x027b6809u));
        TSIP.REG_00H.WORD = 0x00002183u;
        TSIP.REG_104H.WORD = 0x00000031u;
        TSIP.REG_B0H.WORD = 0x00000700u;
        TSIP.REG_A4H.WORD = 0x00d0c9a7u;
        TSIP.REG_D0H.WORD = 0x00000700u;
        TSIP.REG_C4H.WORD = 0x00e0888fu;
        TSIP.REG_04H.WORD = 0x00000282u;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_104H.BIT.B31)
        {
            /* waiting */
        }
        TSIP.REG_100H.WORD = InData_InstData[iLoop + 0];
        TSIP.REG_100H.WORD = InData_InstData[iLoop + 1];
        TSIP.REG_100H.WORD = InData_InstData[iLoop + 2];
        TSIP.REG_100H.WORD = InData_InstData[iLoop + 3];
        for (iLoop = iLoop + 4; iLoop < 96 ; iLoop = iLoop + 4)
        {
            /* WAIT_LOOP */
            while (1u != TSIP.REG_104H.BIT.B31)
            {
                /* waiting */
            }
            TSIP.REG_100H.WORD = InData_InstData[iLoop + 0];
            TSIP.REG_100H.WORD = InData_InstData[iLoop + 1];
            TSIP.REG_100H.WORD = InData_InstData[iLoop + 2];
            TSIP.REG_100H.WORD = InData_InstData[iLoop + 3];
            /* WAIT_LOOP */
            while (1u != TSIP.REG_04H.BIT.B30)
            {
                /* waiting */
            }
            OutData_KeyIndex[iLoop + 0] = TSIP.REG_100H.WORD;
            OutData_KeyIndex[iLoop + 1] = TSIP.REG_100H.WORD;
            OutData_KeyIndex[iLoop + 2] = TSIP.REG_100H.WORD;
            OutData_KeyIndex[iLoop + 3] = TSIP.REG_100H.WORD;
        }
        /* WAIT_LOOP */
        while (1u != TSIP.REG_04H.BIT.B30)
        {
            /* waiting */
        }
        OutData_KeyIndex[iLoop + 0] = TSIP.REG_100H.WORD;
        OutData_KeyIndex[iLoop + 1] = TSIP.REG_100H.WORD;
        OutData_KeyIndex[iLoop + 2] = TSIP.REG_100H.WORD;
        OutData_KeyIndex[iLoop + 3] = TSIP.REG_100H.WORD;
        TSIP.REG_104H.WORD = 0x00000000u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_00H.BIT.B25)
        {
            /* waiting */
        }
        TSIP.REG_1CH.WORD = 0x00001800u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_C8H.BIT.B6)
        {
            /* waiting */
        }
        RX65NHU_func100(change_endian_long(0x4ddaee42u), change_endian_long(0xceeb82fau), change_endian_long(0x7b1ab48eu), change_endian_long(0x62c00d98u));
        TSIP.REG_104H.WORD = 0x00000361u;
        TSIP.REG_A4H.WORD = 0x00d049a5u;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_104H.BIT.B31)
        {
            /* waiting */
        }
        TSIP.REG_100H.WORD = InData_InstData[96];
        TSIP.REG_100H.WORD = InData_InstData[97];
        TSIP.REG_100H.WORD = InData_InstData[98];
        TSIP.REG_100H.WORD = InData_InstData[99];
        TSIP.REG_C4H.WORD = 0x00e0088du;
        TSIP.REG_00H.WORD = 0x00002113u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_00H.BIT.B25)
        {
            /* waiting */
        }
        TSIP.REG_1CH.WORD = 0x00001800u;
        TSIP.REG_04H.WORD = 0x00000213u;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_04H.BIT.B30)
        {
            /* waiting */
        }
        OutData_KeyIndex[100] = TSIP.REG_100H.WORD;
        OutData_KeyIndex[101] = TSIP.REG_100H.WORD;
        OutData_KeyIndex[102] = TSIP.REG_100H.WORD;
        OutData_KeyIndex[103] = TSIP.REG_100H.WORD;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_C8H.BIT.B6)
        {
            /* waiting */
        }
        RX65NHU_func100(change_endian_long(0x563cbeb3u), change_endian_long(0xc00d00b9u), change_endian_long(0xe7af1273u), change_endian_long(0x844f2307u));
        TSIP.REG_104H.WORD = 0x00000052u;
        TSIP.REG_C4H.WORD = 0x00000c84u;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_104H.BIT.B31)
        {
            /* waiting */
        }
        TSIP.REG_100H.WORD = change_endian_long(0x00000000u);
        TSIP.REG_C4H.WORD = 0x000009cdu;
        TSIP.REG_00H.WORD = 0x00002213u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_00H.BIT.B25)
        {
            /* waiting */
        }
        TSIP.REG_1CH.WORD = 0x00001800u;
        TSIP.REG_04H.WORD = 0x00000213u;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_04H.BIT.B30)
        {
            /* waiting */
        }
        OutData_KeyIndex[104] = TSIP.REG_100H.WORD;
        OutData_KeyIndex[105] = TSIP.REG_100H.WORD;
        OutData_KeyIndex[106] = TSIP.REG_100H.WORD;
        OutData_KeyIndex[107] = TSIP.REG_100H.WORD;
        TSIP.REG_104H.WORD = 0x00000361u;
        TSIP.REG_A4H.WORD = 0x000049a5u;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_104H.BIT.B31)
        {
            /* waiting */
        }
        TSIP.REG_100H.WORD = InData_InstData[100];
        TSIP.REG_100H.WORD = InData_InstData[101];
        TSIP.REG_100H.WORD = InData_InstData[102];
        TSIP.REG_100H.WORD = InData_InstData[103];
        TSIP.REG_A4H.WORD = 0x00900c45u;
        TSIP.REG_00H.WORD = 0x00001113u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_00H.BIT.B25)
        {
            /* waiting */
        }
        TSIP.REG_1CH.WORD = 0x00001800u;
        RX65NHU_func100(change_endian_long(0xfc9982ebu), change_endian_long(0x3896b961u), change_endian_long(0x06023b2au), change_endian_long(0x4c6624fbu));
        TSIP.REG_1CH.WORD = 0x00400000u;
        TSIP.REG_1D0H.WORD = 0x00000000u;
        if (1u == (TSIP.REG_1CH.BIT.B22))
        {
            RX65NHU_func102(change_endian_long(0xe9c350f4u), change_endian_long(0x51ce27d6u), change_endian_long(0xf05b28aeu), change_endian_long(0x39ccb3e2u));
            TSIP.REG_1B4H.WORD = 0x00000040u;
            /* WAIT_LOOP */
            while (0u != TSIP.REG_18H.BIT.B12)
            {
                /* waiting */
            }
            #if TSIP_MULTI_THREADING == 1
            TSIP_MULTI_THREADING_UNLOCK_FUNCTION();
            #endif /* TSIP_MULTI_THREADING == 1 */
            return TSIP_ERR_FAIL;
        }
        else
        {
            RX65NHU_func100(change_endian_long(0x1e23e7a5u), change_endian_long(0xd4be52b8u), change_endian_long(0x06d92b07u), change_endian_long(0x5623cfd3u));
            TSIP.REG_E0H.WORD = 0x81040000u;
            TSIP.REG_04H.WORD = 0x00000612u;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_04H.BIT.B30)
            {
                /* waiting */
            }
            OutData_KeyIndex[0] = TSIP.REG_100H.WORD;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_04H.BIT.B30)
            {
                /* waiting */
            }
            OutData_KeyIndex[1] = TSIP.REG_100H.WORD;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_04H.BIT.B30)
            {
                /* waiting */
            }
            OutData_KeyIndex[2] = TSIP.REG_100H.WORD;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_04H.BIT.B30)
            {
                /* waiting */
            }
            OutData_KeyIndex[3] = TSIP.REG_100H.WORD;
            RX65NHU_func102(change_endian_long(0xa326b831u), change_endian_long(0x0ee6ce7fu), change_endian_long(0x133ba54eu), change_endian_long(0x7dae7029u));
            TSIP.REG_1B4H.WORD = 0x00000040u;
            /* WAIT_LOOP */
            while (0u != TSIP.REG_18H.BIT.B12)
            {
                /* waiting */
            }
            #if TSIP_MULTI_THREADING == 1
            TSIP_MULTI_THREADING_UNLOCK_FUNCTION();
            #endif /* TSIP_MULTI_THREADING == 1 */
            return TSIP_SUCCESS;
        }
    }
}
/**********************************************************************************************************************
 End of function ./input_dir/RX65NHU/RX65NHU_paa_r1.prc
 *********************************************************************************************************************/
#endif /* #if TSIP_PRV_USE_RSA_3072 == 1 */
