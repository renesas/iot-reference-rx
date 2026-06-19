/**
 * @file ota_pal_test.c
 * @brief Various tests for validating an implementation to the OTA PAL.
 */

#include "test_execution_config.h"
#if ( OTA_PAL_TEST_ENABLED == 1 )

/* Standard includes. */
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>

/* Unity Test framework include. */
#include "unity_fixture.h"
#include "unity.h"

/* Include OTA headers. */
#include "ota_appversion32.h"
#include "ota_pal.h"

#include "ota_pal_test.h"
#include "test_param_config.h"

#ifndef OTA_PAL_TEST_CERT_TYPE
    #error "Please define OTA_PAL_TEST_CERT_TYPE"
#endif

#if( OTA_PAL_TEST_CERT_TYPE == OTA_RSA_SHA1 )
    #include "aws_test_ota_pal_rsa_sha1_signature.h"
#elif( OTA_PAL_TEST_CERT_TYPE == OTA_RSA_SHA256 )
    #include "aws_test_ota_pal_rsa_sha256_signature.h"
#elif( OTA_PAL_TEST_CERT_TYPE == OTA_ECDSA_SHA256 )
    #include "aws_test_ota_pal_ecdsa_sha256_signature.h"
#else
    #error "Invalid OTA_PAL_TEST_CERT_TYPE"
#endif

/* For the otaPal_WriteBlock_WriteManyBlocks test this is the number of blocks
 * of ucDummyData to write to the non-volatile memory. */
#define testotapalNUM_WRITE_BLOCKS         10

/**
 * @ingroup ota_constants
 * @brief A composite cryptographic signature structure able to hold our largest supported signature.
 */
#define maxSignatureSize           384 /* Max bytes supported for a file signature (3072 bit RSA is 384 bytes). */

static OtaPalTestParam_t testParam;

/*
 * @brief: This dummy data is prepended by a SHA1 hash generated from the
 * rsa-sha1-signer certificate and keys in tests/common/ota/test_files.
 *
 * The RSA SHA256 signature and ECDSA 256 signature are generated from this
 * entire data block as is.
 */
static uint8_t ucDummyData[] =
{
		0x01, 0x00, 0x00, 0x00, 0x00, 0x03, 0xF0, 0xFF, 0x80, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF,
		0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
		0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
		0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
		0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
		0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
		0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
		0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
		0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
		0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
		0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
		0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
		0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
		0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
		0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
		0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
		0x83, 0x0B, 0xF0, 0x6A, 0x81, 0xD6, 0xCA, 0xD7, 0x08, 0x22, 0x0D, 0x6A, 0x33, 0xFA, 0x31, 0x9F,
		0xA9, 0x5F, 0xB5, 0x26, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0C,
		0x0C, 0x0D, 0x0E, 0x0F, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B,
		0x1C, 0x1D, 0x1E, 0x1F, 0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2A, 0x2B,
		0x2C, 0x2D, 0x2E, 0x2F, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3A, 0x3B,
		0x3C, 0x3D, 0x3E, 0x3F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
		0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
		0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF
};

/* Global static OTA file context used in every test. This variable is reset to
 * all zeros before every test. */
static AfrOtaJobDocumentFields_t xOtaFile;

/**
 * @brief Test group definition.
 */
TEST_GROUP( Full_OTA_PAL );

TEST_SETUP( Full_OTA_PAL )
{
    /* Reset the OTA file context before each test. */
    memset( &xOtaFile, 0, sizeof( xOtaFile ) );
}

TEST_TEAR_DOWN( Full_OTA_PAL )
{
    OtaPalStatus_t xOtaStatus;

    /* Abort the OTA file after every test. This closes the OTA file.
     * otaPal_Abort should only return OtaPalSuccess or OtaPalAbortFailed. */
    xOtaStatus = otaPal_Abort( &xOtaFile );
    TEST_ASSERT( xOtaStatus == OtaPalSuccess ||
                 xOtaStatus == OtaPalAbortFailed );
}

