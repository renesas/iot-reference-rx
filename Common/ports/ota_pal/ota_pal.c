/*
 * FreeRTOS OTA PAL for Renesas RX65N-RSK V2.0.0
 * Copyright (C) 2021 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 * Modifications Copyright (C) 2023 Renesas Electronics Corporation. or its affiliates.
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

/* OTA library includes. */
#include "ota.h"
#include "ota_private.h"

/* OTA PAL Port include. */
#include "ota_pal.h"

/* Renesas RX Driver Package include */
#include "platform.h"
#include "r_fwup_if.h"
#include "r_fwup_private.h"
#include "./src/targets/rx65n/r_flash_rx65n.h"

const char OTA_JsonFileSignatureKey[ OTA_FILE_SIG_KEY_STR_MAX_LENGTH ] = "sig-sha256-ecdsa";
static OtaImageState_t OtaImageState;
uint32_t s_receiving_count = 0;
BaseType_t s_first_block_received = pdFALSE;
uint8_t *s_first_ota_blocks[otaconfigMAX_NUM_BLOCKS_REQUEST];

OtaFileContext_t *pOTAFileContext;

OtaPalStatus_t otaPal_CreateFileForRx( OtaFileContext_t * const pFileContext )
{
    OtaPalMainStatus_t eResult = OtaPalUninitialized;

    static uint8_t hdl;
    pFileContext->pFile = &hdl;

    s_receiving_count = 0;
    s_first_block_received = pdFALSE;

    for (uint8_t i = 0; i < otaconfigMAX_NUM_BLOCKS_REQUEST; i++)
    {
    	s_first_ota_blocks[i] = NULL;
    }

    if (FWUP_SUCCESS != R_FWUP_Open())
    {
        eResult = OtaPalRxFileCreateFailed;
    }
    else
    {
        OtaImageState = OtaImageStateUnknown;
    	eResult = OtaPalSuccess;
    }

    LogDebug( ("otaPal_CreateFileForRx: receives %d data blocks at the same time", otaconfigMAX_NUM_BLOCKS_REQUEST) );

    return OTA_PAL_COMBINE_ERR( eResult, 0 );
}

int16_t otaPal_WriteBlock( OtaFileContext_t * const pFileContext,
                           uint32_t ulOffset,
                           uint8_t * const pData,
                           uint32_t ulBlockSize )
{
    ( void ) pFileContext;

    e_fwup_err_t eResult = FWUP_SUCCESS;

    uint16_t usBlockIndx = ulOffset/ulBlockSize;

    LogDebug( ("otaPal_WriteBlock: receives OTA block #%d with size = %d!", usBlockIndx, ulBlockSize) );

    if ( 0 == ulOffset )
    {
    	R_FWUP_Close();
    	R_FWUP_Open();

    	s_first_block_received = pdTRUE;
    }

    if (ulBlockSize % FLASH_CF_MIN_PGM_SIZE != 0)
    {
    	paddingsize = FLASH_CF_MIN_PGM_SIZE*((int32_t)(ulBlockSize/FLASH_CF_MIN_PGM_SIZE) + 1);
		uint8_t *pBuffTmp = pvPortMalloc( paddingsize );
		memset(pBuffTmp, 0xFF, paddingsize);
		(void)memcpy(pBuffTmp, pData, ulBlockSize);
 
		eResult = R_FWUP_WriteImageProgram(FWUP_AREA_BUFFER, pBuffTmp,
				ulOffset+sizeof(st_fw_header_t),
				paddingsize);
		vPortFree( pBuffTmp );
 
    }
    else
    {

#if (otaconfigMAX_NUM_BLOCKS_REQUEST > 1)
    // first received blocks is not block #0
    if ( (pdTRUE != s_first_block_received ) && (s_receiving_count < otaconfigMAX_NUM_BLOCKS_REQUEST) )
    {
		LogDebug( ("otaPal_WriteBlock: block #%d is received before block #0!", usBlockIndx) );

		// Store the blocks in order to write them later
		s_first_ota_blocks[usBlockIndx] = pvPortMalloc( ulBlockSize );
				(void)memcpy( s_first_ota_blocks[usBlockIndx], pData, ulBlockSize );

		s_receiving_count++;
		vPortFree( s_first_ota_blocks[usBlockIndx] );
		return ulBlockSize;
    }

	if (ulOffset == 0)
	{
    	eResult = R_FWUP_WriteImageProgram(FWUP_AREA_BUFFER, pData,
    				sizeof(st_fw_header_t), ulBlockSize);
		LogDebug( ("otaPal_WriteBlock: write block #0 returns %d!", eResult) );

    	if (s_receiving_count > 0) // there are stored blocks
    	{
        	for (uint8_t i = 1; i < otaconfigMAX_NUM_BLOCKS_REQUEST; i++)
        	{
        		if ( NULL != s_first_ota_blocks[i] )
        		{
    				eResult = R_FWUP_WriteImageProgram(FWUP_AREA_BUFFER,
    										s_first_ota_blocks[i],
    										sizeof(st_fw_header_t) + i*OTA_FILE_BLOCK_SIZE,
    										OTA_FILE_BLOCK_SIZE);
    				LogDebug( ("otaPal_WriteBlock: R_FWUP_WriteImageProgram() write block #%d returns %d!", i, eResult) );
        		}
        	}
    	}

    	return ulBlockSize;
	}
#endif // (otaconfigMAX_NUM_BLOCKS_REQUEST > 1)

    // Calculate the offset from top of RSU file
    uint32_t rsu_offset = ulOffset + sizeof(st_fw_header_t);

    eResult = R_FWUP_WriteImageProgram(FWUP_AREA_BUFFER,
    						pData, rsu_offset, ulBlockSize);
    }
    if ( ( FWUP_ERR_FLASH == eResult ) )
    {
    	LogDebug( ("otaPal_WriteBlock: index = %d, NG, error = %d\r\n", usBlockIndx, eResult) );
        return 0;
    }
    LogDebug ( ("otaPal_WriteBlock: index = %d, OK, %d bytes\r\n", usBlockIndx, ulBlockSize) );
    return ulBlockSize;
}

