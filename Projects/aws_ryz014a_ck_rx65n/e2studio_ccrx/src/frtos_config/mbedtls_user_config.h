/**********************************************************************************************************************
 * Copyright (C) 2023 Renesas Electronics Corporation. All rights reserved.
 *
 * DISCLAIMER
 * This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products. No
 * other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all
 * applicable laws, including copyright laws.
 * THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
 * THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED. TO THE MAXIMUM
 * EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES
 * SHALL BE LIABLE FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO THIS
 * SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
 * Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability of
 * this software. By using this software, you agree to the additional terms and conditions found by accessing the
 * following link:
 * http://www.renesas.com/disclaimer
 *********************************************************************************************************************/

#ifndef _MBEDTLS_USER_CONFIG_H_
#define _MBEDTLS_USER_CONFIG_H_

/* Remove mbedTLS's unused *_self_test functions to remove 
 * linker warnings of the GNURX projects such as 
 * '_fstat is not implemented and will always fail', 
 * '_isatty is not implemented and will always fail', 
 * and so on. These are cause by using fprintf function. 
 * (But such warnings cannot be removed by only this.)"
 */
#if defined(MBEDTLS_SELF_TEST)
#undef MBEDTLS_SELF_TEST
#endif

/* Remove mbedTLS's unnecessary link of standard calloc 
 * and free functions.
 */
#if !defined(MBEDTLS_PLATFORM_NO_STD_FUNCTIONS)
#define MBEDTLS_PLATFORM_NO_STD_FUNCTIONS

#if !defined(MBEDTLS_PLATFORM_MEMORY)
#define MBEDTLS_PLATFORM_MEMORY
#endif
#if defined(MBEDTLS_PLATFORM_STD_CALLOC)
#undef MBEDTLS_PLATFORM_STD_CALLOC
#endif
#if defined(MBEDTLS_PLATFORM_STD_FREE)
#undef MBEDTLS_PLATFORM_STD_FREE
#endif

#if !defined(MBEDTLS_PLATFORM_SNPRINTF_MACRO)
#define MBEDTLS_PLATFORM_SNPRINTF_MACRO snprintf
#endif

#endif /* !MBEDTLS_PLATFORM_NO_STD_FUNCTIONS */

/* Workaround for CC-RX's non-standard 2nd argument type of 
 * memset function. CC-RX uses long but standard uses int.
 */
#if defined(__CCRX__)
#include "string.h"
typedef void * (* mbedtls_platform_zeroize_memset_t)( void *, int, size_t );
#undef memset
#define memset ((mbedtls_platform_zeroize_memset_t)memset)
#endif

#endif