TEST_GROUP_RUNNER( Full_OTA_PAL )
{
    /************* otaPAL_CloseFile Tests *************/
    RUN_TEST_CASE( Full_OTA_PAL, otaPal_CloseFile_ValidSignature );
    RUN_TEST_CASE( Full_OTA_PAL, otaPal_CloseFile_InvalidSignatureBlockWritten );
    RUN_TEST_CASE( Full_OTA_PAL, otaPal_CloseFile_InvalidSignatureNoBlockWritten );
    RUN_TEST_CASE( Full_OTA_PAL, otaPal_CloseFile_NonexistingCodeSignerCertificate );

    /********** otaPal_CreateFileForRx Tests **********/
    RUN_TEST_CASE( Full_OTA_PAL, otaPal_CreateFileForRx_CreateAnyFile );

    /* Other failure case testing for otaPal_CreateFileForRx() is not possible
     * because the MCU's underlying non-volatile memory will have to be mocked
     * to return failures for creating a new file. */

    /*************** otaPal_Abort Tests ***************/
    RUN_TEST_CASE( Full_OTA_PAL, otaPal_Abort_OpenFile );
    RUN_TEST_CASE( Full_OTA_PAL, otaPal_Abort_FileWithBlockWritten );
    RUN_TEST_CASE( Full_OTA_PAL, otaPal_Abort_NullFileHandle );
    RUN_TEST_CASE( Full_OTA_PAL, otaPal_Abort_NonExistentFile );

    /************* otaPal_WriteBlock Tests ************/
    RUN_TEST_CASE( Full_OTA_PAL, otaPal_WriteBlock_WriteSingleByte );
    RUN_TEST_CASE( Full_OTA_PAL, otaPal_WriteBlock_WriteManyBlocks );

    /******** otaPal_SetPlatformImageState Tests ******/
    RUN_TEST_CASE( Full_OTA_PAL, otaPal_SetPlatformImageState_AcceptedStateWithoutClose );
    RUN_TEST_CASE( Full_OTA_PAL, otaPal_SetPlatformImageState_InvalidImageState );
    RUN_TEST_CASE( Full_OTA_PAL, otaPal_SetPlatformImageState_UnknownImageState );

    /* Setting the image with the accepted state is not supported because that
     * requires an image that was written, verified, and rebooted. */

    /******* otaPal_GetPlatformImageState Tests *******/
    RUN_TEST_CASE( Full_OTA_PAL, otaPal_GetPlatformImageState_InvalidImageStateFromFileCloseFailure );
}

/**
 * @brief Test otaPal_CloseFile with a valid signature and signature verification
 * certificate. Verify the success.
 */
TEST( Full_OTA_PAL, otaPal_CloseFile_ValidSignature )
{
	OtaPalJobDocProcessingResult_t xOtaStatus;
    int16_t bytesWritten;
    char sig[maxSignatureSize];

    /* We use a dummy file name here because closing the system designated bootable
     * image with content that is not runnable may cause issues. */
    xOtaFile.filepath = ( const char * ) ( "test_happy_path_image.bin" );
    xOtaFile.fileSize = sizeof( ucDummyData );
    xOtaStatus = otaPal_CreateFileForRx( &xOtaFile );
    TEST_ASSERT_EQUAL( OtaPalJobDocFileCreated,  xOtaStatus );

    /* We still want to close the file if the test fails somewhere here. */
    if( TEST_PROTECT() )
    {
        /* Write data to the file. */
        bytesWritten = otaPal_WriteBlock( &xOtaFile,
                                           0,
                                           ucDummyData,
                                           sizeof( ucDummyData ) );
        TEST_ASSERT_EQUAL( sizeof( ucDummyData ), bytesWritten );

        xOtaFile.signatureLen = ucValidSignatureLength;
        xOtaFile.signature = sig;
        memcpy( (void*)xOtaFile.signature, ucValidSignature, ucValidSignatureLength );
        xOtaFile.certfile = ( const char * ) OTA_PAL_CERTIFICATE_FILE;

        OtaPalStatus_t xCloseFileStatus = otaPal_CloseFile( &xOtaFile );
        TEST_ASSERT_EQUAL_INT( OtaPalSuccess, xCloseFileStatus );
    }
}

/**
 * @brief Call otaPal_CloseFile with an invalid signature in the file context.
 * The close is called after we have a written a block of dummy data to the file.
 * Verify the correct OTA Agent level error code is returned from otaPal_CloseFile.
 */
