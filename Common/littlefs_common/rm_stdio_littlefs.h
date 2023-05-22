/***********************************************************************************************************************
 * Copyright 2023 Renesas Electronics Corporation and/or its affiliates.  All Rights Reserved.
 *
 * This software and documentation are supplied by Renesas Electronics America Inc. and may only be used with products
 * of Renesas Electronics Corp. and its affiliates ("Renesas").  No other uses are authorized.  Renesas products are
 * sold pursuant to Renesas terms and conditions of sale.  Purchasers are solely responsible for the selection and use
 * of Renesas products and Renesas assumes no liability.  No license, express or implied, to any intellectual property
 * right is granted by Renesas. This software is protected under all applicable laws, including copyright laws. Renesas
 * reserves the right to change or discontinue this software and/or this documentation. THE SOFTWARE AND DOCUMENTATION
 * IS DELIVERED TO YOU "AS IS," AND RENESAS MAKES NO REPRESENTATIONS OR WARRANTIES, AND TO THE FULLEST EXTENT
 * PERMISSIBLE UNDER APPLICABLE LAW, DISCLAIMS ALL WARRANTIES, WHETHER EXPLICITLY OR IMPLICITLY, INCLUDING WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, AND NONINFRINGEMENT, WITH RESPECT TO THE SOFTWARE OR
 * DOCUMENTATION.  RENESAS SHALL HAVE NO LIABILITY ARISING OUT OF ANY SECURITY VULNERABILITY OR BREACH.  TO THE MAXIMUM
 * EXTENT PERMITTED BY LAW, IN NO EVENT WILL RENESAS BE LIABLE TO YOU IN CONNECTION WITH THE SOFTWARE OR DOCUMENTATION
 * (OR ANY PERSON OR ENTITY CLAIMING RIGHTS DERIVED FROM YOU) FOR ANY LOSS, DAMAGES, OR CLAIMS WHATSOEVER, INCLUDING,
 * WITHOUT LIMITATION, ANY DIRECT, CONSEQUENTIAL, SPECIAL, INDIRECT, PUNITIVE, OR INCIDENTAL DAMAGES; ANY LOST PROFITS,
 * OTHER ECONOMIC DAMAGE, PROPERTY DAMAGE, OR PERSONAL INJURY; AND EVEN IF RENESAS HAS BEEN ADVISED OF THE POSSIBILITY
 * OF SUCH LOSS, DAMAGES, CLAIMS OR COSTS.
 **********************************************************************************************************************/

/* ${REA_DISCLAIMER_PLACEHOLDER} */

#ifndef RM_STDIO_LITTLEFS_H
#define RM_STDIO_LITTLEFS_H

/**********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
#include "lfs.h"
#include "lfs_util.h"
#include <stdio.h>

/** Common macro for FSP header files. There is also a corresponding FSP_FOOTER macro at the end of this file. */
FSP_HEADER

/**********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/

/* By default IAR does not define FILE unless DLIB is used. */
#if defined(__ICCARM__)
 #if !_DLIB_FILE_DESCRIPTOR
typedef void FILE;
 #endif
#endif

extern lfs_t RM_STDIO_LITTLEFS_CFG_LFS;
#define MBEDTLS_CONFIG_LFS    &RM_STDIO_LITTLEFS_CFG_LFS

#pragma inline_asm rm_littlefs_fopen
static FILE * rm_littlefs_fopen (const char * path, const char * mode)
{
    /* Allocate a LittleFS file instance. */
    lfs_file_t * file = malloc(sizeof(lfs_file_t));
    int          lfs_status;

    if (NULL != file)
    {
        /* Check if the file is being opened for read or write and set the LittleFS flags. */
        int flags = ('r' == mode[0]) ? LFS_O_RDONLY : LFS_O_WRONLY | LFS_O_CREAT;

        lfs_status = lfs_file_open(MBEDTLS_CONFIG_LFS, file, path, flags);

        /* If the file failed to open free the memory and return NULL. */
        if (lfs_status != LFS_ERR_OK)
        {
            free(file);
            file = NULL;
        }
    }

    return (FILE *) file;
}

/* Use the above inline function to replace fopen. */
#define fopen    rm_littlefs_fopen

#define fwrite(data, n, len, stream)    lfs_file_write(MBEDTLS_CONFIG_LFS, (lfs_file_t *) stream, data, n * len)

#define fread(data, n, len, stream)     lfs_file_read(MBEDTLS_CONFIG_LFS, (lfs_file_t *) stream, data, n * len)

#define fseek(stream, offset,                                                                              \
              seek)                     ((lfs_file_seek(MBEDTLS_CONFIG_LFS, (lfs_file_t *) stream, offset, \
                                                        seek) >= 0) ? 0 : -1)

#define remove(file)                    lfs_remove(MBEDTLS_CONFIG_LFS, file)

/* Free the memory associated with this file after it is closed. */
#define fclose(stream)                  lfs_file_close(MBEDTLS_CONFIG_LFS, (lfs_file_t *) stream); free(stream)

#define rename(oldpath, newpath)        lfs_rename(MBEDTLS_CONFIG_LFS, oldpath, newpath)

/**********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/

/** Common macro for FSP header files. There is also a corresponding FSP_HEADER macro at the top of this file. */
FSP_FOOTER

#endif
