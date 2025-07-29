/*
 * FreeRTOS OTA PAL for Renesas RX65N V1.0.0
 * Copyright (C) 2021 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 * Modifications Copyright (C) 2023-2025 Renesas Electronics Corporation or its affiliates.
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
 *
 * http://aws.amazon.com/freertos
 * http://www.FreeRTOS.org
 */

/* C Runtime includes. */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* FreeRTOS include. */
#include "FreeRTOS.h"

/* OTA PAL Port include. */
#include "ota_pal.h"
#include "MQTTFileDownloader_config.h"

/* Renesas RX Driver Package include */
#include "platform.h"
#include "r_fwup_if.h"
#include "r_fwup_private.h"
#include "./src/targets/rx65n/r_flash_rx65n.h"
#include "mbedtls/ecdsa.h"
#include "mbedtls/asn1.h"
#include "mbedtls/error.h"

#define MAX_LENGTH      (32)
#define MAX_SIG_LENGTH  (64)
#define HALF_SIG_LENGTH (MAX_SIG_LENGTH/2)

const char OTA_JsonFileSignatureKey[OTA_FILE_SIG_KEY_STR_MAX_LENGTH] = "sig-sha256-ecdsa";
static OtaImageState_t OtaImageState;
uint32_t receiving_count = 0;
BaseType_t first_block_received = pdFALSE;
uint8_t * first_ota_blocks[mqttFileDownloader_MAX_NUM_BLOCKS_REQUEST];

AfrOtaJobDocumentFields_t * pOTAFileContext = NULL;

static int ExtractECDSASignature (const unsigned char * derSignature, size_t derSignatureLength, unsigned char * rawSignature);

/* Function Name: otaPal_CreateFileForRx */
/******************************************************************************************************************//**
 * @brief Creates OTA file for receiving
 * @param[in] pFileContext
 * @return OTA Job processing result
 * @retval OtaPalJobDocFileCreateFailed
 * @retval OtaPalJobDocProcessingStateInvalid
 * @retval OtaPalJobDocFileCreated
 *********************************************************************************************************************/
OtaPalJobDocProcessingResult_t otaPal_CreateFileForRx(AfrOtaJobDocumentFields_t * const pFileContext)
{
    OtaPalJobDocProcessingResult_t eResult = OtaPalJobDocProcessingStateInvalid;

    static uint8_t hdl;
    pFileContext->fileId = hdl;

    receiving_count      = 0;
    first_block_received = pdFALSE;

    for (uint8_t i = 0; i < mqttFileDownloader_MAX_NUM_BLOCKS_REQUEST; i++)
    {
        first_ota_blocks[i] = NULL;
    }

    if (FWUP_SUCCESS != R_FWUP_Open())
    {
        eResult = OtaPalJobDocFileCreateFailed;
        LogError(("otaPal_CreateFileForRx: failed!"));
    }
    else
    {
        OtaImageState = OtaImageStateUnknown;
        eResult       = OtaPalJobDocFileCreated;
    }

    LogDebug(("otaPal_CreateFileForRx: receives %d data blocks at the same time", mqttFileDownloader_MAX_NUM_BLOCKS_REQUEST));

    return eResult;
}
/**********************************************************************************************************************
 End of function otaPal_CreateFileForRx
 *********************************************************************************************************************/

/* Function Name: otaPal_WriteBlock */
/******************************************************************************************************************//**
 * @brief Write OTA data blocks
 * @param[in] pFileContext
 * @param[in] ulOffset
 * @param[in] pData
 * @param[in] ulBlockSize
 * @return Size of written block
 * @retval 0
 * @retval ulBlockSize
 *********************************************************************************************************************/