static OtaPalStatus_t otaPal_CheckFileSignature( OtaFileContext_t * const pFileContext )
{
	OtaPalMainStatus_t eResult = OtaPalUninitialized;
	e_fwup_err_t eRet = FWUP_SUCCESS;

	pOTAFileContext = pFileContext; // store the OTA file context to be used by FWUP verify wrapper

	// extract the signature for verification by bootloader
    uint8_t sig[64];
    uint16_t idx;

    /*
     * C->pxSignature->ucData includes some ASN1 tags.
     * 30 44 02 20 32byte(R) 02 20 32byte(S)  <- need to remove ^.
     * ^^^^^^^^^^^           ^^^^^
     * <tag><size><tag><size1><R-byte-sequence><tag><size2><S-byte-sequence>
     */

    if (NULL == pFileContext)
    {
    	LogError( ("otaPal_CheckFileSignature: null file context") );
    	return OTA_PAL_COMBINE_ERR( OtaPalNullFileContext, 0 );
    }

    if ( NULL == pFileContext->pSignature )
    {
    	LogError( ("otaPal_CheckFileSignature: null file signature") );
		return OTA_PAL_COMBINE_ERR( OtaPalSignatureCheckFailed, 0 );
    }

    LogDebug( ("otaPal_CheckFileSignature: signature size in OTA file context = %d", pFileContext->pSignature->size) );

    if (0 >= pFileContext->pSignature->size )
    {
    	LogError( ("otaPal_CheckFileSignature: file signature size is invalid") );
    	return OTA_PAL_COMBINE_ERR( OtaPalSignatureCheckFailed, 0 );
    }

    // Extract signature ASN1
    // SIG(R)
    idx = 3;
    if (0x21 == pFileContext->pSignature->data[idx++])
    {
        idx++; /* Skip 0x00 */
    }
    memcpy(sig, &pFileContext->pSignature->data[idx], 32);

    /* SIG(S) */
    idx += 32;
    idx++;
    if (0x21 == pFileContext->pSignature->data[idx++])
    {
        idx++; /* Skip 0x00 */
    }
    memcpy(sig+32, &pFileContext->pSignature->data[idx], 32);

    eRet = R_FWUP_WriteImageHeader( FWUP_AREA_BUFFER,
                                (uint8_t FWUP_FAR *)OTA_JsonFileSignatureKey, sig, 64);

    if ( FWUP_SUCCESS != eRet )
    {
        eResult = OtaPalBadSignerCert;
        LogError( ("otaPal_CheckFileSignature: R_FWUP_WriteImageHeader returns error = %d\r\n", eRet) );

        return OTA_PAL_COMBINE_ERR( eResult, 0 );
    }

    // Verify the signature
	eRet = R_FWUP_VerifyImage(FWUP_AREA_BUFFER);

    if (FWUP_SUCCESS != eRet)
    {
        eResult = OtaPalSignatureCheckFailed;
        LogError( ("otaPal_CheckFileSignature: R_FWUP_VerifyImage returns error = %d\r\n", eRet) );

        return OTA_PAL_COMBINE_ERR( eResult, 0 );
    }
    else {
    	eResult = OtaPalSuccess;
    }

	return OTA_PAL_COMBINE_ERR( eResult, 0 );
}

