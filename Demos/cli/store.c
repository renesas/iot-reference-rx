/*
 * Copyright (C) 2023 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
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
 * https://www.FreeRTOS.org
 * https://aws.amazon.com/freertos
 *
 */

#include "store.h"
#include "pkcs11_operations.h"
/* Key provisioning includes. */

#include "aws_dev_mode_key_provisioning.h"

const char * keys[ KVS_NUM_KEYS ] = KVSTORE_KEYS;
KeyValueStore_t gKeyValueStore = { 0 };
extern volatile uint32_t pvwrite;
extern CK_RV vDevModeKeyPreProvisioning( KeyValueStore_t Keystore, KVStoreKey_t ID, int32_t xvaluelength );
BaseType_t xPending;
/*
 * @brief Write a value for a given key to Data Flash.
 * @param[in] KVStoreKey_t Key to store the given value in.
 * @param[in] pucData Pointer to a buffer containing the value to be stored.
 * @param[in] ulDataSize length of the value given in pucData.
 */
BaseType_t xprvWriteValueToImpl (KVStoreKey_t keyIndex, char *pucData, uint32_t ulDataSize)
{
	lfs_file_t file;
	lfs_ssize_t lfs_err;

	lfs_err = lfs_remove(&RM_STDIO_LITTLEFS_CFG_LFS, (char *) keys[ keyIndex ]);

	if ((LFS_ERR_NOENT != lfs_err) && (LFS_ERR_OK != lfs_err))
	{
		return -1;
	}

	lfs_err = lfs_file_open(&RM_STDIO_LITTLEFS_CFG_LFS, &file, (char *) keys[ keyIndex ], LFS_O_WRONLY | LFS_O_TRUNC | LFS_O_CREAT);

	if (LFS_ERR_OK != lfs_err)
	{
		return -1;
	}

	lfs_err = lfs_file_write(&RM_STDIO_LITTLEFS_CFG_LFS, &file, pucData, ulDataSize);
	pvwrite +=ulDataSize;
	vLfsSSizeToErr( &lfs_err, (lfs_ssize_t)ulDataSize );

	lfs_file_close(&RM_STDIO_LITTLEFS_CFG_LFS, &file);
	return( lfs_err == LFS_ERR_OK );
}

/*
 * @brief Read a value for a given key to Data Flash.
 * @param[in] KVStoreKey_t Key to store the given value in.
 * @param[in] pucData Pointer to a buffer containing the value to be stored.
 * @param[in] ulDataSize length of the value given in pucData.
 */
int32_t xprvReadValueFromImpl (KVStoreKey_t keyIndex,
        char **ppucData,
		uint32_t *pulDataSize,
		size_t xBufferSize)
{

	lfs_file_t file;
	lfs_ssize_t lfs_ret =
		lfs_file_open(&RM_STDIO_LITTLEFS_CFG_LFS,
					  &file,
					  (char *) keys[ keyIndex ],
					  LFS_O_RDONLY);

	lfs_ret = lfs_file_size(&RM_STDIO_LITTLEFS_CFG_LFS, &file);

	*ppucData = pvPortMalloc((size_t) lfs_ret);

	if ((lfs_ret >= 0) && (NULL != *ppucData))
	{
		lfs_ret = lfs_file_read(&RM_STDIO_LITTLEFS_CFG_LFS, &file, *ppucData, (lfs_size_t) lfs_ret);

		if (lfs_ret >= 0)
		{
			*pulDataSize = (uint32_t) lfs_ret;

			vLfsSSizeToErr( &lfs_ret, (lfs_ssize_t)xBufferSize );
		}
	}
	if( lfs_ret == LFS_ERR_OK )
	{
		lfs_file_close(&RM_STDIO_LITTLEFS_CFG_LFS, &file);
	}


	return (lfs_ret == LFS_ERR_OK);
}

/*
 * @brief Get the length of a value stored in the Data Flash
 * @param[in] KVStoreKey_t Key to lookup
 * @return length of the value stored in the KVStore or 0 if not found.
 */