int16_t otaPal_WriteBlock(AfrOtaJobDocumentFields_t * const pFileContext,
                           uint32_t ulOffset,
                           uint8_t * const pData,
                           uint32_t ulBlockSize)
{
    (void) pFileContext;

    e_fwup_err_t eResult = FWUP_SUCCESS;

    uint16_t usBlockIndx = ulOffset/ulBlockSize;

    LogDebug(("otaPal_WriteBlock: receives OTA block #%d with size = %d!", usBlockIndx, ulBlockSize));

    if (0 == ulOffset)
    {
        R_FWUP_Close();
        R_FWUP_Open();

        R_FWUP_EraseArea(FWUP_AREA_BUFFER);

        first_block_received = pdTRUE;
    }

    if ((ulBlockSize % FLASH_CF_MIN_PGM_SIZE) != 0)
    {
        uint32_t  paddingsize = FLASH_CF_MIN_PGM_SIZE*((int32_t)(ulBlockSize/FLASH_CF_MIN_PGM_SIZE)+1); // cast to int32_t
        uint8_t * pBuffTmp    = pvPortMalloc(paddingsize);
        memset(pBuffTmp, 0xFF, paddingsize);
        (void)memcpy(pBuffTmp, pData, ulBlockSize);
 
        eResult = R_FWUP_WriteImageProgram(FWUP_AREA_BUFFER, pBuffTmp,
                ulOffset + sizeof(st_fw_header_t),
                paddingsize);
        vPortFree(pBuffTmp);
 
    }
    else
    {
    /* Calculate the offset from top of RSU file */
    uint32_t rsu_offset = ulOffset+sizeof(st_fw_header_t);

    eResult = R_FWUP_WriteImageProgram(FWUP_AREA_BUFFER,
                            pData, rsu_offset, ulBlockSize);
    }
    
    if ((FWUP_ERR_FLASH == eResult))
    {
        LogDebug(("otaPal_WriteBlock: index = %d, NG, error = %d\r\n", usBlockIndx, eResult));
        return 0;
    }
    LogDebug (("otaPal_WriteBlock: index = %d, OK, %d bytes\r\n", usBlockIndx, ulBlockSize));
    return (int16_t)ulBlockSize; // casting to the correct data type for return value
}
/**********************************************************************************************************************
 End of function otaPal_WriteBlock
 *********************************************************************************************************************/

/* Function Name: otaPal_CheckFileSignature */
/******************************************************************************************************************//**
 * @brief Verify the signature of the received file
 * @param[in] pFileContext
 * @return Signature verification result
 * @retval OtaPalNullFileContext
 * @retval OtaPalSignatureCheckFailed
 * @retval OtaPalBadSignerCert
 * @retval OtaPalSuccess
 *********************************************************************************************************************/
static OtaPalStatus_t otaPal_CheckFileSignature(AfrOtaJobDocumentFields_t * const pFileContext)
{
    OtaPalStatus_t eResult = OtaPalUninitialized;
    e_fwup_err_t   eRet    = FWUP_SUCCESS;

    /* Buffer to hold the raw ECDSA signature */
    unsigned char rawSignature[MAX_SIG_LENGTH];

    pOTAFileContext = pFileContext; // store the OTA file context to be used by FWUP verify wrapper

    /*
     * C->pxSignature->ucData includes some ASN1 tags.
     * 30 44 02 20 32byte(R) 02 20 32byte(S)  <- need to remove ^.
     * ^^^^^^^^^^^           ^^^^^
     * <tag><size><tag><size1><R-byte-sequence><tag><size2><S-byte-sequence>
     */

    if (NULL == pFileContext)
    {
        LogError(("otaPal_CheckFileSignature: null file context"));
        return OtaPalNullFileContext;
    }

    if (NULL == pFileContext->signature)
    {
        LogError(("otaPal_CheckFileSignature: null file signature"));
        return OtaPalSignatureCheckFailed;
    }

    LogDebug(("otaPal_CheckFileSignature: signature size in OTA file context = %d", pFileContext->signatureLen));

    if (0 >= pFileContext->signatureLen)
    {
        LogError(("otaPal_CheckFileSignature: file signature size is invalid"));
        return OtaPalSignatureCheckFailed;
    }


    if (0 != ExtractECDSASignature((const unsigned char*)pFileContext->signature, // cast to const unsigned char*
                                    pFileContext->signatureLen, rawSignature))
    {
        eResult = OtaPalBadSignerCert;
        LogError(("Error ECDSASignature extraction\r\n"));
        return eResult;
    }

    eRet = R_FWUP_WriteImageHeader(FWUP_AREA_BUFFER,
                                (uint8_t FWUP_FAR *)OTA_JsonFileSignatureKey, rawSignature, MAX_SIG_LENGTH);

    if (FWUP_SUCCESS != eRet)
    {
        eResult = OtaPalBadSignerCert;
        LogError(("otaPal_CheckFileSignature: R_FWUP_WriteImageHeader returns error = %d\r\n", eRet));

        return eResult;
    }


    /* Verify the signature */
    eRet = R_FWUP_VerifyImage(FWUP_AREA_BUFFER);

    if (FWUP_SUCCESS != eRet)
    {
        eResult = OtaPalSignatureCheckFailed;
        LogError(("otaPal_CheckFileSignature: R_FWUP_VerifyImage returns error = %d\r\n", eRet));

        return eResult;
    }
    else
    {
        eResult = OtaPalSuccess;
    }

    return eResult;
}
/**********************************************************************************************************************
 End of function otaPal_CheckFileSignature
 *********************************************************************************************************************/

