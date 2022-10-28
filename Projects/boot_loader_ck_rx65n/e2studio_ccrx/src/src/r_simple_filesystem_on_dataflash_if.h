
/***********************************************************************************************************************
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
* Copyright (C) 2020 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/

/* Return error codes */
typedef enum sfd_err_t
{
    SFD_SUCCESS = 0,
	SFD_INVALID_ARGUMENT,
	SFD_END_OF_LIST,
    SFD_FATAL_ERROR
} sfd_err_t;

typedef uint32_t SFD_HANDLE;

#define SFD_CONTROL_BLOCK_SIZE 1024

#define SFD_HANDLE_INVALID 0xffffffff

#define SFD_HANDLES_LABEL_MAX_LENGTH 40
#define SFD_OBJECT_HANDLES_NUM 5

#define SFD_SHA256_LENGTH 32

//#define USE_MBEDTLS
#define USE_TINYCRYPT

typedef struct _sfd_descriptor
{
    uint8_t label[SFD_HANDLES_LABEL_MAX_LENGTH];
    uint32_t label_length;
    uint32_t local_storage_index;
    uint32_t data_length;
    uint32_t status;
    SFD_HANDLE xHandle;
} SFD_DESCRIPTOR;

#define SFD_LOCAL_STORAGE_SIZE (SFD_CONTROL_BLOCK_SIZE - (sizeof(SFD_DESCRIPTOR) * SFD_OBJECT_HANDLES_NUM) - SFD_SHA256_LENGTH)

#define SFD_SECTION_NAME _BOOTLOADER_KEY_STORAGE
#define SFD_MIRROR_SECTION_NAME _BOOTLOADER_KEY_STORAGE_MIRROR

/******************************************************************************
 Interface funcsions
 ******************************************************************************/
sfd_err_t R_SFD_Open(void);
SFD_HANDLE R_SFD_SaveObject(uint8_t *label, uint32_t label_length, uint8_t *data, uint32_t data_length);
SFD_HANDLE R_SFD_FindObject( uint8_t *label, uint8_t label_length );
sfd_err_t R_SFD_GetObjectValue(SFD_HANDLE xHandle, uint8_t **data, uint32_t *data_length);
sfd_err_t R_SFD_Scan(uint8_t **label, uint32_t *label_length, uint8_t **data, uint32_t *data_length);
sfd_err_t R_SFD_ResetScan(void);
uint32_t R_SFD_ReadPysicalSize(void);
uint32_t R_SFD_ReadAllocatedStorageSize(void);
uint32_t R_SFD_ReadFreeSize(void);
sfd_err_t R_SFD_Close(void);
