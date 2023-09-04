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

/* FreeRTOS Includes. */
#if defined(__ARMCC_VERSION)
 #pragma GCC diagnostic push
 #pragma GCC diagnostic ignored "-Wmacro-redefined"
#endif

#include "core_pkcs11.h"

#if defined(__ARMCC_VERSION)
 #pragma GCC diagnostic pop
#endif

#include "core_pkcs11_config.h"
#include "portmacro.h"
#include "core_pkcs11_pal.h"
#include "FreeRTOS.h"
#include "task.h"
#include "mbedtls/sha256.h"

/* C runtime includes. */
#include <stdio.h>
#include <string.h>

#include "lfs.h"
#include <lfs_util_config.h>


#include "transport_mbedtls_pkcs11.h"

/* mbedTLS includes. */
#include "mbedtls_config.h"
#include "mbedtls/compat-2.x.h"
extern lfs_t RM_STDIO_LITTLEFS_CFG_LFS;
volatile uint32_t pvwrite = 0;
enum eObjectHandles
{
    eInvalidHandle       = 0,          /* According to PKCS #11 spec, 0 is never a valid object handle. */
    eAwsDevicePrivateKey = 1,
    eAwsDevicePublicKey,
    eAwsDeviceCertificate,
	eAwsCodeSigningKey,
	eAwsClaimCertificate,
	eAwsClaimPrivateKey,
#if pkcs11configMAX_NUM_OBJECTS >= 8
	eAwsRootCertificate,
#endif
#if pkcs11configMAX_NUM_OBJECTS >= 9
    eAwsJitpCertificate,
#endif
};

uint8_t g_object_handle_dictionary[pkcs11configMAX_NUM_OBJECTS][pkcs11configMAX_LABEL_LENGTH + 1] =
{
    "",
    pkcs11configLABEL_DEVICE_PRIVATE_KEY_FOR_TLS,
    pkcs11configLABEL_DEVICE_PUBLIC_KEY_FOR_TLS,
    pkcs11configLABEL_DEVICE_CERTIFICATE_FOR_TLS,
    pkcs11configLABEL_CODE_VERIFICATION_KEY,
	pkcs11configLABEL_CLAIM_CERTIFICATE,
	pkcs11configLABEL_CLAIM_PRIVATE_KEY,
#if pkcs11configMAX_NUM_OBJECTS >= 8
    pkcs11configLABEL_ROOT_CERTIFICATE,
#endif
#if pkcs11configMAX_NUM_OBJECTS >= 9
    pkcs11configLABEL_JITP_CERTIFICATE,
#endif
};
TlsTransportStatus_t Crypto( void );

TlsTransportStatus_t Crypto( void )
{
    TlsTransportStatus_t returnStatus = TLS_TRANSPORT_SUCCESS;

    /* Set the mutex functions for mbed TLS thread safety. */
    mbedtls_threading_set_alt( mbedtls_platform_mutex_init,
                               mbedtls_platform_mutex_free,
                               mbedtls_platform_mutex_lock,
                               mbedtls_platform_mutex_unlock );

    if( returnStatus == TLS_TRANSPORT_SUCCESS )
    {
        LogDebug( ( "Successfully initialized mbedTLS." ) );
    }

    return returnStatus;
}
/*
 *  @brief Initialize the PAL.
 */
CK_RV PKCS11_PAL_Initialize ()
{
	Crypto();
    return CKR_OK;
}

/**
 * @brief Writes a file to local storage.
 *
 * Port-specific file write for cryptographic information.
 *
 * @param[in] pxLabel       Label of the object to be saved.
 * @param[in] pucData       Data buffer to be written to file
 * @param[in] ulDataSize    Size (in bytes) of data to be saved.
 *
 * @return The file handle of the object that was stored.
 */
CK_OBJECT_HANDLE PKCS11_PAL_SaveObject (CK_ATTRIBUTE_PTR pxLabel, CK_BYTE_PTR pucData, CK_ULONG ulDataSize)
{
    CK_OBJECT_HANDLE xHandle = eInvalidHandle;

    /* search specified label value from g_object_handle_dictionary */
    for (uint32_t i = 1; i < pkcs11configMAX_NUM_OBJECTS; i++)
    {
        if (!strcmp((char *) &g_object_handle_dictionary[i], pxLabel->pValue))
        {
            xHandle = i;
        }
    }

    if (eInvalidHandle == xHandle)
    {
        return eInvalidHandle;
    }

    lfs_file_t file;

    volatile int lfs_err = lfs_remove(&RM_STDIO_LITTLEFS_CFG_LFS, pxLabel->pValue);

    if ((LFS_ERR_NOENT != lfs_err) && (LFS_ERR_OK != lfs_err))
    {
        return eInvalidHandle;
    }

    lfs_err = lfs_file_open(&RM_STDIO_LITTLEFS_CFG_LFS, &file, pxLabel->pValue, LFS_O_WRONLY | LFS_O_TRUNC | LFS_O_CREAT);

    if (LFS_ERR_OK != lfs_err)
    {
        return eInvalidHandle;
    }

    lfs_err = lfs_file_write(&RM_STDIO_LITTLEFS_CFG_LFS, &file, pucData, ulDataSize);

    pvwrite +=ulDataSize;
    if (lfs_err < 0)
    {
        xHandle = eInvalidHandle;
    }

    lfs_file_close(&RM_STDIO_LITTLEFS_CFG_LFS, &file);

    return xHandle;
}