/* Function Name: otaPal_CloseFile */
/******************************************************************************************************************//**
 * @brief Close OTA file
 * @param[in] pFileContext
 * @return Close file result
 * @retval OtaPalNullFileContext
 * @retval OtaPalSignatureCheckFailed
 * @retval OtaPalBadSignerCert
 * @retval OtaPalSuccess
 *********************************************************************************************************************/
OtaPalStatus_t otaPal_CloseFile(AfrOtaJobDocumentFields_t * const pFileContext)
{
    OtaPalStatus_t eResult = OtaPalSuccess;

    eResult = otaPal_CheckFileSignature(pFileContext);
    if (OtaPalSuccess != eResult)
    {
        OtaImageState = OtaImageStateRejected;
    }
    else
    {
        OtaImageState = OtaImageStateTesting;
    }

    if (pFileContext != NULL)
    {
        pFileContext->fileId = 0;
    }

    R_FWUP_Close();

    return eResult;
}
/**********************************************************************************************************************
 End of function otaPal_CloseFile
 *********************************************************************************************************************/

/* Function Name: otaPal_CloseFileNoSignatureCheck */
/******************************************************************************************************************//**
 * @brief Close OTA file without verifying the signature
 * @param[in] pFileContext
 * @return Close file result
 * @retval OtaPalSuccess
 *********************************************************************************************************************/
OtaPalStatus_t otaPal_CloseFileNoSignatureCheck(AfrOtaJobDocumentFields_t * const pFileContext)
{
    OtaPalStatus_t eResult = OtaPalSuccess;
    pFileContext->fileId = 0;

    R_FWUP_Close();

    return eResult;
}
/**********************************************************************************************************************
 End of function otaPal_CloseFileNoSignatureCheck
 *********************************************************************************************************************/


/* Function Name: otaPal_ResetDevice */
/******************************************************************************************************************//**
 * @brief Reset the device
 * @param[in] pFileContext
 * @return Reset result
 * @retval OtaPalSuccess
 *********************************************************************************************************************/
OtaPalStatus_t otaPal_ResetDevice(AfrOtaJobDocumentFields_t * const pFileContext)
{
    (void) pFileContext;

    R_FWUP_SoftwareReset();
    return OtaPalSuccess;
}
/**********************************************************************************************************************
 End of function otaPal_ResetDevice
 *********************************************************************************************************************/

/* Function Name: otaPal_ActivateNewImage */
/******************************************************************************************************************//**
 * @brief Activate the new OTA image
 * @param[in] pFileContext
 * @return Activation result
 * @retval OtaPalSuccess
 * @retval OtaPalActivateFailed
 *********************************************************************************************************************/
OtaPalStatus_t otaPal_ActivateNewImage(AfrOtaJobDocumentFields_t * const pFileContext)
{
    (void) pFileContext;

    e_fwup_err_t eResult = R_FWUP_ActivateImage();

    if (FWUP_SUCCESS != eResult)
    {
        return OtaPalActivateFailed;
    }

    R_FWUP_SoftwareReset();
    return OtaPalSuccess;
}
/**********************************************************************************************************************
 End of function otaPal_ActivateNewImage
 *********************************************************************************************************************/

/* Function Name: otaPal_Abort */
/******************************************************************************************************************//**
 * @brief Abort the OTA process
 * @param[in] pFileContext
 * @return Abortion result
 * @retval OtaPalSuccess
 * @retval OtaPalFileClose
 *********************************************************************************************************************/