int32_t xprvGetValueLengthFromImpl( KVStoreKey_t keyIndex)
{
	size_t xLength = 0;
	struct lfs_info xFileInfo = { 0 };

	if( lfs_stat( &RM_STDIO_LITTLEFS_CFG_LFS, (char *) keys[ keyIndex ], &xFileInfo ) == LFS_ERR_OK )
	{
		xLength =  xFileInfo.size;
	}
	return xLength;

}

/*
 * @brief Get the length of a value stored in the Data Flash
 * @param[in] KVStoreKey_t Key to lookup
 * @return length of the value stored in the KVStore or 0 if not found.
 */
int32_t GetTotalLengthFromImpl()
{
    size_t xLength = 0;
    struct lfs_info xFileInfo = { 0 };
    for (uint32_t i = 0; i < KVS_NUM_KEYS; i++)
    {
        if (lfs_stat (&RM_STDIO_LITTLEFS_CFG_LFS, (char*) keys[i], &xFileInfo) == LFS_ERR_OK)
        {
            xLength += xFileInfo.size;
        }
    }

    return xLength;
}

/*
 * @brief Write a given and key / value pair to the cache
 * @param[in] size_t Length of the key to store
 * @param[in] Key Pointer to the key data to be copied into the cache.
 * @param[in] ValueLength Length of the data to store.
 * @param[in] pvNewValue Pointer to the new data to be copied into the cache.
 * @return key.
 */
int32_t xprvWriteCacheEntry(size_t KeyLength,
						char * Key,
						size_t ValueLength,
						char * pvNewValue )
{
	KVStoreKey_t xKey;
	xKey = (KVStoreKey_t)Filename2Handle( Key,KeyLength);

	if (xKey < 0)
	{
		return xKey;
	}

	if ((KVS_TSIP_ROOTCA_PUBKEY_ID == xKey) ||
	    (KVS_TSIP_CLIENT_PUBKEY_ID == xKey) ||
	    (KVS_TSIP_CLIENT_PRIKEY_ID == xKey))
	{
		/* TSIP key cancels write */
		return xKey;
	}

	if((xKey == KVS_CLAIM_CERT_ID) || (xKey == KVS_CLAIM_PRIVKEY_ID) || (xKey == KVS_ROOT_CA_ID))
    {
        ValueLength++; //add 1 more byte to store string ending \0
    }

	strcpy( gKeyValueStore.table[xKey ].key, keys[ xKey ] );
	if (gKeyValueStore.table[xKey ].type == KV_TYPE_NONE)
	{

		vAllocateDataBuffer(xKey,ValueLength);
		gKeyValueStore.table[xKey ].type = KV_TYPE_STRING;
		gKeyValueStore.table[xKey ].xChangePending = pdTRUE;
	}
	else if (gKeyValueStore.table[ xKey ].valueLength != ValueLength)
	{

		vReallocDataBuffer( xKey, ValueLength );
		gKeyValueStore.table[xKey ].type = KV_TYPE_STRING;
		gKeyValueStore.table[xKey ].xChangePending = pdTRUE;
	}
	else
	{
		const void * pvReadPtr = pvGetDataReadPtr( xKey );
		if( ( pvReadPtr == NULL ) ||
		            ( memcmp( pvReadPtr, pvNewValue, ValueLength ) != 0 ) )
		{
			gKeyValueStore.table[xKey ].xChangePending = pdTRUE;
		}

	}
	if( gKeyValueStore.table[xKey ].xChangePending == pdTRUE )
	{
		void * pvDataWrite = pvGetDataWritePtr( xKey );

		if( pvDataWrite != NULL )
		{
			( void ) memcpy( pvGetDataWritePtr( xKey ), pvNewValue, ValueLength );
			if((xKey == KVS_CLAIM_CERT_ID) || (xKey == KVS_CLAIM_PRIVKEY_ID) || (xKey == KVS_ROOT_CA_ID))
            {
                //Set string ending \0
                gKeyValueStore.table[xKey].value[ValueLength-1] = '\0';
            }

		}
	}
	return xKey;
}
static inline void vAllocateDataBuffer( uint32_t key,
                                        size_t xNewLength )
{
    if( xNewLength > 0 )
    {
    	gKeyValueStore.table[ key ].value = pvPortMalloc( xNewLength);
    	gKeyValueStore.table[ key ].valueLength = xNewLength;
    }
    else
    {
    	gKeyValueStore.table[ key ].value = 0;
    	gKeyValueStore.table[ key ].valueLength = xNewLength;
    }
}


