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
* Function Name: R_TSIP_TlsSVDecryptPreMasterSecretSub
*******************************************************************************************************************/ /**
* @details       RX65NHU RSA2048 Decryption with Public Key for Server
* @param[in]     InData_Keyindex
* @param[in]     InData_PreMasterSecret
* @param[out]    OutData_PreMasterSecret
* @retval        TSIP_SUCCESS
* @retval        TSIP_ERR_RESOURCE_CONFLICT
* @retval        TSIP_ERR_KEY_SET
* @note          None
*/
e_tsip_err_t R_TSIP_TlsSVDecryptPreMasterSecretSub(uint32_t *InData_KeyIndex, uint32_t *InData_PreMasterSecret, uint32_t *OutData_PreMasterSecret)
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
    TSIP.REG_84H.WORD = 0x0000c301u;
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
    TSIP.REG_A4H.WORD = 0x200c3b0du;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = InData_KeyIndex[0];
    TSIP.REG_100H.WORD = InData_KeyIndex[1];
    TSIP.REG_100H.WORD = InData_KeyIndex[2];
    TSIP.REG_100H.WORD = InData_KeyIndex[3];
    TSIP.REG_A4H.WORD = 0x000c0b0cu;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0x3770f901u);
    TSIP.REG_28H.WORD = 0x00bf0001u;
    TSIP.REG_00H.WORD = 0x00013103u;
    TSIP.REG_2CH.WORD = 0x00000014u;
    TSIP.REG_104H.WORD = 0x0000bf61u;
    TSIP.REG_B0H.WORD = 0x00000f00u;
    TSIP.REG_A4H.WORD = 0x02f087bfu;
    for(iLoop=4; iLoop<68; iLoop=iLoop+4)
    {
        /* WAIT_LOOP */
        while (1u != TSIP.REG_104H.BIT.B31)
        {
            /* waiting */
        }
        TSIP.REG_100H.WORD = InData_KeyIndex[iLoop + 0];
        TSIP.REG_100H.WORD = InData_KeyIndex[iLoop + 1];
        TSIP.REG_100H.WORD = InData_KeyIndex[iLoop + 2];
        TSIP.REG_100H.WORD = InData_KeyIndex[iLoop + 3];
    }
    /* WAIT_LOOP */
    while (0u != TSIP.REG_00H.BIT.B25)
    {
        /* waiting */
    }
    TSIP.REG_1CH.WORD = 0x00001800u;
    TSIP.REG_ECH.WORD = 0x00000821u;
    TSIP.REG_E0H.WORD = 0x80c00001u;
    TSIP.REG_B0H.WORD = 0x00000f00u;
    TSIP.REG_A4H.WORD = 0x00f087bfu;
    TSIP.REG_00H.WORD = 0x00018103u;
    for(iLoop=68; iLoop<132; iLoop=iLoop+4)
    {
        /* WAIT_LOOP */
        while (1u != TSIP.REG_104H.BIT.B31)
        {
            /* waiting */
        }
        TSIP.REG_100H.WORD = InData_KeyIndex[iLoop + 0];
        TSIP.REG_100H.WORD = InData_KeyIndex[iLoop + 1];
        TSIP.REG_100H.WORD = InData_KeyIndex[iLoop + 2];
        TSIP.REG_100H.WORD = InData_KeyIndex[iLoop + 3];
    }
    /* WAIT_LOOP */
    while (0u != TSIP.REG_00H.BIT.B25)
    {
        /* waiting */
    }
    TSIP.REG_1CH.WORD = 0x00001800u;
    TSIP.REG_00H.WORD = 0x00013103u;
    TSIP.REG_2CH.WORD = 0x00000012u;
    TSIP.REG_B0H.WORD = 0x00000f00u;
    TSIP.REG_A4H.WORD = 0x00f087bfu;
    for(iLoop=132; iLoop<196; iLoop=iLoop+4)
    {
        /* WAIT_LOOP */
        while (1u != TSIP.REG_104H.BIT.B31)
        {
            /* waiting */
        }
        TSIP.REG_100H.WORD = InData_KeyIndex[iLoop + 0];
        TSIP.REG_100H.WORD = InData_KeyIndex[iLoop + 1];
        TSIP.REG_100H.WORD = InData_KeyIndex[iLoop + 2];
        TSIP.REG_100H.WORD = InData_KeyIndex[iLoop + 3];
    }
    /* WAIT_LOOP */
    while (0u != TSIP.REG_00H.BIT.B25)
    {
        /* waiting */
    }
    TSIP.REG_1CH.WORD = 0x00001800u;
    TSIP.REG_104H.WORD = 0x00000361u;
    TSIP.REG_A4H.WORD = 0x000007bdu;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = InData_KeyIndex[196];
    TSIP.REG_100H.WORD = InData_KeyIndex[197];
    TSIP.REG_100H.WORD = InData_KeyIndex[198];
    TSIP.REG_100H.WORD = InData_KeyIndex[199];
    TSIP.REG_A4H.WORD = 0x00900c45u;
    TSIP.REG_00H.WORD = 0x00001113u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_00H.BIT.B25)
    {
        /* waiting */
    }
    TSIP.REG_1CH.WORD = 0x00001800u;
    RX65NHU_func100(change_endian_long(0x99e3b881u), change_endian_long(0x937e81cdu), change_endian_long(0xba0c8156u), change_endian_long(0xe0ab0c96u));
    TSIP.REG_1CH.WORD = 0x00400000u;
    TSIP.REG_1D0H.WORD = 0x00000000u;
    if (1u == (TSIP.REG_1CH.BIT.B22))
    {
        RX65NHU_func102(change_endian_long(0x2b954b26u), change_endian_long(0x2896971bu), change_endian_long(0x11aa1badu), change_endian_long(0x038f71c1u));
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
        RX65NHU_func100(change_endian_long(0xef45dd87u), change_endian_long(0x051e7200u), change_endian_long(0xf0f18058u), change_endian_long(0x238f2375u));
        TSIP.REG_2CH.WORD = 0x00000010u;
        TSIP.REG_104H.WORD = 0x00003f67u;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_104H.BIT.B31)
        {
            /* waiting */
        }
        TSIP.REG_100H.WORD = InData_PreMasterSecret[0];
        TSIP.REG_100H.WORD = InData_PreMasterSecret[1];
        TSIP.REG_100H.WORD = InData_PreMasterSecret[2];
        TSIP.REG_100H.WORD = InData_PreMasterSecret[3];
        TSIP.REG_100H.WORD = InData_PreMasterSecret[4];
        TSIP.REG_100H.WORD = InData_PreMasterSecret[5];
        TSIP.REG_100H.WORD = InData_PreMasterSecret[6];
        TSIP.REG_100H.WORD = InData_PreMasterSecret[7];
        TSIP.REG_100H.WORD = InData_PreMasterSecret[8];
        TSIP.REG_100H.WORD = InData_PreMasterSecret[9];
        TSIP.REG_100H.WORD = InData_PreMasterSecret[10];
        TSIP.REG_100H.WORD = InData_PreMasterSecret[11];
        TSIP.REG_100H.WORD = InData_PreMasterSecret[12];
        TSIP.REG_100H.WORD = InData_PreMasterSecret[13];
        TSIP.REG_100H.WORD = InData_PreMasterSecret[14];
        TSIP.REG_100H.WORD = InData_PreMasterSecret[15];
        TSIP.REG_100H.WORD = InData_PreMasterSecret[16];
        TSIP.REG_100H.WORD = InData_PreMasterSecret[17];
        TSIP.REG_100H.WORD = InData_PreMasterSecret[18];
        TSIP.REG_100H.WORD = InData_PreMasterSecret[19];
        TSIP.REG_100H.WORD = InData_PreMasterSecret[20];
        TSIP.REG_100H.WORD = InData_PreMasterSecret[21];
        TSIP.REG_100H.WORD = InData_PreMasterSecret[22];
        TSIP.REG_100H.WORD = InData_PreMasterSecret[23];
        TSIP.REG_100H.WORD = InData_PreMasterSecret[24];
        TSIP.REG_100H.WORD = InData_PreMasterSecret[25];
        TSIP.REG_100H.WORD = InData_PreMasterSecret[26];
        TSIP.REG_100H.WORD = InData_PreMasterSecret[27];
        TSIP.REG_100H.WORD = InData_PreMasterSecret[28];
        TSIP.REG_100H.WORD = InData_PreMasterSecret[29];
        TSIP.REG_100H.WORD = InData_PreMasterSecret[30];
        TSIP.REG_100H.WORD = InData_PreMasterSecret[31];
        TSIP.REG_100H.WORD = InData_PreMasterSecret[32];
        TSIP.REG_100H.WORD = InData_PreMasterSecret[33];
        TSIP.REG_100H.WORD = InData_PreMasterSecret[34];
        TSIP.REG_100H.WORD = InData_PreMasterSecret[35];
        TSIP.REG_100H.WORD = InData_PreMasterSecret[36];
        TSIP.REG_100H.WORD = InData_PreMasterSecret[37];
        TSIP.REG_100H.WORD = InData_PreMasterSecret[38];
        TSIP.REG_100H.WORD = InData_PreMasterSecret[39];
        TSIP.REG_100H.WORD = InData_PreMasterSecret[40];
        TSIP.REG_100H.WORD = InData_PreMasterSecret[41];
        TSIP.REG_100H.WORD = InData_PreMasterSecret[42];
        TSIP.REG_100H.WORD = InData_PreMasterSecret[43];
        TSIP.REG_100H.WORD = InData_PreMasterSecret[44];
        TSIP.REG_100H.WORD = InData_PreMasterSecret[45];
        TSIP.REG_100H.WORD = InData_PreMasterSecret[46];
        TSIP.REG_100H.WORD = InData_PreMasterSecret[47];
        TSIP.REG_100H.WORD = InData_PreMasterSecret[48];
        TSIP.REG_100H.WORD = InData_PreMasterSecret[49];
        TSIP.REG_100H.WORD = InData_PreMasterSecret[50];
        TSIP.REG_100H.WORD = InData_PreMasterSecret[51];
        TSIP.REG_100H.WORD = InData_PreMasterSecret[52];
        TSIP.REG_100H.WORD = InData_PreMasterSecret[53];
        TSIP.REG_100H.WORD = InData_PreMasterSecret[54];
        TSIP.REG_100H.WORD = InData_PreMasterSecret[55];
        TSIP.REG_100H.WORD = InData_PreMasterSecret[56];
        TSIP.REG_100H.WORD = InData_PreMasterSecret[57];
        TSIP.REG_100H.WORD = InData_PreMasterSecret[58];
        TSIP.REG_100H.WORD = InData_PreMasterSecret[59];
        TSIP.REG_100H.WORD = InData_PreMasterSecret[60];
        TSIP.REG_100H.WORD = InData_PreMasterSecret[61];
        TSIP.REG_100H.WORD = InData_PreMasterSecret[62];
        TSIP.REG_100H.WORD = InData_PreMasterSecret[63];
        TSIP.REG_E0H.WORD = 0x00000100u;
        TSIP.REG_ECH.WORD = 0x0000b7e0u;
        TSIP.REG_ECH.WORD = 0x000000fcu;
        TSIP.REG_18H.WORD = 0x00000004u;
        TSIP.REG_38H.WORD = 0x00000248u;
        TSIP.REG_E0H.WORD = 0x81c0001fu;
        TSIP.REG_00H.WORD = 0x00013803u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_18H.BIT.B10)
        {
            /* waiting */
        }
        TSIP.REG_18H.WORD = 0x00000000u;
        TSIP.REG_E0H.WORD = 0x00000000u;
        TSIP.REG_28H.WORD = 0x008b0001u;
        RX65NHU_func100(change_endian_long(0x2026018bu), change_endian_long(0x1b7d36e3u), change_endian_long(0xa9962f1au), change_endian_long(0xcb43bdb9u));
        TSIP.REG_ECH.WORD = 0x00000800u;
        TSIP.REG_E0H.WORD = 0x808c0000u;
        TSIP.REG_00H.WORD = 0x00008333u;
        TSIP.REG_2CH.WORD = 0x00000020u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_00H.BIT.B25)
        {
            /* waiting */
        }
        TSIP.REG_1CH.WORD = 0x00001800u;
        RX65NHU_func103();
        RX65NHU_func100(change_endian_long(0x1bcc8a52u), change_endian_long(0xfab9e7e8u), change_endian_long(0xbb058bdau), change_endian_long(0xddf168fau));
        TSIP.REG_104H.WORD = 0x00000052u;
        TSIP.REG_C4H.WORD = 0x01000c84u;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_104H.BIT.B31)
        {
            /* waiting */
        }
        TSIP.REG_100H.WORD = change_endian_long(0x00000000u);
        TSIP.REG_ECH.WORD = 0x0000b420u;
        TSIP.REG_ECH.WORD = 0x00000030u;
        TSIP.REG_E0H.WORD = 0x80840001u;
        TSIP.REG_00H.WORD = 0x00008213u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_00H.BIT.B25)
        {
            /* waiting */
        }
        TSIP.REG_1CH.WORD = 0x00001800u;
        RX65NHU_func103();
        RX65NHU_func100(change_endian_long(0x03c179a7u), change_endian_long(0xa139727fu), change_endian_long(0x8bebbd3au), change_endian_long(0xa43c25ebu));
        TSIP.REG_104H.WORD = 0x00000052u;
        TSIP.REG_C4H.WORD = 0x01000c84u;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_104H.BIT.B31)
        {
            /* waiting */
        }
        TSIP.REG_100H.WORD = change_endian_long(0x00000000u);
        TSIP.REG_ECH.WORD = 0x0000a420u;
        TSIP.REG_ECH.WORD = 0x00000010u;
        TSIP.REG_E0H.WORD = 0x80840001u;
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
        TSIP.REG_ECH.WORD = 0x0000a420u;
        TSIP.REG_ECH.WORD = 0x00000010u;
        TSIP.REG_E0H.WORD = 0x80840001u;
        TSIP.REG_00H.WORD = 0x00008213u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_00H.BIT.B25)
        {
            /* waiting */
        }
        TSIP.REG_1CH.WORD = 0x00001800u;
        TSIP.REG_ECH.WORD = 0x00000800u;
        TSIP.REG_ECH.WORD = 0x01003820u;
        TSIP.REG_ECH.WORD = 0x30008820u;
        TSIP.REG_ECH.WORD = 0x00000303u;
        TSIP.REG_ECH.WORD = 0x00020020u;
        TSIP.REG_ECH.WORD = 0x00000842u;
        TSIP.REG_ECH.WORD = 0x0000b460u;
        TSIP.REG_ECH.WORD = 0x00000030u;
        TSIP.REG_ECH.WORD = 0x0000b480u;
        TSIP.REG_ECH.WORD = 0x00000004u;
        for (iLoop = 0; iLoop < 12; iLoop = iLoop + 1)
        {
            TSIP.REG_ECH.WORD = 0x00003803u;
            TSIP.REG_ECH.WORD = 0x00003c02u;
            TSIP.REG_ECH.WORD = 0x00002444u;
            TSIP.REG_ECH.WORD = 0x00002464u;
        }
        TSIP.REG_ECH.WORD = 0x00000080u;
        RX65NHU_func100(change_endian_long(0x4b379e08u), change_endian_long(0x8044fc5bu), change_endian_long(0xeb2662eeu), change_endian_long(0xbce6dc74u));
        RX65NHU_func103();
        TSIP.REG_C4H.WORD = 0x0108258cu;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_104H.BIT.B31)
        {
            /* waiting */
        }
        TSIP.REG_100H.WORD = change_endian_long(0x00000000u);
        TSIP.REG_C4H.WORD = 0x400c0b0cu;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_104H.BIT.B31)
        {
            /* waiting */
        }
        TSIP.REG_100H.WORD = change_endian_long(0x1c7100adu);
        RX65NHU_func100(change_endian_long(0x01e09e66u), change_endian_long(0x7cbee7dcu), change_endian_long(0x5d878fffu), change_endian_long(0xd1abb43fu));
        TSIP.REG_D0H.WORD = 0x00000200u;
        TSIP.REG_C4H.WORD = 0x02e087bfu;
        TSIP.REG_ECH.WORD = 0x00000800u;
        TSIP.REG_E0H.WORD = 0x818c0000u;
        TSIP.REG_00H.WORD = 0x00002833u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_00H.BIT.B25)
        {
            /* waiting */
        }
        TSIP.REG_1CH.WORD = 0x00001800u;
        TSIP.REG_04H.WORD = 0x00000233u;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_04H.BIT.B30)
        {
            /* waiting */
        }
        OutData_PreMasterSecret[4] = TSIP.REG_100H.WORD;
        OutData_PreMasterSecret[5] = TSIP.REG_100H.WORD;
        OutData_PreMasterSecret[6] = TSIP.REG_100H.WORD;
        OutData_PreMasterSecret[7] = TSIP.REG_100H.WORD;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_04H.BIT.B30)
        {
            /* waiting */
        }
        OutData_PreMasterSecret[8] = TSIP.REG_100H.WORD;
        OutData_PreMasterSecret[9] = TSIP.REG_100H.WORD;
        OutData_PreMasterSecret[10] = TSIP.REG_100H.WORD;
        OutData_PreMasterSecret[11] = TSIP.REG_100H.WORD;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_04H.BIT.B30)
        {
            /* waiting */
        }
        OutData_PreMasterSecret[12] = TSIP.REG_100H.WORD;
        OutData_PreMasterSecret[13] = TSIP.REG_100H.WORD;
        OutData_PreMasterSecret[14] = TSIP.REG_100H.WORD;
        OutData_PreMasterSecret[15] = TSIP.REG_100H.WORD;
        RX65NHU_func100(change_endian_long(0x9205f7c4u), change_endian_long(0x1fb664fbu), change_endian_long(0xe992454au), change_endian_long(0x02554f8fu));
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
        OutData_PreMasterSecret[16] = TSIP.REG_100H.WORD;
        OutData_PreMasterSecret[17] = TSIP.REG_100H.WORD;
        OutData_PreMasterSecret[18] = TSIP.REG_100H.WORD;
        OutData_PreMasterSecret[19] = TSIP.REG_100H.WORD;
        RX65NHU_func100(change_endian_long(0xc3c93995u), change_endian_long(0x3f96141cu), change_endian_long(0x4526262au), change_endian_long(0x1e00d531u));
        TSIP.REG_104H.WORD = 0x00000052u;
        TSIP.REG_C4H.WORD = 0x01000c84u;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_104H.BIT.B31)
        {
            /* waiting */
        }
        TSIP.REG_100H.WORD = change_endian_long(0x00000000u);
        TSIP.REG_04H.WORD = 0x00000213u;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_04H.BIT.B30)
        {
            /* waiting */
        }
        OutData_PreMasterSecret[0] = TSIP.REG_100H.WORD;
        OutData_PreMasterSecret[1] = TSIP.REG_100H.WORD;
        OutData_PreMasterSecret[2] = TSIP.REG_100H.WORD;
        OutData_PreMasterSecret[3] = TSIP.REG_100H.WORD;
        RX65NHU_func102(change_endian_long(0xfaacfc8eu), change_endian_long(0xc225d172u), change_endian_long(0x83a85390u), change_endian_long(0xd79f9577u));
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
/**********************************************************************************************************************
 End of function ./input_dir/RX65NHU/RX65NHU_pc3_r1.prc
 *********************************************************************************************************************/
