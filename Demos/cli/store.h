/*
 * store.h
 *
 *  Created on: Jan 25, 2023
 *      Author: MyPC
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
    KVS_CORE_THING_NAME = 0,
    KVS_CORE_MQTT_ENDPOINT,
    KVS_DEVICE_CERT_ID,
    KVS_DEVICE_PRIVKEY_ID,
	KVS_DEVICE_PUBKEY_ID,
	KVS_ROOT_CA_ID,
	KVS_TEMPLATE_NAME,
	KVS_CLAIM_CERT_ID,
	KVS_CLAIM_PRIVKEY_ID,
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
        [ KVS_DEVICE_CERT_ID ] = "cert_id",                \
		[ KVS_DEVICE_PRIVKEY_ID ] = "priv_key_id",         \
		[ KVS_DEVICE_PUBKEY_ID ] = "pub_key_id",         \
		[ KVS_ROOT_CA_ID ] = "root_ca_id",         \
		[ KVS_TEMPLATE_NAME ] = "template_name",         \
		[ KVS_CLAIM_CERT_ID ] = "claim_cert_id",         \
		[ KVS_CLAIM_PRIVKEY_ID ] = "claim_priv_key_id",         \
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
    }
typedef enum KVStoreKeytype
{
    KV_TYPE_NONE = 0,
    KV_TYPE_INT32,
    KV_TYPE_UINT32,
    KV_TYPE_STRING
} KVStoreValueType_t;

BaseType_t prvSaveConfigStore( void );
int32_t xprvGetValueLengthFromImpl( KVStoreKey_t keyIndex);
BaseType_t xprvWriteValueToImpl (KVStoreKey_t keyIndex, char *pucData, uint32_t ulDataSize);
int32_t xprvReadValueFromImpl (KVStoreKey_t keyIndex,
        char **ppucData,
		uint32_t *pulDataSize,
		size_t xBufferSize);
void LoadConfigStore(int32_t xHandle);
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

#endif /* APPLICATION_CODE_STORE_H_ */
