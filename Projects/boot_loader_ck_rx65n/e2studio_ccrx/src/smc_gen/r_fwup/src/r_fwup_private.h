/**********************************************************************************************************************
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
 *
 * Copyright (C) 2022 Renesas Electronics Corporation. All rights reserved.
 *********************************************************************************************************************/
/**********************************************************************************************************************
 * File Name    : r_fwup_private.h
 * Description  : Functions for using Firmware update.
 *********************************************************************************************************************/

/**********************************************************************************************************************
 Includes   <System Includes> , "Project Includes"
 *********************************************************************************************************************/
#include <stdio.h>
#include <string.h>
#include "platform.h"
#include "r_fwup_config.h"

#ifndef R_FWUP_PRIVATE_H
#define R_FWUP_PRIVATE_H

/**********************************************************************************************************************
 Macro definitions
 *********************************************************************************************************************/
/* Stdlib*/
#if defined(__RX)
  #define STRCMP(s1, s2)                  ( strcmp((s1), (s2)) )
  #if !defined(STRSTR)
    #define STRSTR(s1, s2)                ( strstr((s1), (s2)) )
  #endif
  #define STRLEN(s)                       ( strlen((s)) )
  #define MEMCMP(s1, s2, n)               ( memcmp((s1), (s2), (n)) )
  #define MEMCPY(s1, s2, n)               ( memcpy((s1), (s2), (n)) )
#else
  #if defined(__ICCRL78__)
    #define STRCMP(s1, s2)                ( strcmp((s1), (s2)) )
    #if !defined(STRSTR)
      #define STRSTR(s1, s2)              ( strstr((s1), (s2)) )
    #endif
    #define STRLEN(s)                     ( strlen((s)) )
    #define MEMCMP(s1, s2, n)             ( memcmp((s1), (s2), (n)) )
    #define MEMCPY(s1, s2, n)             ( memcpy((s1), (s2), (n)) )
  #elif defined (__CCRL__)
    #define STRCMP(s1, s2)                ( _COM_strcmp_ff((s1), (s2)) )
    #if !defined(STRSTR)
      #define STRSTR(s1, s2)              ( _COM_strstr_ff((s1), (s2)) )
    #endif
    #define STRLEN(s)                     ( _COM_strlen_f((s)) )
    #define MEMCMP(s1, s2, n)             ( _COM_memcmp_ff((s1), (s2), (n)) )
    #define MEMCPY(s1, s2, n)             ( _COM_memcpy_ff((s1), (s2), (n)) )
  #elif defined (__llvm__)
    #define STRCMP(s1, s2)                ( _COM_strcmp_ff((s1), (s2)) )
    #if !defined(STRSTR)
      #define STRSTR(s1, s2)              ( _COM_strstr_ff((s1), (s2)) )
    #endif
    #define STRLEN(s)                     ( _COM_strlen_f((s)) )
    #define MEMCMP(s1, s2, n)             ( _COM_memcmp_ff((s1), (s2), (n)) )
    #define MEMCPY(s1, s2, n)             ( _COM_memcpy_ff((s1), (s2), (n)) )
  #endif /* defined(__ICCRL78__) */
#endif /* defined(__RX) */

/* __far */
#if defined(__RX)
  #define FWUP_FAR
  #define FWUP_FAR_FUNC
#else
  #if (__ICCRL78__)
    #define FWUP_FAR                __far
    #define FWUP_FAR_FUNC           __far_func
  #else
    #define FWUP_FAR                __far
    #define FWUP_FAR_FUNC           __far
  #endif /* defined(__ICCRL78__) */
#endif /* defined(__RX) */

#define CH_FAR                      char FWUP_FAR
#define C_CH_FAR                    const char FWUP_FAR
#define C_U8_FAR                    const uint8_t FWUP_FAR
#define S_C_CH_FAR                  static C_CH_FAR
#define S_C_U8_FAR                  static C_U8_FAR


#if (FWUP_CFG_PRINTF_DISABLE == 1)
#define FWUP_LOG_ERR(...)
#define FWUP_LOG_INFO(...)
#define FWUP_LOG_DBG(...)
#else
#define FWUP_LOG_ERR                printf
#define FWUP_LOG_INFO               printf
#define FWUP_LOG_DBG                printf
#endif /* (FWUP_CFG_PRINTF_DISABLE == 1) */


#define FWUP_IMAGE_BLOCKS           (31)
#define FWUP_MAGIC_CODE_LEN         (7)
#define FWUP_SIG_TYPE_LENGTH        (32)
#define FWUP_SIG_SIZE_MAX           (64)

/**********************************************************************************************************************
 Global Typedef definitions
 *********************************************************************************************************************/
/* Firmware header */
typedef struct st_fw_header
{
    uint8_t  magic_code[FWUP_MAGIC_CODE_LEN];
    uint8_t  image_flag;
    uint8_t  sig_type[FWUP_SIG_TYPE_LENGTH];
    uint32_t sig_size;
    uint8_t  sig[FWUP_SIG_SIZE_MAX];
    uint32_t image_file_size;
    uint8_t  rsv[400];
} st_fw_header_t;

/* Firmware descriptor */
typedef struct st_fw_image
{
    uint32_t addr;
    uint32_t size;
} st_fw_image_t;

typedef struct st_fw_desc
{
    uint32_t n;
    st_fw_image_t fw[FWUP_IMAGE_BLOCKS];
    uint8_t  rsv[4];
} st_fw_desc_t;

/* Firmware control block */
typedef struct st_fw_ctrlblk
{
    st_fw_header_t head;
    st_fw_desc_t   desc;
} st_fw_ctrlblk_t;

/**********************************************************************************************************************
 External global variables
 *********************************************************************************************************************/

/**********************************************************************************************************************
 Exported global functions
 *********************************************************************************************************************/

#endif /* R_FWUP_PRIVATE_H */
