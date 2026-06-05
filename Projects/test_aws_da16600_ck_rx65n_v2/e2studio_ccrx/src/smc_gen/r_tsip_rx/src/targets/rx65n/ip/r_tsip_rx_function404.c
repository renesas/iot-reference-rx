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
* Function Name: RX65NHU_func404
*******************************************************************************************************************/ /**
* @details       RX65NHU func404
* @param[in]     ARG1
* @param[in]     ARG2
* @param[in]     ARG3
* @param[in]     ARG4
* @param[in]     ARG5
* @param[in]     ARG6
* @param[in]     ARG7
* @param[in]     ARG8
* @param[in]     ARG9
* @param[in]     ARG10
* @note          None
*/
void RX65NHU_func404(uint32_t *ARG1, uint32_t *ARG2, uint32_t *ARG3, uint32_t *ARG4, uint32_t *ARG5, uint32_t *ARG6, uint32_t *ARG7, uint32_t *ARG8, uint32_t *ARG9, uint32_t *ARG10)
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
    TSIP.REG_E0H.WORD = 0x800103e0u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = ARG1[0];
    TSIP.REG_ECH.WORD = 0x3420abe0u;
    TSIP.REG_ECH.WORD = 0x00000008u;
    TSIP.REG_E0H.WORD = 0x00000080u;
    TSIP.REG_1CH.WORD = 0x00A60000u;
    RX65NHU_func100(change_endian_long(0x3b63c37du), change_endian_long(0xa2b00829u), change_endian_long(0x0d0cc4b3u), change_endian_long(0x5451249du));
    TSIP.REG_1CH.WORD = 0x00400000u;
    TSIP.REG_1D0H.WORD = 0x00000000u;
    if (1u == (TSIP.REG_1CH.BIT.B22))
    {
        TSIP.REG_ECH.WORD = 0x0000b400u;
        TSIP.REG_ECH.WORD = 0xffffffffu;
        RX65NHU_func101(change_endian_long(0xa5f4db82u), change_endian_long(0xa471ae40u), change_endian_long(0x03620e9du), change_endian_long(0xb2230c74u));
    }
    else
    {
        TSIP.REG_C4H.WORD = 0x00082b8du;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_104H.BIT.B31)
        {
            /* waiting */
        }
        TSIP.REG_100H.WORD = ARG2[0];
        TSIP.REG_100H.WORD = ARG2[1];
        TSIP.REG_100H.WORD = ARG2[2];
        TSIP.REG_100H.WORD = ARG2[3];
        TSIP.REG_C4H.WORD = 0x400c0b0cu;
        TSIP.REG_E0H.WORD = 0x81010380u;
        TSIP.REG_00H.WORD = 0x00002807u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_00H.BIT.B25)
        {
            /* waiting */
        }
        TSIP.REG_1CH.WORD = 0x00001800u;
        TSIP.REG_104H.WORD = 0x00000b62u;
        TSIP.REG_D0H.WORD = 0x00000200u;
        TSIP.REG_C4H.WORD = 0x02f087bfu;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_104H.BIT.B31)
        {
            /* waiting */
        }
        TSIP.REG_100H.WORD = ARG2[4];
        TSIP.REG_100H.WORD = ARG2[5];
        TSIP.REG_100H.WORD = ARG2[6];
        TSIP.REG_100H.WORD = ARG2[7];
        /* WAIT_LOOP */
        while (1u != TSIP.REG_104H.BIT.B31)
        {
            /* waiting */
        }
        TSIP.REG_100H.WORD = ARG2[8];
        TSIP.REG_100H.WORD = ARG2[9];
        TSIP.REG_100H.WORD = ARG2[10];
        TSIP.REG_100H.WORD = ARG2[11];
        /* WAIT_LOOP */
        while (1u != TSIP.REG_104H.BIT.B31)
        {
            /* waiting */
        }
        TSIP.REG_100H.WORD = ARG2[12];
        TSIP.REG_100H.WORD = ARG2[13];
        TSIP.REG_100H.WORD = ARG2[14];
        TSIP.REG_100H.WORD = ARG2[15];
        TSIP.REG_ECH.WORD = 0x0000b400u;
        TSIP.REG_ECH.WORD = 0x00000070u;
        TSIP.REG_E0H.WORD = 0x808c0000u;
        TSIP.REG_00H.WORD = 0x00008233u;
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
        TSIP.REG_100H.WORD = ARG2[16];
        TSIP.REG_100H.WORD = ARG2[17];
        TSIP.REG_100H.WORD = ARG2[18];
        TSIP.REG_100H.WORD = ARG2[19];
        TSIP.REG_C4H.WORD = 0x00900c45u;
        TSIP.REG_00H.WORD = 0x00002213u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_00H.BIT.B25)
        {
            /* waiting */
        }
        TSIP.REG_1CH.WORD = 0x00001800u;
        RX65NHU_func100(change_endian_long(0x06855a3eu), change_endian_long(0x3504b70fu), change_endian_long(0x92404524u), change_endian_long(0x14065d25u));
        TSIP.REG_1CH.WORD = 0x00400000u;
        TSIP.REG_1D0H.WORD = 0x00000000u;
        if (1u == (TSIP.REG_1CH.BIT.B22))
        {
            TSIP.REG_ECH.WORD = 0x0000b400u;
            TSIP.REG_ECH.WORD = 0xffffffffu;
            RX65NHU_func101(change_endian_long(0x2a61347eu), change_endian_long(0x9ae06929u), change_endian_long(0xffbde278u), change_endian_long(0x0cf5e475u));
        }
        else
        {
            TSIP.REG_104H.WORD = 0x00000352u;
            TSIP.REG_C4H.WORD = 0x00000885u;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_104H.BIT.B31)
            {
                /* waiting */
            }
            TSIP.REG_100H.WORD = change_endian_long(0x6b657920u);
            TSIP.REG_100H.WORD = change_endian_long(0x65787061u);
            TSIP.REG_100H.WORD = change_endian_long(0x6e73696fu);
            TSIP.REG_100H.WORD = change_endian_long(0x6e000000u);
            TSIP.REG_ECH.WORD = 0x0000b460u;
            TSIP.REG_ECH.WORD = 0x00000020u;
            TSIP.REG_E0H.WORD = 0x80830003u;
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
            TSIP.REG_104H.WORD = 0x00000068u;
            TSIP.REG_E0H.WORD = 0x80010000u;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_104H.BIT.B31)
            {
                /* waiting */
            }
            TSIP.REG_100H.WORD = change_endian_long(0x0000006eu);
            TSIP.REG_D0H.WORD = 0x00000100u;
            TSIP.REG_C4H.WORD = 0x00008887u;
            TSIP.REG_00H.WORD = 0x00002323u;
            TSIP.REG_2CH.WORD = 0x00000022u;
            /* WAIT_LOOP */
            while (0u != TSIP.REG_00H.BIT.B25)
            {
                /* waiting */
            }
            TSIP.REG_1CH.WORD = 0x00001800u;
            TSIP.REG_ECH.WORD = 0x0000b460u;
            TSIP.REG_ECH.WORD = 0x0000002cu;
            for (iLoop = 0; iLoop < 8; iLoop = iLoop + 1)
            {
                TSIP.REG_E0H.WORD = 0x80010020u;
                TSIP.REG_00H.WORD = 0x00008207u;
                /* WAIT_LOOP */
                while (0u != TSIP.REG_00H.BIT.B25)
                {
                    /* waiting */
                }
                TSIP.REG_1CH.WORD = 0x00001800u;
                TSIP.REG_ECH.WORD = 0x01986c01u;
                TSIP.REG_ECH.WORD = 0x00003c03u;
                TSIP.REG_ECH.WORD = 0x00003401u;
                TSIP.REG_ECH.WORD = 0x0000a460u;
                TSIP.REG_ECH.WORD = 0x00000004u;
            }
            TSIP.REG_104H.WORD = 0x00000762u;
            TSIP.REG_D0H.WORD = 0x00000100u;
            TSIP.REG_C4H.WORD = 0x00008887u;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_104H.BIT.B31)
            {
                /* waiting */
            }
            TSIP.REG_100H.WORD = ARG3[0];
            TSIP.REG_100H.WORD = ARG3[1];
            TSIP.REG_100H.WORD = ARG3[2];
            TSIP.REG_100H.WORD = ARG3[3];
            /* WAIT_LOOP */
            while (1u != TSIP.REG_104H.BIT.B31)
            {
                /* waiting */
            }
            TSIP.REG_100H.WORD = ARG3[4];
            TSIP.REG_100H.WORD = ARG3[5];
            TSIP.REG_100H.WORD = ARG3[6];
            TSIP.REG_100H.WORD = ARG3[7];
            for (iLoop = 0; iLoop < 8; iLoop = iLoop + 1)
            {
                TSIP.REG_E0H.WORD = 0x80010020u;
                TSIP.REG_00H.WORD = 0x00008207u;
                /* WAIT_LOOP */
                while (0u != TSIP.REG_00H.BIT.B25)
                {
                    /* waiting */
                }
                TSIP.REG_1CH.WORD = 0x00001800u;
                TSIP.REG_ECH.WORD = 0x01986c01u;
                TSIP.REG_ECH.WORD = 0x00003c03u;
                TSIP.REG_ECH.WORD = 0x00003401u;
                TSIP.REG_ECH.WORD = 0x0000a460u;
                TSIP.REG_ECH.WORD = 0x00000004u;
            }
            TSIP.REG_ECH.WORD = 0x00186c00u;
            TSIP.REG_ECH.WORD = 0x00003c03u;
            TSIP.REG_ECH.WORD = 0x0000b420u;
            TSIP.REG_ECH.WORD = 0x00000020u;
            TSIP.REG_ECH.WORD = 0x0000b560u;
            TSIP.REG_ECH.WORD = 0x00000020u;
            TSIP.REG_ECH.WORD = 0x0000098cu;
            TSIP.REG_ECH.WORD = 0x0000b5c0u;
            TSIP.REG_ECH.WORD = 0x0000004du;
            RX65NHU_func101(change_endian_long(0x721612fau), change_endian_long(0xab53130fu), change_endian_long(0xa90f98aeu), change_endian_long(0xa7d32047u));
            RX65NHU_func032();
            TSIP.REG_ECH.WORD = 0x00002c20u;
            TSIP.REG_ECH.WORD = 0x0000096bu;
            TSIP.REG_ECH.WORD = 0x0000b580u;
            TSIP.REG_ECH.WORD = 0x000000a0u;
            TSIP.REG_ECH.WORD = 0x0000b5c0u;
            TSIP.REG_ECH.WORD = 0x0000006du;
            RX65NHU_func101(change_endian_long(0x4e49b2fbu), change_endian_long(0xbcd21438u), change_endian_long(0xb633b5aeu), change_endian_long(0xfe3591c9u));
            RX65NHU_func032();
            TSIP.REG_E0H.WORD = 0x8188000cu;
            TSIP.REG_00H.WORD = 0x00003823u;
            TSIP.REG_2CH.WORD = 0x00000013u;
            /* WAIT_LOOP */
            while (0u != TSIP.REG_00H.BIT.B25)
            {
                /* waiting */
            }
            TSIP.REG_1CH.WORD = 0x00001800u;
            TSIP.REG_ECH.WORD = 0x00002c20u;
            TSIP.REG_ECH.WORD = 0x0000096bu;
            TSIP.REG_ECH.WORD = 0x0000098cu;
            TSIP.REG_ECH.WORD = 0x0000b5c0u;
            TSIP.REG_ECH.WORD = 0x00000020u;
            RX65NHU_func101(change_endian_long(0x9de23bddu), change_endian_long(0x22552db6u), change_endian_long(0x6286ef54u), change_endian_long(0x8a9d790bu));
            RX65NHU_func032();
            TSIP.REG_ECH.WORD = 0x00002c20u;
            TSIP.REG_ECH.WORD = 0x0000096bu;
            TSIP.REG_ECH.WORD = 0x0000b580u;
            TSIP.REG_ECH.WORD = 0x000000a0u;
            TSIP.REG_ECH.WORD = 0x0000b5c0u;
            TSIP.REG_ECH.WORD = 0x0000006du;
            RX65NHU_func101(change_endian_long(0x08c21088u), change_endian_long(0xe5c00c42u), change_endian_long(0x8250cc0eu), change_endian_long(0x72d5d281u));
            RX65NHU_func032();
            TSIP.REG_E0H.WORD = 0x8188000cu;
            TSIP.REG_00H.WORD = 0x00003823u;
            TSIP.REG_2CH.WORD = 0x0000009au;
            /* WAIT_LOOP */
            while (0u != TSIP.REG_00H.BIT.B25)
            {
                /* waiting */
            }
            TSIP.REG_1CH.WORD = 0x00001800u;
            TSIP.REG_ECH.WORD = 0x00002c20u;
            TSIP.REG_ECH.WORD = 0x0000096bu;
            TSIP.REG_ECH.WORD = 0x0000098cu;
            TSIP.REG_ECH.WORD = 0x0000b5c0u;
            TSIP.REG_ECH.WORD = 0x00000020u;
            RX65NHU_func101(change_endian_long(0xb3891e08u), change_endian_long(0xf0eae463u), change_endian_long(0x06196a93u), change_endian_long(0xb7e52291u));
            RX65NHU_func032();
            TSIP.REG_ECH.WORD = 0x00002c20u;
            TSIP.REG_ECH.WORD = 0x0000096bu;
            TSIP.REG_ECH.WORD = 0x0000b580u;
            TSIP.REG_ECH.WORD = 0x000000a0u;
            TSIP.REG_ECH.WORD = 0x0000b5c0u;
            TSIP.REG_ECH.WORD = 0x0000006du;
            RX65NHU_func101(change_endian_long(0x5a1a744cu), change_endian_long(0x42e37972u), change_endian_long(0x27d3c32au), change_endian_long(0xb66452a3u));
            RX65NHU_func032();
            TSIP.REG_E0H.WORD = 0x8188000cu;
            TSIP.REG_00H.WORD = 0x00003823u;
            TSIP.REG_2CH.WORD = 0x0000001au;
            /* WAIT_LOOP */
            while (0u != TSIP.REG_00H.BIT.B25)
            {
                /* waiting */
            }
            TSIP.REG_1CH.WORD = 0x00001800u;
            TSIP.REG_ECH.WORD = 0x00002c20u;
            TSIP.REG_ECH.WORD = 0x0000096bu;
            TSIP.REG_ECH.WORD = 0x0000098cu;
            TSIP.REG_ECH.WORD = 0x0000b5c0u;
            TSIP.REG_ECH.WORD = 0x00000020u;
            RX65NHU_func101(change_endian_long(0x30f59983u), change_endian_long(0x43cc761cu), change_endian_long(0x882920cau), change_endian_long(0xfe7c2b20u));
            RX65NHU_func032();
            TSIP.REG_ECH.WORD = 0x00002c20u;
            TSIP.REG_ECH.WORD = 0x0000096bu;
            TSIP.REG_ECH.WORD = 0x0000b580u;
            TSIP.REG_ECH.WORD = 0x000000a0u;
            TSIP.REG_ECH.WORD = 0x0000b5c0u;
            TSIP.REG_ECH.WORD = 0x0000006du;
            RX65NHU_func101(change_endian_long(0x662f6228u), change_endian_long(0xd7ce117cu), change_endian_long(0xf817a58cu), change_endian_long(0x7648ab43u));
            RX65NHU_func032();
            TSIP.REG_E0H.WORD = 0x8188000cu;
            TSIP.REG_00H.WORD = 0x00003823u;
            TSIP.REG_2CH.WORD = 0x00000092u;
            /* WAIT_LOOP */
            while (0u != TSIP.REG_00H.BIT.B25)
            {
                /* waiting */
            }
            TSIP.REG_1CH.WORD = 0x00001800u;
            TSIP.REG_ECH.WORD = 0x00002c20u;
            TSIP.REG_ECH.WORD = 0x0000096bu;
            TSIP.REG_ECH.WORD = 0x0000098cu;
            TSIP.REG_ECH.WORD = 0x0000b5c0u;
            TSIP.REG_ECH.WORD = 0x00000020u;
            RX65NHU_func101(change_endian_long(0x2bd4a518u), change_endian_long(0x24e52ee0u), change_endian_long(0x01fb69c9u), change_endian_long(0x3237cd5du));
            RX65NHU_func032();
            TSIP.REG_ECH.WORD = 0x00002c20u;
            TSIP.REG_ECH.WORD = 0x0000096bu;
            TSIP.REG_ECH.WORD = 0x0000b580u;
            TSIP.REG_ECH.WORD = 0x000000a0u;
            TSIP.REG_ECH.WORD = 0x0000b5c0u;
            TSIP.REG_ECH.WORD = 0x0000006du;
            RX65NHU_func101(change_endian_long(0x3e44f720u), change_endian_long(0xc93bb919u), change_endian_long(0xcdc52fd9u), change_endian_long(0x02183705u));
            RX65NHU_func032();
            RX65NHU_func100(change_endian_long(0xef76e592u), change_endian_long(0xb98b88b6u), change_endian_long(0xccd7a1c8u), change_endian_long(0xdc20844au));
            TSIP.REG_E0H.WORD = 0x8188000cu;
            TSIP.REG_00H.WORD = 0x00003823u;
            TSIP.REG_2CH.WORD = 0x00000012u;
            /* WAIT_LOOP */
            while (0u != TSIP.REG_00H.BIT.B25)
            {
                /* waiting */
            }
            TSIP.REG_1CH.WORD = 0x00001800u;
            TSIP.REG_28H.WORD = 0x00a70001u;
            TSIP.REG_ECH.WORD = 0x000034dfu;
            TSIP.REG_ECH.WORD = 0x3420abe0u;
            TSIP.REG_ECH.WORD = 0x00000004u;
            TSIP.REG_ECH.WORD = 0x2000b4c0u;
            TSIP.REG_ECH.WORD = 0x00000002u;
            TSIP.REG_ECH.WORD = 0x3420abe0u;
            TSIP.REG_ECH.WORD = 0x00000006u;
            TSIP.REG_ECH.WORD = 0x2000b4c0u;
            TSIP.REG_ECH.WORD = 0x00000006u;
            TSIP.REG_ECH.WORD = 0x00007c06u;
            TSIP.REG_1CH.WORD = 0x00600000u;
            TSIP.REG_1D0H.WORD = 0x00000000u;
            if (0x00000000u == (TSIP.REG_1CH.WORD & 0xff000000u))
            {
                RX65NHU_func100(change_endian_long(0x7e5cc995u), change_endian_long(0x638c1fc8u), change_endian_long(0x48352723u), change_endian_long(0xf3683603u));
                TSIP.REG_104H.WORD = 0x00000058u;
                TSIP.REG_E0H.WORD = 0x80010040u;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_104H.BIT.B31)
                {
                    /* waiting */
                }
                TSIP.REG_100H.WORD = change_endian_long(0x03000100u);
                TSIP.REG_ECH.WORD = 0x00001058u;
                RX65NHU_func103();
                TSIP.REG_104H.WORD = 0x00000052u;
                TSIP.REG_C4H.WORD = 0x01000c84u;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_104H.BIT.B31)
                {
                    /* waiting */
                }
                TSIP.REG_100H.WORD = change_endian_long(0x00000000u);
                TSIP.REG_E0H.WORD = 0x80030060u;
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
                RX65NHU_func100(change_endian_long(0xf82af5c1u), change_endian_long(0xec8f73d5u), change_endian_long(0x01d285cdu), change_endian_long(0x9d4135d7u));
                TSIP.REG_C4H.WORD = 0x000c2b0du;
                TSIP.REG_E0H.WORD = 0x81040040u;
                TSIP.REG_00H.WORD = 0x00002813u;
                /* WAIT_LOOP */
                while (0u != TSIP.REG_00H.BIT.B25)
                {
                    /* waiting */
                }
                TSIP.REG_1CH.WORD = 0x00001800u;
                TSIP.REG_D0H.WORD = 0x00000100u;
                TSIP.REG_C4H.WORD = 0x02e087bfu;
                TSIP.REG_00H.WORD = 0x00002317u;
                TSIP.REG_2CH.WORD = 0x00000022u;
                /* WAIT_LOOP */
                while (0u != TSIP.REG_00H.BIT.B25)
                {
                    /* waiting */
                }
                TSIP.REG_1CH.WORD = 0x00001800u;
                TSIP.REG_00H.WORD = 0x0000200fu;
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
                ARG5[4] = TSIP.REG_100H.WORD;
                ARG5[5] = TSIP.REG_100H.WORD;
                ARG5[6] = TSIP.REG_100H.WORD;
                ARG5[7] = TSIP.REG_100H.WORD;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_04H.BIT.B30)
                {
                    /* waiting */
                }
                ARG5[8] = TSIP.REG_100H.WORD;
                ARG5[9] = TSIP.REG_100H.WORD;
                ARG5[10] = TSIP.REG_100H.WORD;
                ARG5[11] = TSIP.REG_100H.WORD;
                RX65NHU_func100(change_endian_long(0x32cb8965u), change_endian_long(0x765580dbu), change_endian_long(0xe6a20a39u), change_endian_long(0xef57583cu));
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
                ARG5[12] = TSIP.REG_100H.WORD;
                ARG5[13] = TSIP.REG_100H.WORD;
                ARG5[14] = TSIP.REG_100H.WORD;
                ARG5[15] = TSIP.REG_100H.WORD;
                RX65NHU_func100(change_endian_long(0xe143eb9eu), change_endian_long(0x451abbabu), change_endian_long(0xc4f41f5eu), change_endian_long(0x0d17bb12u));
                TSIP.REG_E0H.WORD = 0x81040040u;
                TSIP.REG_04H.WORD = 0x00000612u;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_04H.BIT.B30)
                {
                    /* waiting */
                }
                ARG5[0] = TSIP.REG_100H.WORD;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_04H.BIT.B30)
                {
                    /* waiting */
                }
                ARG5[1] = TSIP.REG_100H.WORD;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_04H.BIT.B30)
                {
                    /* waiting */
                }
                ARG5[2] = TSIP.REG_100H.WORD;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_04H.BIT.B30)
                {
                    /* waiting */
                }
                ARG5[3] = TSIP.REG_100H.WORD;
                RX65NHU_func100(change_endian_long(0x3f4995eeu), change_endian_long(0xb57c6e2eu), change_endian_long(0x06c6cdccu), change_endian_long(0x3a099d8fu));
                TSIP.REG_104H.WORD = 0x00000058u;
                TSIP.REG_E0H.WORD = 0x80010040u;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_104H.BIT.B31)
                {
                    /* waiting */
                }
                TSIP.REG_100H.WORD = change_endian_long(0x03000100u);
                TSIP.REG_ECH.WORD = 0x00001059u;
                RX65NHU_func103();
                TSIP.REG_104H.WORD = 0x00000052u;
                TSIP.REG_C4H.WORD = 0x01000c84u;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_104H.BIT.B31)
                {
                    /* waiting */
                }
                TSIP.REG_100H.WORD = change_endian_long(0x00000000u);
                TSIP.REG_E0H.WORD = 0x80030060u;
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
                RX65NHU_func100(change_endian_long(0xc16686c2u), change_endian_long(0x315336efu), change_endian_long(0x3cb50c63u), change_endian_long(0x2e068dc1u));
                TSIP.REG_C4H.WORD = 0x000c2b0du;
                TSIP.REG_E0H.WORD = 0x81040040u;
                TSIP.REG_00H.WORD = 0x00002813u;
                /* WAIT_LOOP */
                while (0u != TSIP.REG_00H.BIT.B25)
                {
                    /* waiting */
                }
                TSIP.REG_1CH.WORD = 0x00001800u;
                TSIP.REG_D0H.WORD = 0x00000100u;
                TSIP.REG_C4H.WORD = 0x02e087bfu;
                TSIP.REG_00H.WORD = 0x00002317u;
                /* WAIT_LOOP */
                while (0u != TSIP.REG_00H.BIT.B25)
                {
                    /* waiting */
                }
                TSIP.REG_1CH.WORD = 0x00001800u;
                TSIP.REG_00H.WORD = 0x0000200fu;
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
                ARG6[4] = TSIP.REG_100H.WORD;
                ARG6[5] = TSIP.REG_100H.WORD;
                ARG6[6] = TSIP.REG_100H.WORD;
                ARG6[7] = TSIP.REG_100H.WORD;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_04H.BIT.B30)
                {
                    /* waiting */
                }
                ARG6[8] = TSIP.REG_100H.WORD;
                ARG6[9] = TSIP.REG_100H.WORD;
                ARG6[10] = TSIP.REG_100H.WORD;
                ARG6[11] = TSIP.REG_100H.WORD;
                RX65NHU_func100(change_endian_long(0x3dcb4f38u), change_endian_long(0xea99b555u), change_endian_long(0x2501d75eu), change_endian_long(0x4ab962fcu));
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
                ARG6[12] = TSIP.REG_100H.WORD;
                ARG6[13] = TSIP.REG_100H.WORD;
                ARG6[14] = TSIP.REG_100H.WORD;
                ARG6[15] = TSIP.REG_100H.WORD;
                RX65NHU_func100(change_endian_long(0x716a1bc4u), change_endian_long(0x9705c01bu), change_endian_long(0xb88c1eeeu), change_endian_long(0x436f54f6u));
                TSIP.REG_E0H.WORD = 0x81040040u;
                TSIP.REG_04H.WORD = 0x00000612u;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_04H.BIT.B30)
                {
                    /* waiting */
                }
                ARG6[0] = TSIP.REG_100H.WORD;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_04H.BIT.B30)
                {
                    /* waiting */
                }
                ARG6[1] = TSIP.REG_100H.WORD;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_04H.BIT.B30)
                {
                    /* waiting */
                }
                ARG6[2] = TSIP.REG_100H.WORD;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_04H.BIT.B30)
                {
                    /* waiting */
                }
                ARG6[3] = TSIP.REG_100H.WORD;
                RX65NHU_func100(change_endian_long(0xa2796facu), change_endian_long(0x8bb48d93u), change_endian_long(0x20884360u), change_endian_long(0x55fcddb0u));
                TSIP.REG_104H.WORD = 0x00000058u;
                TSIP.REG_E0H.WORD = 0x80010040u;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_104H.BIT.B31)
                {
                    /* waiting */
                }
                TSIP.REG_100H.WORD = change_endian_long(0x00000100u);
                TSIP.REG_ECH.WORD = 0x0000105au;
                RX65NHU_func103();
                TSIP.REG_104H.WORD = 0x00000052u;
                TSIP.REG_C4H.WORD = 0x01000c84u;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_104H.BIT.B31)
                {
                    /* waiting */
                }
                TSIP.REG_100H.WORD = change_endian_long(0x00000000u);
                TSIP.REG_E0H.WORD = 0x80030060u;
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
                RX65NHU_func100(change_endian_long(0xcdde8428u), change_endian_long(0x3e66b406u), change_endian_long(0xbedcb7e1u), change_endian_long(0x3f9a0d18u));
                TSIP.REG_C4H.WORD = 0x000c2b0du;
                TSIP.REG_E0H.WORD = 0x81040040u;
                TSIP.REG_00H.WORD = 0x00002813u;
                /* WAIT_LOOP */
                while (0u != TSIP.REG_00H.BIT.B25)
                {
                    /* waiting */
                }
                TSIP.REG_1CH.WORD = 0x00001800u;
                TSIP.REG_C4H.WORD = 0x02e007bdu;
                TSIP.REG_00H.WORD = 0x00002313u;
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
                ARG7[4] = TSIP.REG_100H.WORD;
                ARG7[5] = TSIP.REG_100H.WORD;
                ARG7[6] = TSIP.REG_100H.WORD;
                ARG7[7] = TSIP.REG_100H.WORD;
                RX65NHU_func100(change_endian_long(0x4bcf105au), change_endian_long(0x31ad4f13u), change_endian_long(0x4aecb25eu), change_endian_long(0x8926c95fu));
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
                ARG7[8] = TSIP.REG_100H.WORD;
                ARG7[9] = TSIP.REG_100H.WORD;
                ARG7[10] = TSIP.REG_100H.WORD;
                ARG7[11] = TSIP.REG_100H.WORD;
                RX65NHU_func100(change_endian_long(0xd71217b1u), change_endian_long(0x5077175eu), change_endian_long(0x7d209ca5u), change_endian_long(0x76812650u));
                TSIP.REG_E0H.WORD = 0x81040040u;
                TSIP.REG_04H.WORD = 0x00000612u;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_04H.BIT.B30)
                {
                    /* waiting */
                }
                ARG7[0] = TSIP.REG_100H.WORD;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_04H.BIT.B30)
                {
                    /* waiting */
                }
                ARG7[1] = TSIP.REG_100H.WORD;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_04H.BIT.B30)
                {
                    /* waiting */
                }
                ARG7[2] = TSIP.REG_100H.WORD;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_04H.BIT.B30)
                {
                    /* waiting */
                }
                ARG7[3] = TSIP.REG_100H.WORD;
                RX65NHU_func100(change_endian_long(0x8a4864c9u), change_endian_long(0x0ab7f85bu), change_endian_long(0xf51f5590u), change_endian_long(0xcae2f178u));
                TSIP.REG_104H.WORD = 0x00000058u;
                TSIP.REG_E0H.WORD = 0x80010040u;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_104H.BIT.B31)
                {
                    /* waiting */
                }
                TSIP.REG_100H.WORD = change_endian_long(0x00000100u);
                TSIP.REG_ECH.WORD = 0x0000105bu;
                RX65NHU_func103();
                TSIP.REG_104H.WORD = 0x00000052u;
                TSIP.REG_C4H.WORD = 0x01000c84u;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_104H.BIT.B31)
                {
                    /* waiting */
                }
                TSIP.REG_100H.WORD = change_endian_long(0x00000000u);
                TSIP.REG_E0H.WORD = 0x80030060u;
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
                RX65NHU_func100(change_endian_long(0x516b9ca1u), change_endian_long(0xb847e224u), change_endian_long(0xc4a4269du), change_endian_long(0xcbf4b14eu));
                TSIP.REG_C4H.WORD = 0x000c2b0du;
                TSIP.REG_E0H.WORD = 0x81040040u;
                TSIP.REG_00H.WORD = 0x00002813u;
                /* WAIT_LOOP */
                while (0u != TSIP.REG_00H.BIT.B25)
                {
                    /* waiting */
                }
                TSIP.REG_1CH.WORD = 0x00001800u;
                TSIP.REG_C4H.WORD = 0x02e007bdu;
                TSIP.REG_00H.WORD = 0x00002313u;
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
                ARG8[4] = TSIP.REG_100H.WORD;
                ARG8[5] = TSIP.REG_100H.WORD;
                ARG8[6] = TSIP.REG_100H.WORD;
                ARG8[7] = TSIP.REG_100H.WORD;
                RX65NHU_func100(change_endian_long(0xca31ebe7u), change_endian_long(0x804816f8u), change_endian_long(0xe1f705c4u), change_endian_long(0x43550dffu));
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
                ARG8[8] = TSIP.REG_100H.WORD;
                ARG8[9] = TSIP.REG_100H.WORD;
                ARG8[10] = TSIP.REG_100H.WORD;
                ARG8[11] = TSIP.REG_100H.WORD;
                RX65NHU_func100(change_endian_long(0x1bea2fc2u), change_endian_long(0xe9597670u), change_endian_long(0x4080a7b6u), change_endian_long(0xa64cbceau));
                TSIP.REG_E0H.WORD = 0x81040040u;
                TSIP.REG_04H.WORD = 0x00000612u;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_04H.BIT.B30)
                {
                    /* waiting */
                }
                ARG8[0] = TSIP.REG_100H.WORD;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_04H.BIT.B30)
                {
                    /* waiting */
                }
                ARG8[1] = TSIP.REG_100H.WORD;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_04H.BIT.B30)
                {
                    /* waiting */
                }
                ARG8[2] = TSIP.REG_100H.WORD;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_04H.BIT.B30)
                {
                    /* waiting */
                }
                ARG8[3] = TSIP.REG_100H.WORD;
                RX65NHU_func100(change_endian_long(0x7824c728u), change_endian_long(0x41eb5e33u), change_endian_long(0x4e79acedu), change_endian_long(0xb8fe829fu));
                TSIP.REG_C4H.WORD = 0x00000885u;
                TSIP.REG_00H.WORD = 0x00002313u;
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
                ARG9[0] = TSIP.REG_100H.WORD;
                ARG9[1] = TSIP.REG_100H.WORD;
                ARG9[2] = TSIP.REG_100H.WORD;
                ARG9[3] = TSIP.REG_100H.WORD;
                RX65NHU_func100(change_endian_long(0x5d6f6435u), change_endian_long(0x2753e5e1u), change_endian_long(0xb9e39126u), change_endian_long(0xc0a9ef86u));
                TSIP.REG_C4H.WORD = 0x00000885u;
                TSIP.REG_00H.WORD = 0x00002313u;
                /* WAIT_LOOP */
                while (0u != TSIP.REG_00H.BIT.B25)
                {
                    /* waiting */
                }
                TSIP.REG_1CH.WORD = 0x00001800u;
                TSIP.REG_00H.WORD = 0x0000033bu;
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
                ARG10[0] = TSIP.REG_100H.WORD;
                ARG10[1] = TSIP.REG_100H.WORD;
                ARG10[2] = TSIP.REG_100H.WORD;
                ARG10[3] = TSIP.REG_100H.WORD;
                RX65NHU_func101(change_endian_long(0xd159eda4u), change_endian_long(0x2d2c551au), change_endian_long(0xed36a1f6u), change_endian_long(0x9fa1dcf7u));
            }
            else if (0x01000000u == (TSIP.REG_1CH.WORD & 0xff000000u))
            {
                RX65NHU_func100(change_endian_long(0x3562863bu), change_endian_long(0xb37da4c7u), change_endian_long(0x687372eeu), change_endian_long(0xac30754bu));
                TSIP.REG_104H.WORD = 0x00000058u;
                TSIP.REG_E0H.WORD = 0x80010040u;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_104H.BIT.B31)
                {
                    /* waiting */
                }
                TSIP.REG_100H.WORD = change_endian_long(0x03000100u);
                TSIP.REG_ECH.WORD = 0x00001058u;
                RX65NHU_func103();
                TSIP.REG_104H.WORD = 0x00000052u;
                TSIP.REG_C4H.WORD = 0x01000c84u;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_104H.BIT.B31)
                {
                    /* waiting */
                }
                TSIP.REG_100H.WORD = change_endian_long(0x00000000u);
                TSIP.REG_E0H.WORD = 0x80030060u;
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
                RX65NHU_func100(change_endian_long(0x964d82d6u), change_endian_long(0xc18f6cf5u), change_endian_long(0x999ce46bu), change_endian_long(0xb763eb9au));
                TSIP.REG_C4H.WORD = 0x000c2b0du;
                TSIP.REG_E0H.WORD = 0x81040040u;
                TSIP.REG_00H.WORD = 0x00002813u;
                /* WAIT_LOOP */
                while (0u != TSIP.REG_00H.BIT.B25)
                {
                    /* waiting */
                }
                TSIP.REG_1CH.WORD = 0x00001800u;
                TSIP.REG_D0H.WORD = 0x00000100u;
                TSIP.REG_C4H.WORD = 0x02e087bfu;
                TSIP.REG_00H.WORD = 0x00002317u;
                TSIP.REG_2CH.WORD = 0x00000022u;
                /* WAIT_LOOP */
                while (0u != TSIP.REG_00H.BIT.B25)
                {
                    /* waiting */
                }
                TSIP.REG_1CH.WORD = 0x00001800u;
                TSIP.REG_00H.WORD = 0x0000200fu;
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
                ARG5[4] = TSIP.REG_100H.WORD;
                ARG5[5] = TSIP.REG_100H.WORD;
                ARG5[6] = TSIP.REG_100H.WORD;
                ARG5[7] = TSIP.REG_100H.WORD;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_04H.BIT.B30)
                {
                    /* waiting */
                }
                ARG5[8] = TSIP.REG_100H.WORD;
                ARG5[9] = TSIP.REG_100H.WORD;
                ARG5[10] = TSIP.REG_100H.WORD;
                ARG5[11] = TSIP.REG_100H.WORD;
                RX65NHU_func100(change_endian_long(0x928c64aau), change_endian_long(0x231313abu), change_endian_long(0x80605d4au), change_endian_long(0xebda171fu));
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
                ARG5[12] = TSIP.REG_100H.WORD;
                ARG5[13] = TSIP.REG_100H.WORD;
                ARG5[14] = TSIP.REG_100H.WORD;
                ARG5[15] = TSIP.REG_100H.WORD;
                RX65NHU_func100(change_endian_long(0x8dec9e34u), change_endian_long(0x55b9afe9u), change_endian_long(0xc98fb6c2u), change_endian_long(0xfcdb0f54u));
                TSIP.REG_E0H.WORD = 0x81040040u;
                TSIP.REG_04H.WORD = 0x00000612u;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_04H.BIT.B30)
                {
                    /* waiting */
                }
                ARG5[0] = TSIP.REG_100H.WORD;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_04H.BIT.B30)
                {
                    /* waiting */
                }
                ARG5[1] = TSIP.REG_100H.WORD;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_04H.BIT.B30)
                {
                    /* waiting */
                }
                ARG5[2] = TSIP.REG_100H.WORD;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_04H.BIT.B30)
                {
                    /* waiting */
                }
                ARG5[3] = TSIP.REG_100H.WORD;
                RX65NHU_func100(change_endian_long(0xcb04ce74u), change_endian_long(0x5fee3aacu), change_endian_long(0x7dc1f7cdu), change_endian_long(0xa789288fu));
                TSIP.REG_104H.WORD = 0x00000058u;
                TSIP.REG_E0H.WORD = 0x80010040u;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_104H.BIT.B31)
                {
                    /* waiting */
                }
                TSIP.REG_100H.WORD = change_endian_long(0x03000100u);
                TSIP.REG_ECH.WORD = 0x00001059u;
                RX65NHU_func103();
                TSIP.REG_104H.WORD = 0x00000052u;
                TSIP.REG_C4H.WORD = 0x01000c84u;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_104H.BIT.B31)
                {
                    /* waiting */
                }
                TSIP.REG_100H.WORD = change_endian_long(0x00000000u);
                TSIP.REG_E0H.WORD = 0x80030060u;
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
                RX65NHU_func100(change_endian_long(0x32173e46u), change_endian_long(0x318c3ee3u), change_endian_long(0x733b3becu), change_endian_long(0x6a4df1b8u));
                TSIP.REG_C4H.WORD = 0x000c2b0du;
                TSIP.REG_E0H.WORD = 0x81040040u;
                TSIP.REG_00H.WORD = 0x00002813u;
                /* WAIT_LOOP */
                while (0u != TSIP.REG_00H.BIT.B25)
                {
                    /* waiting */
                }
                TSIP.REG_1CH.WORD = 0x00001800u;
                TSIP.REG_D0H.WORD = 0x00000100u;
                TSIP.REG_C4H.WORD = 0x02e087bfu;
                TSIP.REG_00H.WORD = 0x00002317u;
                /* WAIT_LOOP */
                while (0u != TSIP.REG_00H.BIT.B25)
                {
                    /* waiting */
                }
                TSIP.REG_1CH.WORD = 0x00001800u;
                TSIP.REG_00H.WORD = 0x0000200fu;
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
                ARG6[4] = TSIP.REG_100H.WORD;
                ARG6[5] = TSIP.REG_100H.WORD;
                ARG6[6] = TSIP.REG_100H.WORD;
                ARG6[7] = TSIP.REG_100H.WORD;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_04H.BIT.B30)
                {
                    /* waiting */
                }
                ARG6[8] = TSIP.REG_100H.WORD;
                ARG6[9] = TSIP.REG_100H.WORD;
                ARG6[10] = TSIP.REG_100H.WORD;
                ARG6[11] = TSIP.REG_100H.WORD;
                RX65NHU_func100(change_endian_long(0x1d924998u), change_endian_long(0x97214e83u), change_endian_long(0x2526b795u), change_endian_long(0x7f2a805au));
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
                ARG6[12] = TSIP.REG_100H.WORD;
                ARG6[13] = TSIP.REG_100H.WORD;
                ARG6[14] = TSIP.REG_100H.WORD;
                ARG6[15] = TSIP.REG_100H.WORD;
                RX65NHU_func100(change_endian_long(0x5efb5a09u), change_endian_long(0xf27e9a82u), change_endian_long(0xa6267379u), change_endian_long(0xa55e45a2u));
                TSIP.REG_E0H.WORD = 0x81040040u;
                TSIP.REG_04H.WORD = 0x00000612u;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_04H.BIT.B30)
                {
                    /* waiting */
                }
                ARG6[0] = TSIP.REG_100H.WORD;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_04H.BIT.B30)
                {
                    /* waiting */
                }
                ARG6[1] = TSIP.REG_100H.WORD;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_04H.BIT.B30)
                {
                    /* waiting */
                }
                ARG6[2] = TSIP.REG_100H.WORD;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_04H.BIT.B30)
                {
                    /* waiting */
                }
                ARG6[3] = TSIP.REG_100H.WORD;
                RX65NHU_func100(change_endian_long(0x8c181203u), change_endian_long(0x03113dd3u), change_endian_long(0x688afc1du), change_endian_long(0xa12e71deu));
                TSIP.REG_104H.WORD = 0x00000058u;
                TSIP.REG_E0H.WORD = 0x80010040u;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_104H.BIT.B31)
                {
                    /* waiting */
                }
                TSIP.REG_100H.WORD = change_endian_long(0x01000100u);
                TSIP.REG_ECH.WORD = 0x0000105au;
                RX65NHU_func103();
                TSIP.REG_104H.WORD = 0x00000052u;
                TSIP.REG_C4H.WORD = 0x01000c84u;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_104H.BIT.B31)
                {
                    /* waiting */
                }
                TSIP.REG_100H.WORD = change_endian_long(0x00000000u);
                TSIP.REG_E0H.WORD = 0x80030060u;
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
                RX65NHU_func100(change_endian_long(0x85d6158eu), change_endian_long(0xc1fc2045u), change_endian_long(0xee9ce8fau), change_endian_long(0x8d842bbbu));
                TSIP.REG_C4H.WORD = 0x000c2b0du;
                TSIP.REG_E0H.WORD = 0x81040040u;
                TSIP.REG_00H.WORD = 0x00002813u;
                /* WAIT_LOOP */
                while (0u != TSIP.REG_00H.BIT.B25)
                {
                    /* waiting */
                }
                TSIP.REG_1CH.WORD = 0x00001800u;
                TSIP.REG_D0H.WORD = 0x00000100u;
                TSIP.REG_C4H.WORD = 0x02e087bfu;
                TSIP.REG_00H.WORD = 0x00002323u;
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
                ARG7[4] = TSIP.REG_100H.WORD;
                ARG7[5] = TSIP.REG_100H.WORD;
                ARG7[6] = TSIP.REG_100H.WORD;
                ARG7[7] = TSIP.REG_100H.WORD;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_04H.BIT.B30)
                {
                    /* waiting */
                }
                ARG7[8] = TSIP.REG_100H.WORD;
                ARG7[9] = TSIP.REG_100H.WORD;
                ARG7[10] = TSIP.REG_100H.WORD;
                ARG7[11] = TSIP.REG_100H.WORD;
                RX65NHU_func100(change_endian_long(0x6999dd30u), change_endian_long(0x5aac1014u), change_endian_long(0xa34a6b8au), change_endian_long(0x168707aeu));
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
                ARG7[12] = TSIP.REG_100H.WORD;
                ARG7[13] = TSIP.REG_100H.WORD;
                ARG7[14] = TSIP.REG_100H.WORD;
                ARG7[15] = TSIP.REG_100H.WORD;
                RX65NHU_func100(change_endian_long(0x2dd992c3u), change_endian_long(0x7e424768u), change_endian_long(0x994582e7u), change_endian_long(0x36d1cc0cu));
                TSIP.REG_E0H.WORD = 0x81040040u;
                TSIP.REG_04H.WORD = 0x00000612u;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_04H.BIT.B30)
                {
                    /* waiting */
                }
                ARG7[0] = TSIP.REG_100H.WORD;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_04H.BIT.B30)
                {
                    /* waiting */
                }
                ARG7[1] = TSIP.REG_100H.WORD;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_04H.BIT.B30)
                {
                    /* waiting */
                }
                ARG7[2] = TSIP.REG_100H.WORD;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_04H.BIT.B30)
                {
                    /* waiting */
                }
                ARG7[3] = TSIP.REG_100H.WORD;
                RX65NHU_func100(change_endian_long(0xa8c7862fu), change_endian_long(0xd5dcd617u), change_endian_long(0xe2f1aae5u), change_endian_long(0x52b1bfe3u));
                TSIP.REG_104H.WORD = 0x00000058u;
                TSIP.REG_E0H.WORD = 0x80010040u;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_104H.BIT.B31)
                {
                    /* waiting */
                }
                TSIP.REG_100H.WORD = change_endian_long(0x01000100u);
                TSIP.REG_ECH.WORD = 0x0000105bu;
                RX65NHU_func103();
                TSIP.REG_104H.WORD = 0x00000052u;
                TSIP.REG_C4H.WORD = 0x01000c84u;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_104H.BIT.B31)
                {
                    /* waiting */
                }
                TSIP.REG_100H.WORD = change_endian_long(0x00000000u);
                TSIP.REG_E0H.WORD = 0x80030060u;
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
                RX65NHU_func100(change_endian_long(0xa4b016f8u), change_endian_long(0x4fdb181du), change_endian_long(0x690fc3e1u), change_endian_long(0xeb202c9du));
                TSIP.REG_C4H.WORD = 0x000c2b0du;
                TSIP.REG_E0H.WORD = 0x81040040u;
                TSIP.REG_00H.WORD = 0x00002813u;
                /* WAIT_LOOP */
                while (0u != TSIP.REG_00H.BIT.B25)
                {
                    /* waiting */
                }
                TSIP.REG_1CH.WORD = 0x00001800u;
                TSIP.REG_D0H.WORD = 0x00000100u;
                TSIP.REG_C4H.WORD = 0x02e087bfu;
                TSIP.REG_00H.WORD = 0x00002323u;
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
                ARG8[4] = TSIP.REG_100H.WORD;
                ARG8[5] = TSIP.REG_100H.WORD;
                ARG8[6] = TSIP.REG_100H.WORD;
                ARG8[7] = TSIP.REG_100H.WORD;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_04H.BIT.B30)
                {
                    /* waiting */
                }
                ARG8[8] = TSIP.REG_100H.WORD;
                ARG8[9] = TSIP.REG_100H.WORD;
                ARG8[10] = TSIP.REG_100H.WORD;
                ARG8[11] = TSIP.REG_100H.WORD;
                RX65NHU_func100(change_endian_long(0x01cf79d3u), change_endian_long(0x9e8dc310u), change_endian_long(0x453f8d95u), change_endian_long(0x570070ddu));
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
                ARG8[12] = TSIP.REG_100H.WORD;
                ARG8[13] = TSIP.REG_100H.WORD;
                ARG8[14] = TSIP.REG_100H.WORD;
                ARG8[15] = TSIP.REG_100H.WORD;
                RX65NHU_func100(change_endian_long(0xe3481024u), change_endian_long(0xb36306ebu), change_endian_long(0x6a0b858au), change_endian_long(0xf71595c2u));
                TSIP.REG_E0H.WORD = 0x81040040u;
                TSIP.REG_04H.WORD = 0x00000612u;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_04H.BIT.B30)
                {
                    /* waiting */
                }
                ARG8[0] = TSIP.REG_100H.WORD;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_04H.BIT.B30)
                {
                    /* waiting */
                }
                ARG8[1] = TSIP.REG_100H.WORD;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_04H.BIT.B30)
                {
                    /* waiting */
                }
                ARG8[2] = TSIP.REG_100H.WORD;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_04H.BIT.B30)
                {
                    /* waiting */
                }
                ARG8[3] = TSIP.REG_100H.WORD;
                RX65NHU_func100(change_endian_long(0x01d358fcu), change_endian_long(0x93561318u), change_endian_long(0xbcd1e17fu), change_endian_long(0x5485a180u));
                TSIP.REG_C4H.WORD = 0x00000885u;
                TSIP.REG_00H.WORD = 0x00002313u;
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
                ARG9[0] = TSIP.REG_100H.WORD;
                ARG9[1] = TSIP.REG_100H.WORD;
                ARG9[2] = TSIP.REG_100H.WORD;
                ARG9[3] = TSIP.REG_100H.WORD;
                RX65NHU_func100(change_endian_long(0xc6a07995u), change_endian_long(0x7bd9dd6eu), change_endian_long(0xe95bad9cu), change_endian_long(0x86ff5fddu));
                TSIP.REG_C4H.WORD = 0x00000885u;
                TSIP.REG_00H.WORD = 0x00002313u;
                /* WAIT_LOOP */
                while (0u != TSIP.REG_00H.BIT.B25)
                {
                    /* waiting */
                }
                TSIP.REG_1CH.WORD = 0x00001800u;
                TSIP.REG_00H.WORD = 0x0000031bu;
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
                ARG10[0] = TSIP.REG_100H.WORD;
                ARG10[1] = TSIP.REG_100H.WORD;
                ARG10[2] = TSIP.REG_100H.WORD;
                ARG10[3] = TSIP.REG_100H.WORD;
                RX65NHU_func101(change_endian_long(0x70d48ae4u), change_endian_long(0x88f757e8u), change_endian_long(0x4c9907d1u), change_endian_long(0xa1273e87u));
            }
            else if (0x02000000u == (TSIP.REG_1CH.WORD & 0xff000000u))
            {
                RX65NHU_func100(change_endian_long(0x6049fe38u), change_endian_long(0x2319aa41u), change_endian_long(0x59c1f2f3u), change_endian_long(0x81ebc1e5u));
                TSIP.REG_104H.WORD = 0x00000058u;
                TSIP.REG_E0H.WORD = 0x80010040u;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_104H.BIT.B31)
                {
                    /* waiting */
                }
                TSIP.REG_100H.WORD = change_endian_long(0x02000100u);
                TSIP.REG_ECH.WORD = 0x00001058u;
                RX65NHU_func103();
                TSIP.REG_104H.WORD = 0x00000052u;
                TSIP.REG_C4H.WORD = 0x01000c84u;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_104H.BIT.B31)
                {
                    /* waiting */
                }
                TSIP.REG_100H.WORD = change_endian_long(0x00000000u);
                TSIP.REG_E0H.WORD = 0x80030060u;
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
                RX65NHU_func100(change_endian_long(0x91c15dc8u), change_endian_long(0x12245cf0u), change_endian_long(0x25b4c3f1u), change_endian_long(0xac23e807u));
                TSIP.REG_C4H.WORD = 0x000c2b0du;
                TSIP.REG_E0H.WORD = 0x81040040u;
                TSIP.REG_00H.WORD = 0x00002813u;
                /* WAIT_LOOP */
                while (0u != TSIP.REG_00H.BIT.B25)
                {
                    /* waiting */
                }
                TSIP.REG_1CH.WORD = 0x00001800u;
                TSIP.REG_D0H.WORD = 0x00000100u;
                TSIP.REG_C4H.WORD = 0x02e087bfu;
                TSIP.REG_00H.WORD = 0x00002323u;
                TSIP.REG_2CH.WORD = 0x00000022u;
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
                ARG5[4] = TSIP.REG_100H.WORD;
                ARG5[5] = TSIP.REG_100H.WORD;
                ARG5[6] = TSIP.REG_100H.WORD;
                ARG5[7] = TSIP.REG_100H.WORD;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_04H.BIT.B30)
                {
                    /* waiting */
                }
                ARG5[8] = TSIP.REG_100H.WORD;
                ARG5[9] = TSIP.REG_100H.WORD;
                ARG5[10] = TSIP.REG_100H.WORD;
                ARG5[11] = TSIP.REG_100H.WORD;
                RX65NHU_func100(change_endian_long(0x12c86bfdu), change_endian_long(0x26f350e9u), change_endian_long(0x0ffc7c91u), change_endian_long(0xa43c8567u));
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
                ARG5[12] = TSIP.REG_100H.WORD;
                ARG5[13] = TSIP.REG_100H.WORD;
                ARG5[14] = TSIP.REG_100H.WORD;
                ARG5[15] = TSIP.REG_100H.WORD;
                RX65NHU_func100(change_endian_long(0x292aa6e3u), change_endian_long(0x1a36bd60u), change_endian_long(0x49fef91du), change_endian_long(0x6b55cbbfu));
                TSIP.REG_E0H.WORD = 0x81040040u;
                TSIP.REG_04H.WORD = 0x00000612u;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_04H.BIT.B30)
                {
                    /* waiting */
                }
                ARG5[0] = TSIP.REG_100H.WORD;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_04H.BIT.B30)
                {
                    /* waiting */
                }
                ARG5[1] = TSIP.REG_100H.WORD;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_04H.BIT.B30)
                {
                    /* waiting */
                }
                ARG5[2] = TSIP.REG_100H.WORD;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_04H.BIT.B30)
                {
                    /* waiting */
                }
                ARG5[3] = TSIP.REG_100H.WORD;
                RX65NHU_func100(change_endian_long(0x253d00eau), change_endian_long(0x68016d24u), change_endian_long(0x63ad6bd6u), change_endian_long(0xbaf98700u));
                TSIP.REG_104H.WORD = 0x00000058u;
                TSIP.REG_E0H.WORD = 0x80010040u;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_104H.BIT.B31)
                {
                    /* waiting */
                }
                TSIP.REG_100H.WORD = change_endian_long(0x02000100u);
                TSIP.REG_ECH.WORD = 0x00001059u;
                RX65NHU_func103();
                TSIP.REG_104H.WORD = 0x00000052u;
                TSIP.REG_C4H.WORD = 0x01000c84u;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_104H.BIT.B31)
                {
                    /* waiting */
                }
                TSIP.REG_100H.WORD = change_endian_long(0x00000000u);
                TSIP.REG_E0H.WORD = 0x80030060u;
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
                RX65NHU_func100(change_endian_long(0x20f05dc3u), change_endian_long(0x053a2c65u), change_endian_long(0xd1adc533u), change_endian_long(0x4dab9ac6u));
                TSIP.REG_C4H.WORD = 0x000c2b0du;
                TSIP.REG_E0H.WORD = 0x81040040u;
                TSIP.REG_00H.WORD = 0x00002813u;
                /* WAIT_LOOP */
                while (0u != TSIP.REG_00H.BIT.B25)
                {
                    /* waiting */
                }
                TSIP.REG_1CH.WORD = 0x00001800u;
                TSIP.REG_D0H.WORD = 0x00000100u;
                TSIP.REG_C4H.WORD = 0x02e087bfu;
                TSIP.REG_00H.WORD = 0x00002323u;
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
                ARG6[4] = TSIP.REG_100H.WORD;
                ARG6[5] = TSIP.REG_100H.WORD;
                ARG6[6] = TSIP.REG_100H.WORD;
                ARG6[7] = TSIP.REG_100H.WORD;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_04H.BIT.B30)
                {
                    /* waiting */
                }
                ARG6[8] = TSIP.REG_100H.WORD;
                ARG6[9] = TSIP.REG_100H.WORD;
                ARG6[10] = TSIP.REG_100H.WORD;
                ARG6[11] = TSIP.REG_100H.WORD;
                RX65NHU_func100(change_endian_long(0x30c4bf32u), change_endian_long(0xa171aaa1u), change_endian_long(0x8756fa42u), change_endian_long(0x3c75d8c6u));
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
                ARG6[12] = TSIP.REG_100H.WORD;
                ARG6[13] = TSIP.REG_100H.WORD;
                ARG6[14] = TSIP.REG_100H.WORD;
                ARG6[15] = TSIP.REG_100H.WORD;
                RX65NHU_func100(change_endian_long(0x650b9284u), change_endian_long(0xeea2c2beu), change_endian_long(0xd200dc5au), change_endian_long(0x4365d6c8u));
                TSIP.REG_E0H.WORD = 0x81040040u;
                TSIP.REG_04H.WORD = 0x00000612u;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_04H.BIT.B30)
                {
                    /* waiting */
                }
                ARG6[0] = TSIP.REG_100H.WORD;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_04H.BIT.B30)
                {
                    /* waiting */
                }
                ARG6[1] = TSIP.REG_100H.WORD;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_04H.BIT.B30)
                {
                    /* waiting */
                }
                ARG6[2] = TSIP.REG_100H.WORD;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_04H.BIT.B30)
                {
                    /* waiting */
                }
                ARG6[3] = TSIP.REG_100H.WORD;
                RX65NHU_func100(change_endian_long(0x3e64ec4au), change_endian_long(0x166f3ca4u), change_endian_long(0xabd5fb6du), change_endian_long(0x01df287du));
                TSIP.REG_104H.WORD = 0x00000058u;
                TSIP.REG_E0H.WORD = 0x80010040u;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_104H.BIT.B31)
                {
                    /* waiting */
                }
                TSIP.REG_100H.WORD = change_endian_long(0x00000100u);
                TSIP.REG_ECH.WORD = 0x0000105au;
                RX65NHU_func103();
                TSIP.REG_104H.WORD = 0x00000052u;
                TSIP.REG_C4H.WORD = 0x01000c84u;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_104H.BIT.B31)
                {
                    /* waiting */
                }
                TSIP.REG_100H.WORD = change_endian_long(0x00000000u);
                TSIP.REG_E0H.WORD = 0x80030060u;
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
                RX65NHU_func100(change_endian_long(0xd5823503u), change_endian_long(0x903867dcu), change_endian_long(0x021d77e1u), change_endian_long(0x18be5201u));
                TSIP.REG_C4H.WORD = 0x000c2b0du;
                TSIP.REG_E0H.WORD = 0x81040040u;
                TSIP.REG_00H.WORD = 0x00002813u;
                /* WAIT_LOOP */
                while (0u != TSIP.REG_00H.BIT.B25)
                {
                    /* waiting */
                }
                TSIP.REG_1CH.WORD = 0x00001800u;
                TSIP.REG_C4H.WORD = 0x02e007bdu;
                TSIP.REG_00H.WORD = 0x00002313u;
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
                ARG7[4] = TSIP.REG_100H.WORD;
                ARG7[5] = TSIP.REG_100H.WORD;
                ARG7[6] = TSIP.REG_100H.WORD;
                ARG7[7] = TSIP.REG_100H.WORD;
                RX65NHU_func100(change_endian_long(0xe54066e5u), change_endian_long(0x1cf7fc4au), change_endian_long(0x9b9297c6u), change_endian_long(0x9254ff65u));
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
                ARG7[8] = TSIP.REG_100H.WORD;
                ARG7[9] = TSIP.REG_100H.WORD;
                ARG7[10] = TSIP.REG_100H.WORD;
                ARG7[11] = TSIP.REG_100H.WORD;
                RX65NHU_func100(change_endian_long(0xefb175d6u), change_endian_long(0x6867807du), change_endian_long(0x9996c9eeu), change_endian_long(0x5628e29eu));
                TSIP.REG_E0H.WORD = 0x81040040u;
                TSIP.REG_04H.WORD = 0x00000612u;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_04H.BIT.B30)
                {
                    /* waiting */
                }
                ARG7[0] = TSIP.REG_100H.WORD;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_04H.BIT.B30)
                {
                    /* waiting */
                }
                ARG7[1] = TSIP.REG_100H.WORD;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_04H.BIT.B30)
                {
                    /* waiting */
                }
                ARG7[2] = TSIP.REG_100H.WORD;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_04H.BIT.B30)
                {
                    /* waiting */
                }
                ARG7[3] = TSIP.REG_100H.WORD;
                RX65NHU_func100(change_endian_long(0x5ee171feu), change_endian_long(0x73131f5cu), change_endian_long(0x3502c811u), change_endian_long(0xcb17b100u));
                TSIP.REG_104H.WORD = 0x00000058u;
                TSIP.REG_E0H.WORD = 0x80010040u;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_104H.BIT.B31)
                {
                    /* waiting */
                }
                TSIP.REG_100H.WORD = change_endian_long(0x00000100u);
                TSIP.REG_ECH.WORD = 0x0000105bu;
                RX65NHU_func103();
                TSIP.REG_104H.WORD = 0x00000052u;
                TSIP.REG_C4H.WORD = 0x01000c84u;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_104H.BIT.B31)
                {
                    /* waiting */
                }
                TSIP.REG_100H.WORD = change_endian_long(0x00000000u);
                TSIP.REG_E0H.WORD = 0x80030060u;
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
                RX65NHU_func100(change_endian_long(0x8f18a034u), change_endian_long(0xf54aedd2u), change_endian_long(0x8bed1ac0u), change_endian_long(0xf2c521dbu));
                TSIP.REG_C4H.WORD = 0x000c2b0du;
                TSIP.REG_E0H.WORD = 0x81040040u;
                TSIP.REG_00H.WORD = 0x00002813u;
                /* WAIT_LOOP */
                while (0u != TSIP.REG_00H.BIT.B25)
                {
                    /* waiting */
                }
                TSIP.REG_1CH.WORD = 0x00001800u;
                TSIP.REG_C4H.WORD = 0x02e007bdu;
                TSIP.REG_00H.WORD = 0x00002313u;
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
                ARG8[4] = TSIP.REG_100H.WORD;
                ARG8[5] = TSIP.REG_100H.WORD;
                ARG8[6] = TSIP.REG_100H.WORD;
                ARG8[7] = TSIP.REG_100H.WORD;
                RX65NHU_func100(change_endian_long(0x68427798u), change_endian_long(0xf6b226f1u), change_endian_long(0x27fec622u), change_endian_long(0x84d88658u));
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
                ARG8[8] = TSIP.REG_100H.WORD;
                ARG8[9] = TSIP.REG_100H.WORD;
                ARG8[10] = TSIP.REG_100H.WORD;
                ARG8[11] = TSIP.REG_100H.WORD;
                RX65NHU_func100(change_endian_long(0xa5875a07u), change_endian_long(0x0eb31462u), change_endian_long(0x93a942fbu), change_endian_long(0x83414d79u));
                TSIP.REG_E0H.WORD = 0x81040040u;
                TSIP.REG_04H.WORD = 0x00000612u;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_04H.BIT.B30)
                {
                    /* waiting */
                }
                ARG8[0] = TSIP.REG_100H.WORD;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_04H.BIT.B30)
                {
                    /* waiting */
                }
                ARG8[1] = TSIP.REG_100H.WORD;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_04H.BIT.B30)
                {
                    /* waiting */
                }
                ARG8[2] = TSIP.REG_100H.WORD;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_04H.BIT.B30)
                {
                    /* waiting */
                }
                ARG8[3] = TSIP.REG_100H.WORD;
                RX65NHU_func100(change_endian_long(0x5c921579u), change_endian_long(0x88c4b1c9u), change_endian_long(0x893d196du), change_endian_long(0xed68671cu));
                TSIP.REG_C4H.WORD = 0x00000885u;
                TSIP.REG_00H.WORD = 0x00002313u;
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
                ARG9[0] = TSIP.REG_100H.WORD;
                ARG9[1] = TSIP.REG_100H.WORD;
                ARG9[2] = TSIP.REG_100H.WORD;
                ARG9[3] = TSIP.REG_100H.WORD;
                RX65NHU_func100(change_endian_long(0xbbd06a50u), change_endian_long(0x1b4f111au), change_endian_long(0x1ced06c0u), change_endian_long(0xacf4579fu));
                TSIP.REG_C4H.WORD = 0x00000885u;
                TSIP.REG_00H.WORD = 0x00002313u;
                /* WAIT_LOOP */
                while (0u != TSIP.REG_00H.BIT.B25)
                {
                    /* waiting */
                }
                TSIP.REG_1CH.WORD = 0x00001800u;
                TSIP.REG_00H.WORD = 0x00000323u;
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
                ARG10[0] = TSIP.REG_100H.WORD;
                ARG10[1] = TSIP.REG_100H.WORD;
                ARG10[2] = TSIP.REG_100H.WORD;
                ARG10[3] = TSIP.REG_100H.WORD;
                RX65NHU_func101(change_endian_long(0x050ceebbu), change_endian_long(0x923489bcu), change_endian_long(0x5edf433cu), change_endian_long(0x51be7b43u));
            }
            else if (0x03000000u == (TSIP.REG_1CH.WORD & 0xff000000u))
            {
                RX65NHU_func100(change_endian_long(0xa7d7eeffu), change_endian_long(0x5bf81b54u), change_endian_long(0x4190ac20u), change_endian_long(0xaba6a49fu));
                TSIP.REG_104H.WORD = 0x00000058u;
                TSIP.REG_E0H.WORD = 0x80010040u;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_104H.BIT.B31)
                {
                    /* waiting */
                }
                TSIP.REG_100H.WORD = change_endian_long(0x02000100u);
                TSIP.REG_ECH.WORD = 0x00001058u;
                RX65NHU_func103();
                TSIP.REG_104H.WORD = 0x00000052u;
                TSIP.REG_C4H.WORD = 0x01000c84u;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_104H.BIT.B31)
                {
                    /* waiting */
                }
                TSIP.REG_100H.WORD = change_endian_long(0x00000000u);
                TSIP.REG_E0H.WORD = 0x80030060u;
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
                RX65NHU_func100(change_endian_long(0xf7e7f588u), change_endian_long(0x1350cd2eu), change_endian_long(0x367f2609u), change_endian_long(0x25cfa404u));
                TSIP.REG_C4H.WORD = 0x000c2b0du;
                TSIP.REG_E0H.WORD = 0x81040040u;
                TSIP.REG_00H.WORD = 0x00002813u;
                /* WAIT_LOOP */
                while (0u != TSIP.REG_00H.BIT.B25)
                {
                    /* waiting */
                }
                TSIP.REG_1CH.WORD = 0x00001800u;
                TSIP.REG_D0H.WORD = 0x00000100u;
                TSIP.REG_C4H.WORD = 0x02e087bfu;
                TSIP.REG_00H.WORD = 0x00002323u;
                TSIP.REG_2CH.WORD = 0x00000022u;
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
                ARG5[4] = TSIP.REG_100H.WORD;
                ARG5[5] = TSIP.REG_100H.WORD;
                ARG5[6] = TSIP.REG_100H.WORD;
                ARG5[7] = TSIP.REG_100H.WORD;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_04H.BIT.B30)
                {
                    /* waiting */
                }
                ARG5[8] = TSIP.REG_100H.WORD;
                ARG5[9] = TSIP.REG_100H.WORD;
                ARG5[10] = TSIP.REG_100H.WORD;
                ARG5[11] = TSIP.REG_100H.WORD;
                RX65NHU_func100(change_endian_long(0x3f316ccfu), change_endian_long(0x84391fcau), change_endian_long(0xb71c617fu), change_endian_long(0x92257345u));
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
                ARG5[12] = TSIP.REG_100H.WORD;
                ARG5[13] = TSIP.REG_100H.WORD;
                ARG5[14] = TSIP.REG_100H.WORD;
                ARG5[15] = TSIP.REG_100H.WORD;
                RX65NHU_func100(change_endian_long(0x24a04520u), change_endian_long(0xf85315c7u), change_endian_long(0xb63d513cu), change_endian_long(0x1d791bafu));
                TSIP.REG_E0H.WORD = 0x81040040u;
                TSIP.REG_04H.WORD = 0x00000612u;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_04H.BIT.B30)
                {
                    /* waiting */
                }
                ARG5[0] = TSIP.REG_100H.WORD;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_04H.BIT.B30)
                {
                    /* waiting */
                }
                ARG5[1] = TSIP.REG_100H.WORD;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_04H.BIT.B30)
                {
                    /* waiting */
                }
                ARG5[2] = TSIP.REG_100H.WORD;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_04H.BIT.B30)
                {
                    /* waiting */
                }
                ARG5[3] = TSIP.REG_100H.WORD;
                RX65NHU_func100(change_endian_long(0x6f019d4cu), change_endian_long(0xfbabf93bu), change_endian_long(0x725b402bu), change_endian_long(0x069e7538u));
                TSIP.REG_104H.WORD = 0x00000058u;
                TSIP.REG_E0H.WORD = 0x80010040u;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_104H.BIT.B31)
                {
                    /* waiting */
                }
                TSIP.REG_100H.WORD = change_endian_long(0x02000100u);
                TSIP.REG_ECH.WORD = 0x00001059u;
                RX65NHU_func103();
                TSIP.REG_104H.WORD = 0x00000052u;
                TSIP.REG_C4H.WORD = 0x01000c84u;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_104H.BIT.B31)
                {
                    /* waiting */
                }
                TSIP.REG_100H.WORD = change_endian_long(0x00000000u);
                TSIP.REG_E0H.WORD = 0x80030060u;
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
                RX65NHU_func100(change_endian_long(0xebe7fd67u), change_endian_long(0xb882c781u), change_endian_long(0xac10231au), change_endian_long(0xb605184bu));
                TSIP.REG_C4H.WORD = 0x000c2b0du;
                TSIP.REG_E0H.WORD = 0x81040040u;
                TSIP.REG_00H.WORD = 0x00002813u;
                /* WAIT_LOOP */
                while (0u != TSIP.REG_00H.BIT.B25)
                {
                    /* waiting */
                }
                TSIP.REG_1CH.WORD = 0x00001800u;
                TSIP.REG_D0H.WORD = 0x00000100u;
                TSIP.REG_C4H.WORD = 0x02e087bfu;
                TSIP.REG_00H.WORD = 0x00002323u;
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
                ARG6[4] = TSIP.REG_100H.WORD;
                ARG6[5] = TSIP.REG_100H.WORD;
                ARG6[6] = TSIP.REG_100H.WORD;
                ARG6[7] = TSIP.REG_100H.WORD;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_04H.BIT.B30)
                {
                    /* waiting */
                }
                ARG6[8] = TSIP.REG_100H.WORD;
                ARG6[9] = TSIP.REG_100H.WORD;
                ARG6[10] = TSIP.REG_100H.WORD;
                ARG6[11] = TSIP.REG_100H.WORD;
                RX65NHU_func100(change_endian_long(0xd84a1ca3u), change_endian_long(0x69ef039cu), change_endian_long(0xba0e8d4eu), change_endian_long(0xa315ff00u));
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
                ARG6[12] = TSIP.REG_100H.WORD;
                ARG6[13] = TSIP.REG_100H.WORD;
                ARG6[14] = TSIP.REG_100H.WORD;
                ARG6[15] = TSIP.REG_100H.WORD;
                RX65NHU_func100(change_endian_long(0x81d70f07u), change_endian_long(0x43af419eu), change_endian_long(0x37c2ba48u), change_endian_long(0xc38c8264u));
                TSIP.REG_E0H.WORD = 0x81040040u;
                TSIP.REG_04H.WORD = 0x00000612u;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_04H.BIT.B30)
                {
                    /* waiting */
                }
                ARG6[0] = TSIP.REG_100H.WORD;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_04H.BIT.B30)
                {
                    /* waiting */
                }
                ARG6[1] = TSIP.REG_100H.WORD;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_04H.BIT.B30)
                {
                    /* waiting */
                }
                ARG6[2] = TSIP.REG_100H.WORD;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_04H.BIT.B30)
                {
                    /* waiting */
                }
                ARG6[3] = TSIP.REG_100H.WORD;
                RX65NHU_func100(change_endian_long(0x0b1555b3u), change_endian_long(0x19181d0eu), change_endian_long(0xecd4b3d5u), change_endian_long(0x8331f70du));
                TSIP.REG_104H.WORD = 0x00000058u;
                TSIP.REG_E0H.WORD = 0x80010040u;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_104H.BIT.B31)
                {
                    /* waiting */
                }
                TSIP.REG_100H.WORD = change_endian_long(0x01000100u);
                TSIP.REG_ECH.WORD = 0x0000105au;
                RX65NHU_func103();
                TSIP.REG_104H.WORD = 0x00000052u;
                TSIP.REG_C4H.WORD = 0x01000c84u;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_104H.BIT.B31)
                {
                    /* waiting */
                }
                TSIP.REG_100H.WORD = change_endian_long(0x00000000u);
                TSIP.REG_E0H.WORD = 0x80030060u;
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
                RX65NHU_func100(change_endian_long(0x55a9efe7u), change_endian_long(0x03690f23u), change_endian_long(0xb859b557u), change_endian_long(0x266673eau));
                TSIP.REG_C4H.WORD = 0x000c2b0du;
                TSIP.REG_E0H.WORD = 0x81040040u;
                TSIP.REG_00H.WORD = 0x00002813u;
                /* WAIT_LOOP */
                while (0u != TSIP.REG_00H.BIT.B25)
                {
                    /* waiting */
                }
                TSIP.REG_1CH.WORD = 0x00001800u;
                TSIP.REG_D0H.WORD = 0x00000100u;
                TSIP.REG_C4H.WORD = 0x02e087bfu;
                TSIP.REG_00H.WORD = 0x00002323u;
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
                ARG7[4] = TSIP.REG_100H.WORD;
                ARG7[5] = TSIP.REG_100H.WORD;
                ARG7[6] = TSIP.REG_100H.WORD;
                ARG7[7] = TSIP.REG_100H.WORD;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_04H.BIT.B30)
                {
                    /* waiting */
                }
                ARG7[8] = TSIP.REG_100H.WORD;
                ARG7[9] = TSIP.REG_100H.WORD;
                ARG7[10] = TSIP.REG_100H.WORD;
                ARG7[11] = TSIP.REG_100H.WORD;
                RX65NHU_func100(change_endian_long(0x51e5413fu), change_endian_long(0x40050746u), change_endian_long(0x1d26e613u), change_endian_long(0xe15ad8f0u));
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
                ARG7[12] = TSIP.REG_100H.WORD;
                ARG7[13] = TSIP.REG_100H.WORD;
                ARG7[14] = TSIP.REG_100H.WORD;
                ARG7[15] = TSIP.REG_100H.WORD;
                RX65NHU_func100(change_endian_long(0x07bb129fu), change_endian_long(0x84a18a28u), change_endian_long(0x21610b88u), change_endian_long(0xe9eb4cc0u));
                TSIP.REG_E0H.WORD = 0x81040040u;
                TSIP.REG_04H.WORD = 0x00000612u;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_04H.BIT.B30)
                {
                    /* waiting */
                }
                ARG7[0] = TSIP.REG_100H.WORD;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_04H.BIT.B30)
                {
                    /* waiting */
                }
                ARG7[1] = TSIP.REG_100H.WORD;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_04H.BIT.B30)
                {
                    /* waiting */
                }
                ARG7[2] = TSIP.REG_100H.WORD;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_04H.BIT.B30)
                {
                    /* waiting */
                }
                ARG7[3] = TSIP.REG_100H.WORD;
                RX65NHU_func100(change_endian_long(0x4dce84e7u), change_endian_long(0xa0dbcad2u), change_endian_long(0x540ca6ecu), change_endian_long(0x6eb553ffu));
                TSIP.REG_104H.WORD = 0x00000058u;
                TSIP.REG_E0H.WORD = 0x80010040u;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_104H.BIT.B31)
                {
                    /* waiting */
                }
                TSIP.REG_100H.WORD = change_endian_long(0x01000100u);
                TSIP.REG_ECH.WORD = 0x0000105bu;
                RX65NHU_func103();
                TSIP.REG_104H.WORD = 0x00000052u;
                TSIP.REG_C4H.WORD = 0x01000c84u;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_104H.BIT.B31)
                {
                    /* waiting */
                }
                TSIP.REG_100H.WORD = change_endian_long(0x00000000u);
                TSIP.REG_E0H.WORD = 0x80030060u;
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
                RX65NHU_func100(change_endian_long(0x28a10228u), change_endian_long(0x878d2863u), change_endian_long(0xcd3a6617u), change_endian_long(0x5eb6e017u));
                TSIP.REG_C4H.WORD = 0x000c2b0du;
                TSIP.REG_E0H.WORD = 0x81040040u;
                TSIP.REG_00H.WORD = 0x00002813u;
                /* WAIT_LOOP */
                while (0u != TSIP.REG_00H.BIT.B25)
                {
                    /* waiting */
                }
                TSIP.REG_1CH.WORD = 0x00001800u;
                TSIP.REG_D0H.WORD = 0x00000100u;
                TSIP.REG_C4H.WORD = 0x02e087bfu;
                TSIP.REG_00H.WORD = 0x00002323u;
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
                ARG8[4] = TSIP.REG_100H.WORD;
                ARG8[5] = TSIP.REG_100H.WORD;
                ARG8[6] = TSIP.REG_100H.WORD;
                ARG8[7] = TSIP.REG_100H.WORD;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_04H.BIT.B30)
                {
                    /* waiting */
                }
                ARG8[8] = TSIP.REG_100H.WORD;
                ARG8[9] = TSIP.REG_100H.WORD;
                ARG8[10] = TSIP.REG_100H.WORD;
                ARG8[11] = TSIP.REG_100H.WORD;
                RX65NHU_func100(change_endian_long(0x494c07e0u), change_endian_long(0x2956db20u), change_endian_long(0xca1fed96u), change_endian_long(0xc8f5034fu));
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
                ARG8[12] = TSIP.REG_100H.WORD;
                ARG8[13] = TSIP.REG_100H.WORD;
                ARG8[14] = TSIP.REG_100H.WORD;
                ARG8[15] = TSIP.REG_100H.WORD;
                RX65NHU_func100(change_endian_long(0x0ddd9544u), change_endian_long(0x0a9a4cacu), change_endian_long(0x3eb1248cu), change_endian_long(0xaac9fa30u));
                TSIP.REG_E0H.WORD = 0x81040040u;
                TSIP.REG_04H.WORD = 0x00000612u;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_04H.BIT.B30)
                {
                    /* waiting */
                }
                ARG8[0] = TSIP.REG_100H.WORD;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_04H.BIT.B30)
                {
                    /* waiting */
                }
                ARG8[1] = TSIP.REG_100H.WORD;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_04H.BIT.B30)
                {
                    /* waiting */
                }
                ARG8[2] = TSIP.REG_100H.WORD;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_04H.BIT.B30)
                {
                    /* waiting */
                }
                ARG8[3] = TSIP.REG_100H.WORD;
                RX65NHU_func100(change_endian_long(0x204c8054u), change_endian_long(0x445a41b7u), change_endian_long(0x4571cda2u), change_endian_long(0x75765256u));
                TSIP.REG_C4H.WORD = 0x00000885u;
                TSIP.REG_00H.WORD = 0x00002313u;
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
                ARG9[0] = TSIP.REG_100H.WORD;
                ARG9[1] = TSIP.REG_100H.WORD;
                ARG9[2] = TSIP.REG_100H.WORD;
                ARG9[3] = TSIP.REG_100H.WORD;
                RX65NHU_func100(change_endian_long(0x34d286f2u), change_endian_long(0x6741329eu), change_endian_long(0xb607485bu), change_endian_long(0x6f1e6296u));
                TSIP.REG_C4H.WORD = 0x00000885u;
                TSIP.REG_00H.WORD = 0x00002313u;
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
                ARG10[0] = TSIP.REG_100H.WORD;
                ARG10[1] = TSIP.REG_100H.WORD;
                ARG10[2] = TSIP.REG_100H.WORD;
                ARG10[3] = TSIP.REG_100H.WORD;
                RX65NHU_func101(change_endian_long(0x75b093f3u), change_endian_long(0x4a50112fu), change_endian_long(0x18e94a93u), change_endian_long(0xcf9e257bu));
            }
            else if (0x06000000u == (TSIP.REG_1CH.WORD & 0xff000000u))
            {
                RX65NHU_func100(change_endian_long(0xbaae52ffu), change_endian_long(0xa5bf6433u), change_endian_long(0xfaeeee15u), change_endian_long(0x8a2f2da0u));
                TSIP.REG_A4H.WORD = 0x00040805u;
                TSIP.REG_00H.WORD = 0x00001313u;
                TSIP.REG_2CH.WORD = 0x00000022u;
                /* WAIT_LOOP */
                while (0u != TSIP.REG_00H.BIT.B25)
                {
                    /* waiting */
                }
                TSIP.REG_1CH.WORD = 0x00001800u;
                TSIP.REG_A4H.WORD = 0x00050805u;
                TSIP.REG_00H.WORD = 0x00001313u;
                /* WAIT_LOOP */
                while (0u != TSIP.REG_00H.BIT.B25)
                {
                    /* waiting */
                }
                TSIP.REG_1CH.WORD = 0x00001800u;
                TSIP.REG_E0H.WORD = 0x80010280u;
                TSIP.REG_00H.WORD = 0x00008307u;
                /* WAIT_LOOP */
                while (0u != TSIP.REG_00H.BIT.B25)
                {
                    /* waiting */
                }
                TSIP.REG_1CH.WORD = 0x00001800u;
                TSIP.REG_E0H.WORD = 0x800202a0u;
                TSIP.REG_104H.WORD = 0x00000168u;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_104H.BIT.B31)
                {
                    /* waiting */
                }
                TSIP.REG_100H.WORD = ARG4[0];
                /* WAIT_LOOP */
                while (1u != TSIP.REG_104H.BIT.B31)
                {
                    /* waiting */
                }
                TSIP.REG_100H.WORD = ARG4[1];
                TSIP.REG_ECH.WORD = 0x0000b6e0u;
                TSIP.REG_ECH.WORD = 0x00000001u;
                TSIP.REG_E0H.WORD = 0x81040280u;
                TSIP.REG_A4H.WORD = 0x00060805u;
                TSIP.REG_00H.WORD = 0x00001813u;
                /* WAIT_LOOP */
                while (0u != TSIP.REG_00H.BIT.B25)
                {
                    /* waiting */
                }
                TSIP.REG_1CH.WORD = 0x00001800u;
                TSIP.REG_E0H.WORD = 0x80010280u;
                TSIP.REG_00H.WORD = 0x00008307u;
                /* WAIT_LOOP */
                while (0u != TSIP.REG_00H.BIT.B25)
                {
                    /* waiting */
                }
                TSIP.REG_1CH.WORD = 0x00001800u;
                TSIP.REG_00H.WORD = 0x0000037bu;
                /* WAIT_LOOP */
                while (0u != TSIP.REG_00H.BIT.B25)
                {
                    /* waiting */
                }
                TSIP.REG_1CH.WORD = 0x00001800u;
                TSIP.REG_E0H.WORD = 0x81040280u;
                TSIP.REG_A4H.WORD = 0x00070805u;
                TSIP.REG_00H.WORD = 0x00001813u;
                /* WAIT_LOOP */
                while (0u != TSIP.REG_00H.BIT.B25)
                {
                    /* waiting */
                }
                TSIP.REG_1CH.WORD = 0x00001800u;
                TSIP.REG_104H.WORD = 0x00000058u;
                TSIP.REG_E0H.WORD = 0x80010040u;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_104H.BIT.B31)
                {
                    /* waiting */
                }
                TSIP.REG_100H.WORD = change_endian_long(0x12000100u);
                TSIP.REG_ECH.WORD = 0x0000105au;
                RX65NHU_func103();
                TSIP.REG_104H.WORD = 0x00000052u;
                TSIP.REG_C4H.WORD = 0x01000c84u;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_104H.BIT.B31)
                {
                    /* waiting */
                }
                TSIP.REG_100H.WORD = change_endian_long(0x00000000u);
                TSIP.REG_E0H.WORD = 0x80030060u;
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
                RX65NHU_func100(change_endian_long(0x934cc1dfu), change_endian_long(0xfc0587eeu), change_endian_long(0x00d3784au), change_endian_long(0xbee96f06u));
                TSIP.REG_C4H.WORD = 0x000c2b0du;
                TSIP.REG_E0H.WORD = 0x81040040u;
                TSIP.REG_00H.WORD = 0x00002813u;
                /* WAIT_LOOP */
                while (0u != TSIP.REG_00H.BIT.B25)
                {
                    /* waiting */
                }
                TSIP.REG_1CH.WORD = 0x00001800u;
                TSIP.REG_D0H.WORD = 0x00000100u;
                TSIP.REG_C4H.WORD = 0x02e087bfu;
                TSIP.REG_104H.WORD = 0x00000151u;
                TSIP.REG_A4H.WORD = 0x00000c84u;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_104H.BIT.B31)
                {
                    /* waiting */
                }
                TSIP.REG_100H.WORD = change_endian_long(0x00000000u);
                TSIP.REG_00H.WORD = 0x00002113u;
                /* WAIT_LOOP */
                while (0u != TSIP.REG_00H.BIT.B25)
                {
                    /* waiting */
                }
                TSIP.REG_1CH.WORD = 0x00001800u;
                TSIP.REG_A4H.WORD = 0x01000c84u;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_104H.BIT.B31)
                {
                    /* waiting */
                }
                TSIP.REG_100H.WORD = change_endian_long(0x00000000u);
                TSIP.REG_00H.WORD = 0x00002113u;
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
                ARG7[4] = TSIP.REG_100H.WORD;
                ARG7[5] = TSIP.REG_100H.WORD;
                ARG7[6] = TSIP.REG_100H.WORD;
                ARG7[7] = TSIP.REG_100H.WORD;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_04H.BIT.B30)
                {
                    /* waiting */
                }
                ARG7[8] = TSIP.REG_100H.WORD;
                ARG7[9] = TSIP.REG_100H.WORD;
                ARG7[10] = TSIP.REG_100H.WORD;
                ARG7[11] = TSIP.REG_100H.WORD;
                RX65NHU_func100(change_endian_long(0xeadf3028u), change_endian_long(0x2db749f1u), change_endian_long(0xcdb64e48u), change_endian_long(0xc97fcd9fu));
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
                ARG7[12] = TSIP.REG_100H.WORD;
                ARG7[13] = TSIP.REG_100H.WORD;
                ARG7[14] = TSIP.REG_100H.WORD;
                ARG7[15] = TSIP.REG_100H.WORD;
                RX65NHU_func100(change_endian_long(0xc1cfbe83u), change_endian_long(0x13e46450u), change_endian_long(0x9ee2b52fu), change_endian_long(0x4c77e560u));
                TSIP.REG_E0H.WORD = 0x81040040u;
                TSIP.REG_04H.WORD = 0x00000612u;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_04H.BIT.B30)
                {
                    /* waiting */
                }
                ARG7[0] = TSIP.REG_100H.WORD;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_04H.BIT.B30)
                {
                    /* waiting */
                }
                ARG7[1] = TSIP.REG_100H.WORD;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_04H.BIT.B30)
                {
                    /* waiting */
                }
                ARG7[2] = TSIP.REG_100H.WORD;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_04H.BIT.B30)
                {
                    /* waiting */
                }
                ARG7[3] = TSIP.REG_100H.WORD;
                RX65NHU_func100(change_endian_long(0x49871b67u), change_endian_long(0xafc744d3u), change_endian_long(0x5ef9ad35u), change_endian_long(0x37cd9396u));
                TSIP.REG_104H.WORD = 0x00000058u;
                TSIP.REG_E0H.WORD = 0x80010040u;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_104H.BIT.B31)
                {
                    /* waiting */
                }
                TSIP.REG_100H.WORD = change_endian_long(0x12000100u);
                TSIP.REG_ECH.WORD = 0x0000105bu;
                RX65NHU_func103();
                TSIP.REG_104H.WORD = 0x00000052u;
                TSIP.REG_C4H.WORD = 0x01000c84u;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_104H.BIT.B31)
                {
                    /* waiting */
                }
                TSIP.REG_100H.WORD = change_endian_long(0x00000000u);
                TSIP.REG_E0H.WORD = 0x80030060u;
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
                RX65NHU_func100(change_endian_long(0xad800601u), change_endian_long(0x8e6a14d7u), change_endian_long(0x38154317u), change_endian_long(0x2f685751u));
                TSIP.REG_C4H.WORD = 0x000c2b0du;
                TSIP.REG_E0H.WORD = 0x81040040u;
                TSIP.REG_00H.WORD = 0x00002813u;
                /* WAIT_LOOP */
                while (0u != TSIP.REG_00H.BIT.B25)
                {
                    /* waiting */
                }
                TSIP.REG_1CH.WORD = 0x00001800u;
                TSIP.REG_D0H.WORD = 0x00000100u;
                TSIP.REG_C4H.WORD = 0x02e087bfu;
                TSIP.REG_104H.WORD = 0x00000151u;
                TSIP.REG_A4H.WORD = 0x00000cc4u;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_104H.BIT.B31)
                {
                    /* waiting */
                }
                TSIP.REG_100H.WORD = change_endian_long(0x00000000u);
                TSIP.REG_00H.WORD = 0x00002113u;
                /* WAIT_LOOP */
                while (0u != TSIP.REG_00H.BIT.B25)
                {
                    /* waiting */
                }
                TSIP.REG_1CH.WORD = 0x00001800u;
                TSIP.REG_A4H.WORD = 0x01000cc4u;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_104H.BIT.B31)
                {
                    /* waiting */
                }
                TSIP.REG_100H.WORD = change_endian_long(0x00000000u);
                TSIP.REG_00H.WORD = 0x00002113u;
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
                ARG8[4] = TSIP.REG_100H.WORD;
                ARG8[5] = TSIP.REG_100H.WORD;
                ARG8[6] = TSIP.REG_100H.WORD;
                ARG8[7] = TSIP.REG_100H.WORD;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_04H.BIT.B30)
                {
                    /* waiting */
                }
                ARG8[8] = TSIP.REG_100H.WORD;
                ARG8[9] = TSIP.REG_100H.WORD;
                ARG8[10] = TSIP.REG_100H.WORD;
                ARG8[11] = TSIP.REG_100H.WORD;
                RX65NHU_func100(change_endian_long(0x2fb903ddu), change_endian_long(0x88381088u), change_endian_long(0x9efe53b4u), change_endian_long(0x0825464cu));
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
                ARG8[12] = TSIP.REG_100H.WORD;
                ARG8[13] = TSIP.REG_100H.WORD;
                ARG8[14] = TSIP.REG_100H.WORD;
                ARG8[15] = TSIP.REG_100H.WORD;
                RX65NHU_func100(change_endian_long(0x69749b7fu), change_endian_long(0x0728d499u), change_endian_long(0xc0235646u), change_endian_long(0x3d93cc26u));
                TSIP.REG_E0H.WORD = 0x81040040u;
                TSIP.REG_04H.WORD = 0x00000612u;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_04H.BIT.B30)
                {
                    /* waiting */
                }
                ARG8[0] = TSIP.REG_100H.WORD;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_04H.BIT.B30)
                {
                    /* waiting */
                }
                ARG8[1] = TSIP.REG_100H.WORD;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_04H.BIT.B30)
                {
                    /* waiting */
                }
                ARG8[2] = TSIP.REG_100H.WORD;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_04H.BIT.B30)
                {
                    /* waiting */
                }
                ARG8[3] = TSIP.REG_100H.WORD;
                RX65NHU_func101(change_endian_long(0x62a02633u), change_endian_long(0x31bf5c4bu), change_endian_long(0xcaf2c1e1u), change_endian_long(0xbc4d725au));
            }
            TSIP.REG_ECH.WORD = 0x00000800u;
            RX65NHU_func101(change_endian_long(0x0ecfaca5u), change_endian_long(0x68200782u), change_endian_long(0x1ab4fba5u), change_endian_long(0xfb6c3ae0u));
        }
    }
    TSIP.REG_ECH.WORD = 0x00007c1du;
    TSIP.REG_1CH.WORD = 0x00602000u;
}
/**********************************************************************************************************************
 End of function ./input_dir/RX65NHU/RX65NHU_func404.prc
 *********************************************************************************************************************/
