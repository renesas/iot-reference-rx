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
* Function Name: RX65NHU_func405
*******************************************************************************************************************/ /**
* @details       RX65NHU func405
* @param[in]     ARG1
* @param[in]     ARG2
* @param[in]     ARG3
* @param[in]     ARG4
* @param[in]     ARG5
* @param[in]     ARG6
* @param[in]     ARG7
* @param[in]     ARG8
* @param[in]     ARG9
* @note          None
*/
void RX65NHU_func405(uint32_t ARG1, uint32_t ARG2, uint32_t *ARG3, uint32_t *ARG4, uint32_t *ARG5, uint32_t *ARG6, uint32_t *ARG7, const uint32_t *ARG8, uint32_t *ARG9)
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
    TSIP.REG_104H.WORD = 0x00000068u;
    TSIP.REG_E0H.WORD = 0x800101e0u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = ARG3[0];
    TSIP.REG_ECH.WORD = 0x00000bffu;
    TSIP.REG_ECH.WORD = 0x38000defu;
    TSIP.REG_ECH.WORD = 0x1000d3e0u;
    TSIP.REG_ECH.WORD = 0x380089e0u;
    TSIP.REG_ECH.WORD = 0x00000002u;
    TSIP.REG_ECH.WORD = 0x1000d3e1u;
    TSIP.REG_ECH.WORD = 0x38000fffu;
    TSIP.REG_E0H.WORD = 0x00000080u;
    TSIP.REG_1CH.WORD = 0x00260000u;
    RX65NHU_func100(change_endian_long(0x2148bb61u), change_endian_long(0xbae581cbu), change_endian_long(0xfc550838u), change_endian_long(0x6cf75d3fu));
    TSIP.REG_1CH.WORD = 0x00400000u;
    TSIP.REG_1D0H.WORD = 0x00000000u;
    if (1u == (TSIP.REG_1CH.BIT.B22))
    {
        TSIP.REG_ECH.WORD = 0x0000b400u;
        TSIP.REG_ECH.WORD = 0xffffffffu;
        RX65NHU_func101(change_endian_long(0xab846a41u), change_endian_long(0x08a341bau), change_endian_long(0xbb314a48u), change_endian_long(0x51bdd828u));
    }
    else
    {
        RX65NHU_func100(change_endian_long(0x4625c400u), change_endian_long(0x745f58f7u), change_endian_long(0xe3acf771u), change_endian_long(0xe2b1e3c1u));
        TSIP.REG_104H.WORD = 0x00000468u;
        TSIP.REG_E0H.WORD = 0x800502c0u;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_104H.BIT.B31)
        {
            /* waiting */
        }
        TSIP.REG_100H.WORD = ARG5[0];
        for (iLoop = 0; iLoop < 4; iLoop = iLoop + 1)
        {
            /* WAIT_LOOP */
            while (1u != TSIP.REG_104H.BIT.B31)
            {
                /* waiting */
            }
            TSIP.REG_100H.WORD = ARG7[iLoop];
        }
        TSIP.REG_ECH.WORD = 0x00007c0fu;
        TSIP.REG_1CH.WORD = 0x00600000u;
        TSIP.REG_1D0H.WORD = 0x00000000u;
        if (0x00000000u == (TSIP.REG_1CH.WORD & 0xff000000u))
        {
            TSIP.REG_ECH.WORD = 0x3c002af8u;
            TSIP.REG_E0H.WORD = 0x00000080u;
            TSIP.REG_1CH.WORD = 0x00260000u;
            TSIP.REG_ECH.WORD = 0x3c002b19u;
            TSIP.REG_E0H.WORD = 0x00000080u;
            TSIP.REG_1CH.WORD = 0x00260000u;
            TSIP.REG_ECH.WORD = 0x3c002b59u;
            TSIP.REG_E0H.WORD = 0x00000080u;
            TSIP.REG_1CH.WORD = 0x00A60000u;
            TSIP.REG_ECH.WORD = 0x3c002b56u;
            TSIP.REG_E0H.WORD = 0x00000080u;
            TSIP.REG_1CH.WORD = 0x00260000u;
            TSIP.REG_ECH.WORD = 0x00003417u;
            TSIP.REG_ECH.WORD = 0x0000a400u;
            TSIP.REG_ECH.WORD = 0x000000ffu;
            TSIP.REG_ECH.WORD = 0x08002818u;
            TSIP.REG_E0H.WORD = 0x00000080u;
            TSIP.REG_1CH.WORD = 0x00A60000u;
            TSIP.REG_ECH.WORD = 0x00003419u;
            TSIP.REG_ECH.WORD = 0x0000a400u;
            TSIP.REG_ECH.WORD = 0x00000003u;
            TSIP.REG_ECH.WORD = 0x0c00281au;
            TSIP.REG_E0H.WORD = 0x00000080u;
            TSIP.REG_1CH.WORD = 0x00A60000u;
            RX65NHU_func101(change_endian_long(0x6eac5bebu), change_endian_long(0x3dab548cu), change_endian_long(0x2b0102e7u), change_endian_long(0x04b490b4u));
        }
        else
        {
            TSIP.REG_ECH.WORD = 0x3c002af8u;
            TSIP.REG_E0H.WORD = 0x00000080u;
            TSIP.REG_1CH.WORD = 0x00260000u;
            TSIP.REG_ECH.WORD = 0x3c002b19u;
            TSIP.REG_E0H.WORD = 0x00000080u;
            TSIP.REG_1CH.WORD = 0x00260000u;
            TSIP.REG_ECH.WORD = 0x3c002b3au;
            TSIP.REG_E0H.WORD = 0x00000080u;
            TSIP.REG_1CH.WORD = 0x00260000u;
            TSIP.REG_ECH.WORD = 0x3c002b56u;
            TSIP.REG_E0H.WORD = 0x00000080u;
            TSIP.REG_1CH.WORD = 0x00260000u;
            TSIP.REG_ECH.WORD = 0x00003417u;
            TSIP.REG_ECH.WORD = 0x0000a400u;
            TSIP.REG_ECH.WORD = 0x0000001fu;
            TSIP.REG_ECH.WORD = 0x08002818u;
            TSIP.REG_E0H.WORD = 0x00000080u;
            TSIP.REG_1CH.WORD = 0x00A60000u;
            TSIP.REG_ECH.WORD = 0x00003419u;
            TSIP.REG_ECH.WORD = 0x0000a400u;
            TSIP.REG_ECH.WORD = 0x0000001fu;
            TSIP.REG_ECH.WORD = 0x0800281au;
            TSIP.REG_E0H.WORD = 0x00000080u;
            TSIP.REG_1CH.WORD = 0x00A60000u;
            RX65NHU_func101(change_endian_long(0xe687ec45u), change_endian_long(0xd29d4fdeu), change_endian_long(0x4111ebb3u), change_endian_long(0x69ac3342u));
        }
        RX65NHU_func100(change_endian_long(0x00979568u), change_endian_long(0x0140c15au), change_endian_long(0xe855c9e0u), change_endian_long(0xcb34eb53u));
        TSIP.REG_1CH.WORD = 0x00400000u;
        TSIP.REG_1D0H.WORD = 0x00000000u;
        if (1u == (TSIP.REG_1CH.BIT.B22))
        {
            TSIP.REG_ECH.WORD = 0x0000b400u;
            TSIP.REG_ECH.WORD = 0xffffffffu;
            RX65NHU_func101(change_endian_long(0x30c98f12u), change_endian_long(0xe5581a8au), change_endian_long(0xb417880fu), change_endian_long(0x54d79bbeu));
        }
        else
        {
            RX65NHU_func100(change_endian_long(0x51607129u), change_endian_long(0x2ab366a0u), change_endian_long(0x92d7c7d6u), change_endian_long(0xc1ed584cu));
            TSIP.REG_C4H.WORD = 0x200c3b0du;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_104H.BIT.B31)
            {
                /* waiting */
            }
            TSIP.REG_100H.WORD = S_INST[ARG1 + 0];
            TSIP.REG_100H.WORD = S_INST[ARG1 + 1];
            TSIP.REG_100H.WORD = S_INST[ARG1 + 2];
            TSIP.REG_100H.WORD = S_INST[ARG1 + 3];
            TSIP.REG_C4H.WORD = 0x00080b8cu;
            TSIP.REG_E0H.WORD = 0x81010240u;
            TSIP.REG_00H.WORD = 0x00002807u;
            /* WAIT_LOOP */
            while (0u != TSIP.REG_00H.BIT.B25)
            {
                /* waiting */
            }
            TSIP.REG_1CH.WORD = 0x00001800u;
            TSIP.REG_C4H.WORD = 0x400c0b0cu;
            TSIP.REG_E0H.WORD = 0x81010260u;
            TSIP.REG_00H.WORD = 0x00002807u;
            /* WAIT_LOOP */
            while (0u != TSIP.REG_00H.BIT.B25)
            {
                /* waiting */
            }
            TSIP.REG_1CH.WORD = 0x00001800u;
            TSIP.REG_28H.WORD = 0x00bf0001u;
            TSIP.REG_00H.WORD = 0x00013203u;
            TSIP.REG_2CH.WORD = 0x00000014u;
            TSIP.REG_104H.WORD = 0x00007f62u;
            TSIP.REG_D0H.WORD = 0x00001f00u;
            TSIP.REG_C4H.WORD = 0x02f087bfu;
            for (iLoop = 0; iLoop < 64; iLoop = iLoop + 4)
            {
                /* WAIT_LOOP */
                while (1u != TSIP.REG_104H.BIT.B31)
                {
                    /* waiting */
                }
                TSIP.REG_100H.WORD = S_INST[ARG2+iLoop + 0];
                TSIP.REG_100H.WORD = S_INST[ARG2+iLoop + 1];
                TSIP.REG_100H.WORD = S_INST[ARG2+iLoop + 2];
                TSIP.REG_100H.WORD = S_INST[ARG2+iLoop + 3];
            }
            /* WAIT_LOOP */
            while (0u != TSIP.REG_00H.BIT.B25)
            {
                /* waiting */
            }
            TSIP.REG_1CH.WORD = 0x00001800u;
            TSIP.REG_00H.WORD = 0x00013203u;
            TSIP.REG_2CH.WORD = 0x00000012u;
            for ( ; iLoop < 128; iLoop = iLoop + 4)
            {
                /* WAIT_LOOP */
                while (1u != TSIP.REG_104H.BIT.B31)
                {
                    /* waiting */
                }
                TSIP.REG_100H.WORD = S_INST[ARG2+iLoop + 0];
                TSIP.REG_100H.WORD = S_INST[ARG2+iLoop + 1];
                TSIP.REG_100H.WORD = S_INST[ARG2+iLoop + 2];
                TSIP.REG_100H.WORD = S_INST[ARG2+iLoop + 3];
            }
            /* WAIT_LOOP */
            while (0u != TSIP.REG_00H.BIT.B25)
            {
                /* waiting */
            }
            TSIP.REG_1CH.WORD = 0x00001800u;
            TSIP.REG_C4H.WORD = 0x00f007bdu;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_104H.BIT.B31)
            {
                /* waiting */
            }
            TSIP.REG_100H.WORD = S_INST[ARG2+iLoop + 0];
            TSIP.REG_100H.WORD = S_INST[ARG2+iLoop + 1];
            TSIP.REG_100H.WORD = S_INST[ARG2+iLoop + 2];
            TSIP.REG_100H.WORD = S_INST[ARG2+iLoop + 3];
            iLoop = iLoop+4;
            TSIP.REG_E0H.WORD = 0x80040140u;
            TSIP.REG_00H.WORD = 0x00008213u;
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
            TSIP.REG_100H.WORD = S_INST[ARG2+iLoop + 0];
            TSIP.REG_100H.WORD = S_INST[ARG2+iLoop + 1];
            TSIP.REG_100H.WORD = S_INST[ARG2+iLoop + 2];
            TSIP.REG_100H.WORD = S_INST[ARG2+iLoop + 3];
            TSIP.REG_C4H.WORD = 0x00800c45u;
            TSIP.REG_00H.WORD = 0x00002213u;
            /* WAIT_LOOP */
            while (0u != TSIP.REG_00H.BIT.B25)
            {
                /* waiting */
            }
            TSIP.REG_1CH.WORD = 0x00001800u;
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
            TSIP.REG_ECH.WORD = 0x00003417u;
            TSIP.REG_ECH.WORD = 0x00046800u;
            TSIP.REG_ECH.WORD = 0x00026c00u;
            TSIP.REG_E0H.WORD = 0x81010000u;
            TSIP.REG_04H.WORD = 0x00000606u;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_04H.BIT.B30)
            {
                /* waiting */
            }
            S_RAM[0] = change_endian_long(TSIP.REG_100H.WORD);
            TSIP.REG_ECH.WORD = 0x000008a5u;
            for (iLoop = 0; iLoop < S_RAM[0]; iLoop = iLoop + 4)
            {
                TSIP.REG_104H.WORD = 0x00000364u;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_104H.BIT.B31)
                {
                    /* waiting */
                }
                TSIP.REG_100H.WORD = ARG4[iLoop+0];
                TSIP.REG_100H.WORD = ARG4[iLoop+1];
                TSIP.REG_100H.WORD = ARG4[iLoop+2];
                TSIP.REG_100H.WORD = ARG4[iLoop+3];
                TSIP.REG_ECH.WORD = 0x0000a4a0u;
                TSIP.REG_ECH.WORD = 0x00000004u;
                RX65NHU_func101(change_endian_long(0x261b3f7du), change_endian_long(0x6a9b2c75u), change_endian_long(0xe518ebe8u), change_endian_long(0x830fc666u));
            }
            RX65NHU_func100(change_endian_long(0x43b3dd55u), change_endian_long(0x53126c5bu), change_endian_long(0x982eb596u), change_endian_long(0x734ba4feu));
            TSIP.REG_ECH.WORD = 0x38000805u;
            TSIP.REG_E0H.WORD = 0x00000080u;
            TSIP.REG_1CH.WORD = 0x00260000u;
            TSIP.REG_1CH.WORD = 0x00402000u;
            TSIP.REG_ECH.WORD = 0x00007c0fu;
            TSIP.REG_1CH.WORD = 0x00600000u;
            TSIP.REG_1D0H.WORD = 0x00000000u;
            if (0x00000000u == (TSIP.REG_1CH.WORD & 0xff000000u))
            {
                RX65NHU_func100(change_endian_long(0x24f4c5ceu), change_endian_long(0xf9bfc95cu), change_endian_long(0x1c1242f9u), change_endian_long(0x4693da8au));
                TSIP.REG_28H.WORD = 0x00bf0001u;
                TSIP.REG_2CH.WORD = 0x00000010u;
                TSIP.REG_ECH.WORD = 0x00003416u;
                TSIP.REG_ECH.WORD = 0x00026800u;
                TSIP.REG_ECH.WORD = 0x38008ec0u;
                TSIP.REG_ECH.WORD = 0x00000003u;
                TSIP.REG_ECH.WORD = 0x20002c00u;
                TSIP.REG_E0H.WORD = 0x81010000u;
                TSIP.REG_04H.WORD = 0x00000606u;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_04H.BIT.B30)
                {
                    /* waiting */
                }
                S_RAM[0] = change_endian_long(TSIP.REG_100H.WORD);
                TSIP.REG_ECH.WORD = 0x00003445u;
                TSIP.REG_ECH.WORD = 0x00026c42u;
                TSIP.REG_ECH.WORD = 0x000034d6u;
                TSIP.REG_ECH.WORD = 0x000030c0u;
                TSIP.REG_ECH.WORD = 0x00000a10u;
                for ( ; iLoop < S_RAM[0]; iLoop = iLoop + 1)
                {
                    TSIP.REG_104H.WORD = 0x00000068u;
                    TSIP.REG_E0H.WORD = 0x80010100u;
                    /* WAIT_LOOP */
                    while (1u != TSIP.REG_104H.BIT.B31)
                    {
                        /* waiting */
                    }
                    TSIP.REG_100H.WORD = ARG4[iLoop];
                    TSIP.REG_ECH.WORD = 0x0000b420u;
                    TSIP.REG_ECH.WORD = 0x00000004u;
                    for (jLoop = 0; jLoop < 4; jLoop = jLoop + 1)
                    {
                        TSIP.REG_ECH.WORD = 0x00003020u;
                        TSIP.REG_ECH.WORD = 0x01886ce8u;
                        TSIP.REG_ECH.WORD = 0x00086d08u;
                        TSIP.REG_ECH.WORD = 0x00000863u;
                        TSIP.REG_ECH.WORD = 0x3c002859u;
                        TSIP.REG_ECH.WORD = 0x20002c60u;
                        TSIP.REG_ECH.WORD = 0x3c002b42u;
                        TSIP.REG_ECH.WORD = 0x20002c60u;
                        TSIP.REG_ECH.WORD = 0x30000c63u;
                        TSIP.REG_ECH.WORD = 0x00030020u;
                        TSIP.REG_ECH.WORD = 0x00186ce7u;
                        TSIP.REG_ECH.WORD = 0x01886e07u;
                        TSIP.REG_ECH.WORD = 0x001868e7u;
                        TSIP.REG_ECH.WORD = 0x00000080u;
                        TSIP.REG_ECH.WORD = 0x00000863u;
                        TSIP.REG_ECH.WORD = 0x3c002857u;
                        TSIP.REG_ECH.WORD = 0x20002c60u;
                        TSIP.REG_ECH.WORD = 0x3c002b02u;
                        TSIP.REG_ECH.WORD = 0x20002c60u;
                        TSIP.REG_ECH.WORD = 0x38000c63u;
                        TSIP.REG_E0H.WORD = 0x00000080u;
                        TSIP.REG_1CH.WORD = 0x00260000u;
                        RX65NHU_func100(change_endian_long(0xefc53e81u), change_endian_long(0x1d6b304fu), change_endian_long(0x8d4feae1u), change_endian_long(0xf60835c9u));
                        TSIP.REG_1CH.WORD = 0x00400000u;
                        TSIP.REG_1D0H.WORD = 0x00000000u;
                        if (1u == (TSIP.REG_1CH.BIT.B22))
                        {
                            TSIP.REG_E0H.WORD = 0x810100e0u;
                            TSIP.REG_00H.WORD = 0x0800080fu;
                            /* WAIT_LOOP */
                            while (0u != TSIP.REG_00H.BIT.B25)
                            {
                                /* waiting */
                            }
                            TSIP.REG_1CH.WORD = 0x00001800u;
                            TSIP.REG_00H.WORD = 0x08003807u;
                            /* WAIT_LOOP */
                            while (0u != TSIP.REG_00H.BIT.B25)
                            {
                                /* waiting */
                            }
                            TSIP.REG_1CH.WORD = 0x00001800u;
                            RX65NHU_func101(change_endian_long(0x1412e082u), change_endian_long(0x4543f9adu), change_endian_long(0x0866bab8u), change_endian_long(0x9326fb4au));
                        }
                        TSIP.REG_ECH.WORD = 0x00000863u;
                        TSIP.REG_ECH.WORD = 0x3c0028c2u;
                        TSIP.REG_ECH.WORD = 0x20002c60u;
                        TSIP.REG_ECH.WORD = 0x38000c63u;
                        TSIP.REG_E0H.WORD = 0x00000080u;
                        TSIP.REG_1CH.WORD = 0x00260000u;
                        RX65NHU_func100(change_endian_long(0xe2095f4au), change_endian_long(0xd7c9c9eeu), change_endian_long(0x91f4cfbeu), change_endian_long(0x2cd1962bu));
                        TSIP.REG_1CH.WORD = 0x00400000u;
                        TSIP.REG_1D0H.WORD = 0x00000000u;
                        if (1u == (TSIP.REG_1CH.BIT.B22))
                        {
                            TSIP.REG_E0H.WORD = 0x810100e0u;
                            TSIP.REG_00H.WORD = 0x0800080fu;
                            /* WAIT_LOOP */
                            while (0u != TSIP.REG_00H.BIT.B25)
                            {
                                /* waiting */
                            }
                            TSIP.REG_1CH.WORD = 0x00001800u;
                            TSIP.REG_00H.WORD = 0x08005807u;
                            /* WAIT_LOOP */
                            while (0u != TSIP.REG_00H.BIT.B25)
                            {
                                /* waiting */
                            }
                            TSIP.REG_1CH.WORD = 0x00001800u;
                            RX65NHU_func101(change_endian_long(0x435885a1u), change_endian_long(0x45144e71u), change_endian_long(0x839cdf20u), change_endian_long(0x69f96112u));
                        }
                        TSIP.REG_ECH.WORD = 0x00002c40u;
                        RX65NHU_func101(change_endian_long(0xa1f18987u), change_endian_long(0x7482bd5du), change_endian_long(0x4ebdc9f2u), change_endian_long(0xe98ade8bu));
                    }
                    TSIP.REG_ECH.WORD = 0x08000c21u;
                    TSIP.REG_E0H.WORD = 0x00000080u;
                    TSIP.REG_1CH.WORD = 0x00260000u;
                    TSIP.REG_1CH.WORD = 0x00402000u;
                    TSIP.REG_ECH.WORD = 0x00002ca0u;
                    RX65NHU_func101(change_endian_long(0x15c83ea7u), change_endian_long(0x6f1e0fd3u), change_endian_long(0x5d195b3eu), change_endian_long(0x37b25de6u));
                }
                TSIP.REG_ECH.WORD = 0x38000805u;
                TSIP.REG_E0H.WORD = 0x00000080u;
                TSIP.REG_1CH.WORD = 0x00260000u;
                TSIP.REG_1CH.WORD = 0x00402000u;
                RX65NHU_func101(change_endian_long(0xb0af9d43u), change_endian_long(0xb95909cau), change_endian_long(0xf625cf73u), change_endian_long(0x0480c4f8u));
            }
            else
            {
                RX65NHU_func100(change_endian_long(0xdcc9e00cu), change_endian_long(0x8f1e85d5u), change_endian_long(0x5604ef46u), change_endian_long(0x7786023bu));
                TSIP.REG_28H.WORD = 0x008f0001u;
                TSIP.REG_2CH.WORD = 0x00000010u;
                TSIP.REG_ECH.WORD = 0x00003416u;
                TSIP.REG_ECH.WORD = 0x00026800u;
                TSIP.REG_ECH.WORD = 0x38008ec0u;
                TSIP.REG_ECH.WORD = 0x00000003u;
                TSIP.REG_ECH.WORD = 0x20002c00u;
                TSIP.REG_E0H.WORD = 0x81010000u;
                TSIP.REG_04H.WORD = 0x00000606u;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_04H.BIT.B30)
                {
                    /* waiting */
                }
                S_RAM[0] = change_endian_long(TSIP.REG_100H.WORD);
                TSIP.REG_ECH.WORD = 0x00003445u;
                TSIP.REG_ECH.WORD = 0x00026c42u;
                TSIP.REG_ECH.WORD = 0x000034d6u;
                TSIP.REG_ECH.WORD = 0x000030c0u;
                TSIP.REG_ECH.WORD = 0x00000a10u;
                for ( ; iLoop < S_RAM[0]; iLoop = iLoop + 1)
                {
                    TSIP.REG_104H.WORD = 0x00000068u;
                    TSIP.REG_E0H.WORD = 0x80010100u;
                    /* WAIT_LOOP */
                    while (1u != TSIP.REG_104H.BIT.B31)
                    {
                        /* waiting */
                    }
                    TSIP.REG_100H.WORD = ARG4[iLoop];
                    TSIP.REG_ECH.WORD = 0x0000b420u;
                    TSIP.REG_ECH.WORD = 0x00000004u;
                    for (jLoop = 0; jLoop < 4; jLoop = jLoop + 1)
                    {
                        TSIP.REG_ECH.WORD = 0x00003020u;
                        TSIP.REG_ECH.WORD = 0x01886ce8u;
                        TSIP.REG_ECH.WORD = 0x00086d08u;
                        TSIP.REG_ECH.WORD = 0x00000863u;
                        TSIP.REG_ECH.WORD = 0x3c002859u;
                        TSIP.REG_ECH.WORD = 0x20002c60u;
                        TSIP.REG_ECH.WORD = 0x3c002b42u;
                        TSIP.REG_ECH.WORD = 0x20002c60u;
                        TSIP.REG_ECH.WORD = 0x38000c63u;
                        TSIP.REG_E0H.WORD = 0x00000080u;
                        TSIP.REG_1CH.WORD = 0x00260000u;
                        RX65NHU_func100(change_endian_long(0x7425b81fu), change_endian_long(0xc0836814u), change_endian_long(0x10ca9249u), change_endian_long(0x43414fcfu));
                        TSIP.REG_1CH.WORD = 0x00400000u;
                        TSIP.REG_1D0H.WORD = 0x00000000u;
                        if (1u == (TSIP.REG_1CH.BIT.B22))
                        {
                            TSIP.REG_E0H.WORD = 0x810100e0u;
                            TSIP.REG_00H.WORD = 0x0800080fu;
                            /* WAIT_LOOP */
                            while (0u != TSIP.REG_00H.BIT.B25)
                            {
                                /* waiting */
                            }
                            TSIP.REG_1CH.WORD = 0x00001800u;
                            TSIP.REG_00H.WORD = 0x08003807u;
                            /* WAIT_LOOP */
                            while (0u != TSIP.REG_00H.BIT.B25)
                            {
                                /* waiting */
                            }
                            TSIP.REG_1CH.WORD = 0x00001800u;
                            RX65NHU_func101(change_endian_long(0x31a8fb32u), change_endian_long(0x275234e6u), change_endian_long(0xb9ba2281u), change_endian_long(0x94b17e64u));
                        }
                        TSIP.REG_ECH.WORD = 0x00000863u;
                        TSIP.REG_ECH.WORD = 0x3c002857u;
                        TSIP.REG_ECH.WORD = 0x20002c60u;
                        TSIP.REG_ECH.WORD = 0x3c002b02u;
                        TSIP.REG_ECH.WORD = 0x20002c60u;
                        TSIP.REG_ECH.WORD = 0x38000c63u;
                        TSIP.REG_E0H.WORD = 0x00000080u;
                        TSIP.REG_1CH.WORD = 0x00260000u;
                        RX65NHU_func100(change_endian_long(0x01ea56fcu), change_endian_long(0x2e2f3c7au), change_endian_long(0x82377e2au), change_endian_long(0xf3613d7cu));
                        TSIP.REG_1CH.WORD = 0x00400000u;
                        TSIP.REG_1D0H.WORD = 0x00000000u;
                        if (1u == (TSIP.REG_1CH.BIT.B22))
                        {
                            TSIP.REG_E0H.WORD = 0x810100e0u;
                            TSIP.REG_00H.WORD = 0x0800080fu;
                            /* WAIT_LOOP */
                            while (0u != TSIP.REG_00H.BIT.B25)
                            {
                                /* waiting */
                            }
                            TSIP.REG_1CH.WORD = 0x00001800u;
                            TSIP.REG_00H.WORD = 0x08003807u;
                            /* WAIT_LOOP */
                            while (0u != TSIP.REG_00H.BIT.B25)
                            {
                                /* waiting */
                            }
                            TSIP.REG_1CH.WORD = 0x00001800u;
                            RX65NHU_func101(change_endian_long(0x3436b868u), change_endian_long(0x50ff4f46u), change_endian_long(0xe2196ae7u), change_endian_long(0xc52d7831u));
                        }
                        TSIP.REG_ECH.WORD = 0x00000863u;
                        TSIP.REG_ECH.WORD = 0x3c0028c2u;
                        TSIP.REG_ECH.WORD = 0x20002c60u;
                        TSIP.REG_ECH.WORD = 0x38000c63u;
                        TSIP.REG_E0H.WORD = 0x00000080u;
                        TSIP.REG_1CH.WORD = 0x00260000u;
                        RX65NHU_func100(change_endian_long(0xdabfc5a9u), change_endian_long(0x04a4175eu), change_endian_long(0xc9a57d94u), change_endian_long(0x972ef830u));
                        TSIP.REG_1CH.WORD = 0x00400000u;
                        TSIP.REG_1D0H.WORD = 0x00000000u;
                        if (1u == (TSIP.REG_1CH.BIT.B22))
                        {
                            TSIP.REG_E0H.WORD = 0x810100e0u;
                            TSIP.REG_00H.WORD = 0x0800080fu;
                            /* WAIT_LOOP */
                            while (0u != TSIP.REG_00H.BIT.B25)
                            {
                                /* waiting */
                            }
                            TSIP.REG_1CH.WORD = 0x00001800u;
                            TSIP.REG_00H.WORD = 0x08005807u;
                            /* WAIT_LOOP */
                            while (0u != TSIP.REG_00H.BIT.B25)
                            {
                                /* waiting */
                            }
                            TSIP.REG_1CH.WORD = 0x00001800u;
                            RX65NHU_func101(change_endian_long(0x33ad0e36u), change_endian_long(0x38505de2u), change_endian_long(0x526f1031u), change_endian_long(0x0abfcc11u));
                        }
                        TSIP.REG_ECH.WORD = 0x00002c40u;
                        RX65NHU_func101(change_endian_long(0x91516bc6u), change_endian_long(0xeac1a4a4u), change_endian_long(0xa2e5298du), change_endian_long(0x419dafb1u));
                    }
                    TSIP.REG_ECH.WORD = 0x08000c21u;
                    TSIP.REG_E0H.WORD = 0x00000080u;
                    TSIP.REG_1CH.WORD = 0x00260000u;
                    TSIP.REG_1CH.WORD = 0x00402000u;
                    TSIP.REG_ECH.WORD = 0x00002ca0u;
                    RX65NHU_func101(change_endian_long(0x031acea2u), change_endian_long(0xfe587002u), change_endian_long(0xbc0c5769u), change_endian_long(0xcb4118ccu));
                }
                TSIP.REG_ECH.WORD = 0x38000805u;
                TSIP.REG_E0H.WORD = 0x00000080u;
                TSIP.REG_1CH.WORD = 0x00260000u;
                TSIP.REG_1CH.WORD = 0x00402000u;
                RX65NHU_func101(change_endian_long(0xad5f746cu), change_endian_long(0xc4f4ece4u), change_endian_long(0xce11a4d8u), change_endian_long(0xfbd6da90u));
            }
            RX65NHU_func100(change_endian_long(0x9fa35a56u), change_endian_long(0x930c45f6u), change_endian_long(0x5768bc04u), change_endian_long(0x577d57bbu));
            TSIP.REG_ECH.WORD = 0x00003416u;
            TSIP.REG_ECH.WORD = 0x00008c00u;
            TSIP.REG_ECH.WORD = 0x00000003u;
            TSIP.REG_ECH.WORD = 0x0000b440u;
            TSIP.REG_ECH.WORD = 0x00000004u;
            TSIP.REG_ECH.WORD = 0x00002840u;
            TSIP.REG_E0H.WORD = 0x81010040u;
            TSIP.REG_04H.WORD = 0x00000606u;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_04H.BIT.B30)
            {
                /* waiting */
            }
            S_RAM[0] = change_endian_long(TSIP.REG_100H.WORD);
            TSIP.REG_ECH.WORD = 0x000008a5u;
            TSIP.REG_ECH.WORD = 0x0000b460u;
            TSIP.REG_ECH.WORD = 0x80000000u;
            for (iLoop = 0; iLoop < S_RAM[0]; iLoop = iLoop + 1)
            {
                TSIP.REG_E0H.WORD = 0x81010060u;
                TSIP.REG_00H.WORD = 0x08005807u;
                /* WAIT_LOOP */
                while (0u != TSIP.REG_00H.BIT.B25)
                {
                    /* waiting */
                }
                TSIP.REG_1CH.WORD = 0x00001800u;
                TSIP.REG_00H.WORD = 0x0800080fu;
                /* WAIT_LOOP */
                while (0u != TSIP.REG_00H.BIT.B25)
                {
                    /* waiting */
                }
                TSIP.REG_1CH.WORD = 0x00001800u;
                TSIP.REG_ECH.WORD = 0x00086c63u;
                TSIP.REG_ECH.WORD = 0x00002ca0u;
                RX65NHU_func101(change_endian_long(0x42a3a9e0u), change_endian_long(0x34b3a5b9u), change_endian_long(0x085b3ef9u), change_endian_long(0x76614312u));
            }
            RX65NHU_func100(change_endian_long(0xb0a6a6b4u), change_endian_long(0x3d32daf0u), change_endian_long(0x5d0d90c0u), change_endian_long(0x4d4f1e67u));
            TSIP.REG_ECH.WORD = 0x38000845u;
            TSIP.REG_E0H.WORD = 0x00000080u;
            TSIP.REG_1CH.WORD = 0x00260000u;
            TSIP.REG_1CH.WORD = 0x00402000u;
            TSIP.REG_F8H.WORD = 0x00000040u;
            TSIP.REG_ECH.WORD = 0x00000800u;
            TSIP.REG_ECH.WORD = 0x00003436u;
            TSIP.REG_ECH.WORD = 0x01836c01u;
            TSIP.REG_ECH.WORD = 0x00036c21u;
            TSIP.REG_E0H.WORD = 0x81020000u;
            TSIP.REG_00H.WORD = 0x0000580bu;
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
            TSIP.REG_ECH.WORD = 0x00007c0fu;
            TSIP.REG_1CH.WORD = 0x00600000u;
            TSIP.REG_1D0H.WORD = 0x00000000u;
            if (0x00000000u == (TSIP.REG_1CH.WORD & 0xff000000u))
            {
                TSIP.REG_D0H.WORD = 0x00000f14u;
                TSIP.REG_C4H.WORD = 0x00008a07u;
                TSIP.REG_00H.WORD = 0x00012303u;
                TSIP.REG_2CH.WORD = 0x00000020u;
                /* WAIT_LOOP */
                while (0u != TSIP.REG_00H.BIT.B25)
                {
                    /* waiting */
                }
                TSIP.REG_1CH.WORD = 0x00001800u;
                RX65NHU_func101(change_endian_long(0xa635154fu), change_endian_long(0xca54cb62u), change_endian_long(0x2ac58883u), change_endian_long(0x9d08aec9u));
            }
            else
            {
                TSIP.REG_D0H.WORD = 0x00000314u;
                TSIP.REG_C4H.WORD = 0x00008a07u;
                TSIP.REG_00H.WORD = 0x00002343u;
                TSIP.REG_2CH.WORD = 0x00000020u;
                /* WAIT_LOOP */
                while (0u != TSIP.REG_00H.BIT.B25)
                {
                    /* waiting */
                }
                TSIP.REG_1CH.WORD = 0x00001800u;
                TSIP.REG_28H.WORD = 0x00bf0001u;
                RX65NHU_func101(change_endian_long(0x2e551bf8u), change_endian_long(0x0b83987bu), change_endian_long(0x543e29e5u), change_endian_long(0xdff5d30au));
            }
            TSIP.REG_2CH.WORD = 0x00000010u;
            TSIP.REG_104H.WORD = 0x00003f67u;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_104H.BIT.B31)
            {
                /* waiting */
            }
            TSIP.REG_100H.WORD = ARG6[0];
            TSIP.REG_100H.WORD = ARG6[1];
            TSIP.REG_100H.WORD = ARG6[2];
            TSIP.REG_100H.WORD = ARG6[3];
            TSIP.REG_100H.WORD = ARG6[4];
            TSIP.REG_100H.WORD = ARG6[5];
            TSIP.REG_100H.WORD = ARG6[6];
            TSIP.REG_100H.WORD = ARG6[7];
            TSIP.REG_100H.WORD = ARG6[8];
            TSIP.REG_100H.WORD = ARG6[9];
            TSIP.REG_100H.WORD = ARG6[10];
            TSIP.REG_100H.WORD = ARG6[11];
            TSIP.REG_100H.WORD = ARG6[12];
            TSIP.REG_100H.WORD = ARG6[13];
            TSIP.REG_100H.WORD = ARG6[14];
            TSIP.REG_100H.WORD = ARG6[15];
            TSIP.REG_100H.WORD = ARG6[16];
            TSIP.REG_100H.WORD = ARG6[17];
            TSIP.REG_100H.WORD = ARG6[18];
            TSIP.REG_100H.WORD = ARG6[19];
            TSIP.REG_100H.WORD = ARG6[20];
            TSIP.REG_100H.WORD = ARG6[21];
            TSIP.REG_100H.WORD = ARG6[22];
            TSIP.REG_100H.WORD = ARG6[23];
            TSIP.REG_100H.WORD = ARG6[24];
            TSIP.REG_100H.WORD = ARG6[25];
            TSIP.REG_100H.WORD = ARG6[26];
            TSIP.REG_100H.WORD = ARG6[27];
            TSIP.REG_100H.WORD = ARG6[28];
            TSIP.REG_100H.WORD = ARG6[29];
            TSIP.REG_100H.WORD = ARG6[30];
            TSIP.REG_100H.WORD = ARG6[31];
            TSIP.REG_100H.WORD = ARG6[32];
            TSIP.REG_100H.WORD = ARG6[33];
            TSIP.REG_100H.WORD = ARG6[34];
            TSIP.REG_100H.WORD = ARG6[35];
            TSIP.REG_100H.WORD = ARG6[36];
            TSIP.REG_100H.WORD = ARG6[37];
            TSIP.REG_100H.WORD = ARG6[38];
            TSIP.REG_100H.WORD = ARG6[39];
            TSIP.REG_100H.WORD = ARG6[40];
            TSIP.REG_100H.WORD = ARG6[41];
            TSIP.REG_100H.WORD = ARG6[42];
            TSIP.REG_100H.WORD = ARG6[43];
            TSIP.REG_100H.WORD = ARG6[44];
            TSIP.REG_100H.WORD = ARG6[45];
            TSIP.REG_100H.WORD = ARG6[46];
            TSIP.REG_100H.WORD = ARG6[47];
            TSIP.REG_100H.WORD = ARG6[48];
            TSIP.REG_100H.WORD = ARG6[49];
            TSIP.REG_100H.WORD = ARG6[50];
            TSIP.REG_100H.WORD = ARG6[51];
            TSIP.REG_100H.WORD = ARG6[52];
            TSIP.REG_100H.WORD = ARG6[53];
            TSIP.REG_100H.WORD = ARG6[54];
            TSIP.REG_100H.WORD = ARG6[55];
            TSIP.REG_100H.WORD = ARG6[56];
            TSIP.REG_100H.WORD = ARG6[57];
            TSIP.REG_100H.WORD = ARG6[58];
            TSIP.REG_100H.WORD = ARG6[59];
            TSIP.REG_100H.WORD = ARG6[60];
            TSIP.REG_100H.WORD = ARG6[61];
            TSIP.REG_100H.WORD = ARG6[62];
            TSIP.REG_100H.WORD = ARG6[63];
            TSIP.REG_ECH.WORD = 0x0000342au;
            TSIP.REG_ECH.WORD = 0x0000b400u;
            TSIP.REG_ECH.WORD = 0x000000e0u;
            TSIP.REG_ECH.WORD = 0x0000b440u;
            TSIP.REG_ECH.WORD = 0x00000020u;
            TSIP.REG_ECH.WORD = 0x0000b4e0u;
            TSIP.REG_ECH.WORD = 0x000000f0u;
            TSIP.REG_E0H.WORD = 0x81840007u;
            TSIP.REG_C4H.WORD = 0x00000885u;
            TSIP.REG_00H.WORD = 0x00002813u;
            /* WAIT_LOOP */
            while (0u != TSIP.REG_00H.BIT.B25)
            {
                /* waiting */
            }
            TSIP.REG_1CH.WORD = 0x00001800u;
            TSIP.REG_E0H.WORD = 0x80040100u;
            TSIP.REG_00H.WORD = 0x00008213u;
            /* WAIT_LOOP */
            while (0u != TSIP.REG_00H.BIT.B25)
            {
                /* waiting */
            }
            TSIP.REG_1CH.WORD = 0x00001800u;
            RX65NHU_func101(change_endian_long(0x477f381cu), change_endian_long(0xec348475u), change_endian_long(0xe3b2950fu), change_endian_long(0x9b96045du));
            R_TSIP_TlsRootCertificateVerificationSubSub();

            TSIP.REG_ECH.WORD = 0x0000b4e0u;
            TSIP.REG_ECH.WORD = 0x000000f0u;
            TSIP.REG_E0H.WORD = 0x81040100u;
            TSIP.REG_C4H.WORD = 0x00000885u;
            TSIP.REG_00H.WORD = 0x00002813u;
            /* WAIT_LOOP */
            while (0u != TSIP.REG_00H.BIT.B25)
            {
                /* waiting */
            }
            TSIP.REG_1CH.WORD = 0x00001800u;
            TSIP.REG_E0H.WORD = 0x80840007u;
            TSIP.REG_00H.WORD = 0x00008213u;
            /* WAIT_LOOP */
            while (0u != TSIP.REG_00H.BIT.B25)
            {
                /* waiting */
            }
            TSIP.REG_1CH.WORD = 0x00001800u;
            RX65NHU_func100(change_endian_long(0x3718b3edu), change_endian_long(0xbc289dceu), change_endian_long(0x86d26077u), change_endian_long(0x1f1b6eedu));
            TSIP.REG_1CH.WORD = 0x00400000u;
            TSIP.REG_1D0H.WORD = 0x00000000u;
            if (1u == (TSIP.REG_1CH.BIT.B22))
            {
                TSIP.REG_ECH.WORD = 0x0000b400u;
                TSIP.REG_ECH.WORD = 0xffffffffu;
                RX65NHU_func101(change_endian_long(0xa4e11c46u), change_endian_long(0x2dab9bfau), change_endian_long(0x700d2bcau), change_endian_long(0x8213f536u));
            }
            else
            {
                RX65NHU_func100(change_endian_long(0xaa8f2a60u), change_endian_long(0xf7319bbcu), change_endian_long(0xbd2776b7u), change_endian_long(0x929e49aau));
                TSIP.REG_ECH.WORD = 0x00007c0fu;
                TSIP.REG_1CH.WORD = 0x00600000u;
                TSIP.REG_1D0H.WORD = 0x00000000u;
                if (0x00000000u == (TSIP.REG_1CH.WORD & 0xff000000u))
                {
                    RX65NHU_func100(change_endian_long(0x54ec1b27u), change_endian_long(0x06f2762cu), change_endian_long(0x95f343c2u), change_endian_long(0x2b628931u));
                    TSIP.REG_28H.WORD = 0x00bf0001u;
                    TSIP.REG_D0H.WORD = 0x00000f1cu;
                    TSIP.REG_C4H.WORD = 0x00008a83u;
                    TSIP.REG_00H.WORD = 0x00013203u;
                    TSIP.REG_2CH.WORD = 0x00000014u;
                    /* WAIT_LOOP */
                    while (0u != TSIP.REG_00H.BIT.B25)
                    {
                        /* waiting */
                    }
                    TSIP.REG_1CH.WORD = 0x00001800u;
                    TSIP.REG_24H.WORD = 0x000005c0u;
                    /* WAIT_LOOP */
                    while (0u != TSIP.REG_24H.BIT.B21)
                    {
                        /* waiting */
                    }
                    TSIP.REG_24H.WORD = 0x000080c1u;
                    /* WAIT_LOOP */
                    while (0u != TSIP.REG_24H.BIT.B21)
                    {
                        /* waiting */
                    }
                    TSIP.REG_18H.WORD = 0x00000004u;
                    TSIP.REG_24H.WORD = 0x00005004u;
                    /* WAIT_LOOP */
                    while (0u != TSIP.REG_18H.BIT.B10)
                    {
                        /* waiting */
                    }
                    TSIP.REG_24H.WORD = 0x00004804u;
                    /* WAIT_LOOP */
                    while (0u != TSIP.REG_18H.BIT.B10)
                    {
                        /* waiting */
                    }
                    TSIP.REG_18H.WORD = 0x00000000u;
                    TSIP.REG_104H.WORD = 0x00000058u;
                    TSIP.REG_E0H.WORD = 0x80010000u;
                    /* WAIT_LOOP */
                    while (1u != TSIP.REG_104H.BIT.B31)
                    {
                        /* waiting */
                    }
                    TSIP.REG_100H.WORD = change_endian_long(0x05080100u);
                    TSIP.REG_ECH.WORD = 0x00001011u;
                    RX65NHU_func103();
                    TSIP.REG_104H.WORD = 0x00000052u;
                    TSIP.REG_C4H.WORD = 0x01000c84u;
                    /* WAIT_LOOP */
                    while (1u != TSIP.REG_104H.BIT.B31)
                    {
                        /* waiting */
                    }
                    TSIP.REG_100H.WORD = change_endian_long(0x00000000u);
                    TSIP.REG_E0H.WORD = 0x80030020u;
                    TSIP.REG_00H.WORD = 0x0000820fu;
                    /* WAIT_LOOP */
                    while (0u != TSIP.REG_00H.BIT.B25)
                    {
                        /* waiting */
                    }
                    TSIP.REG_1CH.WORD = 0x00001800u;
                    TSIP.REG_00H.WORD = 0x00000207u;
                    /* WAIT_LOOP */
                    while (0u != TSIP.REG_00H.BIT.B25)
                    {
                        /* waiting */
                    }
                    TSIP.REG_1CH.WORD = 0x00001800u;
                    RX65NHU_func100(change_endian_long(0x02563188u), change_endian_long(0xedc158a8u), change_endian_long(0x960e07c7u), change_endian_long(0xd33e3858u));
                    TSIP.REG_C4H.WORD = 0x000c2b0du;
                    TSIP.REG_E0H.WORD = 0x81040000u;
                    TSIP.REG_00H.WORD = 0x00002813u;
                    /* WAIT_LOOP */
                    while (0u != TSIP.REG_00H.BIT.B25)
                    {
                        /* waiting */
                    }
                    TSIP.REG_1CH.WORD = 0x00001800u;
                    TSIP.REG_00H.WORD = 0x00012303u;
                    TSIP.REG_2CH.WORD = 0x00000024u;
                    TSIP.REG_D0H.WORD = 0x00000f00u;
                    TSIP.REG_C4H.WORD = 0x02e087bfu;
                    TSIP.REG_04H.WORD = 0x00000202u;
                    for (iLoop = 4; iLoop < 68; iLoop = iLoop + 4)
                    {
                        /* WAIT_LOOP */
                        while (1u != TSIP.REG_04H.BIT.B30)
                        {
                            /* waiting */
                        }
                        ARG9[iLoop] = TSIP.REG_100H.WORD;
                        ARG9[iLoop+1] = TSIP.REG_100H.WORD;
                        ARG9[iLoop+2] = TSIP.REG_100H.WORD;
                        ARG9[iLoop+3] = TSIP.REG_100H.WORD;
                    }
                    RX65NHU_func100(change_endian_long(0x8469087eu), change_endian_long(0xbd7252bcu), change_endian_long(0x3acc3168u), change_endian_long(0x36005c97u));
                    TSIP.REG_00H.WORD = 0x00012303u;
                    TSIP.REG_2CH.WORD = 0x00000022u;
                    TSIP.REG_D0H.WORD = 0x00000f00u;
                    TSIP.REG_C4H.WORD = 0x00e087bfu;
                    TSIP.REG_04H.WORD = 0x00000202u;
                    for ( ; iLoop < 132; iLoop = iLoop + 4)
                    {
                        /* WAIT_LOOP */
                        while (1u != TSIP.REG_04H.BIT.B30)
                        {
                            /* waiting */
                        }
                        ARG9[iLoop] = TSIP.REG_100H.WORD;
                        ARG9[iLoop+1] = TSIP.REG_100H.WORD;
                        ARG9[iLoop+2] = TSIP.REG_100H.WORD;
                        ARG9[iLoop+3] = TSIP.REG_100H.WORD;
                    }
                    RX65NHU_func100(change_endian_long(0xd6a9c8e9u), change_endian_long(0xc740de62u), change_endian_long(0x550d2368u), change_endian_long(0x4df9f28eu));
                    TSIP.REG_C4H.WORD = 0x00e007bcu;
                    TSIP.REG_E0H.WORD = 0x81010200u;
                    TSIP.REG_00H.WORD = 0x00002807u;
                    /* WAIT_LOOP */
                    while (0u != TSIP.REG_00H.BIT.B25)
                    {
                        /* waiting */
                    }
                    TSIP.REG_1CH.WORD = 0x00001800u;
                    TSIP.REG_04H.WORD = 0x00000212u;
                    /* WAIT_LOOP */
                    while (1u != TSIP.REG_04H.BIT.B30)
                    {
                        /* waiting */
                    }
                    ARG9[iLoop] = TSIP.REG_100H.WORD;
                    ARG9[iLoop+1] = TSIP.REG_100H.WORD;
                    ARG9[iLoop+2] = TSIP.REG_100H.WORD;
                    ARG9[iLoop+3] = TSIP.REG_100H.WORD;
                    RX65NHU_func100(change_endian_long(0x26ae0885u), change_endian_long(0x9635c40au), change_endian_long(0xea0e9a95u), change_endian_long(0xc9b239c1u));
                    iLoop = iLoop+4;
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
                    TSIP.REG_04H.WORD = 0x00000212u;
                    /* WAIT_LOOP */
                    while (1u != TSIP.REG_04H.BIT.B30)
                    {
                        /* waiting */
                    }
                    ARG9[iLoop] = TSIP.REG_100H.WORD;
                    ARG9[iLoop+1] = TSIP.REG_100H.WORD;
                    ARG9[iLoop+2] = TSIP.REG_100H.WORD;
                    ARG9[iLoop+3] = TSIP.REG_100H.WORD;
                    RX65NHU_func100(change_endian_long(0x2dadbd8bu), change_endian_long(0x4db25070u), change_endian_long(0xd1326dadu), change_endian_long(0xa6cd579du));
                    TSIP.REG_E0H.WORD = 0x81040000u;
                    TSIP.REG_04H.WORD = 0x00000612u;
                    /* WAIT_LOOP */
                    while (1u != TSIP.REG_04H.BIT.B30)
                    {
                        /* waiting */
                    }
                    ARG9[0] = TSIP.REG_100H.WORD;
                    /* WAIT_LOOP */
                    while (1u != TSIP.REG_04H.BIT.B30)
                    {
                        /* waiting */
                    }
                    ARG9[1] = TSIP.REG_100H.WORD;
                    /* WAIT_LOOP */
                    while (1u != TSIP.REG_04H.BIT.B30)
                    {
                        /* waiting */
                    }
                    ARG9[2] = TSIP.REG_100H.WORD;
                    /* WAIT_LOOP */
                    while (1u != TSIP.REG_04H.BIT.B30)
                    {
                        /* waiting */
                    }
                    ARG9[3] = TSIP.REG_100H.WORD;
                    TSIP.REG_ECH.WORD = 0x00000800u;
                    RX65NHU_func101(change_endian_long(0x3fba0805u), change_endian_long(0x4ade0081u), change_endian_long(0xbea13d92u), change_endian_long(0x26813e7eu));
                }
                else
                {
                    TSIP.REG_28H.WORD = 0x00870001u;
                    TSIP.REG_ECH.WORD = 0x00000b9cu;
                    RX65NHU_func004(ARG8);
                    TSIP.REG_24H.WORD = 0x0000dcd0u;
                    /* WAIT_LOOP */
                    while (0u != TSIP.REG_24H.BIT.B21)
                    {
                        /* waiting */
                    }
                    TSIP.REG_24H.WORD = 0x000084d0u;
                    /* WAIT_LOOP */
                    while (0u != TSIP.REG_24H.BIT.B21)
                    {
                        /* waiting */
                    }
                    TSIP.REG_24H.WORD = 0x00029008u;
                    /* WAIT_LOOP */
                    while (0u != TSIP.REG_24H.BIT.B21)
                    {
                        /* waiting */
                    }
                    TSIP.REG_34H.WORD = 0x00000800u;
                    TSIP.REG_24H.WORD = 0x8000c0c1u;
                    /* WAIT_LOOP */
                    while (0u != TSIP.REG_24H.BIT.B21)
                    {
                        /* waiting */
                    }
                    TSIP.REG_28H.WORD = 0x008f0001u;
                    TSIP.REG_D0H.WORD = 0x0000031cu;
                    TSIP.REG_C4H.WORD = 0x00008a83u;
                    TSIP.REG_00H.WORD = 0x00003243u;
                    TSIP.REG_2CH.WORD = 0x00000010u;
                    /* WAIT_LOOP */
                    while (0u != TSIP.REG_00H.BIT.B25)
                    {
                        /* waiting */
                    }
                    TSIP.REG_1CH.WORD = 0x00001800u;
                    TSIP.REG_28H.WORD = 0x00870001u;
                    TSIP.REG_34H.WORD = 0x00000004u;
                    TSIP.REG_24H.WORD = 0x800070d0u;
                    /* WAIT_LOOP */
                    while (0u != TSIP.REG_24H.BIT.B21)
                    {
                        /* waiting */
                    }
                    TSIP.REG_34H.WORD = 0x00000800u;
                    TSIP.REG_24H.WORD = 0x800070d0u;
                    /* WAIT_LOOP */
                    while (0u != TSIP.REG_24H.BIT.B21)
                    {
                        /* waiting */
                    }
                    RX65NHU_func008();
                    RX65NHU_func100(change_endian_long(0xbe407445u), change_endian_long(0x6728d0aeu), change_endian_long(0x8d95a281u), change_endian_long(0xd4a43961u));
                    TSIP.REG_1CH.WORD = 0x00400000u;
                    TSIP.REG_1D0H.WORD = 0x00000000u;
                    if (1u == (TSIP.REG_1CH.BIT.B22))
                    {
                        TSIP.REG_ECH.WORD = 0x0000b400u;
                        TSIP.REG_ECH.WORD = 0xffffffffu;
                        RX65NHU_func101(change_endian_long(0x7d51c94fu), change_endian_long(0x2f4eacd6u), change_endian_long(0x74ff5327u), change_endian_long(0xf4189686u));
                    }
                    else
                    {
                        RX65NHU_func100(change_endian_long(0x3277806bu), change_endian_long(0xf0a11d6fu), change_endian_long(0x8193e4c6u), change_endian_long(0xb97d7e2eu));
                        TSIP.REG_34H.WORD = 0x00000400u;
                        TSIP.REG_24H.WORD = 0x80008cd0u;
                        /* WAIT_LOOP */
                        while (0u != TSIP.REG_24H.BIT.B21)
                        {
                            /* waiting */
                        }
                        TSIP.REG_34H.WORD = 0x00000404u;
                        TSIP.REG_24H.WORD = 0x80009008u;
                        /* WAIT_LOOP */
                        while (0u != TSIP.REG_24H.BIT.B21)
                        {
                            /* waiting */
                        }
                        TSIP.REG_34H.WORD = 0x00000002u;
                        TSIP.REG_24H.WORD = 0x80008cd0u;
                        /* WAIT_LOOP */
                        while (0u != TSIP.REG_24H.BIT.B21)
                        {
                            /* waiting */
                        }
                        TSIP.REG_24H.WORD = 0x00009008u;
                        /* WAIT_LOOP */
                        while (0u != TSIP.REG_24H.BIT.B21)
                        {
                            /* waiting */
                        }
                        TSIP.REG_28H.WORD = 0x008f0001u;
                        TSIP.REG_104H.WORD = 0x00000058u;
                        TSIP.REG_E0H.WORD = 0x80010000u;
                        /* WAIT_LOOP */
                        while (1u != TSIP.REG_104H.BIT.B31)
                        {
                            /* waiting */
                        }
                        TSIP.REG_100H.WORD = change_endian_long(0x31080100u);
                        TSIP.REG_ECH.WORD = 0x00001011u;
                        RX65NHU_func103();
                        TSIP.REG_104H.WORD = 0x00000052u;
                        TSIP.REG_C4H.WORD = 0x01000c84u;
                        /* WAIT_LOOP */
                        while (1u != TSIP.REG_104H.BIT.B31)
                        {
                            /* waiting */
                        }
                        TSIP.REG_100H.WORD = change_endian_long(0x00000000u);
                        TSIP.REG_E0H.WORD = 0x80030020u;
                        TSIP.REG_00H.WORD = 0x0000820fu;
                        /* WAIT_LOOP */
                        while (0u != TSIP.REG_00H.BIT.B25)
                        {
                            /* waiting */
                        }
                        TSIP.REG_1CH.WORD = 0x00001800u;
                        TSIP.REG_00H.WORD = 0x00000207u;
                        /* WAIT_LOOP */
                        while (0u != TSIP.REG_00H.BIT.B25)
                        {
                            /* waiting */
                        }
                        TSIP.REG_1CH.WORD = 0x00001800u;
                        RX65NHU_func100(change_endian_long(0x1fafb9aeu), change_endian_long(0x6d717440u), change_endian_long(0x5b612c49u), change_endian_long(0x36dab028u));
                        TSIP.REG_C4H.WORD = 0x000c2b0du;
                        TSIP.REG_E0H.WORD = 0x81040000u;
                        TSIP.REG_00H.WORD = 0x00002813u;
                        /* WAIT_LOOP */
                        while (0u != TSIP.REG_00H.BIT.B25)
                        {
                            /* waiting */
                        }
                        TSIP.REG_1CH.WORD = 0x00001800u;
                        TSIP.REG_00H.WORD = 0x00002343u;
                        TSIP.REG_2CH.WORD = 0x00000020u;
                        TSIP.REG_D0H.WORD = 0x00000300u;
                        TSIP.REG_C4H.WORD = 0x02e087bfu;
                        TSIP.REG_04H.WORD = 0x00000242u;
                        for (iLoop = 4; iLoop < 20; iLoop = iLoop + 4)
                        {
                            /* WAIT_LOOP */
                            while (1u != TSIP.REG_04H.BIT.B30)
                            {
                                /* waiting */
                            }
                            ARG9[iLoop] = TSIP.REG_100H.WORD;
                            ARG9[iLoop+1] = TSIP.REG_100H.WORD;
                            ARG9[iLoop+2] = TSIP.REG_100H.WORD;
                            ARG9[iLoop+3] = TSIP.REG_100H.WORD;
                        }
                        /* WAIT_LOOP */
                        while (0u != TSIP.REG_00H.BIT.B25)
                        {
                            /* waiting */
                        }
                        TSIP.REG_1CH.WORD = 0x00001800u;
                        RX65NHU_func100(change_endian_long(0x2ded1004u), change_endian_long(0x5877ce4du), change_endian_long(0x6418138bu), change_endian_long(0x680212f3u));
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
                        TSIP.REG_04H.WORD = 0x00000212u;
                        /* WAIT_LOOP */
                        while (1u != TSIP.REG_04H.BIT.B30)
                        {
                            /* waiting */
                        }
                        ARG9[iLoop] = TSIP.REG_100H.WORD;
                        ARG9[iLoop+1] = TSIP.REG_100H.WORD;
                        ARG9[iLoop+2] = TSIP.REG_100H.WORD;
                        ARG9[iLoop+3] = TSIP.REG_100H.WORD;
                        RX65NHU_func100(change_endian_long(0x2d8ac683u), change_endian_long(0xf0615e81u), change_endian_long(0xf9eea01du), change_endian_long(0x623bbe71u));
                        TSIP.REG_E0H.WORD = 0x81040000u;
                        TSIP.REG_04H.WORD = 0x00000612u;
                        /* WAIT_LOOP */
                        while (1u != TSIP.REG_04H.BIT.B30)
                        {
                            /* waiting */
                        }
                        ARG9[0] = TSIP.REG_100H.WORD;
                        /* WAIT_LOOP */
                        while (1u != TSIP.REG_04H.BIT.B30)
                        {
                            /* waiting */
                        }
                        ARG9[1] = TSIP.REG_100H.WORD;
                        /* WAIT_LOOP */
                        while (1u != TSIP.REG_04H.BIT.B30)
                        {
                            /* waiting */
                        }
                        ARG9[2] = TSIP.REG_100H.WORD;
                        /* WAIT_LOOP */
                        while (1u != TSIP.REG_04H.BIT.B30)
                        {
                            /* waiting */
                        }
                        ARG9[3] = TSIP.REG_100H.WORD;
                        TSIP.REG_ECH.WORD = 0x00000800u;
                        RX65NHU_func101(change_endian_long(0xe3c583deu), change_endian_long(0xfd2a6caau), change_endian_long(0xdaf5813cu), change_endian_long(0x60a08b57u));
                    }
                }
            }
        }
    }
    TSIP.REG_ECH.WORD = 0x00007c14u;
    TSIP.REG_1CH.WORD = 0x00602000u;
}
/**********************************************************************************************************************
 End of function ./input_dir/RX65NHU/RX65NHU_func405.prc
 *********************************************************************************************************************/
