/*******************************************************************************
* Copyright (C) 2023 Renesas Electronics Corporation. All rights reserved.
*
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only
* intended for use with Renesas products. No other uses are authorized. This
* software is owned by Renesas Electronics Corporation and is protected under
* all applicable laws, including copyright laws.
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
* THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT
* LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE
* AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.
* TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS
* ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE
* FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR
* ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE
* BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software
* and to discontinue the availability of this software. By using this software,
* you agree to the additional terms and conditions found by accessing the
* following link:
* http://www.renesas.com/disclaimer
*******************************************************************************/

#if !defined(MBEDTLS_USER_CONFIG_FILE)
#define MBEDTLS_USER_CONFIG_FILE "mbedtls_user_config.h"
#endif

/* Avoid CC-RX's compiler warning message 'M0520193: Zero used for
 * undefined preprocessing identifier "XXXX"' in CC-RX's math.h */
#if !defined(_FEVAL)
#define _FEVAL 0
#endif

#if !defined(_HAS_C9X_FAST_FMA)
#define _HAS_C9X_FAST_FMA 0
#endif

/* Workaround for incompatibility of basic macro definitions. */
#if !defined(__RX__)
#define __RX__ 1
#endif

#if defined(__LIT)
#if !defined(__RX_LITTLE_ENDIAN__)
#define __RX_LITTLE_ENDIAN__ 1
#endif
#if !defined(__ORDER_LITTLE_ENDIAN__)
#define __ORDER_LITTLE_ENDIAN__ 1234
#endif
#if !defined(__ORDER_BIG_ENDIAN__)
#define __ORDER_BIG_ENDIAN__ 4321
#endif
#if !defined(__BYTE_ORDER__)
#define __BYTE_ORDER__ __ORDER_LITTLE_ENDIAN__
#endif
#elif defiend(__BIG)
#if !defined(__RX_BIG_ENDIAN__)
#define __RX_BIG_ENDIAN__ 1
#endif
#if !defined(__ORDER_LITTLE_ENDIAN__)
#define __ORDER_LITTLE_ENDIAN__ 1234
#endif
#if !defined(__ORDER_BIG_ENDIAN__)
#define __ORDER_BIG_ENDIAN__ 4321
#endif
#if !defined(__BYTE_ORDER__)
#define __BYTE_ORDER__ __ORDER_BIG_ENDIAN__
#endif
#endif

#if defined(__FPU)
#if !defined(__RX_FPU_INSNS__)
#define __RX_FPU_INSNS__ 1
#endif
#endif

#if defined(__RXV2)
#if !defined(__RXv2__)
#define __RXv2__ 1
#endif
#endif
