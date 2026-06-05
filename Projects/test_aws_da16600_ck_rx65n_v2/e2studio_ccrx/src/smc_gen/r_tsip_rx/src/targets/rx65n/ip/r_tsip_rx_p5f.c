/*
 * Copyright (c) 2015 Renesas Electronics Corporation and/or its affiliates
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
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
 *         : 10.04.2025 1.22     Added support for RSAES-OAEP, SSH
 *         :                     Updated Firmware Update API
 *********************************************************************************************************************/

/**********************************************************************************************************************
 Includes   <System Includes> , "Project Includes"
 *********************************************************************************************************************/
#include "../r_tsip_rx_private.h"

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
* Function Name: R_TSIP_EcdhP256SshKeyDerivationSub
*******************************************************************************************************************/ /**
* @details       RX65NHU KDF for SSH
* @param[in]     InData_KeyIndexType
* @param[in]     InData_KeyIndex
* @param[in]     InData_PaddedMsg
* @param[in]     InData_PaddedMsgLength
* @param[out]    OutData_KeyIndex1
* @param[out]    OutData_KeyIndex2
* @param[out]    OutData_KeyIndex3
* @param[out]    OutData_KeyIndex4
* @param[out]    OutData_IV1
* @param[out]    OutData_IV2
* @retval        TSIP_SUCCESS
* @retval        TSIP_ERR_FAIL
* @retval        TSIP_ERR_RESOURCE_CONFLICT
* @retval        TSIP_ERR_KEY_SET
* @note          None
*/
e_tsip_err_t R_TSIP_EcdhP256SshKeyDerivationSub(uint32_t *InData_KeyIndexType, uint32_t *InData_KeyIndex, uint32_t *InData_PaddedMsg, uint32_t *InData_PaddedMsgLength, uint32_t *OutData_KeyIndex1, uint32_t *OutData_KeyIndex2, uint32_t *OutData_KeyIndex3, uint32_t *OutData_KeyIndex4, uint32_t *OutData_IV1, uint32_t *OutData_IV2)
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
    if (0x0u != (TSIP.REG_1BCH.WORD & 0x1fu))
    {
        #if TSIP_MULTI_THREADING == 1
        TSIP_MULTI_THREADING_UNLOCK_FUNCTION();
        #endif /* TSIP_MULTI_THREADING == 1 */
        return TSIP_ERR_RESOURCE_CONFLICT;
    }
    TSIP.REG_84H.WORD = 0x00005f01u;
    TSIP.REG_108H.WORD = 0x00000000u;
    TSIP.REG_C4H.WORD = 0x200e1a0du;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = S_RAM[16+0 + 0];
    TSIP.REG_100H.WORD = S_RAM[16+0 + 1];
    TSIP.REG_100H.WORD = S_RAM[16+0 + 2];
    TSIP.REG_100H.WORD = S_RAM[16+0 + 3];
    TSIP.REG_28H.WORD = 0x00870001u;
    TSIP.REG_C4H.WORD = 0x000c2b0du;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = InData_KeyIndex[0];
    TSIP.REG_100H.WORD = InData_KeyIndex[1];
    TSIP.REG_100H.WORD = InData_KeyIndex[2];
    TSIP.REG_100H.WORD = InData_KeyIndex[3];
    TSIP.REG_C4H.WORD = 0x000c0b0cu;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0x718cf94fu);
    TSIP.REG_104H.WORD = 0x00000762u;
    TSIP.REG_D0H.WORD = 0x00000100u;
    TSIP.REG_C4H.WORD = 0x02f087bfu;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = InData_KeyIndex[4];
    TSIP.REG_100H.WORD = InData_KeyIndex[5];
    TSIP.REG_100H.WORD = InData_KeyIndex[6];
    TSIP.REG_100H.WORD = InData_KeyIndex[7];
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = InData_KeyIndex[8];
    TSIP.REG_100H.WORD = InData_KeyIndex[9];
    TSIP.REG_100H.WORD = InData_KeyIndex[10];
    TSIP.REG_100H.WORD = InData_KeyIndex[11];
    TSIP.REG_00H.WORD = 0x00003223u;
    TSIP.REG_2CH.WORD = 0x00000012u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_00H.BIT.B25)
    {
        /* waiting */
    }
    TSIP.REG_1CH.WORD = 0x00001800u;
    TSIP.REG_C4H.WORD = 0x000007bdu;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = InData_KeyIndex[12];
    TSIP.REG_100H.WORD = InData_KeyIndex[13];
    TSIP.REG_100H.WORD = InData_KeyIndex[14];
    TSIP.REG_100H.WORD = InData_KeyIndex[15];
    TSIP.REG_C4H.WORD = 0x00900c45u;
    TSIP.REG_00H.WORD = 0x00002213u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_00H.BIT.B25)
    {
        /* waiting */
    }
    TSIP.REG_1CH.WORD = 0x00001800u;
    RX65NHU_func100(change_endian_long(0xa97e35f9u), change_endian_long(0xd623f96eu), change_endian_long(0x5678a316u), change_endian_long(0x4b4f1cdfu));
    TSIP.REG_1CH.WORD = 0x00400000u;
    TSIP.REG_1D0H.WORD = 0x00000000u;
    if (1u == (TSIP.REG_1CH.BIT.B22))
    {
        RX65NHU_func102(change_endian_long(0x38f61707u), change_endian_long(0xaf8776a0u), change_endian_long(0x360be289u), change_endian_long(0xf973a6b3u));
        TSIP.REG_1BCH.WORD = 0x00000040u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_18H.BIT.B12)
        {
            /* waiting */
        }
        #if TSIP_MULTI_THREADING == 1
        TSIP_MULTI_THREADING_UNLOCK_FUNCTION();
        #endif /* TSIP_MULTI_THREADING == 1 */
        return TSIP_ERR_KEY_SET;
    }
    else
    {
        TSIP.REG_104H.WORD = 0x00000068u;
        TSIP.REG_E0H.WORD = 0x80010360u;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_104H.BIT.B31)
        {
            /* waiting */
        }
        TSIP.REG_100H.WORD = InData_PaddedMsgLength[0];
        TSIP.REG_ECH.WORD = 0x38000f7bu;
        TSIP.REG_E0H.WORD = 0x00000080u;
        TSIP.REG_1CH.WORD = 0x00260000u;
        TSIP.REG_ECH.WORD = 0x38008f60u;
        TSIP.REG_ECH.WORD = 0xfc000000u;
        TSIP.REG_E0H.WORD = 0x00000080u;
        TSIP.REG_1CH.WORD = 0x00A60000u;
        RX65NHU_func100(change_endian_long(0xa9760161u), change_endian_long(0x70aad336u), change_endian_long(0x68be23deu), change_endian_long(0x92f921b9u));
        TSIP.REG_1CH.WORD = 0x00400000u;
        TSIP.REG_1D0H.WORD = 0x00000000u;
        if (1u == (TSIP.REG_1CH.BIT.B22))
        {
            RX65NHU_func102(change_endian_long(0xaeadba4eu), change_endian_long(0x22373c12u), change_endian_long(0x870e62b4u), change_endian_long(0xe019e344u));
            TSIP.REG_1BCH.WORD = 0x00000040u;
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
            TSIP.REG_7CH.WORD = 0x00000011u;
            TSIP.REG_104H.WORD = 0x00000754u;
            TSIP.REG_74H.WORD = 0x00000004u;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_104H.BIT.B31)
            {
                /* waiting */
            }
            TSIP.REG_100H.WORD = change_endian_long(0x6a09e667u);
            TSIP.REG_100H.WORD = change_endian_long(0xbb67ae85u);
            TSIP.REG_100H.WORD = change_endian_long(0x3c6ef372u);
            TSIP.REG_100H.WORD = change_endian_long(0xa54ff53au);
            TSIP.REG_100H.WORD = change_endian_long(0x510e527fu);
            TSIP.REG_100H.WORD = change_endian_long(0x9b05688cu);
            TSIP.REG_100H.WORD = change_endian_long(0x1f83d9abu);
            TSIP.REG_100H.WORD = change_endian_long(0x5be0cd19u);
            TSIP.REG_74H.WORD = 0x00000002u;
            RX65NHU_func100(change_endian_long(0x152d0d40u), change_endian_long(0xa7d2386cu), change_endian_long(0xf4e1b5d1u), change_endian_long(0xf1143284u));
            TSIP.REG_ECH.WORD = 0x0000373bu;
            TSIP.REG_ECH.WORD = 0x0000a720u;
            TSIP.REG_ECH.WORD = 0x00000003u;
            TSIP.REG_ECH.WORD = 0x00026b39u;
            TSIP.REG_ECH.WORD = 0x00003619u;
            TSIP.REG_ECH.WORD = 0x00003200u;
            TSIP.REG_ECH.WORD = 0x00046a10u;
            TSIP.REG_ECH.WORD = 0x00046e10u;
            TSIP.REG_E0H.WORD = 0x81010200u;
            TSIP.REG_04H.WORD = 0x00000606u;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_04H.BIT.B30)
            {
                /* waiting */
            }
            S_RAM[0] = change_endian_long(TSIP.REG_100H.WORD);
            TSIP.REG_ECH.WORD = 0x00000bffu;
            for (iLoop = 0; iLoop < S_RAM[0]; iLoop = iLoop + 16)
            {
                TSIP.REG_104H.WORD = 0x00000f64u;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_104H.BIT.B31)
                {
                    /* waiting */
                }
                TSIP.REG_100H.WORD = InData_PaddedMsg[iLoop + 0];
                TSIP.REG_100H.WORD = InData_PaddedMsg[iLoop + 1];
                TSIP.REG_100H.WORD = InData_PaddedMsg[iLoop + 2];
                TSIP.REG_100H.WORD = InData_PaddedMsg[iLoop + 3];
                TSIP.REG_100H.WORD = InData_PaddedMsg[iLoop + 4];
                TSIP.REG_100H.WORD = InData_PaddedMsg[iLoop + 5];
                TSIP.REG_100H.WORD = InData_PaddedMsg[iLoop + 6];
                TSIP.REG_100H.WORD = InData_PaddedMsg[iLoop + 7];
                TSIP.REG_100H.WORD = InData_PaddedMsg[iLoop + 8];
                TSIP.REG_100H.WORD = InData_PaddedMsg[iLoop + 9];
                TSIP.REG_100H.WORD = InData_PaddedMsg[iLoop + 10];
                TSIP.REG_100H.WORD = InData_PaddedMsg[iLoop + 11];
                TSIP.REG_100H.WORD = InData_PaddedMsg[iLoop + 12];
                TSIP.REG_100H.WORD = InData_PaddedMsg[iLoop + 13];
                TSIP.REG_100H.WORD = InData_PaddedMsg[iLoop + 14];
                TSIP.REG_100H.WORD = InData_PaddedMsg[iLoop + 15];
                TSIP.REG_ECH.WORD = 0x0000a7e0u;
                TSIP.REG_ECH.WORD = 0x00000010u;
                RX65NHU_func101(change_endian_long(0x15eb6759u), change_endian_long(0x2f023e16u), change_endian_long(0x7553e290u), change_endian_long(0x758ca547u));
            }
            RX65NHU_func100(change_endian_long(0xca8fdabbu), change_endian_long(0x8fea4a78u), change_endian_long(0x23733794u), change_endian_long(0x30843021u));
            TSIP.REG_ECH.WORD = 0x38000a1fu;
            TSIP.REG_E0H.WORD = 0x00000080u;
            TSIP.REG_1CH.WORD = 0x00260000u;
            TSIP.REG_1CH.WORD = 0x00402000u;
            TSIP.REG_E0H.WORD = 0x81010320u;
            TSIP.REG_04H.WORD = 0x00000606u;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_04H.BIT.B30)
            {
                /* waiting */
            }
            S_RAM[0] = change_endian_long(TSIP.REG_100H.WORD);
            TSIP.REG_ECH.WORD = 0x00000a10u;
            TSIP.REG_ECH.WORD = 0x0000b6c0u;
            TSIP.REG_ECH.WORD = 0x00000004u;
            for (iLoop = iLoop; iLoop < S_RAM[0]; iLoop = iLoop + 1)
            {
                TSIP.REG_104H.WORD = 0x00000068u;
                TSIP.REG_E0H.WORD = 0x80810010u;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_104H.BIT.B31)
                {
                    /* waiting */
                }
                TSIP.REG_100H.WORD = InData_PaddedMsg[iLoop + 0];
                TSIP.REG_ECH.WORD = 0x00002616u;
                TSIP.REG_ECH.WORD = 0x00002fe0u;
                RX65NHU_func101(change_endian_long(0xa32364a5u), change_endian_long(0xe8d181b0u), change_endian_long(0x86bb7c36u), change_endian_long(0x96b19043u));
            }
            RX65NHU_func100(change_endian_long(0x8cdc05c7u), change_endian_long(0x36f9dcbfu), change_endian_long(0xede9d4a0u), change_endian_long(0x4eebbaa0u));
            TSIP.REG_ECH.WORD = 0x38000b3fu;
            TSIP.REG_E0H.WORD = 0x00000080u;
            TSIP.REG_1CH.WORD = 0x00260000u;
            TSIP.REG_1CH.WORD = 0x00402000u;
            TSIP.REG_ECH.WORD = 0x0000375bu;
            TSIP.REG_ECH.WORD = 0x00003340u;
            TSIP.REG_ECH.WORD = 0x001a6f5au;
            TSIP.REG_ECH.WORD = 0x001a6b5au;
            TSIP.REG_ECH.WORD = 0x00002f40u;
            TSIP.REG_ECH.WORD = 0x0000b700u;
            TSIP.REG_ECH.WORD = 0x00000004u;
            TSIP.REG_ECH.WORD = 0x00000b9cu;
            TSIP.REG_ECH.WORD = 0x00000a52u;
            TSIP.REG_2CH.WORD = 0x00000022u;
            for (iLoop = 0; iLoop < 32; iLoop = iLoop + 1)
            {
                TSIP.REG_E0H.WORD = 0x80010000u;
                TSIP.REG_00H.WORD = 0x0800800fu;
                /* WAIT_LOOP */
                while (0u != TSIP.REG_00H.BIT.B25)
                {
                    /* waiting */
                }
                TSIP.REG_1CH.WORD = 0x00001800u;
                TSIP.REG_00H.WORD = 0x08008307u;
                /* WAIT_LOOP */
                while (0u != TSIP.REG_00H.BIT.B25)
                {
                    /* waiting */
                }
                TSIP.REG_1CH.WORD = 0x00001800u;
                TSIP.REG_ECH.WORD = 0x30000f9cu;
                TSIP.REG_ECH.WORD = 0x00030020u;
                TSIP.REG_ECH.WORD = 0x30000c00u;
                TSIP.REG_ECH.WORD = 0x00020020u;
                TSIP.REG_ECH.WORD = 0x3000d807u;
                TSIP.REG_ECH.WORD = 0x00010020u;
                TSIP.REG_ECH.WORD = 0x0000363au;
                TSIP.REG_ECH.WORD = 0x00002638u;
                TSIP.REG_ECH.WORD = 0x02003e51u;
                TSIP.REG_ECH.WORD = 0x00002e20u;
                TSIP.REG_ECH.WORD = 0x02003c11u;
                TSIP.REG_ECH.WORD = 0x0000a700u;
                TSIP.REG_ECH.WORD = 0x00000002u;
                TSIP.REG_ECH.WORD = 0x00000060u;
                TSIP.REG_ECH.WORD = 0x0000363au;
                TSIP.REG_ECH.WORD = 0x00002638u;
                TSIP.REG_ECH.WORD = 0x02003c11u;
                TSIP.REG_ECH.WORD = 0x00002f00u;
                TSIP.REG_ECH.WORD = 0x00000080u;
                TSIP.REG_ECH.WORD = 0x00002f80u;
                TSIP.REG_ECH.WORD = 0x00000080u;
                TSIP.REG_ECH.WORD = 0x00000060u;
                TSIP.REG_ECH.WORD = 0x0000363au;
                TSIP.REG_ECH.WORD = 0x00002638u;
                TSIP.REG_ECH.WORD = 0x02003c11u;
                TSIP.REG_ECH.WORD = 0x00002f00u;
                TSIP.REG_ECH.WORD = 0x00000080u;
            }
            TSIP.REG_ECH.WORD = 0x00003618u;
            TSIP.REG_ECH.WORD = 0x0020aa00u;
            TSIP.REG_ECH.WORD = 0x00000004u;
            TSIP.REG_ECH.WORD = 0x0000363au;
            TSIP.REG_ECH.WORD = 0x0000a620u;
            TSIP.REG_ECH.WORD = 0x00000003u;
            for (iLoop = 0; iLoop < 4; iLoop = iLoop + 1)
            {
                TSIP.REG_ECH.WORD = 0x02003e11u;
                TSIP.REG_ECH.WORD = 0x00003220u;
                TSIP.REG_ECH.WORD = 0x00086a10u;
            }
            TSIP.REG_ECH.WORD = 0x0000363au;
            TSIP.REG_ECH.WORD = 0x00002638u;
            TSIP.REG_ECH.WORD = 0x00003651u;
            TSIP.REG_ECH.WORD = 0x0000a640u;
            TSIP.REG_ECH.WORD = 0x00000003u;
            TSIP.REG_ECH.WORD = 0x0000b600u;
            TSIP.REG_ECH.WORD = 0x80000000u;
            for (iLoop = 0; iLoop < 4; iLoop = iLoop + 1)
            {
                TSIP.REG_ECH.WORD = 0x02003e12u;
                TSIP.REG_ECH.WORD = 0x00003240u;
                TSIP.REG_ECH.WORD = 0x00086a10u;
            }
            TSIP.REG_ECH.WORD = 0x00003611u;
            TSIP.REG_ECH.WORD = 0x00026a10u;
            TSIP.REG_ECH.WORD = 0x00002e00u;
            TSIP.REG_E0H.WORD = 0x81010200u;
            TSIP.REG_04H.WORD = 0x00000606u;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_04H.BIT.B30)
            {
                /* waiting */
            }
            S_RAM[0] = change_endian_long(TSIP.REG_100H.WORD);
            TSIP.REG_ECH.WORD = 0x00000bffu;
            TSIP.REG_ECH.WORD = 0x00000a52u;
            for (iLoop = 0; iLoop < S_RAM[0]; iLoop = iLoop + 1)
            {
                TSIP.REG_E0H.WORD = 0x81810012u;
                TSIP.REG_00H.WORD = 0x00005807u;
                /* WAIT_LOOP */
                while (0u != TSIP.REG_00H.BIT.B25)
                {
                    /* waiting */
                }
                TSIP.REG_1CH.WORD = 0x00001800u;
                TSIP.REG_ECH.WORD = 0x00002656u;
                TSIP.REG_ECH.WORD = 0x00002fe0u;
                RX65NHU_func101(change_endian_long(0xae6c9507u), change_endian_long(0x42d9e892u), change_endian_long(0x2087db6cu), change_endian_long(0x98906287u));
            }
            RX65NHU_func100(change_endian_long(0x1cd9f024u), change_endian_long(0x1b2b1b8du), change_endian_long(0xafe731c4u), change_endian_long(0xdb10dd48u));
            TSIP.REG_ECH.WORD = 0x38000a1fu;
            TSIP.REG_E0H.WORD = 0x00000080u;
            TSIP.REG_1CH.WORD = 0x00260000u;
            TSIP.REG_1CH.WORD = 0x00402000u;
            TSIP.REG_F8H.WORD = 0x00000040u;
            TSIP.REG_ECH.WORD = 0x000036bbu;
            TSIP.REG_ECH.WORD = 0x000026b8u;
            TSIP.REG_ECH.WORD = 0x00036eb5u;
            TSIP.REG_00H.WORD = 0x00005007u;
            /* WAIT_LOOP */
            while (0u != TSIP.REG_00H.BIT.B25)
            {
                /* waiting */
            }
            TSIP.REG_1CH.WORD = 0x00001800u;
            TSIP.REG_E0H.WORD = 0x810102a0u;
            TSIP.REG_00H.WORD = 0x00005807u;
            /* WAIT_LOOP */
            while (0u != TSIP.REG_00H.BIT.B25)
            {
                /* waiting */
            }
            TSIP.REG_1CH.WORD = 0x00001800u;
            /* WAIT_LOOP */
            while (0u != TSIP.REG_74H.BIT.B18)
            {
                /* waiting */
            }
            TSIP.REG_1CH.WORD = 0x00001600u;
            TSIP.REG_74H.WORD = 0x00000000u;
            TSIP.REG_00H.WORD = 0x00003523u;
            TSIP.REG_74H.WORD = 0x00000008u;
            TSIP.REG_2CH.WORD = 0x00000019u;
            /* WAIT_LOOP */
            while (0u != TSIP.REG_00H.BIT.B25)
            {
                /* waiting */
            }
            TSIP.REG_1CH.WORD = 0x00001800u;
            TSIP.REG_ECH.WORD = 0x00000a73u;
            TSIP.REG_ECH.WORD = 0x0000369au;
            TSIP.REG_E0H.WORD = 0x81010300u;
            TSIP.REG_04H.WORD = 0x00000606u;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_04H.BIT.B30)
            {
                /* waiting */
            }
            S_RAM[0] = change_endian_long(TSIP.REG_100H.WORD);
            TSIP.REG_ECH.WORD = 0x00000bffu;
            for (iLoop = 0; iLoop < S_RAM[0]; iLoop = iLoop + 1)
            {
                TSIP.REG_ECH.WORD = 0x02003814u;
                TSIP.REG_ECH.WORD = 0x02003c13u;
                TSIP.REG_ECH.WORD = 0x00002e60u;
                TSIP.REG_ECH.WORD = 0x00002e80u;
                TSIP.REG_ECH.WORD = 0x00002fe0u;
                RX65NHU_func101(change_endian_long(0xd6347d3au), change_endian_long(0x7cdcccc5u), change_endian_long(0x0e4583b0u), change_endian_long(0x2711367du));
            }
            RX65NHU_func100(change_endian_long(0xb34c2eedu), change_endian_long(0xa2ecde0cu), change_endian_long(0xe36e5a7eu), change_endian_long(0x6fb73923u));
            TSIP.REG_ECH.WORD = 0x38000b1fu;
            TSIP.REG_E0H.WORD = 0x00000080u;
            TSIP.REG_1CH.WORD = 0x00260000u;
            TSIP.REG_1CH.WORD = 0x00402000u;
            TSIP.REG_ECH.WORD = 0x00003618u;
            TSIP.REG_ECH.WORD = 0x00003630u;
            TSIP.REG_ECH.WORD = 0x0000a620u;
            TSIP.REG_ECH.WORD = 0x00000021u;
            TSIP.REG_2CH.WORD = 0x00000029u;
            for (iLoop = 0; iLoop < 32; iLoop = iLoop + 1)
            {
                TSIP.REG_E0H.WORD = 0x80010240u;
                TSIP.REG_00H.WORD = 0x0800800fu;
                /* WAIT_LOOP */
                while (0u != TSIP.REG_00H.BIT.B25)
                {
                    /* waiting */
                }
                TSIP.REG_1CH.WORD = 0x00001800u;
                TSIP.REG_00H.WORD = 0x08008307u;
                /* WAIT_LOOP */
                while (0u != TSIP.REG_00H.BIT.B25)
                {
                    /* waiting */
                }
                TSIP.REG_1CH.WORD = 0x00001800u;
                TSIP.REG_ECH.WORD = 0x02003e50u;
                TSIP.REG_ECH.WORD = 0x02003e51u;
                TSIP.REG_ECH.WORD = 0x00002e00u;
                TSIP.REG_ECH.WORD = 0x00002e20u;
            }
            TSIP.REG_ECH.WORD = 0x00003638u;
            TSIP.REG_ECH.WORD = 0x0000a620u;
            TSIP.REG_ECH.WORD = 0x00000044u;
            TSIP.REG_ECH.WORD = 0x0000b600u;
            TSIP.REG_ECH.WORD = 0x80000000u;
            for (iLoop = 0; iLoop < 4; iLoop = iLoop + 1)
            {
                TSIP.REG_ECH.WORD = 0x02003e11u;
                TSIP.REG_ECH.WORD = 0x00003220u;
                TSIP.REG_ECH.WORD = 0x00086a10u;
            }
            TSIP.REG_ECH.WORD = 0x00003618u;
            TSIP.REG_ECH.WORD = 0x0000a600u;
            TSIP.REG_ECH.WORD = 0x00000045u;
            TSIP.REG_ECH.WORD = 0x00026a10u;
            TSIP.REG_E0H.WORD = 0x81010200u;
            TSIP.REG_04H.WORD = 0x00000606u;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_04H.BIT.B30)
            {
                /* waiting */
            }
            S_RAM[0] = change_endian_long(TSIP.REG_100H.WORD);
            TSIP.REG_ECH.WORD = 0x00003698u;
            TSIP.REG_ECH.WORD = 0x0000a680u;
            TSIP.REG_ECH.WORD = 0x00000041u;
            TSIP.REG_ECH.WORD = 0x00036e94u;
            TSIP.REG_ECH.WORD = 0x0000b620u;
            TSIP.REG_ECH.WORD = 0x00000041u;
            TSIP.REG_ECH.WORD = 0x00000bffu;
            TSIP.REG_ECH.WORD = 0x00003658u;
            TSIP.REG_ECH.WORD = 0x0000a640u;
            TSIP.REG_ECH.WORD = 0x00000020u;
            TSIP.REG_28H.WORD = 0x00af0001u;
            TSIP.REG_2CH.WORD = 0x00000010u;
            for (iLoop = 0; iLoop < 6; iLoop = iLoop + 1)
            {
                TSIP.REG_7CH.WORD = 0x00000011u;
                TSIP.REG_104H.WORD = 0x00000754u;
                TSIP.REG_74H.WORD = 0x00000004u;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_104H.BIT.B31)
                {
                    /* waiting */
                }
                TSIP.REG_100H.WORD = change_endian_long(0x6a09e667u);
                TSIP.REG_100H.WORD = change_endian_long(0xbb67ae85u);
                TSIP.REG_100H.WORD = change_endian_long(0x3c6ef372u);
                TSIP.REG_100H.WORD = change_endian_long(0xa54ff53au);
                TSIP.REG_100H.WORD = change_endian_long(0x510e527fu);
                TSIP.REG_100H.WORD = change_endian_long(0x9b05688cu);
                TSIP.REG_100H.WORD = change_endian_long(0x1f83d9abu);
                TSIP.REG_100H.WORD = change_endian_long(0x5be0cd19u);
                TSIP.REG_74H.WORD = 0x00000002u;
                TSIP.REG_ECH.WORD = 0x00000af7u;
                TSIP.REG_ECH.WORD = 0x00000ab5u;
                TSIP.REG_ECH.WORD = 0x02003e32u;
                for (jLoop = 0; jLoop < S_RAM[0]; jLoop = jLoop + 1)
                {
                    TSIP.REG_E0H.WORD = 0x81810015u;
                    TSIP.REG_00H.WORD = 0x00005807u;
                    /* WAIT_LOOP */
                    while (0u != TSIP.REG_00H.BIT.B25)
                    {
                        /* waiting */
                    }
                    TSIP.REG_1CH.WORD = 0x00001800u;
                    TSIP.REG_ECH.WORD = 0x000026b6u;
                    TSIP.REG_ECH.WORD = 0x00002ee0u;
                    RX65NHU_func101(change_endian_long(0xd534462du), change_endian_long(0xe83d4a6au), change_endian_long(0x76d17548u), change_endian_long(0xb9543a67u));
                }
                TSIP.REG_ECH.WORD = 0x38000a17u;
                TSIP.REG_E0H.WORD = 0x00000080u;
                TSIP.REG_1CH.WORD = 0x00260000u;
                TSIP.REG_1CH.WORD = 0x00402000u;
                TSIP.REG_F8H.WORD = 0x00000040u;
                TSIP.REG_00H.WORD = 0x00005007u;
                /* WAIT_LOOP */
                while (0u != TSIP.REG_00H.BIT.B25)
                {
                    /* waiting */
                }
                TSIP.REG_1CH.WORD = 0x00001800u;
                TSIP.REG_E0H.WORD = 0x81010280u;
                TSIP.REG_00H.WORD = 0x00005807u;
                /* WAIT_LOOP */
                while (0u != TSIP.REG_00H.BIT.B25)
                {
                    /* waiting */
                }
                TSIP.REG_1CH.WORD = 0x00001800u;
                /* WAIT_LOOP */
                while (0u != TSIP.REG_74H.BIT.B18)
                {
                    /* waiting */
                }
                TSIP.REG_1CH.WORD = 0x00001600u;
                TSIP.REG_74H.WORD = 0x00000000u;
                TSIP.REG_00H.WORD = 0x00003523u;
                TSIP.REG_74H.WORD = 0x00000008u;
                /* WAIT_LOOP */
                while (0u != TSIP.REG_00H.BIT.B25)
                {
                    /* waiting */
                }
                TSIP.REG_1CH.WORD = 0x00001800u;
                TSIP.REG_ECH.WORD = 0x00002e20u;
                TSIP.REG_ECH.WORD = 0x00002fe0u;
                RX65NHU_func101(change_endian_long(0x0daad916u), change_endian_long(0xbd121b08u), change_endian_long(0x726e4f68u), change_endian_long(0x0b6639d1u));
            }
            RX65NHU_func100(change_endian_long(0x80d71b65u), change_endian_long(0xd1b657fbu), change_endian_long(0xa5b6d807u), change_endian_long(0x57359e98u));
            TSIP.REG_ECH.WORD = 0x38008be0u;
            TSIP.REG_ECH.WORD = 0x00000006u;
            TSIP.REG_E0H.WORD = 0x00000080u;
            TSIP.REG_1CH.WORD = 0x00260000u;
            TSIP.REG_1CH.WORD = 0x00402000u;
            RX65NHU_func103();
            RX65NHU_func100(change_endian_long(0xed5611bfu), change_endian_long(0xe7f0a3afu), change_endian_long(0xe6344d72u), change_endian_long(0x0d44fbc2u));
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
            RX65NHU_func103();
            RX65NHU_func100(change_endian_long(0x2c584b2au), change_endian_long(0xd6417180u), change_endian_long(0xc2d29494u), change_endian_long(0xc054ffeeu));
            TSIP.REG_104H.WORD = 0x00000052u;
            TSIP.REG_C4H.WORD = 0x01000c84u;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_104H.BIT.B31)
            {
                /* waiting */
            }
            TSIP.REG_100H.WORD = change_endian_long(0x00000000u);
            TSIP.REG_E0H.WORD = 0x80040080u;
            TSIP.REG_00H.WORD = 0x00008213u;
            /* WAIT_LOOP */
            while (0u != TSIP.REG_00H.BIT.B25)
            {
                /* waiting */
            }
            TSIP.REG_1CH.WORD = 0x00001800u;
            RX65NHU_func103();
            RX65NHU_func100(change_endian_long(0x695f1fa9u), change_endian_long(0xbb6f049cu), change_endian_long(0x6e8b15b8u), change_endian_long(0x02ab4de5u));
            TSIP.REG_104H.WORD = 0x00000052u;
            TSIP.REG_C4H.WORD = 0x01000c84u;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_104H.BIT.B31)
            {
                /* waiting */
            }
            TSIP.REG_100H.WORD = change_endian_long(0x00000000u);
            TSIP.REG_E0H.WORD = 0x80040100u;
            TSIP.REG_00H.WORD = 0x00008213u;
            /* WAIT_LOOP */
            while (0u != TSIP.REG_00H.BIT.B25)
            {
                /* waiting */
            }
            TSIP.REG_1CH.WORD = 0x00001800u;
            RX65NHU_func103();
            TSIP.REG_104H.WORD = 0x00000052u;
            TSIP.REG_C4H.WORD = 0x01000c84u;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_104H.BIT.B31)
            {
                /* waiting */
            }
            TSIP.REG_100H.WORD = change_endian_long(0x00000000u);
            TSIP.REG_E0H.WORD = 0x80040180u;
            TSIP.REG_00H.WORD = 0x00008213u;
            /* WAIT_LOOP */
            while (0u != TSIP.REG_00H.BIT.B25)
            {
                /* waiting */
            }
            TSIP.REG_1CH.WORD = 0x00001800u;
            TSIP.REG_104H.WORD = 0x00000068u;
            TSIP.REG_E0H.WORD = 0x800103c0u;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_104H.BIT.B31)
            {
                /* waiting */
            }
            TSIP.REG_100H.WORD = InData_KeyIndexType[0];
            TSIP.REG_ECH.WORD = 0x3020abc0u;
            TSIP.REG_ECH.WORD = 0x00000003u;
            TSIP.REG_ECH.WORD = 0x00060020u;
            TSIP.REG_ECH.WORD = 0x0000b7c0u;
            TSIP.REG_ECH.WORD = 0x00000002u;
            TSIP.REG_ECH.WORD = 0x00000080u;
            RX65NHU_func060();
            TSIP.REG_28H.WORD = 0x00870001u;
            TSIP.REG_ECH.WORD = 0x3420abc0u;
            TSIP.REG_ECH.WORD = 0x00000002u;
            TSIP.REG_E0H.WORD = 0x00000080u;
            TSIP.REG_1CH.WORD = 0x00A60000u;
            RX65NHU_func100(change_endian_long(0x8b185556u), change_endian_long(0xd7eb00a6u), change_endian_long(0xb092049eu), change_endian_long(0x6926b1c4u));
            TSIP.REG_1CH.WORD = 0x00400000u;
            TSIP.REG_1D0H.WORD = 0x00000000u;
            if (1u == (TSIP.REG_1CH.BIT.B22))
            {
                RX65NHU_func100(change_endian_long(0x2b0c4091u), change_endian_long(0x86239023u), change_endian_long(0x483ad5f7u), change_endian_long(0xd45cb072u));
                TSIP.REG_C4H.WORD = 0x000c2b0du;
                TSIP.REG_E0H.WORD = 0x81040080u;
                TSIP.REG_00H.WORD = 0x00002813u;
                /* WAIT_LOOP */
                while (0u != TSIP.REG_00H.BIT.B25)
                {
                    /* waiting */
                }
                TSIP.REG_1CH.WORD = 0x00001800u;
                TSIP.REG_C4H.WORD = 0x000c0b0cu;
                TSIP.REG_E0H.WORD = 0x810103a0u;
                TSIP.REG_00H.WORD = 0x00002807u;
                /* WAIT_LOOP */
                while (0u != TSIP.REG_00H.BIT.B25)
                {
                    /* waiting */
                }
                TSIP.REG_1CH.WORD = 0x00001800u;
                TSIP.REG_D0H.WORD = 0x00000100u;
                TSIP.REG_C4H.WORD = 0x02e087bfu;
                TSIP.REG_00H.WORD = 0x00002323u;
                TSIP.REG_2CH.WORD = 0x000000a0u;
                /* WAIT_LOOP */
                while (0u != TSIP.REG_00H.BIT.B25)
                {
                    /* waiting */
                }
                TSIP.REG_1CH.WORD = 0x00001800u;
                TSIP.REG_104H.WORD = 0x00000352u;
                TSIP.REG_C4H.WORD = 0x000009cdu;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_104H.BIT.B31)
                {
                    /* waiting */
                }
                TSIP.REG_100H.WORD = change_endian_long(0x00000000u);
                TSIP.REG_100H.WORD = change_endian_long(0x00000000u);
                TSIP.REG_100H.WORD = change_endian_long(0x00000000u);
                TSIP.REG_100H.WORD = change_endian_long(0x00000002u);
                TSIP.REG_04H.WORD = 0x00000232u;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_04H.BIT.B30)
                {
                    /* waiting */
                }
                OutData_KeyIndex3[4] = TSIP.REG_100H.WORD;
                OutData_KeyIndex3[5] = TSIP.REG_100H.WORD;
                OutData_KeyIndex3[6] = TSIP.REG_100H.WORD;
                OutData_KeyIndex3[7] = TSIP.REG_100H.WORD;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_04H.BIT.B30)
                {
                    /* waiting */
                }
                OutData_KeyIndex3[8] = TSIP.REG_100H.WORD;
                OutData_KeyIndex3[9] = TSIP.REG_100H.WORD;
                OutData_KeyIndex3[10] = TSIP.REG_100H.WORD;
                OutData_KeyIndex3[11] = TSIP.REG_100H.WORD;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_04H.BIT.B30)
                {
                    /* waiting */
                }
                OutData_KeyIndex3[12] = TSIP.REG_100H.WORD;
                OutData_KeyIndex3[13] = TSIP.REG_100H.WORD;
                OutData_KeyIndex3[14] = TSIP.REG_100H.WORD;
                OutData_KeyIndex3[15] = TSIP.REG_100H.WORD;
                RX65NHU_func100(change_endian_long(0x0838c10du), change_endian_long(0x8c054c92u), change_endian_long(0xe7c9899fu), change_endian_long(0x437a1409u));
                TSIP.REG_C4H.WORD = 0x000c2b0du;
                TSIP.REG_E0H.WORD = 0x81040000u;
                TSIP.REG_00H.WORD = 0x00002813u;
                /* WAIT_LOOP */
                while (0u != TSIP.REG_00H.BIT.B25)
                {
                    /* waiting */
                }
                TSIP.REG_1CH.WORD = 0x00001800u;
                TSIP.REG_C4H.WORD = 0x000c0b0cu;
                TSIP.REG_E0H.WORD = 0x810103a0u;
                TSIP.REG_00H.WORD = 0x00002807u;
                /* WAIT_LOOP */
                while (0u != TSIP.REG_00H.BIT.B25)
                {
                    /* waiting */
                }
                TSIP.REG_1CH.WORD = 0x00001800u;
                TSIP.REG_D0H.WORD = 0x00000100u;
                TSIP.REG_C4H.WORD = 0x02e087bfu;
                TSIP.REG_00H.WORD = 0x00002323u;
                TSIP.REG_2CH.WORD = 0x00000020u;
                /* WAIT_LOOP */
                while (0u != TSIP.REG_00H.BIT.B25)
                {
                    /* waiting */
                }
                TSIP.REG_1CH.WORD = 0x00001800u;
                TSIP.REG_104H.WORD = 0x00000352u;
                TSIP.REG_C4H.WORD = 0x000009cdu;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_104H.BIT.B31)
                {
                    /* waiting */
                }
                TSIP.REG_100H.WORD = change_endian_long(0x00000000u);
                TSIP.REG_100H.WORD = change_endian_long(0x00000000u);
                TSIP.REG_100H.WORD = change_endian_long(0x00000000u);
                TSIP.REG_100H.WORD = change_endian_long(0x00000002u);
                TSIP.REG_04H.WORD = 0x00000232u;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_04H.BIT.B30)
                {
                    /* waiting */
                }
                OutData_KeyIndex4[4] = TSIP.REG_100H.WORD;
                OutData_KeyIndex4[5] = TSIP.REG_100H.WORD;
                OutData_KeyIndex4[6] = TSIP.REG_100H.WORD;
                OutData_KeyIndex4[7] = TSIP.REG_100H.WORD;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_04H.BIT.B30)
                {
                    /* waiting */
                }
                OutData_KeyIndex4[8] = TSIP.REG_100H.WORD;
                OutData_KeyIndex4[9] = TSIP.REG_100H.WORD;
                OutData_KeyIndex4[10] = TSIP.REG_100H.WORD;
                OutData_KeyIndex4[11] = TSIP.REG_100H.WORD;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_04H.BIT.B30)
                {
                    /* waiting */
                }
                OutData_KeyIndex4[12] = TSIP.REG_100H.WORD;
                OutData_KeyIndex4[13] = TSIP.REG_100H.WORD;
                OutData_KeyIndex4[14] = TSIP.REG_100H.WORD;
                OutData_KeyIndex4[15] = TSIP.REG_100H.WORD;
                RX65NHU_func101(change_endian_long(0x2fe51601u), change_endian_long(0xdd2835dbu), change_endian_long(0x75f4fb7du), change_endian_long(0x4998321au));
            }
            else
            {
                TSIP.REG_ECH.WORD = 0x38000fdeu;
                TSIP.REG_E0H.WORD = 0x00000080u;
                TSIP.REG_1CH.WORD = 0x00260000u;
                RX65NHU_func100(change_endian_long(0x21917c06u), change_endian_long(0x6c1109b6u), change_endian_long(0x0be4b025u), change_endian_long(0xd2505510u));
                TSIP.REG_1CH.WORD = 0x00400000u;
                TSIP.REG_1D0H.WORD = 0x00000000u;
                if (1u == (TSIP.REG_1CH.BIT.B22))
                {
                    RX65NHU_func100(change_endian_long(0x51de8ccdu), change_endian_long(0xa48d8308u), change_endian_long(0x100e7459u), change_endian_long(0xcf11ef90u));
                    TSIP.REG_C4H.WORD = 0x000c2b0du;
                    TSIP.REG_E0H.WORD = 0x81040180u;
                    TSIP.REG_00H.WORD = 0x00002813u;
                    /* WAIT_LOOP */
                    while (0u != TSIP.REG_00H.BIT.B25)
                    {
                        /* waiting */
                    }
                    TSIP.REG_1CH.WORD = 0x00001800u;
                    TSIP.REG_C4H.WORD = 0x000c0b0cu;
                    TSIP.REG_E0H.WORD = 0x810103a0u;
                    TSIP.REG_00H.WORD = 0x00002807u;
                    /* WAIT_LOOP */
                    while (0u != TSIP.REG_00H.BIT.B25)
                    {
                        /* waiting */
                    }
                    TSIP.REG_1CH.WORD = 0x00001800u;
                    TSIP.REG_C4H.WORD = 0x02e007bdu;
                    TSIP.REG_00H.WORD = 0x00002313u;
                    TSIP.REG_2CH.WORD = 0x000000a8u;
                    /* WAIT_LOOP */
                    while (0u != TSIP.REG_00H.BIT.B25)
                    {
                        /* waiting */
                    }
                    TSIP.REG_1CH.WORD = 0x00001800u;
                    TSIP.REG_00H.WORD = 0x00000313u;
                    /* WAIT_LOOP */
                    while (0u != TSIP.REG_00H.BIT.B25)
                    {
                        /* waiting */
                    }
                    TSIP.REG_1CH.WORD = 0x00001800u;
                    TSIP.REG_104H.WORD = 0x00000352u;
                    TSIP.REG_C4H.WORD = 0x000009cdu;
                    /* WAIT_LOOP */
                    while (1u != TSIP.REG_104H.BIT.B31)
                    {
                        /* waiting */
                    }
                    TSIP.REG_100H.WORD = change_endian_long(0x00000000u);
                    TSIP.REG_100H.WORD = change_endian_long(0x00000000u);
                    TSIP.REG_100H.WORD = change_endian_long(0x00000000u);
                    TSIP.REG_100H.WORD = change_endian_long(0x00000001u);
                    TSIP.REG_04H.WORD = 0x00000222u;
                    /* WAIT_LOOP */
                    while (1u != TSIP.REG_04H.BIT.B30)
                    {
                        /* waiting */
                    }
                    OutData_KeyIndex1[4] = TSIP.REG_100H.WORD;
                    OutData_KeyIndex1[5] = TSIP.REG_100H.WORD;
                    OutData_KeyIndex1[6] = TSIP.REG_100H.WORD;
                    OutData_KeyIndex1[7] = TSIP.REG_100H.WORD;
                    /* WAIT_LOOP */
                    while (1u != TSIP.REG_04H.BIT.B30)
                    {
                        /* waiting */
                    }
                    OutData_KeyIndex1[8] = TSIP.REG_100H.WORD;
                    OutData_KeyIndex1[9] = TSIP.REG_100H.WORD;
                    OutData_KeyIndex1[10] = TSIP.REG_100H.WORD;
                    OutData_KeyIndex1[11] = TSIP.REG_100H.WORD;
                    RX65NHU_func100(change_endian_long(0x1fd8c694u), change_endian_long(0x74e705bfu), change_endian_long(0x2a713278u), change_endian_long(0x54b47723u));
                    TSIP.REG_C4H.WORD = 0x000c2b0du;
                    TSIP.REG_E0H.WORD = 0x81040100u;
                    TSIP.REG_00H.WORD = 0x00002813u;
                    /* WAIT_LOOP */
                    while (0u != TSIP.REG_00H.BIT.B25)
                    {
                        /* waiting */
                    }
                    TSIP.REG_1CH.WORD = 0x00001800u;
                    TSIP.REG_C4H.WORD = 0x000c0b0cu;
                    TSIP.REG_E0H.WORD = 0x810103a0u;
                    TSIP.REG_00H.WORD = 0x00002807u;
                    /* WAIT_LOOP */
                    while (0u != TSIP.REG_00H.BIT.B25)
                    {
                        /* waiting */
                    }
                    TSIP.REG_1CH.WORD = 0x00001800u;
                    TSIP.REG_C4H.WORD = 0x02e007bdu;
                    TSIP.REG_00H.WORD = 0x00002313u;
                    TSIP.REG_2CH.WORD = 0x00000028u;
                    /* WAIT_LOOP */
                    while (0u != TSIP.REG_00H.BIT.B25)
                    {
                        /* waiting */
                    }
                    TSIP.REG_1CH.WORD = 0x00001800u;
                    TSIP.REG_00H.WORD = 0x00000313u;
                    /* WAIT_LOOP */
                    while (0u != TSIP.REG_00H.BIT.B25)
                    {
                        /* waiting */
                    }
                    TSIP.REG_1CH.WORD = 0x00001800u;
                    TSIP.REG_104H.WORD = 0x00000352u;
                    TSIP.REG_C4H.WORD = 0x000009cdu;
                    /* WAIT_LOOP */
                    while (1u != TSIP.REG_104H.BIT.B31)
                    {
                        /* waiting */
                    }
                    TSIP.REG_100H.WORD = change_endian_long(0x00000000u);
                    TSIP.REG_100H.WORD = change_endian_long(0x00000000u);
                    TSIP.REG_100H.WORD = change_endian_long(0x00000000u);
                    TSIP.REG_100H.WORD = change_endian_long(0x00000001u);
                    TSIP.REG_04H.WORD = 0x00000222u;
                    /* WAIT_LOOP */
                    while (1u != TSIP.REG_04H.BIT.B30)
                    {
                        /* waiting */
                    }
                    OutData_KeyIndex2[4] = TSIP.REG_100H.WORD;
                    OutData_KeyIndex2[5] = TSIP.REG_100H.WORD;
                    OutData_KeyIndex2[6] = TSIP.REG_100H.WORD;
                    OutData_KeyIndex2[7] = TSIP.REG_100H.WORD;
                    /* WAIT_LOOP */
                    while (1u != TSIP.REG_04H.BIT.B30)
                    {
                        /* waiting */
                    }
                    OutData_KeyIndex2[8] = TSIP.REG_100H.WORD;
                    OutData_KeyIndex2[9] = TSIP.REG_100H.WORD;
                    OutData_KeyIndex2[10] = TSIP.REG_100H.WORD;
                    OutData_KeyIndex2[11] = TSIP.REG_100H.WORD;
                    RX65NHU_func100(change_endian_long(0xf018ecb9u), change_endian_long(0x4dca2769u), change_endian_long(0x71c3fe5bu), change_endian_long(0x778611a1u));
                    TSIP.REG_C4H.WORD = 0x000c2b0du;
                    TSIP.REG_E0H.WORD = 0x81040080u;
                    TSIP.REG_00H.WORD = 0x00002813u;
                    /* WAIT_LOOP */
                    while (0u != TSIP.REG_00H.BIT.B25)
                    {
                        /* waiting */
                    }
                    TSIP.REG_1CH.WORD = 0x00001800u;
                    TSIP.REG_C4H.WORD = 0x000c0b0cu;
                    TSIP.REG_E0H.WORD = 0x810103a0u;
                    TSIP.REG_00H.WORD = 0x00002807u;
                    /* WAIT_LOOP */
                    while (0u != TSIP.REG_00H.BIT.B25)
                    {
                        /* waiting */
                    }
                    TSIP.REG_1CH.WORD = 0x00001800u;
                    TSIP.REG_C4H.WORD = 0x02e007bdu;
                    TSIP.REG_00H.WORD = 0x00002313u;
                    TSIP.REG_2CH.WORD = 0x000000a0u;
                    /* WAIT_LOOP */
                    while (0u != TSIP.REG_00H.BIT.B25)
                    {
                        /* waiting */
                    }
                    TSIP.REG_1CH.WORD = 0x00001800u;
                    TSIP.REG_00H.WORD = 0x00000313u;
                    /* WAIT_LOOP */
                    while (0u != TSIP.REG_00H.BIT.B25)
                    {
                        /* waiting */
                    }
                    TSIP.REG_1CH.WORD = 0x00001800u;
                    TSIP.REG_104H.WORD = 0x00000352u;
                    TSIP.REG_C4H.WORD = 0x000009cdu;
                    /* WAIT_LOOP */
                    while (1u != TSIP.REG_104H.BIT.B31)
                    {
                        /* waiting */
                    }
                    TSIP.REG_100H.WORD = change_endian_long(0x00000000u);
                    TSIP.REG_100H.WORD = change_endian_long(0x00000000u);
                    TSIP.REG_100H.WORD = change_endian_long(0x00000000u);
                    TSIP.REG_100H.WORD = change_endian_long(0x00000001u);
                    TSIP.REG_04H.WORD = 0x00000222u;
                    /* WAIT_LOOP */
                    while (1u != TSIP.REG_04H.BIT.B30)
                    {
                        /* waiting */
                    }
                    OutData_KeyIndex3[4] = TSIP.REG_100H.WORD;
                    OutData_KeyIndex3[5] = TSIP.REG_100H.WORD;
                    OutData_KeyIndex3[6] = TSIP.REG_100H.WORD;
                    OutData_KeyIndex3[7] = TSIP.REG_100H.WORD;
                    /* WAIT_LOOP */
                    while (1u != TSIP.REG_04H.BIT.B30)
                    {
                        /* waiting */
                    }
                    OutData_KeyIndex3[8] = TSIP.REG_100H.WORD;
                    OutData_KeyIndex3[9] = TSIP.REG_100H.WORD;
                    OutData_KeyIndex3[10] = TSIP.REG_100H.WORD;
                    OutData_KeyIndex3[11] = TSIP.REG_100H.WORD;
                    RX65NHU_func100(change_endian_long(0x69eea369u), change_endian_long(0x6bc52b1fu), change_endian_long(0x5db5b609u), change_endian_long(0xbab53d0bu));
                    TSIP.REG_C4H.WORD = 0x000c2b0du;
                    TSIP.REG_E0H.WORD = 0x81040000u;
                    TSIP.REG_00H.WORD = 0x00002813u;
                    /* WAIT_LOOP */
                    while (0u != TSIP.REG_00H.BIT.B25)
                    {
                        /* waiting */
                    }
                    TSIP.REG_1CH.WORD = 0x00001800u;
                    TSIP.REG_C4H.WORD = 0x000c0b0cu;
                    TSIP.REG_E0H.WORD = 0x810103a0u;
                    TSIP.REG_00H.WORD = 0x00002807u;
                    /* WAIT_LOOP */
                    while (0u != TSIP.REG_00H.BIT.B25)
                    {
                        /* waiting */
                    }
                    TSIP.REG_1CH.WORD = 0x00001800u;
                    TSIP.REG_C4H.WORD = 0x02e007bdu;
                    TSIP.REG_00H.WORD = 0x00002313u;
                    TSIP.REG_2CH.WORD = 0x00000020u;
                    /* WAIT_LOOP */
                    while (0u != TSIP.REG_00H.BIT.B25)
                    {
                        /* waiting */
                    }
                    TSIP.REG_1CH.WORD = 0x00001800u;
                    TSIP.REG_00H.WORD = 0x00000313u;
                    /* WAIT_LOOP */
                    while (0u != TSIP.REG_00H.BIT.B25)
                    {
                        /* waiting */
                    }
                    TSIP.REG_1CH.WORD = 0x00001800u;
                    TSIP.REG_104H.WORD = 0x00000352u;
                    TSIP.REG_C4H.WORD = 0x000009cdu;
                    /* WAIT_LOOP */
                    while (1u != TSIP.REG_104H.BIT.B31)
                    {
                        /* waiting */
                    }
                    TSIP.REG_100H.WORD = change_endian_long(0x00000000u);
                    TSIP.REG_100H.WORD = change_endian_long(0x00000000u);
                    TSIP.REG_100H.WORD = change_endian_long(0x00000000u);
                    TSIP.REG_100H.WORD = change_endian_long(0x00000001u);
                    TSIP.REG_04H.WORD = 0x00000222u;
                    /* WAIT_LOOP */
                    while (1u != TSIP.REG_04H.BIT.B30)
                    {
                        /* waiting */
                    }
                    OutData_KeyIndex4[4] = TSIP.REG_100H.WORD;
                    OutData_KeyIndex4[5] = TSIP.REG_100H.WORD;
                    OutData_KeyIndex4[6] = TSIP.REG_100H.WORD;
                    OutData_KeyIndex4[7] = TSIP.REG_100H.WORD;
                    /* WAIT_LOOP */
                    while (1u != TSIP.REG_04H.BIT.B30)
                    {
                        /* waiting */
                    }
                    OutData_KeyIndex4[8] = TSIP.REG_100H.WORD;
                    OutData_KeyIndex4[9] = TSIP.REG_100H.WORD;
                    OutData_KeyIndex4[10] = TSIP.REG_100H.WORD;
                    OutData_KeyIndex4[11] = TSIP.REG_100H.WORD;
                    RX65NHU_func101(change_endian_long(0x9538b472u), change_endian_long(0xaa22c03cu), change_endian_long(0x61899dfau), change_endian_long(0x04696f6cu));
                }
                else
                {
                    RX65NHU_func100(change_endian_long(0xc05f88f3u), change_endian_long(0x5b7756fbu), change_endian_long(0x23f77974u), change_endian_long(0xfaad2900u));
                    TSIP.REG_C4H.WORD = 0x000c2b0du;
                    TSIP.REG_E0H.WORD = 0x81040180u;
                    TSIP.REG_00H.WORD = 0x00002813u;
                    /* WAIT_LOOP */
                    while (0u != TSIP.REG_00H.BIT.B25)
                    {
                        /* waiting */
                    }
                    TSIP.REG_1CH.WORD = 0x00001800u;
                    TSIP.REG_C4H.WORD = 0x000c0b0cu;
                    TSIP.REG_E0H.WORD = 0x810103a0u;
                    TSIP.REG_00H.WORD = 0x00002807u;
                    /* WAIT_LOOP */
                    while (0u != TSIP.REG_00H.BIT.B25)
                    {
                        /* waiting */
                    }
                    TSIP.REG_1CH.WORD = 0x00001800u;
                    TSIP.REG_D0H.WORD = 0x00000100u;
                    TSIP.REG_C4H.WORD = 0x02e087bfu;
                    TSIP.REG_00H.WORD = 0x00002323u;
                    TSIP.REG_2CH.WORD = 0x000000a8u;
                    /* WAIT_LOOP */
                    while (0u != TSIP.REG_00H.BIT.B25)
                    {
                        /* waiting */
                    }
                    TSIP.REG_1CH.WORD = 0x00001800u;
                    TSIP.REG_104H.WORD = 0x00000352u;
                    TSIP.REG_C4H.WORD = 0x000009cdu;
                    /* WAIT_LOOP */
                    while (1u != TSIP.REG_104H.BIT.B31)
                    {
                        /* waiting */
                    }
                    TSIP.REG_100H.WORD = change_endian_long(0x00000000u);
                    TSIP.REG_100H.WORD = change_endian_long(0x00000000u);
                    TSIP.REG_100H.WORD = change_endian_long(0x00000000u);
                    TSIP.REG_100H.WORD = change_endian_long(0x00000002u);
                    TSIP.REG_04H.WORD = 0x00000232u;
                    /* WAIT_LOOP */
                    while (1u != TSIP.REG_04H.BIT.B30)
                    {
                        /* waiting */
                    }
                    OutData_KeyIndex1[4] = TSIP.REG_100H.WORD;
                    OutData_KeyIndex1[5] = TSIP.REG_100H.WORD;
                    OutData_KeyIndex1[6] = TSIP.REG_100H.WORD;
                    OutData_KeyIndex1[7] = TSIP.REG_100H.WORD;
                    /* WAIT_LOOP */
                    while (1u != TSIP.REG_04H.BIT.B30)
                    {
                        /* waiting */
                    }
                    OutData_KeyIndex1[8] = TSIP.REG_100H.WORD;
                    OutData_KeyIndex1[9] = TSIP.REG_100H.WORD;
                    OutData_KeyIndex1[10] = TSIP.REG_100H.WORD;
                    OutData_KeyIndex1[11] = TSIP.REG_100H.WORD;
                    /* WAIT_LOOP */
                    while (1u != TSIP.REG_04H.BIT.B30)
                    {
                        /* waiting */
                    }
                    OutData_KeyIndex1[12] = TSIP.REG_100H.WORD;
                    OutData_KeyIndex1[13] = TSIP.REG_100H.WORD;
                    OutData_KeyIndex1[14] = TSIP.REG_100H.WORD;
                    OutData_KeyIndex1[15] = TSIP.REG_100H.WORD;
                    RX65NHU_func100(change_endian_long(0x29c782b7u), change_endian_long(0xd6394658u), change_endian_long(0x5fbbfe85u), change_endian_long(0xaae99b3cu));
                    TSIP.REG_C4H.WORD = 0x000c2b0du;
                    TSIP.REG_E0H.WORD = 0x81040100u;
                    TSIP.REG_00H.WORD = 0x00002813u;
                    /* WAIT_LOOP */
                    while (0u != TSIP.REG_00H.BIT.B25)
                    {
                        /* waiting */
                    }
                    TSIP.REG_1CH.WORD = 0x00001800u;
                    TSIP.REG_C4H.WORD = 0x000c0b0cu;
                    TSIP.REG_E0H.WORD = 0x810103a0u;
                    TSIP.REG_00H.WORD = 0x00002807u;
                    /* WAIT_LOOP */
                    while (0u != TSIP.REG_00H.BIT.B25)
                    {
                        /* waiting */
                    }
                    TSIP.REG_1CH.WORD = 0x00001800u;
                    TSIP.REG_D0H.WORD = 0x00000100u;
                    TSIP.REG_C4H.WORD = 0x02e087bfu;
                    TSIP.REG_00H.WORD = 0x00002323u;
                    TSIP.REG_2CH.WORD = 0x00000028u;
                    /* WAIT_LOOP */
                    while (0u != TSIP.REG_00H.BIT.B25)
                    {
                        /* waiting */
                    }
                    TSIP.REG_1CH.WORD = 0x00001800u;
                    TSIP.REG_104H.WORD = 0x00000352u;
                    TSIP.REG_C4H.WORD = 0x000009cdu;
                    /* WAIT_LOOP */
                    while (1u != TSIP.REG_104H.BIT.B31)
                    {
                        /* waiting */
                    }
                    TSIP.REG_100H.WORD = change_endian_long(0x00000000u);
                    TSIP.REG_100H.WORD = change_endian_long(0x00000000u);
                    TSIP.REG_100H.WORD = change_endian_long(0x00000000u);
                    TSIP.REG_100H.WORD = change_endian_long(0x00000002u);
                    TSIP.REG_04H.WORD = 0x00000232u;
                    /* WAIT_LOOP */
                    while (1u != TSIP.REG_04H.BIT.B30)
                    {
                        /* waiting */
                    }
                    OutData_KeyIndex2[4] = TSIP.REG_100H.WORD;
                    OutData_KeyIndex2[5] = TSIP.REG_100H.WORD;
                    OutData_KeyIndex2[6] = TSIP.REG_100H.WORD;
                    OutData_KeyIndex2[7] = TSIP.REG_100H.WORD;
                    /* WAIT_LOOP */
                    while (1u != TSIP.REG_04H.BIT.B30)
                    {
                        /* waiting */
                    }
                    OutData_KeyIndex2[8] = TSIP.REG_100H.WORD;
                    OutData_KeyIndex2[9] = TSIP.REG_100H.WORD;
                    OutData_KeyIndex2[10] = TSIP.REG_100H.WORD;
                    OutData_KeyIndex2[11] = TSIP.REG_100H.WORD;
                    /* WAIT_LOOP */
                    while (1u != TSIP.REG_04H.BIT.B30)
                    {
                        /* waiting */
                    }
                    OutData_KeyIndex2[12] = TSIP.REG_100H.WORD;
                    OutData_KeyIndex2[13] = TSIP.REG_100H.WORD;
                    OutData_KeyIndex2[14] = TSIP.REG_100H.WORD;
                    OutData_KeyIndex2[15] = TSIP.REG_100H.WORD;
                    RX65NHU_func100(change_endian_long(0xee5c1846u), change_endian_long(0xbc924eeau), change_endian_long(0xd0415c70u), change_endian_long(0x81d5e31cu));
                    TSIP.REG_C4H.WORD = 0x000c2b0du;
                    TSIP.REG_E0H.WORD = 0x81040080u;
                    TSIP.REG_00H.WORD = 0x00002813u;
                    /* WAIT_LOOP */
                    while (0u != TSIP.REG_00H.BIT.B25)
                    {
                        /* waiting */
                    }
                    TSIP.REG_1CH.WORD = 0x00001800u;
                    TSIP.REG_C4H.WORD = 0x000c0b0cu;
                    TSIP.REG_E0H.WORD = 0x810103a0u;
                    TSIP.REG_00H.WORD = 0x00002807u;
                    /* WAIT_LOOP */
                    while (0u != TSIP.REG_00H.BIT.B25)
                    {
                        /* waiting */
                    }
                    TSIP.REG_1CH.WORD = 0x00001800u;
                    TSIP.REG_D0H.WORD = 0x00000100u;
                    TSIP.REG_C4H.WORD = 0x02e087bfu;
                    TSIP.REG_00H.WORD = 0x00002323u;
                    TSIP.REG_2CH.WORD = 0x000000a0u;
                    /* WAIT_LOOP */
                    while (0u != TSIP.REG_00H.BIT.B25)
                    {
                        /* waiting */
                    }
                    TSIP.REG_1CH.WORD = 0x00001800u;
                    TSIP.REG_104H.WORD = 0x00000352u;
                    TSIP.REG_C4H.WORD = 0x000009cdu;
                    /* WAIT_LOOP */
                    while (1u != TSIP.REG_104H.BIT.B31)
                    {
                        /* waiting */
                    }
                    TSIP.REG_100H.WORD = change_endian_long(0x00000000u);
                    TSIP.REG_100H.WORD = change_endian_long(0x00000000u);
                    TSIP.REG_100H.WORD = change_endian_long(0x00000000u);
                    TSIP.REG_100H.WORD = change_endian_long(0x00000002u);
                    TSIP.REG_04H.WORD = 0x00000232u;
                    /* WAIT_LOOP */
                    while (1u != TSIP.REG_04H.BIT.B30)
                    {
                        /* waiting */
                    }
                    OutData_KeyIndex3[4] = TSIP.REG_100H.WORD;
                    OutData_KeyIndex3[5] = TSIP.REG_100H.WORD;
                    OutData_KeyIndex3[6] = TSIP.REG_100H.WORD;
                    OutData_KeyIndex3[7] = TSIP.REG_100H.WORD;
                    /* WAIT_LOOP */
                    while (1u != TSIP.REG_04H.BIT.B30)
                    {
                        /* waiting */
                    }
                    OutData_KeyIndex3[8] = TSIP.REG_100H.WORD;
                    OutData_KeyIndex3[9] = TSIP.REG_100H.WORD;
                    OutData_KeyIndex3[10] = TSIP.REG_100H.WORD;
                    OutData_KeyIndex3[11] = TSIP.REG_100H.WORD;
                    /* WAIT_LOOP */
                    while (1u != TSIP.REG_04H.BIT.B30)
                    {
                        /* waiting */
                    }
                    OutData_KeyIndex3[12] = TSIP.REG_100H.WORD;
                    OutData_KeyIndex3[13] = TSIP.REG_100H.WORD;
                    OutData_KeyIndex3[14] = TSIP.REG_100H.WORD;
                    OutData_KeyIndex3[15] = TSIP.REG_100H.WORD;
                    RX65NHU_func100(change_endian_long(0x1bab2b0au), change_endian_long(0x5725764cu), change_endian_long(0x6401752cu), change_endian_long(0xe2925bcau));
                    TSIP.REG_C4H.WORD = 0x000c2b0du;
                    TSIP.REG_E0H.WORD = 0x81040000u;
                    TSIP.REG_00H.WORD = 0x00002813u;
                    /* WAIT_LOOP */
                    while (0u != TSIP.REG_00H.BIT.B25)
                    {
                        /* waiting */
                    }
                    TSIP.REG_1CH.WORD = 0x00001800u;
                    TSIP.REG_C4H.WORD = 0x000c0b0cu;
                    TSIP.REG_E0H.WORD = 0x810103a0u;
                    TSIP.REG_00H.WORD = 0x00002807u;
                    /* WAIT_LOOP */
                    while (0u != TSIP.REG_00H.BIT.B25)
                    {
                        /* waiting */
                    }
                    TSIP.REG_1CH.WORD = 0x00001800u;
                    TSIP.REG_D0H.WORD = 0x00000100u;
                    TSIP.REG_C4H.WORD = 0x02e087bfu;
                    TSIP.REG_00H.WORD = 0x00002323u;
                    TSIP.REG_2CH.WORD = 0x00000020u;
                    /* WAIT_LOOP */
                    while (0u != TSIP.REG_00H.BIT.B25)
                    {
                        /* waiting */
                    }
                    TSIP.REG_1CH.WORD = 0x00001800u;
                    TSIP.REG_104H.WORD = 0x00000352u;
                    TSIP.REG_C4H.WORD = 0x000009cdu;
                    /* WAIT_LOOP */
                    while (1u != TSIP.REG_104H.BIT.B31)
                    {
                        /* waiting */
                    }
                    TSIP.REG_100H.WORD = change_endian_long(0x00000000u);
                    TSIP.REG_100H.WORD = change_endian_long(0x00000000u);
                    TSIP.REG_100H.WORD = change_endian_long(0x00000000u);
                    TSIP.REG_100H.WORD = change_endian_long(0x00000002u);
                    TSIP.REG_04H.WORD = 0x00000232u;
                    /* WAIT_LOOP */
                    while (1u != TSIP.REG_04H.BIT.B30)
                    {
                        /* waiting */
                    }
                    OutData_KeyIndex4[4] = TSIP.REG_100H.WORD;
                    OutData_KeyIndex4[5] = TSIP.REG_100H.WORD;
                    OutData_KeyIndex4[6] = TSIP.REG_100H.WORD;
                    OutData_KeyIndex4[7] = TSIP.REG_100H.WORD;
                    /* WAIT_LOOP */
                    while (1u != TSIP.REG_04H.BIT.B30)
                    {
                        /* waiting */
                    }
                    OutData_KeyIndex4[8] = TSIP.REG_100H.WORD;
                    OutData_KeyIndex4[9] = TSIP.REG_100H.WORD;
                    OutData_KeyIndex4[10] = TSIP.REG_100H.WORD;
                    OutData_KeyIndex4[11] = TSIP.REG_100H.WORD;
                    /* WAIT_LOOP */
                    while (1u != TSIP.REG_04H.BIT.B30)
                    {
                        /* waiting */
                    }
                    OutData_KeyIndex4[12] = TSIP.REG_100H.WORD;
                    OutData_KeyIndex4[13] = TSIP.REG_100H.WORD;
                    OutData_KeyIndex4[14] = TSIP.REG_100H.WORD;
                    OutData_KeyIndex4[15] = TSIP.REG_100H.WORD;
                    RX65NHU_func101(change_endian_long(0x7e81277bu), change_endian_long(0x5cacf06eu), change_endian_long(0x1cfcdc01u), change_endian_long(0xb6196c3fu));
                }
                RX65NHU_func100(change_endian_long(0xbce73082u), change_endian_long(0x301dd3b7u), change_endian_long(0x753a944bu), change_endian_long(0x5c954edcu));
                TSIP.REG_D0H.WORD = 0x00000100u;
                TSIP.REG_C4H.WORD = 0x00008887u;
                TSIP.REG_00H.WORD = 0x00002313u;
                TSIP.REG_2CH.WORD = 0x000000a1u;
                /* WAIT_LOOP */
                while (0u != TSIP.REG_00H.BIT.B25)
                {
                    /* waiting */
                }
                TSIP.REG_1CH.WORD = 0x00001800u;
                TSIP.REG_00H.WORD = 0x00000313u;
                /* WAIT_LOOP */
                while (0u != TSIP.REG_00H.BIT.B25)
                {
                    /* waiting */
                }
                TSIP.REG_1CH.WORD = 0x00001800u;
                TSIP.REG_00H.WORD = 0x00002313u;
                TSIP.REG_2CH.WORD = 0x00000021u;
                /* WAIT_LOOP */
                while (0u != TSIP.REG_00H.BIT.B25)
                {
                    /* waiting */
                }
                TSIP.REG_1CH.WORD = 0x00001800u;
                TSIP.REG_00H.WORD = 0x00000313u;
                /* WAIT_LOOP */
                while (0u != TSIP.REG_00H.BIT.B25)
                {
                    /* waiting */
                }
                TSIP.REG_1CH.WORD = 0x00001800u;
                TSIP.REG_04H.WORD = 0x00000222u;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_04H.BIT.B30)
                {
                    /* waiting */
                }
                OutData_IV1[0] = TSIP.REG_100H.WORD;
                OutData_IV1[1] = TSIP.REG_100H.WORD;
                OutData_IV1[2] = TSIP.REG_100H.WORD;
                OutData_IV1[3] = TSIP.REG_100H.WORD;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_04H.BIT.B30)
                {
                    /* waiting */
                }
                OutData_IV2[0] = TSIP.REG_100H.WORD;
                OutData_IV2[1] = TSIP.REG_100H.WORD;
                OutData_IV2[2] = TSIP.REG_100H.WORD;
                OutData_IV2[3] = TSIP.REG_100H.WORD;
                RX65NHU_func101(change_endian_long(0x5e2d1295u), change_endian_long(0x53b88d13u), change_endian_long(0x2a80e0c8u), change_endian_long(0x98c3886fu));
            }
            RX65NHU_func100(change_endian_long(0x845b104eu), change_endian_long(0xe0d874b4u), change_endian_long(0xe8bab4afu), change_endian_long(0x7aae5755u));
            TSIP.REG_E0H.WORD = 0x81100000u;
            TSIP.REG_04H.WORD = 0x00000642u;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_04H.BIT.B30)
            {
                /* waiting */
            }
            OutData_KeyIndex4[0] = TSIP.REG_100H.WORD;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_04H.BIT.B30)
            {
                /* waiting */
            }
            OutData_KeyIndex4[1] = TSIP.REG_100H.WORD;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_04H.BIT.B30)
            {
                /* waiting */
            }
            OutData_KeyIndex4[2] = TSIP.REG_100H.WORD;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_04H.BIT.B30)
            {
                /* waiting */
            }
            OutData_KeyIndex4[3] = TSIP.REG_100H.WORD;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_04H.BIT.B30)
            {
                /* waiting */
            }
            OutData_KeyIndex3[0] = TSIP.REG_100H.WORD;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_04H.BIT.B30)
            {
                /* waiting */
            }
            OutData_KeyIndex3[1] = TSIP.REG_100H.WORD;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_04H.BIT.B30)
            {
                /* waiting */
            }
            OutData_KeyIndex3[2] = TSIP.REG_100H.WORD;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_04H.BIT.B30)
            {
                /* waiting */
            }
            OutData_KeyIndex3[3] = TSIP.REG_100H.WORD;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_04H.BIT.B30)
            {
                /* waiting */
            }
            OutData_KeyIndex2[0] = TSIP.REG_100H.WORD;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_04H.BIT.B30)
            {
                /* waiting */
            }
            OutData_KeyIndex2[1] = TSIP.REG_100H.WORD;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_04H.BIT.B30)
            {
                /* waiting */
            }
            OutData_KeyIndex2[2] = TSIP.REG_100H.WORD;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_04H.BIT.B30)
            {
                /* waiting */
            }
            OutData_KeyIndex2[3] = TSIP.REG_100H.WORD;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_04H.BIT.B30)
            {
                /* waiting */
            }
            OutData_KeyIndex1[0] = TSIP.REG_100H.WORD;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_04H.BIT.B30)
            {
                /* waiting */
            }
            OutData_KeyIndex1[1] = TSIP.REG_100H.WORD;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_04H.BIT.B30)
            {
                /* waiting */
            }
            OutData_KeyIndex1[2] = TSIP.REG_100H.WORD;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_04H.BIT.B30)
            {
                /* waiting */
            }
            OutData_KeyIndex1[3] = TSIP.REG_100H.WORD;
            RX65NHU_func102(change_endian_long(0x427d323cu), change_endian_long(0x284e3773u), change_endian_long(0x78340f9fu), change_endian_long(0x6f6529d8u));
            TSIP.REG_1BCH.WORD = 0x00000040u;
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
 End of function ./input_dir/RX65NHU/RX65NHU_p5f.prc
 *********************************************************************************************************************/