static inline void vClearDataBuffer( KVStoreKey_t key )
{
    /* Check if data is heap allocated > 0 */
    if( gKeyValueStore.table[ key ].valueLength > 0 )
    {
        vPortFree( gKeyValueStore.table[ key ].value );
        gKeyValueStore.table[ key ].value = NULL;
        gKeyValueStore.table[ key ].valueLength = 0;
    }
    else /* Statically allocated */
    {
    	gKeyValueStore.table[ key ].value = 0;
		gKeyValueStore.table[ key ].valueLength = 0;
    }
}
static inline void vReallocDataBuffer( KVStoreKey_t key,
                                       size_t xNewLength )
{
    if( xNewLength > gKeyValueStore.table[ key ].valueLength )
    {
        /* Need to allocate a bigger buffer */
        vClearDataBuffer( key );
        vAllocateDataBuffer( key, xNewLength);
    }
    else /* New value is same size or smaller. Re-use already allocated buffer */
    {
    	gKeyValueStore.table[ key ].valueLength = xNewLength;
    }
}

int32_t Filename2Handle( char * pcFileName,size_t KeyLength)
{
    (void) KeyLength; /* String comparison to use the length of KVStore key */
	int32_t xHandle = -1;
	char * CLIcmdkeys[ KVS_NUM_KEYS ] = CLICMDKEYS;
    if( pcFileName != NULL )
    {
        for (uint32_t i = 0; i < KVS_NUM_KEYS; i++)
        {
        	if (strncmp( pcFileName, CLIcmdkeys[i], strlen(CLIcmdkeys[i]) ) == 0)
            {
                xHandle = i;
                break;
            }
        }
    }

    return xHandle;
}

/*
 * @brief Commit data and save it into Data Flash
 * @return True if successfully.
 */
BaseType_t KVStore_xCommitChanges( void )
{
    BaseType_t xSuccess = pdFALSE;
    CK_SESSION_HANDLE xP11Session;
    CK_RV xResult = CKR_OK;

    /* Initialize the PKCS Module */
	xResult = xInitializePkcs11Token();

	if( xResult == CKR_OK )
	{
		xResult = xInitializePkcs11Session( &xP11Session );
	}

    for( size_t i = 0; i < KVS_NUM_KEYS ; i++ )
    {
        if( gKeyValueStore.table[ i ].xChangePending == pdTRUE )
        {
        	/*
        	 * Check if certificate or privatekey or publickey
        	 */
        	if ((i  == KVS_DEVICE_CERT_ID ) || (i  == KVS_DEVICE_PRIVKEY_ID )|| (i  == KVS_DEVICE_PUBKEY_ID ))
			{
				xSuccess = vDevModeKeyPreProvisioning(gKeyValueStore, (KVStoreKey_t)i,gKeyValueStore.table[ i ].valueLength);
				if (xSuccess == pdFALSE)
				{
					return xSuccess;
				}
				gKeyValueStore.table[ i ].xChangePending = pdFALSE;
			}
        	else if ((i  == KVS_CLAIM_CERT_ID ))
			{
        		xResult = provisionCertificate(xP11Session,
												(char*) gKeyValueStore.table[i].value,
												gKeyValueStore.table[i].valueLength,
												pkcs11configLABEL_CLAIM_CERTIFICATE);

				if (xResult != CKR_OK)
				{
					LogError( ( "Failed to store claim certificate." ) );
					return pdFALSE;
				}
				else
				{
					xSuccess = pdTRUE;
					gKeyValueStore.table[ i ].xChangePending = pdFALSE;
				}
			}
        	else if ((i  == KVS_CLAIM_PRIVKEY_ID ))
			{
        		xResult = provisionPrivateKey(xP11Session,
												(char*) gKeyValueStore.table[i].value,
												gKeyValueStore.table[i].valueLength,
												pkcs11configLABEL_CLAIM_PRIVATE_KEY);

				if (xResult != CKR_OK)
				{
					LogError( ( "Failed to store claim private key." ) );
					return pdFALSE;
				}
				else
				{
					xSuccess = pdTRUE;
					gKeyValueStore.table[ i ].xChangePending = pdFALSE;
				}
			}
        	else if ((KVS_TSIP_ROOTCA_PUBKEY_ID == i) ||
        	            (KVS_TSIP_CLIENT_PUBKEY_ID == i) ||
        	            (KVS_TSIP_CLIENT_PRIKEY_ID == i))
        	{
        		/* No commit processing */
        	}
        	/*
        	 * Check if others
        	 */
        	else
        	{
        		xSuccess = xprvWriteValueToImpl((KVStoreKey_t)i,(char *)gKeyValueStore.table[ i ].value,
        		    											gKeyValueStore.table[ i ].valueLength);
				if (xSuccess != pdTRUE)
				{
					return pdFALSE;
				}
				gKeyValueStore.table[ i ].xChangePending = pdFALSE;
        	}

        }
    }
    xPkcs11CloseSession( xP11Session );
    return xSuccess;
}