OtaPalStatus_t otaPal_Abort(AfrOtaJobDocumentFields_t * const pFileContext)
{
    OtaPalStatus_t eResult = OtaPalSuccess;

    if (NULL == pFileContext)
    {
        eResult = OtaPalFileClose;
    }

    if (OtaPalSuccess == eResult)
    {
        pFileContext->fileId = 0;
    }

    return eResult;

}
/**********************************************************************************************************************
 End of function otaPal_Abort
 *********************************************************************************************************************/

/* Function Name: otaPal_SetPlatformImageState */
/******************************************************************************************************************//**
 * @brief Set the state of the image
 * @param[in] pFileContext
 * @param[in] eState
 * @return Setting result
 * @retval OtaPalSuccess
 * @retval OtaPalBadImageState
 * @retval OtaPalCommitFailed
 *********************************************************************************************************************/
OtaPalStatus_t otaPal_SetPlatformImageState(AfrOtaJobDocumentFields_t * const pFileContext,
                                             OtaImageState_t eState)
{
    (void) pFileContext;

    OtaPalStatus_t eResult = OtaPalUninitialized;

    if (OtaImageStateTesting == OtaImageState)
    {
        switch (eState)
        {
            case OtaImageStateAccepted:
                R_FWUP_Close();
                R_FWUP_Open();
                LogInfo(("Accepted and committed final image."));
                eResult = OtaPalSuccess;
                break;

            case OtaImageStateRejected:
                LogInfo(("Rejected image."));
                eResult = OtaPalSuccess;
                break;

            case OtaImageStateAborted:
                LogInfo(("Aborted image."));
                eResult = OtaPalSuccess;
                break;

            case OtaImageStateTesting:
                LogInfo(("Testing."));
                eResult = OtaPalSuccess;
                break;

            default:
                LogError(("Unknown state received %d.", (int32_t)eState)); // cast eState to int32_t
                eResult = OtaPalBadImageState;
                break;
        }
    }
    else
    {
        switch (eState)
        {
            case OtaImageStateAccepted:
                LogError(("Not in commit pending so can not mark image valid (%d)."));
                eResult = OtaPalCommitFailed;
                break;

            case OtaImageStateRejected:
                LogInfo(("Rejected image."));
                eResult = OtaPalSuccess;
                break;

            case OtaImageStateAborted:
                LogInfo(("Aborted image."));
                eResult = OtaPalSuccess;
                break;

            case OtaImageStateTesting:
                LogInfo(("Testing."));
                eResult = OtaPalSuccess;
                break;

            default:
                LogError(("Unknown state received %d.", (int32_t)eState)); // cast eState to int32_t
                eResult = OtaPalBadImageState;
                break;
        }
    }

    OtaImageState = eState;

    return eResult;

}
/**********************************************************************************************************************
 End of function otaPal_SetPlatformImageState
 *********************************************************************************************************************/

/* Function Name: otaPal_GetPlatformImageState */
/******************************************************************************************************************//**
 * @brief Get the state of the image
 * @param[in] pFileContext
 * @return The image state
 * @retval OtaPalImageStatePendingCommit
 * @retval OtaPalImageStateValid
 * @retval OtaPalImageStateUnknown
 *********************************************************************************************************************/
OtaPalImageState_t otaPal_GetPlatformImageState(AfrOtaJobDocumentFields_t * const pFileContext)
{
    (void) pFileContext;

    OtaPalImageState_t ePalState = OtaPalImageStateUnknown;

    switch (OtaImageState)
    {
        case OtaImageStateTesting:
            ePalState = OtaPalImageStatePendingCommit;
            break;

        case OtaImageStateAccepted:
            ePalState = OtaPalImageStateValid;
            break;

        case OtaImageStateUnknown: /* Uninitialized image state, assume a factory image */
            ePalState = OtaPalImageStateValid;
            break;

        case OtaImageStateRejected:
        case OtaImageStateAborted:
        default:
            ePalState = OtaPalImageStateInvalid;
            break;
    }

    LogDebug(("Function called is otaPal_GetPlatformImageState: Platform State is [%d]", ePalState));
    return ePalState;
}
/**********************************************************************************************************************
 End of function otaPal_GetPlatformImageState
 *********************************************************************************************************************/

