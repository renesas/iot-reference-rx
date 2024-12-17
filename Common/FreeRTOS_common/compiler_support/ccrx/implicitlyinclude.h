/*
 * implicitlyinclude.h v1.0.0
 * Copyright (C) Renesas Electronics Corporation and/or its affiliates.
 * All Rights Reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#if !defined(MBEDTLS_USER_CONFIG_FILE)
#define MBEDTLS_USER_CONFIG_FILE "mbedtls_user_config.h"
#endif

/* Avoid CC-RX's compiler warning message 'M0520193: Zero used for
 * undefined preprocessing identifier "XXXX"' in CC-RX's math.h */
#if !defined(_FEVAL)
#define _FEVAL (0)
#endif

#if !defined(_HAS_C9X_FAST_FMA)
#define _HAS_C9X_FAST_FMA (0)
#endif

/* Workaround for incompatibility of basic macro definitions. */
#if !defined(__RX__)
#define __RX__ (1)
#endif

#if defined(__LIT)
#if !defined(__RX_LITTLE_ENDIAN__)
#define __RX_LITTLE_ENDIAN__ (1)
#endif
#if !defined(__ORDER_LITTLE_ENDIAN__)
#define __ORDER_LITTLE_ENDIAN__ (1234)
#endif
#if !defined(__ORDER_BIG_ENDIAN__)
#define __ORDER_BIG_ENDIAN__ (4321)
#endif
#if !defined(__BYTE_ORDER__)
#define __BYTE_ORDER__ (__ORDER_LITTLE_ENDIAN__)
#endif
#elif defiend(__BIG)
#if !defined(__RX_BIG_ENDIAN__)
#define __RX_BIG_ENDIAN__ (1)
#endif
#if !defined(__ORDER_LITTLE_ENDIAN__)
#define __ORDER_LITTLE_ENDIAN__ (1234)
#endif
#if !defined(__ORDER_BIG_ENDIAN__)
#define __ORDER_BIG_ENDIAN__ (4321)
#endif
#if !defined(__BYTE_ORDER__)
#define __BYTE_ORDER__ (__ORDER_BIG_ENDIAN__)
#endif
#endif

#if defined(__FPU)
#if !defined(__RX_FPU_INSNS__)
#define __RX_FPU_INSNS__ (1)
#endif
#endif

#if defined(__RXV2)
#if !defined(__RXv2__)
#define __RXv2__ (1)
#endif
#endif