static inline void vLfsSSizeToErr( lfs_ssize_t * pxReturnValue,
                                   size_t xExpectedLength )
{
    if( *pxReturnValue == xExpectedLength )
    {
        *pxReturnValue = LFS_ERR_OK;
    }
    else if( *pxReturnValue >= 0 )
    {
        *pxReturnValue = LFS_ERR_CORRUPT;
    }
    else
    {
        /* Pass through the error code otherwise */
    }
}

static inline const void * pvGetDataReadPtr( KVStoreKey_t key )
{
    const void * pvData = NULL;

    if( gKeyValueStore.table[ key ].type == KV_TYPE_NONE )
    {
        pvData = NULL;
    }
    else if( gKeyValueStore.table[ key ].valueLength > 0 )
    {
        pvData = gKeyValueStore.table[ key ].value;
    }
    else
    {
        pvData = ( void * ) &( gKeyValueStore.table[ key ].value );
    }

    return pvData;
}

BaseType_t xprvCopyValueFromCache( KVStoreKey_t xKey,
                                   KVStoreValueType_t * pxDataType,
                                   size_t * pxDataLength,
                                   void * pvBuffer,
                                   size_t xBufferSize)
{
    const void * pvDataPtr = NULL;
    size_t xDataLen = 0;

    configASSERT( xKey < KVS_NUM_KEYS );
    configASSERT( pvBuffer != NULL );

    pvDataPtr = pvGetDataReadPtr( xKey );

    if( pvDataPtr != NULL )
    {
        xDataLen = gKeyValueStore.table[ xKey ].valueLength;

        if( xBufferSize < xDataLen )
        {
            xDataLen = xBufferSize;
        }

        ( void ) memcpy( pvBuffer, pvDataPtr, xDataLen );

        if( pxDataType != NULL )
        {
            *pxDataType = (KVStoreValueType_t)gKeyValueStore.table[ xKey ].type;
        }

        if( pxDataLength != NULL )
        {
            *pxDataLength = gKeyValueStore.table[ xKey ].valueLength;
        }

        xPending = gKeyValueStore.table[ xKey ].xChangePending;
    }

    return( xDataLen > 0 );
}

static inline void * pvGetDataWritePtr( KVStoreKey_t key )
{
    void * pvData = NULL;

    if( gKeyValueStore.table[ key ].valueLength > 0 )
    {
        pvData = gKeyValueStore.table[ key ].value;
    }
    else
    {

        pvData = ( void * ) &( gKeyValueStore.table[ key ].value );
    }

    configASSERT( pvData != NULL );
    return pvData;
}

/*
 * @brief Initialize the Key Value Store Cache by reading each entry from the Data Flash.
 */