/* Function Name: ExtractECDSASignature */
/******************************************************************************************************************//**
 * @brief .
 * @param[in]  derSignature
 * @param[in]  derSignatureLength
 * @param[out] rawSignature
 * @return Extraction result
 * @retval .
 *********************************************************************************************************************/
static int ExtractECDSASignature(const unsigned char *derSignature, size_t derSignatureLength, unsigned char *rawSignature)
{
    unsigned char *       p   = (unsigned char*) derSignature; // cast to unsigned char
    const unsigned char * end = derSignature+derSignatureLength;
    int                   ret = MBEDTLS_ERR_ERROR_CORRUPTION_DETECTED;

    size_t len;

    mbedtls_mpi r;
    mbedtls_mpi s;

    /* Start reusing the process of mbedtls_ecdsa_read_signature_restartable function */

    /* Check the parameters. */
    configASSERT(NULL != derSignature);
    mbedtls_mpi_init(&r);
    mbedtls_mpi_init(&s);

    if (0 != (ret = mbedtls_asn1_get_tag(&p, end, &len, MBEDTLS_ASN1_CONSTRUCTED | MBEDTLS_ASN1_SEQUENCE)))
    {
    ret += MBEDTLS_ERR_ECP_BAD_INPUT_DATA;
        goto cleanup;
    }

    if ((p + len) != end)
    {
        ret = MBEDTLS_ERROR_ADD(MBEDTLS_ERR_ECP_BAD_INPUT_DATA,
                                MBEDTLS_ERR_ASN1_LENGTH_MISMATCH);
    goto cleanup;
    }

    /* Get R,S component */
    if ((0 != (ret = mbedtls_asn1_get_mpi(&p, end, &r))) ||
        (0 != (ret = mbedtls_asn1_get_mpi(&p, end, &s))))
    {
        ret += MBEDTLS_ERR_ECP_BAD_INPUT_DATA;
        goto cleanup;
    }

    /* Finished reusing the process of mbedtls_ecdsa_read_signature_restartable function */

    /* Convert MPIs to raw byte strings
       The raw ECDSA signature in rawSignature */
    ret = mbedtls_mpi_write_binary(&r, &rawSignature[0], HALF_SIG_LENGTH);
    if (0 != ret)
    {
        goto cleanup;
    }
    ret = mbedtls_mpi_write_binary(&s, &rawSignature[HALF_SIG_LENGTH], HALF_SIG_LENGTH);
    if (0 != ret)
    {
        goto cleanup;
    }

    /* Start reusing the process of mbedtls_ecdsa_read_signature_restartable function */

    /* At this point we know that the buffer starts with a valid signature.
    * Return 0 if the buffer just contains the signature, and a specific
    * error code if the valid signature is followed by more data. */
    if (p != end)
    {
        ret = MBEDTLS_ERR_ECP_SIG_LEN_MISMATCH;
    }

cleanup:
    mbedtls_mpi_free(&r);
    mbedtls_mpi_free(&s);

    /* Finished reusing the process of mbedtls_ecdsa_read_signature_restartable function */

    return ret;
}
/**********************************************************************************************************************
 End of function ExtractECDSASignature
 *********************************************************************************************************************/

/* Function Name: otaPal_EraseArea */
/******************************************************************************************************************//**
 * @brief Erase FWUP area
 * @param[in] area
 * @return The erasure result
 * @retval OtaPalEraseFailed
 * @retval OtaPalSuccess
 *********************************************************************************************************************/
OtaPalStatus_t otaPal_EraseArea(uint8_t area)
{
    OtaPalStatus_t eResult      = OtaPalEraseFailed;
    e_fwup_err_t   eEraseResult = FWUP_ERR_FAILURE;

    eEraseResult = R_FWUP_EraseArea((e_fwup_area_t)area); // cast to e_fwup_area_t
    if (FWUP_SUCCESS == eEraseResult)
    {
        eResult = OtaPalSuccess;
    }
    else
    {
        LogError(("Error R_FWUP_EraseArea()\r\n"));
    }

    return eResult;
}
/**********************************************************************************************************************
 End of function otaPal_EraseArea
 *********************************************************************************************************************/