TEST( Full_OTA_PAL, otaPal_CloseFile_InvalidSignatureBlockWritten )
{
	OtaPalJobDocProcessingResult_t xOtaStatus;
    int16_t bytesWritten;
    char sig[maxSignatureSize];

    /* Create a local file using the PAL. */
    xOtaFile.filepath = ( const char * ) OTA_PAL_FIRMWARE_FILE;
    xOtaFile.fileSize = sizeof( ucDummyData );

    xOtaStatus = otaPal_CreateFileForRx( &xOtaFile );
    TEST_ASSERT_EQUAL( OtaPalJobDocFileCreated, xOtaStatus );

    /* We still want to close the file if the test fails somewhere here. */
    if( TEST_PROTECT() )
    {
        /* Write data to the file. */
        bytesWritten = otaPal_WriteBlock( &xOtaFile,
                                           0,
                                           ucDummyData,
                                           sizeof( ucDummyData ) );
        TEST_ASSERT_EQUAL( sizeof( ucDummyData ), bytesWritten );

        /* Fill out an incorrect signature. */
        xOtaFile.signatureLen = ucInvalidSignatureLength;
        xOtaFile.signature = sig;
        memcpy( (void*)xOtaFile.signature, ucInvalidSignature, ucInvalidSignatureLength );
        xOtaFile.certfile = ( const char * ) OTA_PAL_CERTIFICATE_FILE;

        /* Try to close the file. */
        OtaPalStatus_t xCloseFileStatus = otaPal_CloseFile( &xOtaFile );
        TEST_ASSERT_EQUAL( OtaPalSignatureCheckFailed, xCloseFileStatus );
    }
}

/**
 * @brief Call otaPal_CloseFile with an invalid signature in the file context.
 * The close is called when no blocks have been written to the file.
 * Verify the correct OTA Agent level error code is returned from otaPal_CloseFile.
 */
TEST( Full_OTA_PAL, otaPal_CloseFile_InvalidSignatureNoBlockWritten )
{
	OtaPalJobDocProcessingResult_t xOtaStatus;
    char sig[maxSignatureSize];

    /* Create a local file using the PAL. */
    xOtaFile.filepath = ( const char * ) OTA_PAL_FIRMWARE_FILE;
    xOtaFile.fileSize = sizeof( ucDummyData );
    xOtaStatus = otaPal_CreateFileForRx( &xOtaFile );
    TEST_ASSERT_EQUAL( OtaPalJobDocFileCreated, xOtaStatus );

    /* Fill out an incorrect signature. */
    xOtaFile.signatureLen = ucInvalidSignatureLength;
    xOtaFile.signature = sig;
    memcpy( (void *)xOtaFile.signature, ucInvalidSignature, ucInvalidSignatureLength );
    xOtaFile.certfile = ( const char * ) OTA_PAL_CERTIFICATE_FILE;

    /* We still want to close the file if the test fails somewhere here. */
    if( TEST_PROTECT() )
    {
        /* Try to close the file. */
    	OtaPalStatus_t xCloseFileStatus = otaPal_CloseFile( &xOtaFile );
        TEST_ASSERT_EQUAL( OtaPalSignatureCheckFailed, xCloseFileStatus );
    }
}

/**
 * @brief Call otaPal_CloseFile with a signature verification certificate path does
 * not exist in the system. Verify the correct OTA Agent level error code is returned
 * from otaPal_CloseFile.
 *
 * @note This test is only valid if your device uses a file system in your non-volatile memory.
 * Some devices may revert to using aws_codesigner_certificate.h if a file is not found, but
 * that option is not being enforced.
 */
