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
* Function Name: RX65NHU_func403
*******************************************************************************************************************/ /**
* @details       RX65NHU func403
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
void RX65NHU_func403(uint32_t *ARG1, uint32_t *ARG2, uint32_t *ARG3, uint32_t *ARG4, uint32_t *ARG5, uint32_t *ARG6, uint32_t *ARG7, const uint32_t *ARG8, uint32_t *ARG9)
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
    TSIP.REG_104H.WORD = 0x00000168u;
    TSIP.REG_E0H.WORD = 0x800201c0u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = ARG1[0];
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = ARG2[0];
    TSIP.REG_ECH.WORD = 0x3420a9c0u;
    TSIP.REG_ECH.WORD = 0x00000004u;
    TSIP.REG_E0H.WORD = 0x00000080u;
    TSIP.REG_1CH.WORD = 0x00A60000u;
    TSIP.REG_ECH.WORD = 0x380089e0u;
    TSIP.REG_ECH.WORD = 0x00000004u;
    TSIP.REG_E0H.WORD = 0x00000080u;
    TSIP.REG_1CH.WORD = 0x00260000u;
    TSIP.REG_ECH.WORD = 0x3420a9e0u;
    TSIP.REG_ECH.WORD = 0x00000006u;
    TSIP.REG_E0H.WORD = 0x00000080u;
    TSIP.REG_1CH.WORD = 0x00A60000u;
    RX65NHU_func100(change_endian_long(0x214d7c88u), change_endian_long(0x7bf2d19fu), change_endian_long(0x42e8ef17u), change_endian_long(0x421813dfu));
    TSIP.REG_1CH.WORD = 0x00400000u;
    TSIP.REG_1D0H.WORD = 0x00000000u;
    if (1u == (TSIP.REG_1CH.BIT.B22))
    {
        TSIP.REG_ECH.WORD = 0x0000b400u;
        TSIP.REG_ECH.WORD = 0xffffffffu;
        RX65NHU_func101(change_endian_long(0xf01c1959u), change_endian_long(0xc9c826ffu), change_endian_long(0xcc979904u), change_endian_long(0x3f410593u));
    }
    else
    {
        TSIP.REG_ECH.WORD = 0x000035afu;
        TSIP.REG_ECH.WORD = 0x380089a0u;
        TSIP.REG_ECH.WORD = 0x00000002u;
        TSIP.REG_ECH.WORD = 0x2000b5a0u;
        TSIP.REG_ECH.WORD = 0x00000000u;
        TSIP.REG_104H.WORD = 0x00000068u;
        TSIP.REG_E0H.WORD = 0x800102c0u;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_104H.BIT.B31)
        {
            /* waiting */
        }
        TSIP.REG_100H.WORD = ARG5[0];
        TSIP.REG_ECH.WORD = 0x380089e0u;
        TSIP.REG_ECH.WORD = 0x00000005u;
        TSIP.REG_E0H.WORD = 0x00000080u;
        TSIP.REG_1CH.WORD = 0x00A60000u;
        RX65NHU_func100(change_endian_long(0x9e83fd0eu), change_endian_long(0xf4f50d8bu), change_endian_long(0x9ccbbc5eu), change_endian_long(0x23597fd1u));
        TSIP.REG_1CH.WORD = 0x00400000u;
        TSIP.REG_1D0H.WORD = 0x00000000u;
        if (1u == (TSIP.REG_1CH.BIT.B22))
        {
            RX65NHU_func100(change_endian_long(0xfa878da4u), change_endian_long(0x2e13d3beu), change_endian_long(0xe24632c7u), change_endian_long(0x75395accu));
            TSIP.REG_104H.WORD = 0x00000368u;
            TSIP.REG_E0H.WORD = 0x800402e0u;
            for (iLoop = 0; iLoop < 4; iLoop = iLoop + 1)
            {
                /* WAIT_LOOP */
                while (1u != TSIP.REG_104H.BIT.B31)
                {
                    /* waiting */
                }
                TSIP.REG_100H.WORD = ARG7[iLoop];
            }
            TSIP.REG_ECH.WORD = 0x00007c0du;
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
                TSIP.REG_ECH.WORD = 0x38000defu;
                TSIP.REG_E0H.WORD = 0x00000080u;
                TSIP.REG_1CH.WORD = 0x00260000u;
                TSIP.REG_ECH.WORD = 0x380089e0u;
                TSIP.REG_ECH.WORD = 0x00000003u;
                TSIP.REG_E0H.WORD = 0x00000080u;
                TSIP.REG_1CH.WORD = 0x00260000u;
                RX65NHU_func100(change_endian_long(0x5a2d3605u), change_endian_long(0xb03f4254u), change_endian_long(0x3307f8c5u), change_endian_long(0xc78d250cu));
                TSIP.REG_1CH.WORD = 0x00400000u;
                TSIP.REG_1D0H.WORD = 0x00000000u;
                if (1u == (TSIP.REG_1CH.BIT.B22))
                {
                    TSIP.REG_ECH.WORD = 0x00003417u;
                    TSIP.REG_ECH.WORD = 0x0000a400u;
                    TSIP.REG_ECH.WORD = 0x000000ffu;
                    TSIP.REG_ECH.WORD = 0x08002818u;
                    TSIP.REG_E0H.WORD = 0x00000080u;
                    TSIP.REG_1CH.WORD = 0x00A60000u;
                    RX65NHU_func101(change_endian_long(0x539d887bu), change_endian_long(0x233c4c0eu), change_endian_long(0x2b61f13cu), change_endian_long(0x3f98003du));
                }
                else
                {
                    TSIP.REG_ECH.WORD = 0x00003417u;
                    TSIP.REG_ECH.WORD = 0x0000a400u;
                    TSIP.REG_ECH.WORD = 0x000001ffu;
                    TSIP.REG_ECH.WORD = 0x08002818u;
                    TSIP.REG_E0H.WORD = 0x00000080u;
                    TSIP.REG_1CH.WORD = 0x00A60000u;
                    RX65NHU_func101(change_endian_long(0xc410bf06u), change_endian_long(0x36dddd2eu), change_endian_long(0xf8306472u), change_endian_long(0xf811e497u));
                }
                TSIP.REG_ECH.WORD = 0x00003419u;
                TSIP.REG_ECH.WORD = 0x0000a400u;
                TSIP.REG_ECH.WORD = 0x00000003u;
                TSIP.REG_ECH.WORD = 0x0c00281au;
                TSIP.REG_E0H.WORD = 0x00000080u;
                TSIP.REG_1CH.WORD = 0x00A60000u;
                RX65NHU_func101(change_endian_long(0x4af393d7u), change_endian_long(0x1936b141u), change_endian_long(0x93b33b05u), change_endian_long(0xe6f30405u));
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
                RX65NHU_func101(change_endian_long(0x8bda616eu), change_endian_long(0x9485f502u), change_endian_long(0x997a17e9u), change_endian_long(0x93687c2fu));
            }
            RX65NHU_func101(change_endian_long(0xdb37997du), change_endian_long(0x9a8407e9u), change_endian_long(0x04bcb492u), change_endian_long(0x8610cc65u));
        }
        else
        {
            RX65NHU_func101(change_endian_long(0xd7f38b9eu), change_endian_long(0x49a98f2fu), change_endian_long(0x35f4934du), change_endian_long(0x52dfdbdau));
        }
        RX65NHU_func100(change_endian_long(0xbf6adc51u), change_endian_long(0x3ffcdad7u), change_endian_long(0xad62d525u), change_endian_long(0xc3595c30u));
        TSIP.REG_1CH.WORD = 0x00400000u;
        TSIP.REG_1D0H.WORD = 0x00000000u;
        if (1u == (TSIP.REG_1CH.BIT.B22))
        {
            TSIP.REG_ECH.WORD = 0x0000b400u;
            TSIP.REG_ECH.WORD = 0xffffffffu;
            RX65NHU_func101(change_endian_long(0x984cc0a8u), change_endian_long(0xd06c5f59u), change_endian_long(0x4f826006u), change_endian_long(0xce562846u));
        }
        else
        {
            TSIP.REG_104H.WORD = 0x00000368u;
            TSIP.REG_E0H.WORD = 0x80040000u;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_104H.BIT.B31)
            {
                /* waiting */
            }
            TSIP.REG_100H.WORD = ARG3[0];
            /* WAIT_LOOP */
            while (1u != TSIP.REG_104H.BIT.B31)
            {
                /* waiting */
            }
            TSIP.REG_100H.WORD = ARG3[1];
            /* WAIT_LOOP */
            while (1u != TSIP.REG_104H.BIT.B31)
            {
                /* waiting */
            }
            TSIP.REG_100H.WORD = ARG3[2];
            /* WAIT_LOOP */
            while (1u != TSIP.REG_104H.BIT.B31)
            {
                /* waiting */
            }
            TSIP.REG_100H.WORD = ARG3[3];
            TSIP.REG_ECH.WORD = 0x00000bdeu;
            TSIP.REG_ECH.WORD = 0x000037e0u;
            TSIP.REG_ECH.WORD = 0x00008fe0u;
            TSIP.REG_ECH.WORD = 0xff000000u;
            TSIP.REG_ECH.WORD = 0x00003629u;
            TSIP.REG_ECH.WORD = 0x00009220u;
            TSIP.REG_ECH.WORD = 0x05000000u;
            TSIP.REG_ECH.WORD = 0x38000bf1u;
            TSIP.REG_ECH.WORD = 0x1000d3c0u;
            TSIP.REG_ECH.WORD = 0x00003629u;
            TSIP.REG_ECH.WORD = 0x00009220u;
            TSIP.REG_ECH.WORD = 0x0a000000u;
            TSIP.REG_ECH.WORD = 0x38000bf1u;
            TSIP.REG_ECH.WORD = 0x1000d3c0u;
            TSIP.REG_ECH.WORD = 0x00003629u;
            TSIP.REG_ECH.WORD = 0x00009220u;
            TSIP.REG_ECH.WORD = 0x31000000u;
            TSIP.REG_ECH.WORD = 0x38000bf1u;
            TSIP.REG_ECH.WORD = 0x1000d3c0u;
            TSIP.REG_ECH.WORD = 0x3800d813u;
            TSIP.REG_ECH.WORD = 0x2000d3c1u;
            TSIP.REG_ECH.WORD = 0x000037e0u;
            TSIP.REG_ECH.WORD = 0x00008fe0u;
            TSIP.REG_ECH.WORD = 0x0000ff00u;
            TSIP.REG_ECH.WORD = 0x38008be0u;
            TSIP.REG_ECH.WORD = 0x00000100u;
            TSIP.REG_ECH.WORD = 0x1000d3c2u;
            TSIP.REG_ECH.WORD = 0x38008bc0u;
            TSIP.REG_ECH.WORD = 0x00000007u;
            TSIP.REG_E0H.WORD = 0x00000080u;
            TSIP.REG_1CH.WORD = 0x00A60000u;
            RX65NHU_func100(change_endian_long(0xafd21407u), change_endian_long(0xb7217becu), change_endian_long(0xbb86a46cu), change_endian_long(0x264e6e78u));
            TSIP.REG_1CH.WORD = 0x00400000u;
            TSIP.REG_1D0H.WORD = 0x00000000u;
            if (1u == (TSIP.REG_1CH.BIT.B22))
            {
                TSIP.REG_ECH.WORD = 0x0000b400u;
                TSIP.REG_ECH.WORD = 0xffffffffu;
                RX65NHU_func101(change_endian_long(0xd0cc3902u), change_endian_long(0xcbef85f2u), change_endian_long(0x5acaca71u), change_endian_long(0xfc560ea0u));
            }
            else
            {
                RX65NHU_func100(change_endian_long(0x40e302bdu), change_endian_long(0x3684f494u), change_endian_long(0x68b6f027u), change_endian_long(0x8e9e7c21u));
                TSIP.REG_C4H.WORD = 0x00082b8du;
                TSIP.REG_E0H.WORD = 0x81040000u;
                TSIP.REG_00H.WORD = 0x00002813u;
                /* WAIT_LOOP */
                while (0u != TSIP.REG_00H.BIT.B25)
                {
                    /* waiting */
                }
                TSIP.REG_1CH.WORD = 0x00001800u;
                TSIP.REG_ECH.WORD = 0x0000362eu;
                TSIP.REG_ECH.WORD = 0x38008a20u;
                TSIP.REG_ECH.WORD = 0x00000002u;
                TSIP.REG_ECH.WORD = 0x2000b620u;
                TSIP.REG_ECH.WORD = 0x00000000u;
                TSIP.REG_ECH.WORD = 0x00007c11u;
                TSIP.REG_1CH.WORD = 0x00600000u;
                TSIP.REG_1D0H.WORD = 0x00000000u;
                if (0x00000000u == (TSIP.REG_1CH.WORD & 0xff000000u))
                {
                    TSIP.REG_28H.WORD = 0x00bf0001u;
                    TSIP.REG_104H.WORD = 0x00008362u;
                    TSIP.REG_D0H.WORD = 0x00001f00u;
                    TSIP.REG_C4H.WORD = 0x42f087bfu;
                    TSIP.REG_00H.WORD = 0x00013203u;
                    TSIP.REG_2CH.WORD = 0x00000014u;
                    for (iLoop = 0; iLoop < 64; iLoop = iLoop + 4)
                    {
                        /* WAIT_LOOP */
                        while (1u != TSIP.REG_104H.BIT.B31)
                        {
                            /* waiting */
                        }
                        TSIP.REG_100H.WORD = ARG3[iLoop+4];
                        TSIP.REG_100H.WORD = ARG3[iLoop+5];
                        TSIP.REG_100H.WORD = ARG3[iLoop+6];
                        TSIP.REG_100H.WORD = ARG3[iLoop+7];
                    }
                    /* WAIT_LOOP */
                    while (0u != TSIP.REG_00H.BIT.B25)
                    {
                        /* waiting */
                    }
                    TSIP.REG_1CH.WORD = 0x00001800u;
                    TSIP.REG_00H.WORD = 0x00013203u;
                    TSIP.REG_2CH.WORD = 0x00000012u;
                    for (iLoop = 0; iLoop < 64; iLoop = iLoop + 4)
                    {
                        /* WAIT_LOOP */
                        while (1u != TSIP.REG_104H.BIT.B31)
                        {
                            /* waiting */
                        }
                        TSIP.REG_100H.WORD = ARG3[iLoop+68];
                        TSIP.REG_100H.WORD = ARG3[iLoop+69];
                        TSIP.REG_100H.WORD = ARG3[iLoop+70];
                        TSIP.REG_100H.WORD = ARG3[iLoop+71];
                    }
                    /* WAIT_LOOP */
                    while (0u != TSIP.REG_00H.BIT.B25)
                    {
                        /* waiting */
                    }
                    TSIP.REG_1CH.WORD = 0x00001800u;
                    TSIP.REG_C4H.WORD = 0x40f007bdu;
                    for (iLoop = 0; iLoop < 4; iLoop = iLoop + 4)
                    {
                        /* WAIT_LOOP */
                        while (1u != TSIP.REG_104H.BIT.B31)
                        {
                            /* waiting */
                        }
                        TSIP.REG_100H.WORD = ARG3[iLoop+132];
                        TSIP.REG_100H.WORD = ARG3[iLoop+133];
                        TSIP.REG_100H.WORD = ARG3[iLoop+134];
                        TSIP.REG_100H.WORD = ARG3[iLoop+135];
                    }
                    TSIP.REG_E0H.WORD = 0x80010140u;
                    TSIP.REG_00H.WORD = 0x00008207u;
                    /* WAIT_LOOP */
                    while (0u != TSIP.REG_00H.BIT.B25)
                    {
                        /* waiting */
                    }
                    TSIP.REG_1CH.WORD = 0x00001800u;
                    TSIP.REG_00H.WORD = 0x0000020fu;
                    /* WAIT_LOOP */
                    while (0u != TSIP.REG_00H.BIT.B25)
                    {
                        /* waiting */
                    }
                    TSIP.REG_1CH.WORD = 0x00001800u;
                    TSIP.REG_C4H.WORD = 0x400007bdu;
                    /* WAIT_LOOP */
                    while (1u != TSIP.REG_104H.BIT.B31)
                    {
                        /* waiting */
                    }
                    TSIP.REG_100H.WORD = ARG3[136];
                    TSIP.REG_100H.WORD = ARG3[137];
                    TSIP.REG_100H.WORD = ARG3[138];
                    TSIP.REG_100H.WORD = ARG3[139];
                    TSIP.REG_C4H.WORD = 0x00900c45u;
                    TSIP.REG_00H.WORD = 0x00002213u;
                    /* WAIT_LOOP */
                    while (0u != TSIP.REG_00H.BIT.B25)
                    {
                        /* waiting */
                    }
                    TSIP.REG_1CH.WORD = 0x00001800u;
                    RX65NHU_func101(change_endian_long(0x6925cb48u), change_endian_long(0xa615b46fu), change_endian_long(0xa72f0244u), change_endian_long(0xc158ceb2u));
                }
                else
                {
                    TSIP.REG_28H.WORD = 0x00870001u;
                    TSIP.REG_104H.WORD = 0x00000f62u;
                    TSIP.REG_D0H.WORD = 0x00000300u;
                    TSIP.REG_C4H.WORD = 0x42f087bfu;
                    TSIP.REG_00H.WORD = 0x00003223u;
                    TSIP.REG_2CH.WORD = 0x0000009bu;
                    for (iLoop = 0; iLoop < 8; iLoop = iLoop + 4)
                    {
                        /* WAIT_LOOP */
                        while (1u != TSIP.REG_104H.BIT.B31)
                        {
                            /* waiting */
                        }
                        TSIP.REG_100H.WORD = ARG3[iLoop+4];
                        TSIP.REG_100H.WORD = ARG3[iLoop+5];
                        TSIP.REG_100H.WORD = ARG3[iLoop+6];
                        TSIP.REG_100H.WORD = ARG3[iLoop+7];
                    }
                    /* WAIT_LOOP */
                    while (0u != TSIP.REG_00H.BIT.B25)
                    {
                        /* waiting */
                    }
                    TSIP.REG_1CH.WORD = 0x00001800u;
                    TSIP.REG_00H.WORD = 0x00003223u;
                    TSIP.REG_2CH.WORD = 0x00000012u;
                    for (iLoop = 8; iLoop < 16; iLoop = iLoop + 4)
                    {
                        /* WAIT_LOOP */
                        while (1u != TSIP.REG_104H.BIT.B31)
                        {
                            /* waiting */
                        }
                        TSIP.REG_100H.WORD = ARG3[iLoop+4];
                        TSIP.REG_100H.WORD = ARG3[iLoop+5];
                        TSIP.REG_100H.WORD = ARG3[iLoop+6];
                        TSIP.REG_100H.WORD = ARG3[iLoop+7];
                    }
                    /* WAIT_LOOP */
                    while (0u != TSIP.REG_00H.BIT.B25)
                    {
                        /* waiting */
                    }
                    TSIP.REG_1CH.WORD = 0x00001800u;
                    TSIP.REG_C4H.WORD = 0x400007bdu;
                    /* WAIT_LOOP */
                    while (1u != TSIP.REG_104H.BIT.B31)
                    {
                        /* waiting */
                    }
                    TSIP.REG_100H.WORD = ARG3[20];
                    TSIP.REG_100H.WORD = ARG3[21];
                    TSIP.REG_100H.WORD = ARG3[22];
                    TSIP.REG_100H.WORD = ARG3[23];
                    TSIP.REG_C4H.WORD = 0x00900c45u;
                    TSIP.REG_00H.WORD = 0x00002213u;
                    /* WAIT_LOOP */
                    while (0u != TSIP.REG_00H.BIT.B25)
                    {
                        /* waiting */
                    }
                    TSIP.REG_1CH.WORD = 0x00001800u;
                    RX65NHU_func101(change_endian_long(0x76b9d05cu), change_endian_long(0xe92ea73bu), change_endian_long(0x28db5115u), change_endian_long(0x1f1b90d4u));
                }
                RX65NHU_func100(change_endian_long(0xddef8a23u), change_endian_long(0x06efcaacu), change_endian_long(0xeab379c1u), change_endian_long(0x6cc3ffb5u));
                TSIP.REG_1CH.WORD = 0x00400000u;
                TSIP.REG_1D0H.WORD = 0x00000000u;
                if (1u == (TSIP.REG_1CH.BIT.B22))
                {
                    TSIP.REG_ECH.WORD = 0x0000b400u;
                    TSIP.REG_ECH.WORD = 0xffffffffu;
                    RX65NHU_func101(change_endian_long(0x0d07af66u), change_endian_long(0x20321b88u), change_endian_long(0x237d6fa9u), change_endian_long(0x5af92aafu));
                }
                else
                {
                    TSIP.REG_ECH.WORD = 0x380089e0u;
                    TSIP.REG_ECH.WORD = 0x00000005u;
                    TSIP.REG_E0H.WORD = 0x00000080u;
                    TSIP.REG_1CH.WORD = 0x00A60000u;
                    RX65NHU_func100(change_endian_long(0x59f3cbfeu), change_endian_long(0x07da6435u), change_endian_long(0x25d2d6bbu), change_endian_long(0x31616cd8u));
                    TSIP.REG_1CH.WORD = 0x00400000u;
                    TSIP.REG_1D0H.WORD = 0x00000000u;
                    if (1u == (TSIP.REG_1CH.BIT.B22))
                    {
                        RX65NHU_func100(change_endian_long(0x7e7ecd40u), change_endian_long(0x3bb4f9e7u), change_endian_long(0x27fe7d76u), change_endian_long(0xceb10f64u));
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
                            RX65NHU_func101(change_endian_long(0xeec12b1fu), change_endian_long(0x8c9c6f3fu), change_endian_long(0x98895c96u), change_endian_long(0x8e8473e9u));
                        }
                        RX65NHU_func100(change_endian_long(0x6d3fa357u), change_endian_long(0x8f15febau), change_endian_long(0x7b21a214u), change_endian_long(0x9601998eu));
                        TSIP.REG_ECH.WORD = 0x38000805u;
                        TSIP.REG_E0H.WORD = 0x00000080u;
                        TSIP.REG_1CH.WORD = 0x00260000u;
                        TSIP.REG_1CH.WORD = 0x00402000u;
                        TSIP.REG_ECH.WORD = 0x00007c0du;
                        TSIP.REG_1CH.WORD = 0x00600000u;
                        TSIP.REG_1D0H.WORD = 0x00000000u;
                        if (0x00000000u == (TSIP.REG_1CH.WORD & 0xff000000u))
                        {
                            RX65NHU_func100(change_endian_long(0xf2271e6du), change_endian_long(0x6057bf59u), change_endian_long(0x3888ccf9u), change_endian_long(0x393fde39u));
                            TSIP.REG_ECH.WORD = 0x00000a52u;
                            TSIP.REG_ECH.WORD = 0x00000a73u;
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
                                    RX65NHU_func100(change_endian_long(0x966bc3f7u), change_endian_long(0x4a10282du), change_endian_long(0xd888685bu), change_endian_long(0xcaa2ee03u));
                                    TSIP.REG_1CH.WORD = 0x00400000u;
                                    TSIP.REG_1D0H.WORD = 0x00000000u;
                                    if (1u == (TSIP.REG_1CH.BIT.B22))
                                    {
                                        TSIP.REG_ECH.WORD = 0x3420aa60u;
                                        TSIP.REG_ECH.WORD = 0x00000100u;
                                        TSIP.REG_E0H.WORD = 0x00000080u;
                                        TSIP.REG_1CH.WORD = 0x00260000u;
                                        RX65NHU_func100(change_endian_long(0x1ab84c99u), change_endian_long(0x1022f75au), change_endian_long(0xe4617659u), change_endian_long(0x43f9a46au));
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
                                            TSIP.REG_ECH.WORD = 0x0000a660u;
                                            TSIP.REG_ECH.WORD = 0x00000001u;
                                            RX65NHU_func101(change_endian_long(0x4bda24f3u), change_endian_long(0xaad82159u), change_endian_long(0x9ecc7c49u), change_endian_long(0xc5fba2aau));
                                        }
                                        else
                                        {
                                            TSIP.REG_ECH.WORD = 0x02003cf2u;
                                            TSIP.REG_ECH.WORD = 0x0000a640u;
                                            TSIP.REG_ECH.WORD = 0x00000001u;
                                            RX65NHU_func101(change_endian_long(0xf11a5a02u), change_endian_long(0x14c94f76u), change_endian_long(0x7558da87u), change_endian_long(0x469ad806u));
                                        }
                                        RX65NHU_func101(change_endian_long(0xb3f1f167u), change_endian_long(0xc39f2305u), change_endian_long(0x5b46b4cau), change_endian_long(0x5b410a35u));
                                    }
                                    TSIP.REG_ECH.WORD = 0x00000863u;
                                    TSIP.REG_ECH.WORD = 0x3c0028c2u;
                                    TSIP.REG_ECH.WORD = 0x20002c60u;
                                    TSIP.REG_ECH.WORD = 0x38000c63u;
                                    TSIP.REG_E0H.WORD = 0x00000080u;
                                    TSIP.REG_1CH.WORD = 0x00260000u;
                                    RX65NHU_func100(change_endian_long(0xe7a7390fu), change_endian_long(0x6a62ae53u), change_endian_long(0xf91d5d7du), change_endian_long(0x1c696e8cu));
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
                                        RX65NHU_func101(change_endian_long(0x5104e5b1u), change_endian_long(0xd4c4702cu), change_endian_long(0x587d1e81u), change_endian_long(0x3067415du));
                                    }
                                    TSIP.REG_ECH.WORD = 0x00002c40u;
                                    RX65NHU_func101(change_endian_long(0xe92dc4dfu), change_endian_long(0xf7d28ba3u), change_endian_long(0x95ab4bb0u), change_endian_long(0xf1ebcbbbu));
                                }
                                TSIP.REG_ECH.WORD = 0x08000c21u;
                                TSIP.REG_E0H.WORD = 0x00000080u;
                                TSIP.REG_1CH.WORD = 0x00260000u;
                                TSIP.REG_1CH.WORD = 0x00402000u;
                                TSIP.REG_ECH.WORD = 0x00002ca0u;
                                RX65NHU_func101(change_endian_long(0x94eb5b95u), change_endian_long(0x466d67e8u), change_endian_long(0xe98ff86au), change_endian_long(0xdf3e2ec4u));
                            }
                            TSIP.REG_ECH.WORD = 0x38000805u;
                            TSIP.REG_E0H.WORD = 0x00000080u;
                            TSIP.REG_1CH.WORD = 0x00260000u;
                            TSIP.REG_1CH.WORD = 0x00402000u;
                            RX65NHU_func101(change_endian_long(0x95118c69u), change_endian_long(0xc7b01a3bu), change_endian_long(0x0fc6dff4u), change_endian_long(0x2239f1aau));
                        }
                        else
                        {
                            RX65NHU_func100(change_endian_long(0x8898c2b5u), change_endian_long(0x0f76b43eu), change_endian_long(0x9b8a3f62u), change_endian_long(0x56e37b7du));
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
                                    RX65NHU_func100(change_endian_long(0x7593fd8cu), change_endian_long(0xea634a90u), change_endian_long(0x01ad1fbdu), change_endian_long(0xf0b0ad12u));
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
                                        RX65NHU_func101(change_endian_long(0xe23afdc5u), change_endian_long(0xd4c6e124u), change_endian_long(0x574c66b5u), change_endian_long(0x6d9408f7u));
                                    }
                                    TSIP.REG_ECH.WORD = 0x00000863u;
                                    TSIP.REG_ECH.WORD = 0x3c002857u;
                                    TSIP.REG_ECH.WORD = 0x20002c60u;
                                    TSIP.REG_ECH.WORD = 0x3c002b02u;
                                    TSIP.REG_ECH.WORD = 0x20002c60u;
                                    TSIP.REG_ECH.WORD = 0x38000c63u;
                                    TSIP.REG_E0H.WORD = 0x00000080u;
                                    TSIP.REG_1CH.WORD = 0x00260000u;
                                    RX65NHU_func100(change_endian_long(0x1d29af57u), change_endian_long(0x8da75594u), change_endian_long(0xc8eae665u), change_endian_long(0x33179238u));
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
                                        RX65NHU_func101(change_endian_long(0x4f231b96u), change_endian_long(0x46b0d3deu), change_endian_long(0x494221abu), change_endian_long(0xbd579e45u));
                                    }
                                    TSIP.REG_ECH.WORD = 0x00000863u;
                                    TSIP.REG_ECH.WORD = 0x3c0028c2u;
                                    TSIP.REG_ECH.WORD = 0x20002c60u;
                                    TSIP.REG_ECH.WORD = 0x38000c63u;
                                    TSIP.REG_E0H.WORD = 0x00000080u;
                                    TSIP.REG_1CH.WORD = 0x00260000u;
                                    RX65NHU_func100(change_endian_long(0x734b2798u), change_endian_long(0xa2be6cffu), change_endian_long(0x3b495e0au), change_endian_long(0x0ce9a8a8u));
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
                                        RX65NHU_func101(change_endian_long(0xa4fdc8bfu), change_endian_long(0x9b1460e8u), change_endian_long(0xb4cb1d44u), change_endian_long(0x636a3b5du));
                                    }
                                    TSIP.REG_ECH.WORD = 0x00002c40u;
                                    RX65NHU_func101(change_endian_long(0xdf9db3dbu), change_endian_long(0xe25c5a8du), change_endian_long(0x078d02e7u), change_endian_long(0x8588f459u));
                                }
                                TSIP.REG_ECH.WORD = 0x08000c21u;
                                TSIP.REG_E0H.WORD = 0x00000080u;
                                TSIP.REG_1CH.WORD = 0x00260000u;
                                TSIP.REG_1CH.WORD = 0x00402000u;
                                TSIP.REG_ECH.WORD = 0x00002ca0u;
                                RX65NHU_func101(change_endian_long(0x216a4662u), change_endian_long(0xc6f61f7cu), change_endian_long(0x565106aau), change_endian_long(0x8275a38du));
                            }
                            TSIP.REG_ECH.WORD = 0x38000805u;
                            TSIP.REG_E0H.WORD = 0x00000080u;
                            TSIP.REG_1CH.WORD = 0x00260000u;
                            TSIP.REG_1CH.WORD = 0x00402000u;
                            RX65NHU_func101(change_endian_long(0x7dafc7dcu), change_endian_long(0x053d3df0u), change_endian_long(0xe3eaba7bu), change_endian_long(0x5717ee6eu));
                        }
                        RX65NHU_func101(change_endian_long(0x69bb4395u), change_endian_long(0x0fc7279eu), change_endian_long(0x93598062u), change_endian_long(0x5ccfedd6u));
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
                        RX65NHU_func100(change_endian_long(0xcf827d1eu), change_endian_long(0xe65fa1a3u), change_endian_long(0x247674c7u), change_endian_long(0x61afacf1u));
                        TSIP.REG_ECH.WORD = 0x00003416u;
                        TSIP.REG_ECH.WORD = 0x0000a400u;
                        TSIP.REG_ECH.WORD = 0x00000003u;
                        TSIP.REG_ECH.WORD = 0x00026800u;
                        TSIP.REG_ECH.WORD = 0x00003000u;
                        TSIP.REG_E0H.WORD = 0x81010000u;
                        TSIP.REG_04H.WORD = 0x00000606u;
                        /* WAIT_LOOP */
                        while (1u != TSIP.REG_04H.BIT.B30)
                        {
                            /* waiting */
                        }
                        S_RAM[0] = change_endian_long(TSIP.REG_100H.WORD);
                        TSIP.REG_ECH.WORD = 0x000008a5u;
                        for (iLoop = 0; iLoop < S_RAM[0]; iLoop = iLoop + 1)
                        {
                            TSIP.REG_104H.WORD = 0x00000064u;
                            /* WAIT_LOOP */
                            while (1u != TSIP.REG_104H.BIT.B31)
                            {
                                /* waiting */
                            }
                            TSIP.REG_100H.WORD = ARG4[iLoop];
                            TSIP.REG_ECH.WORD = 0x0000a4a0u;
                            TSIP.REG_ECH.WORD = 0x00000001u;
                            RX65NHU_func101(change_endian_long(0x5213d595u), change_endian_long(0x46b382b3u), change_endian_long(0x5e573b75u), change_endian_long(0x957ef0c7u));
                        }
                        TSIP.REG_ECH.WORD = 0x38000805u;
                        TSIP.REG_E0H.WORD = 0x00000080u;
                        TSIP.REG_1CH.WORD = 0x00260000u;
                        TSIP.REG_1CH.WORD = 0x00402000u;
                        RX65NHU_func100(change_endian_long(0x51a0a5edu), change_endian_long(0xb03b6e06u), change_endian_long(0xb8b56f53u), change_endian_long(0x616fff25u));
                        TSIP.REG_104H.WORD = 0x00000068u;
                        TSIP.REG_E0H.WORD = 0x80010060u;
                        /* WAIT_LOOP */
                        while (1u != TSIP.REG_104H.BIT.B31)
                        {
                            /* waiting */
                        }
                        TSIP.REG_100H.WORD = ARG4[iLoop];
                        TSIP.REG_ECH.WORD = 0x00003416u;
                        TSIP.REG_ECH.WORD = 0x00008c00u;
                        TSIP.REG_ECH.WORD = 0x00000003u;
                        TSIP.REG_ECH.WORD = 0x38000c00u;
                        TSIP.REG_ECH.WORD = 0x00030020u;
                        TSIP.REG_ECH.WORD = 0x0000b400u;
                        TSIP.REG_ECH.WORD = 0x00000004u;
                        TSIP.REG_ECH.WORD = 0x00000080u;
                        TSIP.REG_E0H.WORD = 0x81010000u;
                        TSIP.REG_04H.WORD = 0x00000606u;
                        /* WAIT_LOOP */
                        while (1u != TSIP.REG_04H.BIT.B30)
                        {
                            /* waiting */
                        }
                        S_RAM[0] = change_endian_long(TSIP.REG_100H.WORD);
                        TSIP.REG_ECH.WORD = 0x000008a5u;
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
                            RX65NHU_func101(change_endian_long(0x3132487du), change_endian_long(0x4e0f3b87u), change_endian_long(0x756b0ee6u), change_endian_long(0xc6de9828u));
                        }
                        TSIP.REG_ECH.WORD = 0x38000805u;
                        TSIP.REG_E0H.WORD = 0x00000080u;
                        TSIP.REG_1CH.WORD = 0x00260000u;
                        TSIP.REG_1CH.WORD = 0x00402000u;
                        RX65NHU_func101(change_endian_long(0xb5075f3eu), change_endian_long(0x0d11a9d1u), change_endian_long(0x18288be6u), change_endian_long(0xa665bd78u));
                    }
                    RX65NHU_func100(change_endian_long(0x165b6361u), change_endian_long(0x25192522u), change_endian_long(0x139fa557u), change_endian_long(0x5c1fa77eu));
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
                        RX65NHU_func101(change_endian_long(0xa45f83c8u), change_endian_long(0x2e83333du), change_endian_long(0xce3c9860u), change_endian_long(0x6ab372cdu));
                    }
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
                    RX65NHU_func100(change_endian_long(0xd0c0eb52u), change_endian_long(0x98ae779du), change_endian_long(0xe0b1bd6eu), change_endian_long(0x790c1469u));
                    RX65NHU_func103();
                    RX65NHU_func100(change_endian_long(0x1a536cedu), change_endian_long(0xb710bb77u), change_endian_long(0xf8ef8809u), change_endian_long(0xdd377170u));
                    TSIP.REG_104H.WORD = 0x00000052u;
                    TSIP.REG_C4H.WORD = 0x01000c84u;
                    /* WAIT_LOOP */
                    while (1u != TSIP.REG_104H.BIT.B31)
                    {
                        /* waiting */
                    }
                    TSIP.REG_100H.WORD = change_endian_long(0x00000000u);
                    TSIP.REG_E0H.WORD = 0x800402e0u;
                    TSIP.REG_00H.WORD = 0x00008213u;
                    /* WAIT_LOOP */
                    while (0u != TSIP.REG_00H.BIT.B25)
                    {
                        /* waiting */
                    }
                    TSIP.REG_1CH.WORD = 0x00001800u;
                    TSIP.REG_C4H.WORD = 0x000c0805u;
                    TSIP.REG_E0H.WORD = 0x810402e0u;
                    TSIP.REG_00H.WORD = 0x00002813u;
                    /* WAIT_LOOP */
                    while (0u != TSIP.REG_00H.BIT.B25)
                    {
                        /* waiting */
                    }
                    TSIP.REG_1CH.WORD = 0x00001800u;
                    TSIP.REG_ECH.WORD = 0x380089e0u;
                    TSIP.REG_ECH.WORD = 0x00000005u;
                    TSIP.REG_E0H.WORD = 0x00000080u;
                    TSIP.REG_1CH.WORD = 0x00A60000u;
                    RX65NHU_func100(change_endian_long(0xa9f2a813u), change_endian_long(0x841867ffu), change_endian_long(0x3621eeb6u), change_endian_long(0xfa7f6f2fu));
                    TSIP.REG_1CH.WORD = 0x00400000u;
                    TSIP.REG_1D0H.WORD = 0x00000000u;
                    if (1u == (TSIP.REG_1CH.BIT.B22))
                    {
                        RX65NHU_func100(change_endian_long(0x15012d63u), change_endian_long(0xd9154bf4u), change_endian_long(0x07291235u), change_endian_long(0x55a674cau));
                        TSIP.REG_ECH.WORD = 0x00007c0du;
                        TSIP.REG_1CH.WORD = 0x00600000u;
                        TSIP.REG_1D0H.WORD = 0x00000000u;
                        if (0x00000000u == (TSIP.REG_1CH.WORD & 0xff000000u))
                        {
                            TSIP.REG_ECH.WORD = 0x38000defu;
                            TSIP.REG_E0H.WORD = 0x00000080u;
                            TSIP.REG_1CH.WORD = 0x00260000u;
                            TSIP.REG_ECH.WORD = 0x380089e0u;
                            TSIP.REG_ECH.WORD = 0x00000003u;
                            TSIP.REG_E0H.WORD = 0x00000080u;
                            TSIP.REG_1CH.WORD = 0x00260000u;
                            RX65NHU_func100(change_endian_long(0x336901ecu), change_endian_long(0x67b14c60u), change_endian_long(0xbd74ca62u), change_endian_long(0xb1fe98a2u));
                            TSIP.REG_1CH.WORD = 0x00400000u;
                            TSIP.REG_1D0H.WORD = 0x00000000u;
                            if (1u == (TSIP.REG_1CH.BIT.B22))
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
                                RX65NHU_func101(change_endian_long(0xeacca12cu), change_endian_long(0xf6015943u), change_endian_long(0x2c330815u), change_endian_long(0x18eba787u));
                            }
                            else
                            {
                                RX65NHU_func100(change_endian_long(0xf22445abu), change_endian_long(0x1875f241u), change_endian_long(0xf9dead11u), change_endian_long(0xd6616a66u));
                                TSIP.REG_ECH.WORD = 0x0000342bu;
                                RX65NHU_func080();
                                RX65NHU_func100(change_endian_long(0x663e681eu), change_endian_long(0x47d2d834u), change_endian_long(0x32d5e682u), change_endian_long(0xfaffbf7bu));
                                TSIP.REG_00H.WORD = 0x00012303u;
                                TSIP.REG_2CH.WORD = 0x00000020u;
                                RX65NHU_func313(1496);
                                RX65NHU_func100(change_endian_long(0xa873bdfbu), change_endian_long(0x30d1c285u), change_endian_long(0x098be966u), change_endian_long(0xb19bf9c1u));
                                RX65NHU_func314(1496+64);
                                RX65NHU_func100(change_endian_long(0x37bdebfbu), change_endian_long(0xb03ee9beu), change_endian_long(0xb186de62u), change_endian_long(0x7dfd72b3u));
                                TSIP.REG_ECH.WORD = 0x0000342cu;
                                RX65NHU_func080();
                                RX65NHU_func100(change_endian_long(0x87af3514u), change_endian_long(0x60a12fd6u), change_endian_long(0x9c3a05f5u), change_endian_long(0x7da4f837u));
                                TSIP.REG_ECH.WORD = 0x00000800u;
                                TSIP.REG_E0H.WORD = 0x81c00000u;
                                TSIP.REG_00H.WORD = 0x00012803u;
                                RX65NHU_func313(1496+68);
                                RX65NHU_func100(change_endian_long(0x9d79371au), change_endian_long(0x4aaa4af1u), change_endian_long(0x77da051au), change_endian_long(0xbcbbbfd3u));
                                RX65NHU_func314(1496+132);
                                RX65NHU_func101(change_endian_long(0x3422da5fu), change_endian_long(0xf5804565u), change_endian_long(0x1efe5f0cu), change_endian_long(0xe1cd518bu));
                            }
                            RX65NHU_func101(change_endian_long(0x1cae46e2u), change_endian_long(0xcaa58e93u), change_endian_long(0x8228bb0fu), change_endian_long(0xb738062du));
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
                            RX65NHU_func101(change_endian_long(0x10b9e2fdu), change_endian_long(0x3a7e4151u), change_endian_long(0x8368fb48u), change_endian_long(0x503e761bu));
                        }
                        RX65NHU_func101(change_endian_long(0x6915077au), change_endian_long(0xcaadada3u), change_endian_long(0xdbe085f9u), change_endian_long(0xb673e389u));
                    }
                    else
                    {
                        RX65NHU_func101(change_endian_long(0x82aa8520u), change_endian_long(0x88ea6f01u), change_endian_long(0x6dde5c97u), change_endian_long(0x5d90c14fu));
                    }
                    RX65NHU_func100(change_endian_long(0xd5c9fd00u), change_endian_long(0x126ec973u), change_endian_long(0xbc69292bu), change_endian_long(0x21f19a82u));
                    TSIP.REG_ECH.WORD = 0x00007c0eu;
                    TSIP.REG_1CH.WORD = 0x00600000u;
                    TSIP.REG_1D0H.WORD = 0x00000000u;
                    if (0x00000000u == (TSIP.REG_1CH.WORD & 0xff000000u))
                    {
                        RX65NHU_func100(change_endian_long(0xa96b8821u), change_endian_long(0xb6cd2e21u), change_endian_long(0xd2e5ef61u), change_endian_long(0x5095e823u));
                        TSIP.REG_28H.WORD = 0x00bf0001u;
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
                        TSIP.REG_18H.WORD = 0x00000004u;
                        TSIP.REG_34H.WORD = 0x00000000u;
                        TSIP.REG_38H.WORD = 0x00000338u;
                        TSIP.REG_E0H.WORD = 0x81010140u;
                        TSIP.REG_00H.WORD = 0x00003807u;
                        /* WAIT_LOOP */
                        while (0u != TSIP.REG_00H.BIT.B25)
                        {
                            /* waiting */
                        }
                        TSIP.REG_1CH.WORD = 0x00001800u;
                        /* WAIT_LOOP */
                        while (0u != TSIP.REG_18H.BIT.B10)
                        {
                            /* waiting */
                        }
                        TSIP.REG_18H.WORD = 0x00000000u;
                        TSIP.REG_104H.WORD = 0x00003757u;
                        TSIP.REG_2CH.WORD = 0x00000012u;
                        /* WAIT_LOOP */
                        while (1u != TSIP.REG_104H.BIT.B31)
                        {
                            /* waiting */
                        }
                        TSIP.REG_100H.WORD = change_endian_long(0x0001ffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        /* WAIT_LOOP */
                        while (1u != TSIP.REG_104H.BIT.B31)
                        {
                            /* waiting */
                        }
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        /* WAIT_LOOP */
                        while (1u != TSIP.REG_104H.BIT.B31)
                        {
                            /* waiting */
                        }
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        /* WAIT_LOOP */
                        while (1u != TSIP.REG_104H.BIT.B31)
                        {
                            /* waiting */
                        }
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        /* WAIT_LOOP */
                        while (1u != TSIP.REG_104H.BIT.B31)
                        {
                            /* waiting */
                        }
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        /* WAIT_LOOP */
                        while (1u != TSIP.REG_104H.BIT.B31)
                        {
                            /* waiting */
                        }
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        /* WAIT_LOOP */
                        while (1u != TSIP.REG_104H.BIT.B31)
                        {
                            /* waiting */
                        }
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0x00303130u);
                        TSIP.REG_100H.WORD = change_endian_long(0x0d060960u);
                        TSIP.REG_100H.WORD = change_endian_long(0x86480165u);
                        TSIP.REG_100H.WORD = change_endian_long(0x03040201u);
                        TSIP.REG_100H.WORD = change_endian_long(0x05000420u);
                        TSIP.REG_00H.WORD = 0x00003523u;
                        TSIP.REG_74H.WORD = 0x00000008u;
                        /* WAIT_LOOP */
                        while (0u != TSIP.REG_00H.BIT.B25)
                        {
                            /* waiting */
                        }
                        TSIP.REG_1CH.WORD = 0x00001800u;
                        TSIP.REG_24H.WORD = 0x000009c0u;
                        /* WAIT_LOOP */
                        while (0u != TSIP.REG_24H.BIT.B21)
                        {
                            /* waiting */
                        }
                        TSIP.REG_24H.WORD = 0x00001191u;
                        /* WAIT_LOOP */
                        while (0u != TSIP.REG_24H.BIT.B21)
                        {
                            /* waiting */
                        }
                        TSIP.REG_1CH.WORD = 0x00210000u;
                        TSIP.REG_24H.WORD = 0x000011c0u;
                        /* WAIT_LOOP */
                        while (0u != TSIP.REG_24H.BIT.B21)
                        {
                            /* waiting */
                        }
                        TSIP.REG_24H.WORD = 0x00000991u;
                        /* WAIT_LOOP */
                        while (0u != TSIP.REG_24H.BIT.B21)
                        {
                            /* waiting */
                        }
                        TSIP.REG_1CH.WORD = 0x00210000u;
                        RX65NHU_func101(change_endian_long(0x693ab8feu), change_endian_long(0xcac32aa2u), change_endian_long(0x35affe1au), change_endian_long(0x3013264du));
                    }
                    else if (0x01000000u == (TSIP.REG_1CH.WORD & 0xff000000u))
                    {
                        TSIP.REG_28H.WORD = 0x00bf0001u;
                        TSIP.REG_24H.WORD = 0x000098d0u;
                        /* WAIT_LOOP */
                        while (0u != TSIP.REG_24H.BIT.B21)
                        {
                            /* waiting */
                        }
                        TSIP.REG_ECH.WORD = 0x0000368au;
                        TSIP.REG_ECH.WORD = 0x000034b7u;
                        TSIP.REG_ECH.WORD = 0x000034d8u;
                        TSIP.REG_ECH.WORD = 0x000034f9u;
                        TSIP.REG_ECH.WORD = 0x0000351au;
                        RX65NHU_func100(change_endian_long(0xa5f179b5u), change_endian_long(0x199352a2u), change_endian_long(0x2c0c242eu), change_endian_long(0x33074661u));
                        TSIP.REG_104H.WORD = 0x00000058u;
                        TSIP.REG_E0H.WORD = 0x80010020u;
                        /* WAIT_LOOP */
                        while (1u != TSIP.REG_104H.BIT.B31)
                        {
                            /* waiting */
                        }
                        TSIP.REG_100H.WORD = change_endian_long(0x5fe685f6u);
                        RX65NHU_func080();
                        RX65NHU_func100(change_endian_long(0x5e43952fu), change_endian_long(0xdfa2ab50u), change_endian_long(0x89ade5f8u), change_endian_long(0xfba854a3u));
                        TSIP.REG_00H.WORD = 0x00012303u;
                        TSIP.REG_2CH.WORD = 0x00000020u;
                        RX65NHU_func313(748);
                        RX65NHU_func100(change_endian_long(0x2e0ac9feu), change_endian_long(0x7d28b0acu), change_endian_long(0xc3576563u), change_endian_long(0x17f301efu));
                        RX65NHU_func314(748+64);
                        RX65NHU_func100(change_endian_long(0xca5368e5u), change_endian_long(0x9a684517u), change_endian_long(0x8713aacfu), change_endian_long(0x16bac76fu));
                        TSIP.REG_104H.WORD = 0x00000058u;
                        TSIP.REG_E0H.WORD = 0x80010020u;
                        /* WAIT_LOOP */
                        while (1u != TSIP.REG_104H.BIT.B31)
                        {
                            /* waiting */
                        }
                        TSIP.REG_100H.WORD = change_endian_long(0x1c5a2b81u);
                        RX65NHU_func080();
                        RX65NHU_func100(change_endian_long(0xfd1b0904u), change_endian_long(0xdfeb1fa0u), change_endian_long(0x836b03d8u), change_endian_long(0x78c39437u));
                        TSIP.REG_00H.WORD = 0x00012303u;
                        TSIP.REG_2CH.WORD = 0x00000022u;
                        RX65NHU_func313(680);
                        RX65NHU_func100(change_endian_long(0x380e9b18u), change_endian_long(0xf4e6e00au), change_endian_long(0x9d0c2664u), change_endian_long(0xe7a2696au));
                        RX65NHU_func314(680+64);
                        RX65NHU_func100(change_endian_long(0x548238adu), change_endian_long(0x50708e1au), change_endian_long(0xb3364911u), change_endian_long(0x7626b0fbu));
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
                        TSIP.REG_104H.WORD = 0x00000058u;
                        TSIP.REG_E0H.WORD = 0x80010020u;
                        /* WAIT_LOOP */
                        while (1u != TSIP.REG_104H.BIT.B31)
                        {
                            /* waiting */
                        }
                        TSIP.REG_100H.WORD = change_endian_long(0x5dd049feu);
                        RX65NHU_func080();
                        RX65NHU_func100(change_endian_long(0xaaa1d5a6u), change_endian_long(0x567ef624u), change_endian_long(0x916df4e1u), change_endian_long(0x2b8aa664u));
                        TSIP.REG_00H.WORD = 0x00012303u;
                        TSIP.REG_2CH.WORD = 0x00000020u;
                        RX65NHU_func313(68);
                        RX65NHU_func100(change_endian_long(0xca501101u), change_endian_long(0xd5f92cfau), change_endian_long(0xee43d8a0u), change_endian_long(0xdf7efc50u));
                        RX65NHU_func314(68+64);
                        RX65NHU_func100(change_endian_long(0xa75dff7eu), change_endian_long(0x07717be6u), change_endian_long(0x39b00832u), change_endian_long(0x0a633faau));
                        TSIP.REG_2CH.WORD = 0x00000014u;
                        TSIP.REG_104H.WORD = 0x00003f67u;
                        /* WAIT_LOOP */
                        while (1u != TSIP.REG_104H.BIT.B31)
                        {
                            /* waiting */
                        }
                        TSIP.REG_100H.WORD = ARG6[64];
                        TSIP.REG_100H.WORD = ARG6[65];
                        TSIP.REG_100H.WORD = ARG6[66];
                        TSIP.REG_100H.WORD = ARG6[67];
                        TSIP.REG_100H.WORD = ARG6[68];
                        TSIP.REG_100H.WORD = ARG6[69];
                        TSIP.REG_100H.WORD = ARG6[70];
                        TSIP.REG_100H.WORD = ARG6[71];
                        TSIP.REG_100H.WORD = ARG6[72];
                        TSIP.REG_100H.WORD = ARG6[73];
                        TSIP.REG_100H.WORD = ARG6[74];
                        TSIP.REG_100H.WORD = ARG6[75];
                        TSIP.REG_100H.WORD = ARG6[76];
                        TSIP.REG_100H.WORD = ARG6[77];
                        TSIP.REG_100H.WORD = ARG6[78];
                        TSIP.REG_100H.WORD = ARG6[79];
                        TSIP.REG_100H.WORD = ARG6[80];
                        TSIP.REG_100H.WORD = ARG6[81];
                        TSIP.REG_100H.WORD = ARG6[82];
                        TSIP.REG_100H.WORD = ARG6[83];
                        TSIP.REG_100H.WORD = ARG6[84];
                        TSIP.REG_100H.WORD = ARG6[85];
                        TSIP.REG_100H.WORD = ARG6[86];
                        TSIP.REG_100H.WORD = ARG6[87];
                        TSIP.REG_100H.WORD = ARG6[88];
                        TSIP.REG_100H.WORD = ARG6[89];
                        TSIP.REG_100H.WORD = ARG6[90];
                        TSIP.REG_100H.WORD = ARG6[91];
                        TSIP.REG_100H.WORD = ARG6[92];
                        TSIP.REG_100H.WORD = ARG6[93];
                        TSIP.REG_100H.WORD = ARG6[94];
                        TSIP.REG_100H.WORD = ARG6[95];
                        TSIP.REG_100H.WORD = ARG6[96];
                        TSIP.REG_100H.WORD = ARG6[97];
                        TSIP.REG_100H.WORD = ARG6[98];
                        TSIP.REG_100H.WORD = ARG6[99];
                        TSIP.REG_100H.WORD = ARG6[100];
                        TSIP.REG_100H.WORD = ARG6[101];
                        TSIP.REG_100H.WORD = ARG6[102];
                        TSIP.REG_100H.WORD = ARG6[103];
                        TSIP.REG_100H.WORD = ARG6[104];
                        TSIP.REG_100H.WORD = ARG6[105];
                        TSIP.REG_100H.WORD = ARG6[106];
                        TSIP.REG_100H.WORD = ARG6[107];
                        TSIP.REG_100H.WORD = ARG6[108];
                        TSIP.REG_100H.WORD = ARG6[109];
                        TSIP.REG_100H.WORD = ARG6[110];
                        TSIP.REG_100H.WORD = ARG6[111];
                        TSIP.REG_100H.WORD = ARG6[112];
                        TSIP.REG_100H.WORD = ARG6[113];
                        TSIP.REG_100H.WORD = ARG6[114];
                        TSIP.REG_100H.WORD = ARG6[115];
                        TSIP.REG_100H.WORD = ARG6[116];
                        TSIP.REG_100H.WORD = ARG6[117];
                        TSIP.REG_100H.WORD = ARG6[118];
                        TSIP.REG_100H.WORD = ARG6[119];
                        TSIP.REG_100H.WORD = ARG6[120];
                        TSIP.REG_100H.WORD = ARG6[121];
                        TSIP.REG_100H.WORD = ARG6[122];
                        TSIP.REG_100H.WORD = ARG6[123];
                        TSIP.REG_100H.WORD = ARG6[124];
                        TSIP.REG_100H.WORD = ARG6[125];
                        TSIP.REG_100H.WORD = ARG6[126];
                        TSIP.REG_100H.WORD = ARG6[127];
                        TSIP.REG_104H.WORD = 0x00000058u;
                        TSIP.REG_E0H.WORD = 0x80010020u;
                        /* WAIT_LOOP */
                        while (1u != TSIP.REG_104H.BIT.B31)
                        {
                            /* waiting */
                        }
                        TSIP.REG_100H.WORD = change_endian_long(0x945d2158u);
                        RX65NHU_func080();
                        RX65NHU_func100(change_endian_long(0x355e360du), change_endian_long(0x28d806d3u), change_endian_long(0x9e1f4437u), change_endian_long(0x5c83863du));
                        TSIP.REG_00H.WORD = 0x00012303u;
                        TSIP.REG_2CH.WORD = 0x00000024u;
                        RX65NHU_func313(0);
                        RX65NHU_func100(change_endian_long(0x6857fbdbu), change_endian_long(0x262108c5u), change_endian_long(0xaddf91b1u), change_endian_long(0xb512fadbu));
                        RX65NHU_func314(0+64);
                        TSIP.REG_ECH.WORD = 0x0000b400u;
                        TSIP.REG_ECH.WORD = 0x000000c1u;
                        RX65NHU_func101(change_endian_long(0x5f64b050u), change_endian_long(0x0cd68d91u), change_endian_long(0x6269a8bdu), change_endian_long(0xb471600cu));
                        RX65NHU_func307();
                        TSIP.REG_104H.WORD = 0x00003f57u;
                        TSIP.REG_2CH.WORD = 0x00000014u;
                        /* WAIT_LOOP */
                        while (1u != TSIP.REG_104H.BIT.B31)
                        {
                            /* waiting */
                        }
                        TSIP.REG_100H.WORD = change_endian_long(0x0001ffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        /* WAIT_LOOP */
                        while (1u != TSIP.REG_104H.BIT.B31)
                        {
                            /* waiting */
                        }
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        /* WAIT_LOOP */
                        while (1u != TSIP.REG_104H.BIT.B31)
                        {
                            /* waiting */
                        }
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        /* WAIT_LOOP */
                        while (1u != TSIP.REG_104H.BIT.B31)
                        {
                            /* waiting */
                        }
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        /* WAIT_LOOP */
                        while (1u != TSIP.REG_104H.BIT.B31)
                        {
                            /* waiting */
                        }
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        /* WAIT_LOOP */
                        while (1u != TSIP.REG_104H.BIT.B31)
                        {
                            /* waiting */
                        }
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        /* WAIT_LOOP */
                        while (1u != TSIP.REG_104H.BIT.B31)
                        {
                            /* waiting */
                        }
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        /* WAIT_LOOP */
                        while (1u != TSIP.REG_104H.BIT.B31)
                        {
                            /* waiting */
                        }
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_24H.WORD = 0x000019c0u;
                        /* WAIT_LOOP */
                        while (0u != TSIP.REG_24H.BIT.B21)
                        {
                            /* waiting */
                        }
                        TSIP.REG_24H.WORD = 0x00001191u;
                        /* WAIT_LOOP */
                        while (0u != TSIP.REG_24H.BIT.B21)
                        {
                            /* waiting */
                        }
                        TSIP.REG_1CH.WORD = 0x00210000u;
                        TSIP.REG_24H.WORD = 0x000011c0u;
                        /* WAIT_LOOP */
                        while (0u != TSIP.REG_24H.BIT.B21)
                        {
                            /* waiting */
                        }
                        TSIP.REG_24H.WORD = 0x00001991u;
                        /* WAIT_LOOP */
                        while (0u != TSIP.REG_24H.BIT.B21)
                        {
                            /* waiting */
                        }
                        TSIP.REG_1CH.WORD = 0x00210000u;
                        TSIP.REG_104H.WORD = 0x00003757u;
                        TSIP.REG_2CH.WORD = 0x00000014u;
                        /* WAIT_LOOP */
                        while (1u != TSIP.REG_104H.BIT.B31)
                        {
                            /* waiting */
                        }
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        /* WAIT_LOOP */
                        while (1u != TSIP.REG_104H.BIT.B31)
                        {
                            /* waiting */
                        }
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        /* WAIT_LOOP */
                        while (1u != TSIP.REG_104H.BIT.B31)
                        {
                            /* waiting */
                        }
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        /* WAIT_LOOP */
                        while (1u != TSIP.REG_104H.BIT.B31)
                        {
                            /* waiting */
                        }
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        /* WAIT_LOOP */
                        while (1u != TSIP.REG_104H.BIT.B31)
                        {
                            /* waiting */
                        }
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        /* WAIT_LOOP */
                        while (1u != TSIP.REG_104H.BIT.B31)
                        {
                            /* waiting */
                        }
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        /* WAIT_LOOP */
                        while (1u != TSIP.REG_104H.BIT.B31)
                        {
                            /* waiting */
                        }
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0x00303130u);
                        TSIP.REG_100H.WORD = change_endian_long(0x0d060960u);
                        TSIP.REG_100H.WORD = change_endian_long(0x86480165u);
                        TSIP.REG_100H.WORD = change_endian_long(0x03040201u);
                        TSIP.REG_100H.WORD = change_endian_long(0x05000420u);
                        TSIP.REG_00H.WORD = 0x00003523u;
                        TSIP.REG_74H.WORD = 0x00000008u;
                        /* WAIT_LOOP */
                        while (0u != TSIP.REG_00H.BIT.B25)
                        {
                            /* waiting */
                        }
                        TSIP.REG_1CH.WORD = 0x00001800u;
                        TSIP.REG_24H.WORD = 0x000019c0u;
                        /* WAIT_LOOP */
                        while (0u != TSIP.REG_24H.BIT.B21)
                        {
                            /* waiting */
                        }
                        TSIP.REG_24H.WORD = 0x00000991u;
                        /* WAIT_LOOP */
                        while (0u != TSIP.REG_24H.BIT.B21)
                        {
                            /* waiting */
                        }
                        TSIP.REG_1CH.WORD = 0x00210000u;
                        TSIP.REG_24H.WORD = 0x000009c0u;
                        /* WAIT_LOOP */
                        while (0u != TSIP.REG_24H.BIT.B21)
                        {
                            /* waiting */
                        }
                        TSIP.REG_24H.WORD = 0x00001991u;
                        /* WAIT_LOOP */
                        while (0u != TSIP.REG_24H.BIT.B21)
                        {
                            /* waiting */
                        }
                        TSIP.REG_1CH.WORD = 0x00210000u;
                        TSIP.REG_ECH.WORD = 0x000036e5u;
                        TSIP.REG_ECH.WORD = 0x00003706u;
                        TSIP.REG_ECH.WORD = 0x00003727u;
                        TSIP.REG_ECH.WORD = 0x00003748u;
                        RX65NHU_func101(change_endian_long(0xdc4b807fu), change_endian_long(0xbf24cc82u), change_endian_long(0x48942d79u), change_endian_long(0xa63867d0u));
                    }
                    else if (0x02000000u == (TSIP.REG_1CH.WORD & 0xff000000u))
                    {
                        TSIP.REG_28H.WORD = 0x00870001u;
                        TSIP.REG_34H.WORD = 0x00000400u;
                        TSIP.REG_24H.WORD = 0x8000a8d0u;
                        /* WAIT_LOOP */
                        while (0u != TSIP.REG_24H.BIT.B21)
                        {
                            /* waiting */
                        }
                        TSIP.REG_ECH.WORD = 0x00000b9cu;
                        TSIP.REG_104H.WORD = 0x00000058u;
                        TSIP.REG_E0H.WORD = 0x800103a0u;
                        /* WAIT_LOOP */
                        while (1u != TSIP.REG_104H.BIT.B31)
                        {
                            /* waiting */
                        }
                        TSIP.REG_100H.WORD = change_endian_long(0x000000c1u);
                        RX65NHU_func101(change_endian_long(0x9b41d865u), change_endian_long(0x71684040u), change_endian_long(0x1e42b943u), change_endian_long(0x673413ccu));
                        RX65NHU_func003(ARG6);
                        RX65NHU_func101(change_endian_long(0xcfa9bd7fu), change_endian_long(0xb9936920u), change_endian_long(0x5e88bb95u), change_endian_long(0x5c1f9cf7u));
                    }
                    else if (0x03000000u == (TSIP.REG_1CH.WORD & 0xff000000u))
                    {
                        TSIP.REG_28H.WORD = 0x00bf0001u;
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
                        TSIP.REG_ECH.WORD = 0x000000c1u;
                        TSIP.REG_ECH.WORD = 0x0000b440u;
                        TSIP.REG_ECH.WORD = 0x00000020u;
                        RX65NHU_func101(change_endian_long(0x027620c8u), change_endian_long(0xb08a8495u), change_endian_long(0x3119db8fu), change_endian_long(0x00a08d79u));
                        RX65NHU_func015();
                        RX65NHU_func101(change_endian_long(0xf36e46d6u), change_endian_long(0x9e5cd594u), change_endian_long(0xe03b55d8u), change_endian_long(0x89fbc1ecu));
                    }
                    RX65NHU_func100(change_endian_long(0x12def152u), change_endian_long(0x4d4b5718u), change_endian_long(0x873c8ee7u), change_endian_long(0x0df60ee9u));
                    TSIP.REG_1CH.WORD = 0x00400000u;
                    TSIP.REG_1D0H.WORD = 0x00000000u;
                    if (1u == (TSIP.REG_1CH.BIT.B22))
                    {
                        TSIP.REG_ECH.WORD = 0x0000b400u;
                        TSIP.REG_ECH.WORD = 0xffffffffu;
                        RX65NHU_func101(change_endian_long(0x99bc5a2eu), change_endian_long(0x156ca03fu), change_endian_long(0x10f3cb62u), change_endian_long(0xfe89a724u));
                    }
                    else
                    {
                        TSIP.REG_ECH.WORD = 0x380089e0u;
                        TSIP.REG_ECH.WORD = 0x00000005u;
                        TSIP.REG_E0H.WORD = 0x00000080u;
                        TSIP.REG_1CH.WORD = 0x00260000u;
                        RX65NHU_func100(change_endian_long(0x2e79211eu), change_endian_long(0xe2357ba1u), change_endian_long(0x396c32b2u), change_endian_long(0x8428cdf8u));
                        TSIP.REG_1CH.WORD = 0x00400000u;
                        TSIP.REG_1D0H.WORD = 0x00000000u;
                        if (1u == (TSIP.REG_1CH.BIT.B22))
                        {
                            TSIP.REG_ECH.WORD = 0x00000800u;
                            RX65NHU_func101(change_endian_long(0xb4710c8cu), change_endian_long(0x77a20bc9u), change_endian_long(0x9c9792a8u), change_endian_long(0x6a391896u));
                        }
                        else
                        {
                            RX65NHU_func100(change_endian_long(0x89554086u), change_endian_long(0x7a12230fu), change_endian_long(0x82376881u), change_endian_long(0x41a57d7du));
                            TSIP.REG_ECH.WORD = 0x00007c0du;
                            TSIP.REG_1CH.WORD = 0x00600000u;
                            TSIP.REG_1D0H.WORD = 0x00000000u;
                            if (0x00000000u == (TSIP.REG_1CH.WORD & 0xff000000u))
                            {
                                TSIP.REG_28H.WORD = 0x00bf0001u;
                                TSIP.REG_ECH.WORD = 0x38000defu;
                                TSIP.REG_E0H.WORD = 0x00000080u;
                                TSIP.REG_1CH.WORD = 0x00260000u;
                                TSIP.REG_ECH.WORD = 0x380089e0u;
                                TSIP.REG_ECH.WORD = 0x00000003u;
                                TSIP.REG_E0H.WORD = 0x00000080u;
                                TSIP.REG_1CH.WORD = 0x00260000u;
                                RX65NHU_func100(change_endian_long(0x7798fc14u), change_endian_long(0x4f015e24u), change_endian_long(0xd50c558du), change_endian_long(0x59bccc9cu));
                                TSIP.REG_1CH.WORD = 0x00400000u;
                                TSIP.REG_1D0H.WORD = 0x00000000u;
                                if (1u == (TSIP.REG_1CH.BIT.B22))
                                {
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
                                    TSIP.REG_100H.WORD = change_endian_long(0x05880100u);
                                    TSIP.REG_ECH.WORD = 0x00001009u;
                                    RX65NHU_func101(change_endian_long(0xee96ee15u), change_endian_long(0x60316ed1u), change_endian_long(0xc35e6175u), change_endian_long(0xdd87a8ebu));
                                }
                                else
                                {
                                    RX65NHU_func100(change_endian_long(0x781460e8u), change_endian_long(0x77c72711u), change_endian_long(0xbd01ceeeu), change_endian_long(0x42f880f5u));
                                    TSIP.REG_C4H.WORD = 0x000c0805u;
                                    TSIP.REG_E0H.WORD = 0x810402e0u;
                                    TSIP.REG_00H.WORD = 0x00002813u;
                                    /* WAIT_LOOP */
                                    while (0u != TSIP.REG_00H.BIT.B25)
                                    {
                                        /* waiting */
                                    }
                                    TSIP.REG_1CH.WORD = 0x00001800u;
                                    RX65NHU_func100(change_endian_long(0x229bff8du), change_endian_long(0x3815deb8u), change_endian_long(0x2892599bu), change_endian_long(0x8e366981u));
                                    TSIP.REG_ECH.WORD = 0x0000342bu;
                                    RX65NHU_func080();
                                    RX65NHU_func100(change_endian_long(0x00783114u), change_endian_long(0x579eebf5u), change_endian_long(0x65125cd3u), change_endian_long(0x52a30ef7u));
                                    TSIP.REG_00H.WORD = 0x00013203u;
                                    TSIP.REG_2CH.WORD = 0x00000014u;
                                    RX65NHU_func312(1496);
                                    TSIP.REG_ECH.WORD = 0x0000342cu;
                                    RX65NHU_func080();
                                    TSIP.REG_00H.WORD = 0x00013203u;
                                    TSIP.REG_2CH.WORD = 0x00000012u;
                                    RX65NHU_func312(1496+68);
                                    TSIP.REG_104H.WORD = 0x00000058u;
                                    TSIP.REG_E0H.WORD = 0x80010000u;
                                    /* WAIT_LOOP */
                                    while (1u != TSIP.REG_104H.BIT.B31)
                                    {
                                        /* waiting */
                                    }
                                    TSIP.REG_100H.WORD = change_endian_long(0x0a880100u);
                                    TSIP.REG_ECH.WORD = 0x00001009u;
                                    RX65NHU_func101(change_endian_long(0x9bfea215u), change_endian_long(0x718c10d5u), change_endian_long(0x59a81d4eu), change_endian_long(0xe6bed200u));
                                }
                                RX65NHU_func100(change_endian_long(0xddd15aa1u), change_endian_long(0xf0d15587u), change_endian_long(0x53d935aeu), change_endian_long(0x96ba63a4u));
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
                                RX65NHU_func100(change_endian_long(0x084abd6bu), change_endian_long(0xc53dff41u), change_endian_long(0x4a803346u), change_endian_long(0x9e545465u));
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
                                RX65NHU_func100(change_endian_long(0xfb4c9f01u), change_endian_long(0x9ee0086cu), change_endian_long(0x898b1805u), change_endian_long(0xa66f1591u));
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
                                RX65NHU_func100(change_endian_long(0x2f94a16au), change_endian_long(0xc768f8eeu), change_endian_long(0xec582615u), change_endian_long(0xf75b6c62u));
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
                                RX65NHU_func100(change_endian_long(0xfb94435du), change_endian_long(0x0e99afb9u), change_endian_long(0xcff5bc34u), change_endian_long(0xbf87761eu));
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
                                RX65NHU_func100(change_endian_long(0x6c3bf36du), change_endian_long(0xa0083091u), change_endian_long(0x722fd0b3u), change_endian_long(0x1a0a061au));
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
                                RX65NHU_func101(change_endian_long(0x502b60e2u), change_endian_long(0x960f3b00u), change_endian_long(0x9cf3249au), change_endian_long(0xe70e9d90u));
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
                                RX65NHU_func100(change_endian_long(0x5d29ef22u), change_endian_long(0x28913ea2u), change_endian_long(0x8f765dccu), change_endian_long(0xdc3b2c82u));
                                TSIP.REG_1CH.WORD = 0x00400000u;
                                TSIP.REG_1D0H.WORD = 0x00000000u;
                                if (1u == (TSIP.REG_1CH.BIT.B22))
                                {
                                    TSIP.REG_ECH.WORD = 0x0000b400u;
                                    TSIP.REG_ECH.WORD = 0xffffffffu;
                                    RX65NHU_func101(change_endian_long(0x2eec8a4eu), change_endian_long(0x2e8efd6bu), change_endian_long(0x0f175ab3u), change_endian_long(0x508510e4u));
                                }
                                else
                                {
                                    RX65NHU_func100(change_endian_long(0x4bb5ee9eu), change_endian_long(0x1ebe29d2u), change_endian_long(0xaffd2188u), change_endian_long(0x44d96c02u));
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
                                    TSIP.REG_ECH.WORD = 0x00001009u;
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
                                    RX65NHU_func100(change_endian_long(0x69bdc2e0u), change_endian_long(0xf03a860au), change_endian_long(0x657dc3aeu), change_endian_long(0x4ccc0d47u));
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
                                    RX65NHU_func100(change_endian_long(0xa8a88acfu), change_endian_long(0x01deedfcu), change_endian_long(0x6a4aadb2u), change_endian_long(0xdbe85c28u));
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
                                    RX65NHU_func100(change_endian_long(0xff7c9e35u), change_endian_long(0x2e6a9d7fu), change_endian_long(0x3d6a0f85u), change_endian_long(0x9ac63393u));
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
                                    RX65NHU_func101(change_endian_long(0xe16161b9u), change_endian_long(0x97c91e44u), change_endian_long(0xe9d8acf7u), change_endian_long(0x1b67be0fu));
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    TSIP.REG_ECH.WORD = 0x00007c15u;
    TSIP.REG_1CH.WORD = 0x00602000u;
}
/**********************************************************************************************************************
 End of function ./input_dir/RX65NHU/RX65NHU_func403.prc
 *********************************************************************************************************************/