/**
 * @brief Translates a PKCS #11 label into an object handle.
 *
 * Port-specific object handle retrieval.
 *
 *
 * @param[in] pxLabel        Pointer to the label of the object
 *                           who's handle should be found.
 * @param[in] usLength       The length of the label, in bytes.
 *
 * @return The object handle if operation was successful.
 * Returns eInvalidHandle if unsuccessful.
 */
CK_OBJECT_HANDLE PKCS11_PAL_FindObject (CK_BYTE_PTR pxLabel, CK_ULONG usLength)
{
    /* Avoid compiler warnings about unused variables. */
    (void)usLength;

    CK_OBJECT_HANDLE xHandle = eInvalidHandle;
    uint32_t         i;

    for (i = 1; i < pkcs11configMAX_NUM_OBJECTS; i++)
    {
        if (!strcmp((char *) &g_object_handle_dictionary[i], (char *) pxLabel))
        {
            lfs_file_t file;
            struct lfs_info xFileInfo = { 0 };
            if( lfs_stat( &RM_STDIO_LITTLEFS_CFG_LFS, (char *) pxLabel, &xFileInfo ) == LFS_ERR_OK )
			{
            	xHandle = (CK_OBJECT_HANDLE) i;
            	break;
			}
        }
    }

    return xHandle;
}

/**
 * @brief Gets the value of an object in storage, by handle.
 *
 * Port-specific file access for cryptographic information.
 *
 * This call dynamically allocates the buffer which object value
 * data is copied into.  PKCS11_PAL_GetObjectValueCleanup()
 * should be called after each use to free the dynamically allocated
 * buffer.
 *
 * @sa PKCS11_PAL_GetObjectValueCleanup
 *
 * @param[in]  xHandle      Handle of the file to be read.
 * @param[out] ppucData     Pointer to buffer for file data.
 * @param[out] pulDataSize  Size (in bytes) of data located in file.
 * @param[out] pIsPrivate   Boolean indicating if value is private (CK_TRUE)
 *                          or exportable (CK_FALSE)
 *
 * @return CKR_OK if operation was successful.  CKR_KEY_HANDLE_INVALID if
 * no such object handle was found, CKR_DEVICE_MEMORY if memory for
 * buffer could not be allocated, CKR_FUNCTION_FAILED for device driver
 * error.
 */

CK_RV PKCS11_PAL_GetObjectValue (CK_OBJECT_HANDLE xHandle,
                                 CK_BYTE_PTR    * ppucData,
                                 CK_ULONG_PTR     pulDataSize,
                                 CK_BBOOL       * pIsPrivate)
{
    CK_RV            xReturn        = CKR_FUNCTION_FAILED;
    CK_OBJECT_HANDLE xHandleStorage = xHandle;

    if (xHandle != eInvalidHandle)
    {
        lfs_file_t file;

        int lfs_ret =
            lfs_file_open(&RM_STDIO_LITTLEFS_CFG_LFS,
                          &file,
                          (char *) g_object_handle_dictionary[xHandleStorage],
                          LFS_O_RDONLY);

        if (LFS_ERR_OK != lfs_ret)
        {
            return eInvalidHandle;
        }

        lfs_ret = lfs_file_size(&RM_STDIO_LITTLEFS_CFG_LFS, &file);

        *ppucData = pvPortMalloc((size_t) lfs_ret);

        if ((lfs_ret >= 0) && (NULL != *ppucData))
        {
            lfs_ret = lfs_file_read(&RM_STDIO_LITTLEFS_CFG_LFS, &file, *ppucData, (lfs_size_t) lfs_ret);

            if (lfs_ret >= 0)
            {
                *pulDataSize = (uint32_t) lfs_ret;

                xReturn = CKR_OK;
            }

            if (xHandle == eAwsDevicePrivateKey || xHandle == eAwsClaimPrivateKey)
            {
                *pIsPrivate = CK_TRUE;
            }
            else
            {
                *pIsPrivate = CK_FALSE;
            }
        }

        lfs_file_close(&RM_STDIO_LITTLEFS_CFG_LFS, &file);
    }

    return xReturn;
}

/**
 * @brief Cleanup after PKCS11_GetObjectValue().
 *
 * @param[in] pucData       The buffer to free.
 *                          (*ppucData from PKCS11_PAL_GetObjectValue())
 * @param[in] ulDataSize    The length of the buffer to free.
 *                          (*pulDataSize from PKCS11_PAL_GetObjectValue())
 */
void PKCS11_PAL_GetObjectValueCleanup (CK_BYTE_PTR pucData, CK_ULONG ulDataSize)
{
    /* Avoid compiler warnings about unused variables. */
    (void)ulDataSize;

    vPortFree(pucData);
}

/**
 * @brief Deletes a file from storage
 *
 * @param[in] xHandle       Handle of object to delete.
 */
CK_RV PKCS11_PAL_DestroyObject (CK_OBJECT_HANDLE xHandle)
{
    CK_RV xReturn = CKR_FUNCTION_FAILED;

    if (xHandle != eInvalidHandle)
    {
        volatile int lfs_err = lfs_remove(&RM_STDIO_LITTLEFS_CFG_LFS, (char *) g_object_handle_dictionary[xHandle]);

        if (LFS_ERR_OK == lfs_err)
        {
            xReturn = CKR_OK;
        }
    }

    return xReturn;
}

/*-----------------------------------------------------------*/