TEST( Full_OTA_PAL, otaPal_CloseFile_NonexistingCodeSignerCertificate )
{
    #if ( OTA_PAL_USE_FILE_SYSTEM == 1 )
	    OtaPalJobDocProcessingResult_t xOtaStatus;
        int16_t bytesWritten;

        memset( &xOtaFile, 0, sizeof( xOtaFile ) );

        /* Create a local file using the PAL. */
        xOtaFile.filepath = ( uint8_t * ) OTA_PAL_FIRMWARE_FILE;
        xOtaFile.fileSize = sizeof( ucDummyData );

        xOtaStatus = otaPal_CreateFileForRx( &xOtaFile );
        TEST_ASSERT_EQUAL( OtaPalJobDocFileCreated, xOtaStatus );

        /* We still want to close the file if the test fails somewhere here. */
        if( TEST_PROTECT() )
        {
            /* Write data to the file. */
            bytesWritten = otaPal_WriteBlock( &xOtaFile,
                                               0,
                                               ucDummyData,
                                               sizeof( ucDummyData ) );
            TEST_ASSERT_EQUAL( sizeof( ucDummyData ), bytesWritten );

            /* Check the signature (not expected to be valid in this case). */
            xOtaFile.signatureLen = ucValidSignatureLength;
            memcpy( xOtaFile.signature, ucValidSignature, ucValidSignatureLength );
            xOtaFile.certfile = ( const char * ) ( "nonexistingfile.crt" );

            OtaPalStatus_t xCloseFileStatus = otaPal_CloseFile( &xOtaFile );
            TEST_ASSERT_EQUAL( OtaPalBadSignerCert , xCloseFileStatus );
        }
    #endif /* if ( OTA_PAL_USE_FILE_SYSTEM == 1 ) */
}

/**
 * @brief Create a file with any name, and verify success.
 */
TEST( Full_OTA_PAL, otaPal_CreateFileForRx_CreateAnyFile )
{
	OtaPalJobDocProcessingResult_t xOtaStatus;

    memset( &xOtaFile, 0, sizeof( AfrOtaJobDocumentFields_t ) );

    /* TEST: Create a local file using the PAL. Verify error in close. */
    xOtaFile.filepath = ( const char * ) OTA_PAL_FIRMWARE_FILE;
    xOtaFile.fileSize = sizeof( ucDummyData );
    xOtaStatus = otaPal_CreateFileForRx( &xOtaFile );
    TEST_ASSERT_EQUAL( OtaPalJobDocFileCreated, xOtaStatus );
}

/**
 * @brief Abort on an open file. Verify success.
 */
TEST( Full_OTA_PAL, otaPal_Abort_OpenFile )
{
	OtaPalJobDocProcessingResult_t xOtaStatus;

    memset( &xOtaFile, 0, sizeof( AfrOtaJobDocumentFields_t ) );

    xOtaFile.filepath = ( const char * ) OTA_PAL_FIRMWARE_FILE;
    xOtaFile.fileSize = sizeof( ucDummyData );

    /* Create a local file using the PAL. */
    xOtaStatus = otaPal_CreateFileForRx( &xOtaFile );
    TEST_ASSERT_EQUAL( OtaPalJobDocFileCreated, xOtaStatus );

    /* Signal that the download is being aborted. */
    OtaPalStatus_t xAbortStatus = otaPal_Abort( &xOtaFile );
    TEST_ASSERT_EQUAL_INT( OtaPalSuccess, xAbortStatus );

    /* Verify that the file handle is null. */
    TEST_ASSERT_EQUAL_INT( NULL, xOtaFile.fileId );
}

/**
 * @brief Abort after writing a block to an open file. Verify success.
 */
TEST( Full_OTA_PAL, otaPal_Abort_FileWithBlockWritten )
{
	OtaPalJobDocProcessingResult_t xOtaStatus;
    int16_t bytesWritten;

    xOtaFile.filepath = ( const char * ) OTA_PAL_FIRMWARE_FILE;
    xOtaFile.fileSize = sizeof( ucDummyData );

    /* Create a local file again using the PAL. */
    xOtaStatus = otaPal_CreateFileForRx( &xOtaFile );
    TEST_ASSERT_EQUAL( OtaPalJobDocFileCreated, xOtaStatus );

    /* We still want to close the file if the test fails here. */
    if( TEST_PROTECT() )
    {
        /* Write data to the file. */
        bytesWritten = otaPal_WriteBlock( &xOtaFile,
                                           0,
                                           ucDummyData,
                                           sizeof( ucDummyData ) );
        TEST_ASSERT_EQUAL( sizeof( ucDummyData ), bytesWritten );
    }

    /* Signal that the download is being aborted. */
    OtaPalStatus_t xAbortStatus = otaPal_Abort( &xOtaFile );
    TEST_ASSERT_EQUAL_INT( OtaPalSuccess, xAbortStatus );

    /* Verify that the file handle is null. */
    TEST_ASSERT_EQUAL_INT( NULL, xOtaFile.fileId );
}