int32_t vprvCacheInit( void )
{
	int32_t xNvLength = -1;
	CK_RV xResult = CKR_OK;
	CK_BBOOL xIsPrivate = ( CK_BBOOL ) CK_TRUE;

	CK_BYTE pxCertLabel[] =  pkcs11configLABEL_DEVICE_CERTIFICATE_FOR_TLS ;
	CK_BYTE pxPrivKeyLabel[] =  pkcs11configLABEL_DEVICE_PRIVATE_KEY_FOR_TLS ;
	CK_SESSION_HANDLE xSession = 0;

    /* Read from file system into ram */
    for( uint32_t i = 0; i < KVS_NUM_KEYS; i++ )
    {
        /* pvData pointer should be NULL on startup */

    	if ((i  == KVS_DEVICE_CERT_ID ) || (i  == KVS_DEVICE_PRIVKEY_ID ) || (i  == KVS_DEVICE_PUBKEY_ID ) || (i  == KVS_CLAIM_CERT_ID )|| (i  == KVS_CLAIM_PRIVKEY_ID ))
    	{
    		continue;
    	}
    	else
    	{
    		gKeyValueStore.table[ i ].xChangePending = pdFALSE;
			gKeyValueStore.table[ i ].type = KV_TYPE_NONE;

			xNvLength = xprvGetValueLengthFromImpl( (KVStoreKey_t)i );
			if( xNvLength > 0 )
			{
				gKeyValueStore.table[ i ].valueLength = xNvLength;
				size_t * pxLength = &( gKeyValueStore.table[ i ].valueLength );
				strcpy( gKeyValueStore.table[ i ].key, keys[ i ] );
				( void ) xprvReadValueFromImpl( (KVStoreKey_t)i,  &( gKeyValueStore.table[ i ].value ), pxLength, *pxLength );
				gKeyValueStore.table[ i ].type = KV_TYPE_STRING;
			}
    	}

    }
	return xNvLength;

}

/*
 * @brief Format the Key Value Store Cache.
 */
void vprvCacheFormat(void)
{
    for (uint32_t i = 0; i < KVS_NUM_KEYS; i++)
    {
        gKeyValueStore.table[i].xChangePending = pdFALSE;
        gKeyValueStore.table[i].type = KV_TYPE_NONE;
        memset(gKeyValueStore.table[i].key, 0, KVSTORE_KEY_MAX_LEN);
        vClearDataBuffer ((KVStoreKey_t) i);
    }
}

/*
 * @brief Get the length of a value in cache excepted certificate, public and private key
 * @param[in] xKey Key to lookup
 * @param[in] pvBuffer Pointer to a buffer containing the value to be stored.
 * @param[in] xBufferSize buffer size
 * @return length of the value stored in the KVStore or 0 if not found.
 */
size_t xReadEntry( KVStoreKey_t xKey,
                                   void * pvBuffer,
                                   size_t xBufferSize )
{
	 size_t xLength = 0;

	configASSERT( xKey < KVS_NUM_KEYS );
	configASSERT( pvBuffer != NULL );
	( void ) xprvCopyValueFromCache( xKey, NULL, &xLength, pvBuffer, xBufferSize );
	return xLength;

}

/*
 * @brief Get the string value in cache excepted certificate, public and private key
 * @param[in] xKey Key to lookup
 * @param[in] xBufferSize buffer size
 * @return value of buffer.
 */
char *GetStringValue( KVStoreKey_t key, size_t  pxLength )
{
	size_t xSizeWritten = 0;

	char* pcBuffer = NULL;
	if (pxLength > 0)
	{
		/* 1 more byte for the null-terminated character */
		pcBuffer = pvPortMalloc( pxLength + 1 );
		xSizeWritten = xReadEntry(key, pcBuffer, pxLength);
		if (xSizeWritten == 0)
		{
			vPortFree( pcBuffer );
			pcBuffer = NULL;
		}
		else
		{
			/* Assign the null-terminated character */
			pcBuffer[ xSizeWritten] = '\0';
		}
	}
	return pcBuffer;
}

/*
 * @brief Get the string value in cache included certificate, public and private key
 * @param[in] xKey Key to lookup
 * @param[in] xBufferSize buffer size
 * @return value of buffer.
 */
