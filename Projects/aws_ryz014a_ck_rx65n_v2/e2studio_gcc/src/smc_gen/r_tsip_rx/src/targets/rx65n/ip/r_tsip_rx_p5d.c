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

#if TSIP_ECDH_P256 == 1
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
* Function Name: R_TSIP_DlmsCosemCalculateKekSub
*******************************************************************************************************************/ /**
* @details       RX65NHU DLMS/COSEM calculation of KEK
* @param[in]     InData_KeyIndexType
* @param[in]     InData_KeyIndex
* @param[in]     InData_KDFType
* @param[in]     InData_PaddedMsg
* @param[in]     MAX_CNT
* @param[in]     InData_SaltKeyIndex
* @param[out]    OutData_KeyIndex
* @retval        TSIP_SUCCESS
* @retval        TSIP_ERR_RESOURCE_CONFLICT
* @retval        TSIP_ERR_KEY_SET
* @note          None
*/
e_tsip_err_t R_TSIP_DlmsCosemCalculateKekSub(uint32_t *InData_KeyIndexType, uint32_t *InData_KeyIndex, uint32_t *InData_KDFType, uint32_t *InData_PaddedMsg, uint32_t MAX_CNT, uint32_t *InData_SaltKeyIndex, uint32_t *OutData_KeyIndex)
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
    TSIP.REG_84H.WORD = 0x00005d01u;
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
    RX65NHU_func100(change_endian_long(0x185d5371u), change_endian_long(0x3ec77661u), change_endian_long(0xa74bdc72u), change_endian_long(0x444a9a2cu));
    TSIP.REG_1CH.WORD = 0x00400000u;
    TSIP.REG_1D0H.WORD = 0x00000000u;
    if (1u == (TSIP.REG_1CH.BIT.B22))
    {
        RX65NHU_func102(change_endian_long(0xf005a356u), change_endian_long(0xe4badf49u), change_endian_long(0xc75b3b9eu), change_endian_long(0xd947e138u));
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
        TSIP.REG_104H.WORD = 0x00000168u;
        TSIP.REG_E0H.WORD = 0x80020200u;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_104H.BIT.B31)
        {
            /* waiting */
        }
        TSIP.REG_100H.WORD = InData_PaddedMsg[0];
        /* WAIT_LOOP */
        while (1u != TSIP.REG_104H.BIT.B31)
        {
            /* waiting */
        }
        TSIP.REG_100H.WORD = InData_PaddedMsg[1];
        TSIP.REG_104H.WORD = 0x00000068u;
        TSIP.REG_E0H.WORD = 0x800103e0u;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_104H.BIT.B31)
        {
            /* waiting */
        }
        TSIP.REG_100H.WORD = InData_KDFType[0];
        TSIP.REG_ECH.WORD = 0x00000bbdu;
        TSIP.REG_7CH.WORD = 0x00000011u;
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
        TSIP.REG_ECH.WORD = 0x38000fffu;
        TSIP.REG_E0H.WORD = 0x00000080u;
        TSIP.REG_1CH.WORD = 0x00260000u;
        RX65NHU_func100(change_endian_long(0xd68a1406u), change_endian_long(0xdf30e97cu), change_endian_long(0x3455aa40u), change_endian_long(0xfe42bd98u));
        TSIP.REG_1CH.WORD = 0x00400000u;
        TSIP.REG_1D0H.WORD = 0x00000000u;
        if (1u == (TSIP.REG_1CH.BIT.B22))
        {
            TSIP.REG_104H.WORD = 0x00000054u;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_104H.BIT.B31)
            {
                /* waiting */
            }
            TSIP.REG_100H.WORD = change_endian_long(0x00000001u);
            TSIP.REG_00H.WORD = 0x00005323u;
            TSIP.REG_2CH.WORD = 0x00000022u;
            /* WAIT_LOOP */
            while (0u != TSIP.REG_00H.BIT.B25)
            {
                /* waiting */
            }
            TSIP.REG_1CH.WORD = 0x00001800u;
            TSIP.REG_E0H.WORD = 0x81020200u;
            TSIP.REG_00H.WORD = 0x0000580bu;
            /* WAIT_LOOP */
            while (0u != TSIP.REG_00H.BIT.B25)
            {
                /* waiting */
            }
            TSIP.REG_1CH.WORD = 0x00001800u;
            TSIP.REG_104H.WORD = 0x00000464u;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_104H.BIT.B31)
            {
                /* waiting */
            }
            TSIP.REG_100H.WORD = InData_PaddedMsg[2];
            TSIP.REG_100H.WORD = InData_PaddedMsg[3];
            TSIP.REG_100H.WORD = InData_PaddedMsg[4];
            TSIP.REG_100H.WORD = InData_PaddedMsg[5];
            TSIP.REG_100H.WORD = InData_PaddedMsg[6];
            /* WAIT_LOOP */
            while (0u != TSIP.REG_74H.BIT.B18)
            {
                /* waiting */
            }
            TSIP.REG_1CH.WORD = 0x00001600u;
            TSIP.REG_104H.WORD = 0x000000b4u;
            for (iLoop = 7; iLoop < MAX_CNT-7; iLoop = iLoop+16)
            {
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
            }
            /* WAIT_LOOP */
            while (0u != TSIP.REG_74H.BIT.B18)
            {
                /* waiting */
            }
            TSIP.REG_1CH.WORD = 0x00001600u;
            TSIP.REG_74H.WORD = 0x00000000u;
            TSIP.REG_104H.WORD = 0x00000000u;
            RX65NHU_func101(change_endian_long(0xfe56d788u), change_endian_long(0x3ae11ad7u), change_endian_long(0xf5c0444eu), change_endian_long(0x348b6010u));
        }
        else
        {
            TSIP.REG_104H.WORD = 0x00000361u;
            TSIP.REG_A4H.WORD = 0x200c3b0du;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_104H.BIT.B31)
            {
                /* waiting */
            }
            TSIP.REG_100H.WORD = InData_SaltKeyIndex[0];
            TSIP.REG_100H.WORD = InData_SaltKeyIndex[1];
            TSIP.REG_100H.WORD = InData_SaltKeyIndex[2];
            TSIP.REG_100H.WORD = InData_SaltKeyIndex[3];
            TSIP.REG_A4H.WORD = 0x000c0b0cu;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_104H.BIT.B31)
            {
                /* waiting */
            }
            TSIP.REG_100H.WORD = change_endian_long(0xe75dd718u);
            TSIP.REG_104H.WORD = 0x00000761u;
            TSIP.REG_B0H.WORD = 0x00000100u;
            TSIP.REG_A4H.WORD = 0x02f087bfu;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_104H.BIT.B31)
            {
                /* waiting */
            }
            TSIP.REG_100H.WORD = InData_SaltKeyIndex[4];
            TSIP.REG_100H.WORD = InData_SaltKeyIndex[5];
            TSIP.REG_100H.WORD = InData_SaltKeyIndex[6];
            TSIP.REG_100H.WORD = InData_SaltKeyIndex[7];
            /* WAIT_LOOP */
            while (1u != TSIP.REG_104H.BIT.B31)
            {
                /* waiting */
            }
            TSIP.REG_100H.WORD = InData_SaltKeyIndex[8];
            TSIP.REG_100H.WORD = InData_SaltKeyIndex[9];
            TSIP.REG_100H.WORD = InData_SaltKeyIndex[10];
            TSIP.REG_100H.WORD = InData_SaltKeyIndex[11];
            TSIP.REG_E0H.WORD = 0x80080000u;
            TSIP.REG_00H.WORD = 0x00008123u;
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
            TSIP.REG_100H.WORD = InData_SaltKeyIndex[12];
            TSIP.REG_100H.WORD = InData_SaltKeyIndex[13];
            TSIP.REG_100H.WORD = InData_SaltKeyIndex[14];
            TSIP.REG_100H.WORD = InData_SaltKeyIndex[15];
            TSIP.REG_A4H.WORD = 0x00900c45u;
            TSIP.REG_00H.WORD = 0x00001113u;
            /* WAIT_LOOP */
            while (0u != TSIP.REG_00H.BIT.B25)
            {
                /* waiting */
            }
            TSIP.REG_1CH.WORD = 0x00001800u;
            RX65NHU_func100(change_endian_long(0x2e64db3au), change_endian_long(0x225ca098u), change_endian_long(0xae613c8au), change_endian_long(0xc30a89cau));
            TSIP.REG_1CH.WORD = 0x00400000u;
            TSIP.REG_1D0H.WORD = 0x00000000u;
            if (1u == (TSIP.REG_1CH.BIT.B22))
            {
                TSIP.REG_ECH.WORD = 0x0000b7a0u;
                TSIP.REG_ECH.WORD = 0x00000001u;
                RX65NHU_func101(change_endian_long(0x6079749au), change_endian_long(0x6d25d19bu), change_endian_long(0xd6b7168du), change_endian_long(0x0395b701u));
            }
            else
            {
                RX65NHU_func001();
                TSIP.REG_104H.WORD = 0x00000054u;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_104H.BIT.B31)
                {
                    /* waiting */
                }
                TSIP.REG_100H.WORD = change_endian_long(0x00000001u);
                TSIP.REG_00H.WORD = 0x00005323u;
                TSIP.REG_2CH.WORD = 0x00000022u;
                /* WAIT_LOOP */
                while (0u != TSIP.REG_00H.BIT.B25)
                {
                    /* waiting */
                }
                TSIP.REG_1CH.WORD = 0x00001800u;
                TSIP.REG_E0H.WORD = 0x81020200u;
                TSIP.REG_00H.WORD = 0x0000580bu;
                /* WAIT_LOOP */
                while (0u != TSIP.REG_00H.BIT.B25)
                {
                    /* waiting */
                }
                TSIP.REG_1CH.WORD = 0x00001800u;
                TSIP.REG_104H.WORD = 0x00000464u;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_104H.BIT.B31)
                {
                    /* waiting */
                }
                TSIP.REG_100H.WORD = InData_PaddedMsg[2];
                TSIP.REG_100H.WORD = InData_PaddedMsg[3];
                TSIP.REG_100H.WORD = InData_PaddedMsg[4];
                TSIP.REG_100H.WORD = InData_PaddedMsg[5];
                TSIP.REG_100H.WORD = InData_PaddedMsg[6];
                /* WAIT_LOOP */
                while (0u != TSIP.REG_74H.BIT.B18)
                {
                    /* waiting */
                }
                TSIP.REG_1CH.WORD = 0x00001600u;
                TSIP.REG_104H.WORD = 0x000000b4u;
                for (iLoop = 7; iLoop < MAX_CNT-7; iLoop = iLoop+16)
                {
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
                }
                /* WAIT_LOOP */
                while (0u != TSIP.REG_74H.BIT.B18)
                {
                    /* waiting */
                }
                TSIP.REG_1CH.WORD = 0x00001600u;
                TSIP.REG_74H.WORD = 0x00000000u;
                TSIP.REG_104H.WORD = 0x00000000u;
                TSIP.REG_00H.WORD = 0x00003523u;
                TSIP.REG_74H.WORD = 0x00000008u;
                TSIP.REG_2CH.WORD = 0x00000010u;
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
                RX65NHU_func002();
                TSIP.REG_00H.WORD = 0x00005323u;
                TSIP.REG_2CH.WORD = 0x00000020u;
                /* WAIT_LOOP */
                while (0u != TSIP.REG_00H.BIT.B25)
                {
                    /* waiting */
                }
                TSIP.REG_1CH.WORD = 0x00001800u;
                TSIP.REG_104H.WORD = 0x00000054u;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_104H.BIT.B31)
                {
                    /* waiting */
                }
                TSIP.REG_100H.WORD = change_endian_long(0x80000000u);
                TSIP.REG_F8H.WORD = 0x00000040u;
                TSIP.REG_104H.WORD = 0x00000154u;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_104H.BIT.B31)
                {
                    /* waiting */
                }
                TSIP.REG_100H.WORD = change_endian_long(0x00000000u);
                TSIP.REG_100H.WORD = change_endian_long(0x00000300u);
                /* WAIT_LOOP */
                while (0u != TSIP.REG_74H.BIT.B18)
                {
                    /* waiting */
                }
                TSIP.REG_74H.WORD = 0x00000000u;
                TSIP.REG_1CH.WORD = 0x00001600u;
                RX65NHU_func101(change_endian_long(0x55245ddcu), change_endian_long(0xb38c89ddu), change_endian_long(0x32f92a20u), change_endian_long(0xc2b00575u));
            }
        }
        TSIP.REG_ECH.WORD = 0x38000fbdu;
        TSIP.REG_E0H.WORD = 0x00000080u;
        TSIP.REG_1CH.WORD = 0x00A60000u;
        RX65NHU_func100(change_endian_long(0x1fee9c43u), change_endian_long(0xf039e4c0u), change_endian_long(0xd0a9d0a6u), change_endian_long(0x107384dbu));
        TSIP.REG_1CH.WORD = 0x00400000u;
        TSIP.REG_1D0H.WORD = 0x00000000u;
        if (1u == (TSIP.REG_1CH.BIT.B22))
        {
            RX65NHU_func102(change_endian_long(0x00b83c0fu), change_endian_long(0xe69d0875u), change_endian_long(0x4d0e59ddu), change_endian_long(0x270bd285u));
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
            RX65NHU_func100(change_endian_long(0x8cb0b622u), change_endian_long(0xf9f36467u), change_endian_long(0xd8d520d4u), change_endian_long(0xf3c71c3fu));
            RX65NHU_func103();
            RX65NHU_func100(change_endian_long(0x1812710eu), change_endian_long(0x6ae67380u), change_endian_long(0x39d8bb8bu), change_endian_long(0xd51fd018u));
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
            TSIP.REG_C4H.WORD = 0x000c2b0du;
            TSIP.REG_E0H.WORD = 0x81040000u;
            TSIP.REG_00H.WORD = 0x00002813u;
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
            TSIP.REG_ECH.WORD = 0x38000fdeu;
            TSIP.REG_E0H.WORD = 0x00000080u;
            TSIP.REG_1CH.WORD = 0x00260000u;
            RX65NHU_func100(change_endian_long(0xb5a3d8bcu), change_endian_long(0xb0501fe0u), change_endian_long(0x9a1f3014u), change_endian_long(0x8444932bu));
            TSIP.REG_1CH.WORD = 0x00400000u;
            TSIP.REG_1D0H.WORD = 0x00000000u;
            if (1u == (TSIP.REG_1CH.BIT.B22))
            {
                RX65NHU_func100(change_endian_long(0xbe51672cu), change_endian_long(0xc5128957u), change_endian_long(0x91ee6fd9u), change_endian_long(0x396ea8beu));
                TSIP.REG_C4H.WORD = 0x02e007bdu;
                TSIP.REG_00H.WORD = 0x00002513u;
                TSIP.REG_74H.WORD = 0x00000008u;
                /* WAIT_LOOP */
                while (0u != TSIP.REG_00H.BIT.B25)
                {
                    /* waiting */
                }
                TSIP.REG_1CH.WORD = 0x00001800u;
                TSIP.REG_00H.WORD = 0x00000513u;
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
                OutData_KeyIndex[4] = TSIP.REG_100H.WORD;
                OutData_KeyIndex[5] = TSIP.REG_100H.WORD;
                OutData_KeyIndex[6] = TSIP.REG_100H.WORD;
                OutData_KeyIndex[7] = TSIP.REG_100H.WORD;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_04H.BIT.B30)
                {
                    /* waiting */
                }
                OutData_KeyIndex[8] = TSIP.REG_100H.WORD;
                OutData_KeyIndex[9] = TSIP.REG_100H.WORD;
                OutData_KeyIndex[10] = TSIP.REG_100H.WORD;
                OutData_KeyIndex[11] = TSIP.REG_100H.WORD;
                RX65NHU_func101(change_endian_long(0x2f91efddu), change_endian_long(0x405d9df4u), change_endian_long(0xb63e87e0u), change_endian_long(0x30182dcdu));
            }
            else
            {
                RX65NHU_func100(change_endian_long(0x5ab76f5du), change_endian_long(0x74e09911u), change_endian_long(0x66021bffu), change_endian_long(0x4b86d503u));
                TSIP.REG_D0H.WORD = 0x00000100u;
                TSIP.REG_C4H.WORD = 0x02e087bfu;
                TSIP.REG_00H.WORD = 0x00002523u;
                TSIP.REG_74H.WORD = 0x00000008u;
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
                OutData_KeyIndex[4] = TSIP.REG_100H.WORD;
                OutData_KeyIndex[5] = TSIP.REG_100H.WORD;
                OutData_KeyIndex[6] = TSIP.REG_100H.WORD;
                OutData_KeyIndex[7] = TSIP.REG_100H.WORD;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_04H.BIT.B30)
                {
                    /* waiting */
                }
                OutData_KeyIndex[8] = TSIP.REG_100H.WORD;
                OutData_KeyIndex[9] = TSIP.REG_100H.WORD;
                OutData_KeyIndex[10] = TSIP.REG_100H.WORD;
                OutData_KeyIndex[11] = TSIP.REG_100H.WORD;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_04H.BIT.B30)
                {
                    /* waiting */
                }
                OutData_KeyIndex[12] = TSIP.REG_100H.WORD;
                OutData_KeyIndex[13] = TSIP.REG_100H.WORD;
                OutData_KeyIndex[14] = TSIP.REG_100H.WORD;
                OutData_KeyIndex[15] = TSIP.REG_100H.WORD;
                RX65NHU_func101(change_endian_long(0xe54e5826u), change_endian_long(0xe53723b8u), change_endian_long(0x0b6fa54du), change_endian_long(0x9d503e5bu));
            }
            RX65NHU_func102(change_endian_long(0x21ab19c5u), change_endian_long(0x5f251686u), change_endian_long(0x3b2c7830u), change_endian_long(0x220c0fc6u));
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
 End of function ./input_dir/RX65NHU/RX65NHU_p5d_r2.prc
 *********************************************************************************************************************/
#endif /* #if TSIP_ECDH_P256 == 1 */
