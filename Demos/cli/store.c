/*
 * store.c
 *
 *  Created on: Jan 25, 2023
 *      Author: MyPC
 */
#include "store.h"
/* Key provisioning includes. */
#ifdef __TEST__
#include "dev_mode_key_provisioning.h"
#else
#include "aws_dev_mode_key_provisioning.h"
#endif

const char * keys[ KVS_NUM_KEYS ] = KVSTORE_KEYS;
KeyValueStore_t gKeyValueStore = { 0 };
extern volatile uint32_t pvwrite;
extern CK_RV vDevModeKeyPreProvisioning( KeyValueStore_t Keystore, KVStoreKey_t ID, int32_t xvaluelength );
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


int32_t xprvGetValueLengthFromImpl( KVStoreKey_t keyIndex)
{
	size_t xLength = 0;
	struct lfs_info xFileInfo = { 0 };

	lfs_file_t file;
	if( lfs_stat( &RM_STDIO_LITTLEFS_CFG_LFS, (char *) keys[ keyIndex ], &xFileInfo ) == LFS_ERR_OK )
	{
		xLength =  xFileInfo.size;
	}
	return xLength;

}

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
	strcpy( gKeyValueStore.table[xKey ].key, keys[ xKey ] );
	if (gKeyValueStore.table[xKey ].type == KV_TYPE_NONE)
	{

		vAllocateDataBuffer(xKey,ValueLength);
//		memcpy(gKeyValueStore.table[ xKey ].value, pvNewValue,ValueLength);
//		gKeyValueStore.table[xKey].value[ValueLength] = '\0';
		gKeyValueStore.table[xKey ].type = KV_TYPE_STRING;
		gKeyValueStore.table[xKey ].xChangePending = pdTRUE;
	}
	else if (gKeyValueStore.table[ xKey ].valueLength != ValueLength)
	{

		vReallocDataBuffer( xKey, ValueLength );
//		memcpy(gKeyValueStore.table[ xKey ].value, pvNewValue,ValueLength);
//		gKeyValueStore.table[xKey].value[ValueLength] = '\0';
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
		}
	}
	return xKey;
}
static inline void vAllocateDataBuffer( uint32_t key,
                                        size_t xNewLength )
{
    if( xNewLength > sizeof( void * ) )
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
    /* Check if data is heap allocated > sizeof( void * ) */
    if( gKeyValueStore.table[ key ].valueLength > sizeof( void * ) )
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
	int32_t xHandle = -1;
	char * CLIcmdkeys[ KVS_NUM_KEYS ] = CLICMDKEYS;
    if( pcFileName != NULL )
    {
        for (uint32_t i = 0; i < KVS_NUM_KEYS; i++)
        {
        	if (strncmp( pcFileName, CLIcmdkeys[i], KeyLength ) == 0)
            {
                xHandle = i;
                break;
            }
        }
    }

    return xHandle;
}
BaseType_t KVStore_xCommitChanges( void )
{
    BaseType_t xSuccess = pdFALSE;

    for( size_t i = 0; i < KVS_NUM_KEYS ; i++ )
    {
        if( gKeyValueStore.table[ i ].xChangePending == pdTRUE )
        {

        	if ((i  == KVS_DEVICE_CERT_ID ) || (i  == KVS_DEVICE_PRIVKEY_ID )|| (i  == KVS_DEVICE_PUBKEY_ID ))
			{
				xSuccess = vDevModeKeyPreProvisioning(gKeyValueStore, (KVStoreKey_t)i,gKeyValueStore.table[ i ].valueLength);
				if (xSuccess == pdFALSE)
				{
					return xSuccess;
				}
				gKeyValueStore.table[ i ].xChangePending = pdFALSE;
			}
        	else
        	{
        		xSuccess = xprvWriteValueToImpl((KVStoreKey_t)i,(char *)gKeyValueStore.table[ i ].value,
        		    											gKeyValueStore.table[ i ].valueLength);
				if (xSuccess == pdFALSE)
				{
					return xSuccess;
				}
				gKeyValueStore.table[ i ].xChangePending = pdFALSE;
        	}

        }
    }
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
    else if( gKeyValueStore.table[ key ].valueLength > sizeof( void * ) )
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
                                   size_t xBufferSize )
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
    }

    return( xDataLen > 0 );
}

