/*
 * lfs_util_config.h v1.0.0
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

/* generated configuration header file - do not edit */
#ifndef LFS_UTIL_H_
#define LFS_UTIL_H_

#ifdef __cplusplus
        extern "C" {
        #endif

#if (0)
        #define LFS_NO_MALLOC
        #endif

#if (0)
        #define LFS_NO_ASSERT
        #endif

#if (1)
        #define LFS_YES_TRACE
        #endif

#if (1)
#define LFS_NO_DEBUG
#endif

#if (1)
#define LFS_NO_WARN
#endif

#if (1)
#define LFS_NO_ERROR
#endif

#if (1)
        #define LFS_NO_INTRINSICS
        #endif

#define LFS_THREAD_SAFE (1)

/* Defines the LittleFS instance control block to be the stdio wrapper. */
#define RM_STDIO_LITTLEFS_CFG_LFS g_rm_littlefs0_lfs

#include "lfs_util.h"

#ifdef __cplusplus
        }
        #endif
#endif /* LFS_UTIL_H_ */
