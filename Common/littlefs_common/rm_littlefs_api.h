/*
 * rm_littlefs_api.h v1.0.0
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

/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/

/* Register definitions, common services and error codes. */
#include "platform.h"
#include "r_flash_rx_if.h"
#include "lfs.h"

#ifndef RM_LITTLEFS_API_H
#define RM_LITTLEFS_API_H

/*******************************************************************************************************************//**
 * @defgroup RM_LITTLEFS_API LittleFS Interface
 * @ingroup RENESAS_INTERFACES
 * @brief Interface for LittleFS access
 *
 * @section RM_LITTLEFS_API_SUMMARY Summary
 * The LittleFS Port configures a fail-safe filesystem designed for microcontrollers on top of a lower level storage
 * device.
 *
 * Implemented by:
 * @ref RM_LITTLEFS_FLASH
 *
 * @{
 **********************************************************************************************************************/

/* Common macro for SSP header files. There is also a corresponding SSP_FOOTER macro at the end of this file. */
FSP_HEADER

/**********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/

/**********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/

/** User configuration structure, used in open function */
typedef struct st_rm_littlefs_cfg
{
    struct lfs_config const * p_lfs_cfg; ///< Pointer LittleFS configuration structure
    void const              * p_extend;  ///< Pointer to hardware dependent configuration
} rm_littlefs_cfg_t;

/** LittleFS Port API control block.  Allocate an instance specific control block to pass into the LittleFS Port API calls.
 * @par Implemented as
 * - @ref rm_littlefs_flash_instance_ctrl_t
 */
typedef void rm_littlefs_ctrl_t;

/** LittleFS Port interface API. */
typedef struct st_rm_littlefs_api
{
    /** Initialize The lower level storage device.
     * @par Implemented as
     * - @ref RM_LITTLEFS_FLASH_Open
     *
     * @param[in]   p_ctrl              Pointer to control block. Must be declared by user. Elements set here.
     * @param[in]   p_cfg               Pointer to configuration structure. All elements of this structure must be set by user.
     */
    fsp_err_t (* open)(rm_littlefs_ctrl_t * const p_ctrl, rm_littlefs_cfg_t const * const p_cfg);

    /** Closes the module and lower level storage device.
     * @par Implemented as
     * - @ref RM_LITTLEFS_FLASH_Close
     *
     * @param[in]   p_ctrl             Control block set in @ref rm_littlefs_api_t::open call.
     */
    fsp_err_t (* close)(rm_littlefs_ctrl_t * const p_ctrl);
} rm_littlefs_api_t;

/** This structure encompasses everything that is needed to use an instance of this interface. */
typedef struct st_rm_littlefs_instance
{
    rm_littlefs_ctrl_t      * p_ctrl;  ///< Pointer to the control structure for this instance
    rm_littlefs_cfg_t const * p_cfg;   ///< Pointer to the configuration structure for this instance
    rm_littlefs_api_t const * p_api;   ///< Pointer to the API structure for this instance
} rm_littlefs_instance_t;

/* Common macro for SSP header files. There is also a corresponding SSP_HEADER macro at the top of this file. */
FSP_FOOTER

/*******************************************************************************************************************//**
 * @} (end defgroup RM_LITTLEFS_API)
 **********************************************************************************************************************/

#endif                                 /* RM_LITTLEFS_API_H */