/**
 * @brief Call otaPal_Abort on a NULL file handle. Verify there is no error.
 */
TEST( Full_OTA_PAL, otaPal_Abort_NullFileHandle )
{
    OtaPalStatus_t xOtaStatus;

    xOtaFile.filepath = ( const char * ) OTA_PAL_FIRMWARE_FILE;
    xOtaFile.fileId = 0;

    xOtaStatus = otaPal_Abort( &xOtaFile );
    TEST_ASSERT_EQUAL_INT( OtaPalSuccess, xOtaStatus );
}

/**
 * @brief Verify there is no error for aborting a file that doesn't exist.
 */
TEST( Full_OTA_PAL, otaPal_Abort_NonExistentFile )
{
    OtaPalStatus_t xOtaStatus;

    xOtaFile.filepath = ( const char * ) ( "nonexistingfile.bin" );

    xOtaStatus = otaPal_Abort( &xOtaFile );
    TEST_ASSERT_EQUAL_INT( OtaPalSuccess, xOtaStatus );
}

/**
 * @brief Write one byte of data and verify success.
 */
TEST( Full_OTA_PAL, otaPal_WriteBlock_WriteSingleByte )
{
	OtaPalJobDocProcessingResult_t xOtaStatus;
    int16_t bytesWritten;
    uint8_t ucData = 0xAA;

    /* TEST: Write a byte of data. */
    xOtaFile.filepath = ( const char * ) OTA_PAL_FIRMWARE_FILE;
    xOtaFile.fileSize = sizeof( ucDummyData );
    xOtaStatus = otaPal_CreateFileForRx( &xOtaFile );
    TEST_ASSERT_EQUAL( OtaPalJobDocFileCreated, xOtaStatus );

    if( TEST_PROTECT() )
    {
        bytesWritten = otaPal_WriteBlock( &xOtaFile, 0, &ucData, 1 );
        TEST_ASSERT_EQUAL_INT( 1, bytesWritten );
    }
}

/**
 * @brief Write many blocks of data to a file opened in the device. Verify success.
 *
 * Because of Flash property, we might not able to write same page multiple times.
 * So we write one block into one page.
 */
TEST( Full_OTA_PAL, otaPal_WriteBlock_WriteManyBlocks )
{
	OtaPalJobDocProcessingResult_t xOtaStatus;
    int16_t bytesWritten;

    /* The page size must >= dummy data size, so that we can write whole dummy data in one operation. */
    TEST_ASSERT_LESS_OR_EQUAL( testParam.pageSize, sizeof( ucDummyData ) );

    /* Some platforms compare the offset and file size, and it's not legal to write when offset > filesize.
     * We just set file size to the number of block size to make sure file size is big enough for every write. */
    xOtaFile.filepath = ( const char * ) OTA_PAL_FIRMWARE_FILE;
    xOtaFile.fileSize = testParam.pageSize * testotapalNUM_WRITE_BLOCKS;
    /* TEST: Write many bytes of data. */

    xOtaFile.filepath = ( const char * ) OTA_PAL_FIRMWARE_FILE;
    xOtaStatus = otaPal_CreateFileForRx( &xOtaFile );
    TEST_ASSERT_EQUAL( OtaPalJobDocFileCreated, xOtaStatus );

    if( TEST_PROTECT() )
    {
        int lIndex = 0;
        uint32_t writeOffset = 0;

        for( lIndex = 0; lIndex < testotapalNUM_WRITE_BLOCKS; lIndex++ )
        {
            bytesWritten = otaPal_WriteBlock( &xOtaFile, writeOffset, ucDummyData, sizeof( ucDummyData ) );
            TEST_ASSERT_EQUAL_INT( sizeof( ucDummyData ), bytesWritten );

            /* Align the writeOff with page size */
            writeOffset = writeOffset + testParam.pageSize;
        }
    }
}

/**
 * @brief Set the platform state to accepted before closing the file.
 * This should result in otaPal_SetPlatformImageState returning OtaPalCommitFailed.
 */
