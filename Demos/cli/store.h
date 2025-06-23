/*
 * Copyright (C) 2023 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
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
 * https://www.FreeRTOS.org
 * https://aws.amazon.com/freertos
 *
 */



#ifndef APPLICATION_CODE_STORE_H_
#define APPLICATION_CODE_STORE_H_

/* Standard includes. */
#include <stdio.h>
#include <string.h>
#include "../../../../../Common/littlefs_common/lfs_util_config.h"

#include "FreeRTOS.h"
#include "lfs.h"


extern lfs_t RM_STDIO_LITTLEFS_CFG_LFS;

#define KVSTORE_KEY_MAX_LEN    32
#define KVSTORE_VAL_MAX_LEN    2048



typedef enum KVStoreKey
{
    KVS_INVALID_KEY = -1,
    KVS_CORE_THING_NAME = 0,
    KVS_CORE_MQTT_ENDPOINT,
    KVS_DEVICE_CERT_ID,
    KVS_DEVICE_PRIVKEY_ID,
	KVS_DEVICE_PUBKEY_ID,
	KVS_ROOT_CA_ID,
	KVS_TEMPLATE_NAME,
	KVS_CLAIM_CERT_ID,
	KVS_CLAIM_PRIVKEY_ID,
    KVS_CODE_SIGN_CERT_ID,
	KVS_TSIP_ROOTCA_PUBKEY_ID,
	KVS_TSIP_CLIENT_PUBKEY_ID,
	KVS_TSIP_CLIENT_PRIKEY_ID,
    KVS_NUM_KEYS
} KVStoreKey_t;

typedef struct KVStoreEntry
{
	uint32_t type;
	char key[KVSTORE_KEY_MAX_LEN +1 ];
    uint32_t valueLength;
    char *value;
    BaseType_t xChangePending;

} KVStoreEntry_t;

typedef struct KeyValueStore
{
    KVStoreEntry_t table[ KVS_NUM_KEYS ];
} KeyValueStore_t;

/* Array to map between strings and KVStoreKey_t IDs */
#define KVSTORE_KEYS                                       \
    {                                                      \
        [ KVS_CORE_THING_NAME ] = "thing_name",            \
        [ KVS_CORE_MQTT_ENDPOINT ] = "mqtt_endpoint",      \
        [ KVS_DEVICE_CERT_ID ] = pkcs11configLABEL_DEVICE_CERTIFICATE_FOR_TLS,                \
		[ KVS_DEVICE_PRIVKEY_ID ] = pkcs11configLABEL_DEVICE_PRIVATE_KEY_FOR_TLS,         \
		[ KVS_DEVICE_PUBKEY_ID ] = pkcs11configLABEL_DEVICE_PUBLIC_KEY_FOR_TLS,         \
		[ KVS_ROOT_CA_ID ] = "root_ca_id",         \
		[ KVS_TEMPLATE_NAME ] = "template_name",         \
		[ KVS_CLAIM_CERT_ID ] = pkcs11configLABEL_CLAIM_CERTIFICATE,         \
		[ KVS_CLAIM_PRIVKEY_ID ] = pkcs11configLABEL_CLAIM_PRIVATE_KEY,         \
		[ KVS_CODE_SIGN_CERT_ID ] = "code_sign_cert_id",         \
		[ KVS_TSIP_ROOTCA_PUBKEY_ID ] = "tsip_rootca_pub_id",         \
		[ KVS_TSIP_CLIENT_PUBKEY_ID ] = "tsip_client_pub_id",         \
		[ KVS_TSIP_CLIENT_PRIKEY_ID ] = "tsip_client_pri_id",         \
}
#define CLICMDKEYS                                       \
    {                                                      \
        [ KVS_CORE_THING_NAME ] = "thingname",            \
        [ KVS_CORE_MQTT_ENDPOINT ] = "endpoint",      \
        [ KVS_DEVICE_CERT_ID ] = "cert",                \
		[ KVS_DEVICE_PRIVKEY_ID ] = "key",         \
		[ KVS_DEVICE_PUBKEY_ID ] = "pub",         \
		[ KVS_ROOT_CA_ID ] = "rootca",                \
		[ KVS_TEMPLATE_NAME ] = "template",         \
		[ KVS_CLAIM_CERT_ID ] = "claimcert",                \
		[ KVS_CLAIM_PRIVKEY_ID ] = "claimkey",         \
        [ KVS_CODE_SIGN_CERT_ID ] = "codesigncert",         \
		[ KVS_TSIP_ROOTCA_PUBKEY_ID ] = "tsiprootkey",         \
		[ KVS_TSIP_CLIENT_PUBKEY_ID ] = "tsippubkey",         \
		[ KVS_TSIP_CLIENT_PRIKEY_ID ] = "tsipprikey",         \
    }
typedef enum KVStoreKeytype
{
    KV_TYPE_NONE = 0,
    KV_TYPE_INT32,
    KV_TYPE_UINT32,
    KV_TYPE_STRING
} KVStoreValueType_t;

int32_t xprvGetValueLengthFromImpl( KVStoreKey_t keyIndex);
int32_t GetTotalLengthFromImpl();
BaseType_t xprvWriteValueToImpl (KVStoreKey_t keyIndex, char *pucData, uint32_t ulDataSize);
int32_t xprvReadValueFromImpl (KVStoreKey_t keyIndex,
        char **ppucData,
		uint32_t *pulDataSize,
		size_t xBufferSize);
int32_t xprvWriteCacheEntry(size_t KeyLength,
						char * Key,
						size_t ValueLength,
						char * pvNewValue );
static inline void vAllocateDataBuffer( uint32_t key,
                                        size_t xNewLength );
static inline void vReallocDataBuffer( KVStoreKey_t key,
                                       size_t xNewLength );
int32_t Filename2Handle( char * pcFileName,size_t KeyLength);
static inline void vLfsSSizeToErr( lfs_ssize_t * pxReturnValue,
                                   size_t xExpectedLength );
static inline const void * pvGetDataReadPtr( KVStoreKey_t key );
static inline void * pvGetDataWritePtr( KVStoreKey_t key );
int32_t vprvCacheInit( void );
void vprvCacheFormat( void );
BaseType_t xprvCopyValueFromCache( KVStoreKey_t xKey,
                                   KVStoreValueType_t * pxDataType,
                                   size_t * pxDataLength,
                                   void * pvBuffer,
                                   size_t xBufferSize );
size_t xReadEntry( KVStoreKey_t xKey,
                                   void * pvBuffer,
                                   size_t xBufferSize );
char *GetStringValue( KVStoreKey_t key,
        size_t  pxLength );
char *xprvGetCacheEntry(char * Key, size_t pxLength );
BaseType_t KVStore_xCommitChanges( void );
size_t prvGetCacheEntryLength( KVStoreKey_t xKey );
#endif /* APPLICATION_CODE_STORE_H_ */
