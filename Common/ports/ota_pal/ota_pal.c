/*
 * FreeRTOS OTA PAL for Renesas RX65N-RSK V2.0.0
 * Copyright (C) 2021 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
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


const char OTA_JsonFileSignatureKey[ OTA_FILE_SIG_KEY_STR_MAX_LENGTH ] = "sig-sha256-ecdsa";
static OtaPalImageState_t OtaPalImageState;

OtaPalStatus_t otaPal_CreateFileForRx( OtaFileContext_t * const pFileContext )
{
    OtaPalMainStatus_t eResult = OtaPalUninitialized;

    static uint8_t hdl;
    pFileContext->pFile = &hdl;
    if (FWUP_SUCCESS != R_FWUP_Open())
    {
        eResult = OtaPalRxFileCreateFailed;
        OtaPalImageState = OtaPalImageStateUnknown;
    }
    else
    {
    	eResult = OtaPalSuccess;
    }

    return OTA_PAL_COMBINE_ERR( eResult, 0 );
}

int16_t otaPal_WriteBlock( OtaFileContext_t * const pFileContext,
                           uint32_t ulOffset,
                           uint8_t * const pData,
                           uint32_t ulBlockSize )
{
    ( void ) pFileContext;
    ( void ) ulOffset;

    LogInfo( ("otaPal_WriteBlock: write OTA block at offset 0x%X!", ulOffset) );

    if ( 0 == ulOffset )
    {
    	R_FWUP_Close();
    	R_FWUP_Open();
    }

    e_fwup_err_t eResult = R_FWUP_WriteImageProgram(FWUP_AREA_BUFFER, pData, ulBlockSize);

    if ( ( FWUP_SUCCESS != eResult ) && ( FWUP_PROGRESS != eResult ) )
    {
    	LogError( ("otaPal_WriteBlock: index = %d, NG, error = %d\r\n", ulOffset/OTA_FILE_BLOCK_SIZE, eResult) );
        return 0;
    }
    LogDebug ( ("otaPal_WriteBlock: index = %d, OK, %d bytes\r\n", ulOffset/OTA_FILE_BLOCK_SIZE, ulBlockSize) );
    return ulBlockSize;
}

static OtaPalStatus_t otaPal_CheckFileSignature( OtaFileContext_t * const pFileContext )
{
    uint8_t sig[64];
    OtaPalMainStatus_t eResult = OtaPalUninitialized;
    uint8_t size, size1, size2, r_first_byte, s_first_byte;
    e_fwup_err_t eRet = FWUP_SUCCESS;

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

    size = *( pFileContext->pSignature->data + 1 );
    LogDebug( ("otaPal_CheckFileSignature: signature length from the byte sequence = %d", size) );

    if ( size <= 0 )
	{
    	LogError( ("otaPal_CheckFileSignature: Invalid byte sequence, length = %d", size) );
    	return OTA_PAL_COMBINE_ERR( OtaPalSignatureCheckFailed, 0 );
	}

    size1 = *( pFileContext->pSignature->data + 3 );
    size2 = *( pFileContext->pSignature->data + 4 + size1 + 1);

    if ( ( 0 >= size1 ) || ( 0 >= size2 ) )
    {
    	LogError( ("otaPal_CheckFileSignature: Invalid byte sequence") );
    	return OTA_PAL_COMBINE_ERR( OtaPalSignatureCheckFailed, 0 );
    }

    // first by of 'r'
    r_first_byte = *( pFileContext->pSignature->data + 4 );

    // First by of 's'
    s_first_byte = *( pFileContext->pSignature->data + 3 + size1 + 2 + 1);

    LogDebug( ("otaPal_CheckFileSignature: size1=%d, first byte is %d", size1, r_first_byte) );
    if ( r_first_byte == 0x00 ) {
    	// remove first 0x00 padding byte
    	memcpy(sig, pFileContext->pSignature->data + 3 + 1 + 1, size1 - 1);
    }
    else { //no padding
    	memcpy(sig, pFileContext->pSignature->data + 3 + 1, size1);
    }

    LogDebug( ("otaPal_CheckFileSignature: size2=%d, first byte is %d", size2, s_first_byte) );
    if ( s_first_byte == 0x00 ) {
        // remove first 0x00 padding byte
    	memcpy(sig+32, pFileContext->pSignature->data + 3 + 1 + size1 + 2 + 1, size2 - 1);
    }
    else // no padding
    {
    	memcpy(sig+32, pFileContext->pSignature->data + 3 + 1 + size1 + 2, size2);
    }

    LogDebug( ("otaPal_CheckFileSignature: signature size=%d", size2+size1) );
    eRet = R_FWUP_WriteImageHeader( FWUP_AREA_BUFFER,
                                (uint8_t FWUP_FAR *)OTA_JsonFileSignatureKey, sig, 64);

    if ( FWUP_SUCCESS != eRet )
    {
        eResult = OtaPalBadSignerCert;
        LogError( ("otaPal_CheckFileSignature: R_FWUP_WriteImageHeader returns error = %d\r\n", eRet) );

        return OTA_PAL_COMBINE_ERR( eResult, 0 );
    }

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
        OtaPalImageState = OtaImageStateRejected;
    }

    pFileContext->pFile = NULL;

    R_FWUP_Close();

    return OTA_PAL_COMBINE_ERR( eResult, 0 );
}

OtaPalStatus_t otaPal_ResetDevice( OtaFileContext_t * const pFileContext )
{
    ( void ) pFileContext;

    if ( ( OtaImageStateAccepted == OtaPalImageState ) ||
		( OtaImageStateTesting == OtaPalImageState ) )
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
        eResult = OtaPalSuccess; // stub
    }

    pFileContext->pFile = NULL;

    return OTA_PAL_COMBINE_ERR( eResult, 0 );

}

OtaPalStatus_t otaPal_SetPlatformImageState( OtaFileContext_t * const pFileContext,
                                             OtaImageState_t eState )
{
    ( void ) pFileContext;

    OtaPalMainStatus_t eResult = OtaPalUninitialized;

    if (OtaImageStateTesting == OtaPalImageState )
    {
    	switch( eState )
    	{
			case OtaImageStateAccepted:
				R_FWUP_Close();
				R_FWUP_Open();
				if( R_FWUP_EraseArea(FWUP_AREA_BUFFER) == FWUP_SUCCESS)
				{
					LogInfo( ( "Erase install area (code flash): OK" ) );
					LogInfo( ( "Accepted and committed final image." ) );
					eResult = OtaPalSuccess;
				}
				else
				{
                    LogError( ( "Erase install area (code flash): NG" ) );
                    LogError( ( "Accepted final image but commit failed." ) );
                    eResult = OtaPalCommitFailed;
				}
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

    OtaPalImageState = eState;

    return OTA_PAL_COMBINE_ERR( eResult, 0 );

}

OtaPalImageState_t otaPal_GetPlatformImageState( OtaFileContext_t * const pFileContext )
{
    ( void ) pFileContext;

    OtaPalImageState_t ePalState = OtaPalImageStateUnknown;

    switch( OtaPalImageState )
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
