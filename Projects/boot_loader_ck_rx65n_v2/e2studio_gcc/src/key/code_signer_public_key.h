/*
* Copyright (c) 2023-2025 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/
/**********************************************************************************************************************
* File Name    : code_signer_public_key.h
* Description  : Define public key information for code signer.
***********************************************************************************************************************
* History : DD.MM.YYYY Version Description
*         : 20.07.2023 2.00    First Release
*         : 29.09.2023 2.01    Fixed log messages.
*                              Add parameter checking.
*                              Added arguments to R_FWUP_WriteImageProgram API.
*         : 28.03.2024 2.02    Update wrapper functions.
***********************************************************************************************************************/
#ifndef CODE_SIGNER_PUBLIC_KEY_H_
#define CODE_SIGNER_PUBLIC_KEY_H_

/*
 * PEM-encoded code signer public key.
 *
 * Must include the PEM header and footer:
 * "-----BEGIN PUBLIC KEY-----"\
 * "...base64 data..."\
 * "-----END PUBLIC KEY-----"
 */
#define CODE_SIGNER_PUBLIC_KEY_PEM \
"-----BEGIN PUBLIC KEY-----"\
""\
""\
"-----END PUBLIC KEY-----"

#endif /* CODE_SIGNER_PUBLIC_KEY_H_ */