char *xprvGetCacheEntry(char* key, size_t pxLength )
{
	char* buffervalue = NULL;
	size_t* pLength = NULL;
	int32_t xNvLength = -1;
	KVStoreKey_t xKey;
	CK_RV xResult = CKR_OK;


	const char pxCertLabel[] =  pkcs11configLABEL_DEVICE_CERTIFICATE_FOR_TLS ;
	const char pxPrivKeyLabel[] =  pkcs11configLABEL_DEVICE_PRIVATE_KEY_FOR_TLS ;
	const char pxPubKeyLabel[] =  pkcs11configLABEL_DEVICE_PUBLIC_KEY_FOR_TLS ;
	const char pxClaimCert[] =  pkcs11configLABEL_CLAIM_CERTIFICATE ;
    const char pxClaimKey[] =  pkcs11configLABEL_CLAIM_PRIVATE_KEY ;
	CK_FUNCTION_LIST_PTR pxFunctionList = NULL;
	CK_SESSION_HANDLE xSession = 0;
	char *tmp = NULL;
	uint32_t data_length = 0;

	CK_BBOOL xIsPrivate = ( CK_BBOOL ) CK_FALSE;
	CK_OBJECT_HANDLE xPalHandle = CK_INVALID_HANDLE;

	xKey = (KVStoreKey_t)Filename2Handle( key,pxLength);
	if (xKey < 0)
	{
		return NULL;
	}

	if ((KVS_TSIP_ROOTCA_PUBKEY_ID == xKey) ||
	    (KVS_TSIP_CLIENT_PUBKEY_ID == xKey) ||
	    (KVS_TSIP_CLIENT_PRIKEY_ID == xKey))
	{
		/* TSIP key cancels read */
		buffervalue = "The TSIP key index cannot be read.";
		return buffervalue;
	}

    if ((xKey == KVS_DEVICE_CERT_ID) || (xKey == KVS_DEVICE_PRIVKEY_ID) || (xKey == KVS_DEVICE_PUBKEY_ID)
            || (xKey == KVS_CLAIM_CERT_ID) || (xKey == KVS_CLAIM_PRIVKEY_ID))
    {
        xResult = C_GetFunctionList (&pxFunctionList);

        /* Initialize the PKCS Module */
        if (xResult == CKR_OK)
        {
            xResult = xInitializePkcs11Token ();
        }

        if (xResult == CKR_OK)
        {
            xResult = xInitializePkcs11Session (&xSession);
        }
    }

    gKeyValueStore.table[xKey].type = KV_TYPE_STRING;
    gKeyValueStore.table[xKey].xChangePending = pdFALSE;
    if ((xKey == KVS_DEVICE_CERT_ID))
    {

        xPalHandle = PKCS11_PAL_FindObject (pxCertLabel, strlen(pxCertLabel));
        if (xPalHandle != CK_INVALID_HANDLE)
        {
            xResult = PKCS11_PAL_GetObjectValue (xPalHandle, &tmp, &data_length, &xIsPrivate);
            if ((xResult == CKR_OK) && (data_length > 0))
            {
                vAllocateDataBuffer (xKey, data_length);
                strcpy(gKeyValueStore.table[xKey].key, keys[xKey]);
                gKeyValueStore.table[xKey].value = tmp;
            }
        }
    }
    else if (xKey == KVS_DEVICE_PRIVKEY_ID)
    {
        xIsPrivate = (CK_BBOOL ) CK_TRUE;
        xPalHandle = PKCS11_PAL_FindObject (pxPrivKeyLabel, strlen(pxPrivKeyLabel));
        if (xPalHandle != CK_INVALID_HANDLE)
        {

            xResult = PKCS11_PAL_GetObjectValue (xPalHandle, &tmp, &data_length, &xIsPrivate);
            if ((xResult == CKR_OK) && (data_length > 0))
            {
                vAllocateDataBuffer (xKey, data_length);
                strcpy(gKeyValueStore.table[xKey].key, keys[xKey]);
                gKeyValueStore.table[xKey].value = tmp;

            }
        }
    }
    else if (xKey == KVS_DEVICE_PUBKEY_ID)
    {
        xIsPrivate = (CK_BBOOL ) CK_FALSE;
        xPalHandle = PKCS11_PAL_FindObject (pxPubKeyLabel, strlen(pxPubKeyLabel));
        if (xPalHandle != CK_INVALID_HANDLE)
        {

            xResult = PKCS11_PAL_GetObjectValue (xPalHandle, &tmp, &data_length, &xIsPrivate);
            if ((xResult == CKR_OK) && (data_length > 0))
            {
                vAllocateDataBuffer (xKey, data_length);
                strcpy(gKeyValueStore.table[xKey].key, keys[xKey]);
                gKeyValueStore.table[xKey].value = tmp;
            }
        }
    }
    else if (xKey == KVS_CLAIM_CERT_ID)
    {
        xIsPrivate = (CK_BBOOL ) CK_FALSE;
        xPalHandle = PKCS11_PAL_FindObject (pxClaimCert, strlen(pxClaimCert));
        if (xPalHandle != CK_INVALID_HANDLE)
        {
            xResult = PKCS11_PAL_GetObjectValue (xPalHandle, &tmp, &data_length, &xIsPrivate);
            if ((xResult == CKR_OK) && (data_length > 0))
            {
                vAllocateDataBuffer (xKey, data_length);
                strcpy(gKeyValueStore.table[xKey].key, keys[xKey]);
                gKeyValueStore.table[xKey].value = tmp;
            }
        }
    }
    else if (xKey == KVS_CLAIM_PRIVKEY_ID)
    {
        xIsPrivate = (CK_BBOOL ) CK_TRUE;
        xPalHandle = PKCS11_PAL_FindObject (pxClaimKey, strlen(pxClaimKey));
        if (xPalHandle != CK_INVALID_HANDLE)
        {
            xResult = PKCS11_PAL_GetObjectValue (xPalHandle, &tmp, &data_length, &xIsPrivate);
            if ((xResult == CKR_OK) && (data_length > 0))
            {
                vAllocateDataBuffer (xKey, data_length);
                strcpy(gKeyValueStore.table[xKey].key, keys[xKey]);
                gKeyValueStore.table[xKey].value = tmp;
            }
        }
    }
	else
	{
		xNvLength = xprvGetValueLengthFromImpl( xKey );
		if( xNvLength > 0 )
		{
            gKeyValueStore.table[ xKey ].valueLength = xNvLength;
			size_t * valueLength = &( gKeyValueStore.table[ xKey ].valueLength );
			strcpy( gKeyValueStore.table[ xKey ].key, keys[ xKey ] );
			( void ) xprvReadValueFromImpl( xKey,  &( gKeyValueStore.table[ xKey ].value ), valueLength, *valueLength );
		}
	}


	if ((xKey == KVS_DEVICE_CERT_ID) || (xKey == KVS_DEVICE_PRIVKEY_ID) || (xKey == KVS_DEVICE_PUBKEY_ID)
            || (xKey == KVS_CLAIM_CERT_ID) || (xKey == KVS_CLAIM_PRIVKEY_ID))
    {
        if (data_length > 0)
        {
            for (int32_t i = 0; i < strlen(gKeyValueStore.table[xKey].value); i++)
            {
                if (((gKeyValueStore.table[xKey].value)[i] < 0x20) || ((gKeyValueStore.table[xKey].value)[i] > 0x7e))
                {
                    if (((gKeyValueStore.table[xKey].value)[i] != 0x0a)
                            && ((gKeyValueStore.table[xKey].value)[i] != 0x0d))
                    {
                        buffervalue = "Could not print: <binary data> is included.";
                        break;
                    }
                }
            }
            if (xResult == CKR_OK)
            {
                pxFunctionList->C_CloseSession (xSession);
            }
        }

    }
	else
	{

		buffervalue = GetStringValue(xKey,xNvLength);
	}

 	return buffervalue;
}

/*
 * @brief Get the length of a value in cache
 * @param[in] xKey Key to lookup
 * @return length of data stored in the KVstore.
 */
size_t prvGetCacheEntryLength( KVStoreKey_t xKey )
{
	configASSERT( xKey < KVS_NUM_KEYS );
	return gKeyValueStore.table[ xKey ].valueLength;
}

