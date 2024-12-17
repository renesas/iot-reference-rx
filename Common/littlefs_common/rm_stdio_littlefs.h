/*
 * rm_stdio_littlefs.h v1.0.0
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

/* ${REA_DISCLAIMER_PLACEHOLDER} */

#ifndef RM_STDIO_LITTLEFS_H
#define RM_STDIO_LITTLEFS_H

/**********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
#include <stdio.h>
#include "lfs.h"
#include "lfs_util.h"

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

#define MBEDTLS_CONFIG_LFS    (&RM_STDIO_LITTLEFS_CFG_LFS)

#pragma inline_asm rm_littlefs_fopen

/**********************************************************************************************************************
 * Function Name: rm_littlefs_fopen
 * Description  : .
 * Arguments    : path
 *              : mode
 * Return Value : .
 *********************************************************************************************************************/
static FILE * rm_littlefs_fopen(const char * path, const char * mode)
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
/*****************************************************************************************
End of function rm_littlefs_fopen
****************************************************************************************/

/* Use the above inline function to replace fopen. */
#define fopen    (rm_littlefs_fopen)

#define fwrite(data, n, len, stream)    (lfs_file_write(MBEDTLS_CONFIG_LFS, (lfs_file_t *) stream, data, n * len))

#define fread(data, n, len, stream)     (lfs_file_read(MBEDTLS_CONFIG_LFS, (lfs_file_t *) stream, data, n * len))

#define fseek(stream, offset,                                                                              \
                seek)                     ((lfs_file_seek(MBEDTLS_CONFIG_LFS, (lfs_file_t *) stream, offset, \
                                                        seek) >= 0) ? 0 : -1)

#define remove(file)                    (lfs_remove(MBEDTLS_CONFIG_LFS, file))

/* Free the memory associated with this file after it is closed. */
#define fclose(stream)                  (lfs_file_close(MBEDTLS_CONFIG_LFS, (lfs_file_t *) stream); free(stream))

#define rename(oldpath, newpath)        (lfs_rename(MBEDTLS_CONFIG_LFS, oldpath, newpath))

extern lfs_t RM_STDIO_LITTLEFS_CFG_LFS;

/**********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/

/** Common macro for FSP header files. There is also a corresponding FSP_HEADER macro at the top of this file. */
FSP_FOOTER

#endif