TEST( Full_OTA_PAL, otaPal_SetPlatformImageState_AcceptedStateWithoutClose )
{
	OtaPalJobDocProcessingResult_t xOtaStatus;
    OtaImageState_t eImageState = OtaImageStateUnknown;
    int16_t bytesWritten;

    /* Create a local file again using the PAL. */
    xOtaFile.filepath = ( const char * ) OTA_PAL_FIRMWARE_FILE;
    xOtaFile.fileSize = sizeof( ucDummyData );

    xOtaStatus = otaPal_CreateFileForRx( &xOtaFile );
    TEST_ASSERT_EQUAL( OtaPalJobDocFileCreated, xOtaStatus );

    /* We still want to close the file if the test fails. */
    if( TEST_PROTECT() )
    {
        /* Write data to the file. */
        bytesWritten = otaPal_WriteBlock( &xOtaFile,
                                           0,
                                           ucDummyData,
                                           sizeof( ucDummyData ) );
        TEST_ASSERT_EQUAL( sizeof( ucDummyData ), bytesWritten );

        /* Set the image state. */
        eImageState = OtaImageStateAccepted;
        OtaPalStatus_t xSetStateStatus = otaPal_SetPlatformImageState( &xOtaFile, eImageState );
        TEST_ASSERT_EQUAL_INT( OtaPalCommitFailed, xSetStateStatus );
    }
}

/**
 * @brief Set an invalid platform image state exceeding the range and verify
 * the expected error code is returned.
 */
TEST( Full_OTA_PAL, otaPal_SetPlatformImageState_InvalidImageState )
{
	OtaPalJobDocProcessingResult_t xOtaStatus;
    OtaImageState_t eImageState = OtaImageStateUnknown;
    int16_t bytesWritten;

    /* Create a local file again using the PAL. */
    xOtaFile.filepath = ( const char * ) OTA_PAL_FIRMWARE_FILE;
    xOtaFile.fileSize = sizeof( ucDummyData );

    xOtaStatus = otaPal_CreateFileForRx( &xOtaFile );
    TEST_ASSERT_EQUAL( OtaPalJobDocFileCreated, xOtaStatus );

    /* We still want to close the file if the test fails. */
    if( TEST_PROTECT() )
    {
        /* Write data to the file. */
        bytesWritten = otaPal_WriteBlock( &xOtaFile,
                                           0,
                                           ucDummyData,
                                           sizeof( ucDummyData ) );
        TEST_ASSERT_EQUAL( sizeof( ucDummyData ), bytesWritten );

        /* Try to set an invalid image state. */
        eImageState = ( OtaImageState_t ) ( OtaLastImageState + 1 );
        OtaPalStatus_t xSetStateStatus = otaPal_SetPlatformImageState( &xOtaFile, eImageState );
        TEST_ASSERT_EQUAL( OtaPalBadImageState, xSetStateStatus );
    }
}

/**
 * @brief Set the image state to unknown and verify a failure.
 */
TEST( Full_OTA_PAL, otaPal_SetPlatformImageState_UnknownImageState )
{
	OtaPalJobDocProcessingResult_t xOtaStatus;
    OtaImageState_t eImageState = OtaImageStateUnknown;
    int16_t bytesWritten;

    /* Create a local file again using the PAL. */
    xOtaFile.filepath = ( const char * ) OTA_PAL_FIRMWARE_FILE;
    xOtaFile.fileSize = sizeof( ucDummyData );

    xOtaStatus = otaPal_CreateFileForRx( &xOtaFile );
    TEST_ASSERT_EQUAL( OtaPalJobDocFileCreated, xOtaStatus );

    /* We still want to close the file if the test fails. */
    if( TEST_PROTECT() )
    {
        /* Write data to the file. */
        bytesWritten = otaPal_WriteBlock( &xOtaFile,
                                           0,
                                           ucDummyData,
                                           sizeof( ucDummyData ) );
        TEST_ASSERT_EQUAL( sizeof( ucDummyData ), bytesWritten );

        /* Try to set an invalid image state. */
        eImageState = OtaImageStateUnknown;
        OtaPalStatus_t xSetStateStatus = otaPal_SetPlatformImageState( &xOtaFile, eImageState );
        TEST_ASSERT_EQUAL( OtaPalBadImageState, xSetStateStatus );
    }
}

/**
 * @brief Set the platform image state to aborted.
 * We cannot test a abort failed without mocking the underlying non-volatile memory.
 */