static inline void * pvGetDataWritePtr( KVStoreKey_t key )
{
    void * pvData = NULL;

    if( gKeyValueStore.table[ key ].valueLength > sizeof( void * ) )
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

int32_t vprvCacheInit( void )
{
	int32_t xNvLength = -1;
	CK_RV xResult = CKR_OK;
	CK_BBOOL xIsPrivate = ( CK_BBOOL ) CK_TRUE;

	CK_BYTE pxCertLabel[] =  pkcs11configLABEL_DEVICE_CERTIFICATE_FOR_TLS ;
	CK_BYTE pxPrivKeyLabel[] =  pkcs11configLABEL_DEVICE_PRIVATE_KEY_FOR_TLS ;
	CK_FUNCTION_LIST_PTR pxFunctionList = NULL;
	CK_SESSION_HANDLE xSession = 0;

    /* Read from file system into ram */
    for( uint32_t i = 0; i < KVS_NUM_KEYS; i++ )
    {
        /* pvData pointer should be NULL on startup */

    	if ((i  == KVS_DEVICE_CERT_ID ) || (i  == KVS_DEVICE_PRIVKEY_ID )|| (i  == KVS_DEVICE_PUBKEY_ID ))
    	{
    		continue;
    	}
    	else
    	{
    		gKeyValueStore.table[ i ].xChangePending = pdFALSE;
			gKeyValueStore.table[ i ].type = KV_TYPE_NONE;

			xNvLength = xprvGetValueLengthFromImpl( (KVStoreKey_t)i );
			char *xValue = NULL;
			if( xNvLength > 0 )
			{
				vAllocateDataBuffer( i, xNvLength );
				size_t * pxLength = &( gKeyValueStore.table[ i ].valueLength );
				strcpy( gKeyValueStore.table[i ].key, keys[ i ] );
				( void ) xprvReadValueFromImpl( (KVStoreKey_t)i,  &xValue, pxLength, *pxLength );
				gKeyValueStore.table[ i ].value = xValue;
			}
			else
			{
				return xNvLength;
			}
    	}

    }
    xResult = C_GetFunctionList( &pxFunctionList );

	/* Initialize the PKCS Module */
	if( xResult == CKR_OK )
	{
		xResult = xInitializePkcs11Token();
	}

	if( xResult == CKR_OK )
	{
		xResult = xInitializePkcs11Session( &xSession );
	}

	//private key

	xNvLength = PKCS11_PAL_FindObject(pxPrivKeyLabel,strlen(pxPrivKeyLabel));
	if (xNvLength <=0)
	{
		if( xResult == CKR_OK )
		{
			pxFunctionList->C_CloseSession( xSession );
		}
		return xNvLength;
	}
	//cert
	xNvLength = PKCS11_PAL_FindObject(pxCertLabel,strlen(pxCertLabel));
	if (xNvLength <=0)
	{
		if( xResult == CKR_OK )
		{
			pxFunctionList->C_CloseSession( xSession );
		}
		return xNvLength;
	}

	if( xResult == CKR_OK )
	{
		pxFunctionList->C_CloseSession( xSession );
	}
	return xNvLength;

}


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

char *GetStringValue( KVStoreKey_t key, size_t  pxLength )
{
	size_t xSizeWritten = 0;

	char* pcBuffer = NULL;
	if (pxLength > 0)
	{
		pcBuffer = pvPortMalloc( pxLength );
		xSizeWritten = xReadEntry(key, pcBuffer, pxLength);
		if (xSizeWritten == 0)
		{
			vPortFree( pcBuffer );
			pcBuffer = NULL;
		}
		else
		{
			pcBuffer[ xSizeWritten] = '\0';
		}
	}

//	if( pxLength != NULL )
//	{
//		*pxLength = xSizeWritten;
//	}

	return pcBuffer;
}

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
	CK_FUNCTION_LIST_PTR pxFunctionList = NULL;
	CK_SESSION_HANDLE xSession = 0;
    char *client_private_key, *client_certificate;
	char *tmp;
	uint32_t data_length = 0;

	CK_BBOOL xIsPrivate = ( CK_BBOOL ) CK_FALSE;
	CK_OBJECT_HANDLE xPalHandle = CK_INVALID_HANDLE;

	xKey = (KVStoreKey_t)Filename2Handle( key,pxLength);
	if (xKey < 0)
	{
		return NULL;
	}
	if ((xKey  == KVS_DEVICE_CERT_ID ) || (xKey  == KVS_DEVICE_PRIVKEY_ID )|| (xKey  == KVS_DEVICE_PUBKEY_ID ))
	{
		xResult = C_GetFunctionList( &pxFunctionList );

						/* Initialize the PKCS Module */
		if( xResult == CKR_OK )
		{
			xResult = xInitializePkcs11Token();
		}

		if( xResult == CKR_OK )
		{
			xResult = xInitializePkcs11Session( &xSession );
		}
	}

	gKeyValueStore.table[ xKey ].type = KV_TYPE_STRING;
	gKeyValueStore.table[ xKey ].xChangePending = pdFALSE;
	if ((xKey  == KVS_DEVICE_CERT_ID ) )
	{

		xPalHandle = PKCS11_PAL_FindObject(pxCertLabel,strlen(pxCertLabel));
		if (xPalHandle != CK_INVALID_HANDLE)
		{

			xResult = PKCS11_PAL_GetObjectValue( xPalHandle, &tmp, &data_length, &xIsPrivate );
			if( (xResult == CKR_OK )&&(data_length > 0))
			{
				vAllocateDataBuffer( xKey, data_length );
				strcpy( gKeyValueStore.table[xKey ].key, keys[ xKey ] );
				gKeyValueStore.table[ xKey ].value = tmp;
			}

		}


	}
	else if  (xKey  == KVS_DEVICE_PRIVKEY_ID )
	{
		xIsPrivate = ( CK_BBOOL ) CK_TRUE;
		xPalHandle = PKCS11_PAL_FindObject(pxPrivKeyLabel,strlen(pxPrivKeyLabel));
		if (xPalHandle != CK_INVALID_HANDLE)
		{

			xResult = PKCS11_PAL_GetObjectValue( xPalHandle, &tmp, &data_length, &xIsPrivate );
			if( (xResult == CKR_OK )&&(data_length > 0))
			{
				vAllocateDataBuffer( xKey, data_length );
				strcpy( gKeyValueStore.table[xKey ].key, keys[ xKey ] );
				gKeyValueStore.table[ xKey ].value = tmp;

			}
		}

	}
	else if  (xKey  == KVS_DEVICE_PUBKEY_ID )
	{
		xIsPrivate = ( CK_BBOOL ) CK_FALSE;
		xPalHandle = PKCS11_PAL_FindObject(pxPubKeyLabel,strlen(pxPubKeyLabel));
		if (xPalHandle != CK_INVALID_HANDLE)
		{

			xResult = PKCS11_PAL_GetObjectValue( xPalHandle, &tmp, &data_length, &xIsPrivate );
			if( (xResult == CKR_OK )&&(data_length > 0))
			{
				vAllocateDataBuffer( xKey, data_length );
				strcpy( gKeyValueStore.table[xKey ].key, keys[ xKey ] );
				gKeyValueStore.table[ xKey ].value = tmp;

			}
		}

	}

	else
	{


		xNvLength = xprvGetValueLengthFromImpl( xKey );
		char *xValue = NULL;
		if( xNvLength > 0 )
		{
			vAllocateDataBuffer( xKey, xNvLength );
			size_t * valueLength = &( gKeyValueStore.table[ xKey ].valueLength );
			strcpy( gKeyValueStore.table[ xKey ].key, keys[ xKey ] );
			( void ) xprvReadValueFromImpl( xKey,  &xValue, valueLength, *valueLength );
			gKeyValueStore.table[ xKey ].value = xValue;
		}

	}


	if ((xKey  == KVS_DEVICE_CERT_ID ) || (xKey  == KVS_DEVICE_PRIVKEY_ID )|| (xKey  == KVS_DEVICE_PUBKEY_ID ))
	{
		if (data_length > 0)
		{
			for(int32_t i = 0; i < strlen(gKeyValueStore.table[ xKey ].value); i++)
			{
				if(((gKeyValueStore.table[ xKey ].value)[i] < 0x20) || ((gKeyValueStore.table[ xKey ].value)[i] > 0x7e))
				{
					if(((gKeyValueStore.table[ xKey ].value)[i] != 0x0a) && ((gKeyValueStore.table[ xKey ].value)[i] != 0x0d))
					{
						buffervalue =  "Could not print: <binary data> is included.";
						break;
					}
				}
			}
			if( xResult == CKR_OK )
				{
					pxFunctionList->C_CloseSession( xSession );
				}
		}

	}
	else
	{

		buffervalue = GetStringValue(xKey,xNvLength);
	}

 	return buffervalue;
}
