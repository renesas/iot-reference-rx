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