OtaPalStatus_t otaPal_CloseFile( OtaFileContext_t * const pFileContext )
{
    OtaPalMainStatus_t eResult = OtaPalSuccess;

    eResult = OTA_PAL_MAIN_ERR( otaPal_CheckFileSignature( pFileContext ) );
    if ( eResult == OtaPalSuccess )
    {
        OtaPalImageState = OtaPalImageStatePendingCommit;
    }
    else
    {
        OtaImageState = OtaImageStateRejected;
    }

    pFileContext->pFile = NULL;

    R_FWUP_Close();

    return OTA_PAL_COMBINE_ERR( eResult, 0 );
}

OtaPalStatus_t otaPal_ResetDevice( OtaFileContext_t * const pFileContext )
{
    ( void ) pFileContext;

    if ( ( OtaImageStateAccepted == OtaImageState ) ||
		( OtaImageStateTesting == OtaImageState ) )
	{
    	R_FWUP_SoftwareReset();
	}
    else
    {
    	// If the status is rejected, aborted, or error, swap bank and return to the previous image
    	// R_FWUP_ActivateImage();
    	R_FWUP_SoftwareReset();
    }


    return OTA_PAL_COMBINE_ERR( OtaPalSuccess, 0 );
}

OtaPalStatus_t otaPal_ActivateNewImage( OtaFileContext_t * const pFileContext )
{
    ( void ) pFileContext;

    if (FWUP_SUCCESS != R_FWUP_ActivateImage())
    {
        return OTA_PAL_COMBINE_ERR( OtaPalActivateFailed, 0 );
    }

    OtaPalImageState = OtaPalImageStateValid;
    R_FWUP_SoftwareReset();
    return OTA_PAL_COMBINE_ERR( OtaPalSuccess, 0 );
}

OtaPalStatus_t otaPal_Abort( OtaFileContext_t * const pFileContext )
{
    OtaPalMainStatus_t eResult = OtaPalSuccess;

    if ( pFileContext == NULL ) {
        eResult = OtaPalFileClose;
    }

    if ( eResult == OtaPalSuccess ) {
        pFileContext->pFile = NULL;
    }

    return OTA_PAL_COMBINE_ERR( eResult, 0 );

}

OtaPalStatus_t otaPal_SetPlatformImageState( OtaFileContext_t * const pFileContext,
                                             OtaImageState_t eState )
{
    ( void ) pFileContext;

    OtaPalMainStatus_t eResult = OtaPalUninitialized;

    if (OtaImageStateTesting == OtaImageState )
    {
    	switch( eState )
    	{
			case OtaImageStateAccepted:
				R_FWUP_Close();
				R_FWUP_Open();
				LogInfo( ( "Accepted and committed final image." ) );
				eResult = OtaPalSuccess;
				break;

			case OtaImageStateRejected:
                LogInfo( ( "Rejected image." ) );
                eResult = OtaPalSuccess;
				break;

			case OtaImageStateAborted:
                LogInfo( ( "Aborted image." ) );
                eResult = OtaPalSuccess;
				break;

			case OtaImageStateTesting:
                LogInfo( ( "Testing." ) );
                eResult = OtaPalSuccess;
				break;

			default:
                LogError( ( "Unknown state received %d.", ( int32_t ) eState ) );
                eResult = OtaPalBadImageState;
				break;
    	}
    }
    else
    {
    	switch( eState )
		{
			case OtaImageStateAccepted:
                LogError( ( "Not in commit pending so can not mark image valid (%d)." ) );
                eResult = OtaPalCommitFailed;
                break;

			case OtaImageStateRejected:
                LogInfo( ( "Rejected image." ) );
                eResult = OtaPalSuccess;
                break;

			case OtaImageStateAborted:
                LogInfo( ( "Aborted image." ) );
                eResult = OtaPalSuccess;
                break;

			case OtaImageStateTesting:
                LogInfo( ( "Testing." ) );
                eResult = OtaPalSuccess;
                break;

			default:
                LogError( ( "Unknown state received %d.", ( int32_t ) eState ) );
                eResult = OtaPalBadImageState;
                break;
		}
    }

    OtaImageState = eState;

    return OTA_PAL_COMBINE_ERR( eResult, 0 );

}

OtaPalImageState_t otaPal_GetPlatformImageState( OtaFileContext_t * const pFileContext )
{
    ( void ) pFileContext;

    OtaPalImageState_t ePalState = OtaPalImageStateUnknown;

    switch( OtaImageState )
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

    LogDebug( ( "Function called is otaPal_GetPlatformImageState: Platform State is [%d]", ePalState ) );
    return ePalState;
}