TEST( Full_OTA_PAL, otaPal_SetPlatformImageState_AbortImageState )
{
	OtaPalJobDocProcessingResult_t xOtaStatus;
    OtaImageState_t eImageState = OtaImageStateUnknown;
    int16_t bytesWritten;

    /* Create a local file again using the PAL. */
    xOtaFile.filepath = ( const char * ) OTA_PAL_FIRMWARE_FILE;
    xOtaFile.fileSize = sizeof( ucDummyData );

    xOtaStatus = otaPal_CreateFileForRx( &xOtaFile );
    TEST_ASSERT_EQUAL( OtaPalJobDocFileCreated, xOtaStatus );

    /* We still want to close the file if the test fails. */
    if( TEST_PROTECT() )
    {
        /* Write data to the file. */
        bytesWritten = otaPal_WriteBlock( &xOtaFile,
                                           0,
                                           ucDummyData,
                                           sizeof( ucDummyData ) );
        TEST_ASSERT_EQUAL( sizeof( ucDummyData ), bytesWritten );

        eImageState = OtaImageStateAborted;
        OtaPalStatus_t xSetStateStatus = otaPal_SetPlatformImageState( &xOtaFile, eImageState );
        TEST_ASSERT_EQUAL_INT( OtaPalSuccess, xSetStateStatus );
        eImageState = OtaImageStateUnknown;
        OtaPalImageState_t ePalImageState = otaPal_GetPlatformImageState( &xOtaFile );
        TEST_ASSERT_EQUAL( OtaPalImageStateInvalid, ePalImageState);
    }
}

/**
 * @brief Verify that the current image received is in the invalid state after a
 * failure to close the file because of a bad signature.
 */
TEST( Full_OTA_PAL, otaPal_GetPlatformImageState_InvalidImageStateFromFileCloseFailure )
{
	OtaPalJobDocProcessingResult_t xOtaStatus;
    OtaPalImageState_t ePalImageState = OtaPalImageStateUnknown;
    int16_t bytesWritten;
    char sig[maxSignatureSize];

    /* TEST: Invalid image returned from otaPal_GetPlatformImageState(). Using a failure to close. */
    /* Create a local file again using the PAL. */
    xOtaFile.filepath = ( const char * ) OTA_PAL_FIRMWARE_FILE;
    xOtaFile.fileSize = sizeof( ucDummyData );

    xOtaStatus = otaPal_CreateFileForRx( &xOtaFile );
    TEST_ASSERT_EQUAL( OtaPalSuccess, xOtaStatus );

    /* We still want to close the file if the test fails. */
    if( TEST_PROTECT() )
    {
        /* Write data to the file. */
        bytesWritten = otaPal_WriteBlock( &xOtaFile,
                                           0,
                                           ucDummyData,
                                           sizeof( ucDummyData ) );
        TEST_ASSERT_EQUAL( sizeof( ucDummyData ), bytesWritten );

        /* Check the signature. */
        xOtaFile.signatureLen = ucInvalidSignatureLength;
        xOtaFile.signature = sig;
        memcpy( (void *) xOtaFile.signature, ucInvalidSignature, ucInvalidSignatureLength );
        xOtaFile.certfile = ( const char * ) OTA_PAL_CERTIFICATE_FILE;

        OtaPalStatus_t xCloseStatus = otaPal_CloseFile( &xOtaFile );
        TEST_ASSERT_EQUAL( OtaPalSignatureCheckFailed, xCloseStatus );

        /* The file failed to close, so it is invalid or in an unknown state. */
        ePalImageState = otaPal_GetPlatformImageState( &xOtaFile );
        TEST_ASSERT( ( OtaPalImageStateInvalid == ePalImageState ) || ( OtaPalImageStateUnknown == ePalImageState ) );
    }
}

/*-----------------------------------------------------------*/

int RunOtaPalTest( void )
{
    int status = -1;
    SetupOtaPalTestParam( &testParam );

    /* Initialize unity. */
    UnityFixture.Verbose = 1;
    UnityFixture.GroupFilter = 0;
    UnityFixture.NameFilter = 0;
    UnityFixture.RepeatCount = 1;

    UNITY_BEGIN();

    /* Run the test group. */
    RUN_TEST_GROUP( Full_OTA_PAL );

    status = UNITY_END();
    return status;
}

/*-----------------------------------------------------------*/

#endif /* if ( OTA_TEST_ENABLED == 1 ) */
