/*
 * FreeRTOS V202112.00
 * Copyright (C) 2020 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
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
 * https://github.com/FreeRTOS
 *
 */

/*
 * Demo for showing use of the Fleet Provisioning library to use the Fleet
 * Provisioning feature of AWS IoT Core for provisioning devices with
 * credentials. This demo shows how a device can be provisioned with AWS IoT
 * Core using the Certificate Signing Request workflow of the Fleet
 * Provisioning feature.
 *
 * The Fleet Provisioning library provides macros and helper functions for
 * assembling MQTT topics strings, and for determining whether an incoming MQTT
 * message is related to the Fleet Provisioning API of AWS IoT Core. The Fleet
 * Provisioning library does not depend on any particular MQTT library,
 * therefore the functionality for MQTT operations is placed in another file
 * (mqtt_operations.c). This demo uses the coreMQTT library. If needed,
 * mqtt_operations.c can be modified to replace coreMQTT with another MQTT
 * library. This demo requires using the AWS IoT Core broker as Fleet
 * Provisioning is an AWS IoT Core feature.
 *
 * This demo provisions a device certificate using the provisioning by claim
 * workflow with a Certificate Signing Request (CSR). The demo connects to AWS
 * IoT Core using provided claim credentials (whose certificate needs to be
 * registered with IoT Core before running this demo), subscribes to the
 * CreateCertificateFromCsr topics, and obtains a certificate. It then
 * subscribes to the RegisterThing topics and activates the certificate and
 * obtains a Thing using the provisioning template. Finally, it reconnects to
 * AWS IoT Core using the new credentials.
 */

/* Standard includes. */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Kernel includes. */
#include "FreeRTOS.h"
#include "task.h"

/* Demo Config */
#include "demo_config.h"

/* mbedTLS include for configuring threading functions */
#include "mbedtls/threading.h"
#include "threading_alt.h"

/* TinyCBOR library for CBOR encoding and decoding operations. */
#include "cbor.h"

/* corePKCS11 includes. */
#include "core_pkcs11.h"
#include "core_pkcs11_config.h"
#include "core_pkcs11_config_defaults.h"

/* AWS IoT Fleet Provisioning Library. */
#include "fleet_provisioning.h"

/* MQTT agent task API. */
#include "mqtt_agent_task.h"

/* Demo includes. */
#include "mqtt_pkcs11_demo_helpers.h"
#include "pkcs11_operations.h"
#include "tinycbor_serializer.h"
#include "store.h"

#include "unique_id.h"

/**
 * These configurations are required. Throw compilation error if it is not
 * defined.
 */
#ifndef democonfigPROVISIONING_TEMPLATE_NAME
    #error "Please define democonfigPROVISIONING_TEMPLATE_NAME to the template name registered with AWS IoT Core in demo_config.h."
#endif
#ifndef democonfigROOT_CA_PEM
    #error "Please define Root CA certificate of the MQTT broker(democonfigROOT_CA_PEM) in demo_config.h."
#endif

/**
 * @brief The length of #democonfigPROVISIONING_TEMPLATE_NAME.
 */
#define fpdemoPROVISIONING_TEMPLATE_NAME_LENGTH    ( ( uint16_t ) ( sizeof( democonfigPROVISIONING_TEMPLATE_NAME ) - 1 ) )

/**
 * @brief The length of #democonfigFP_DEMO_ID.
 */
#define fpdemoFP_DEMO_ID_LENGTH                    ( ( uint16_t ) ( sizeof( democonfigFP_DEMO_ID ) - 1 ) )

/**
 * @brief Size of AWS IoT Thing name buffer.
 *
 * See https://docs.aws.amazon.com/iot/latest/apireference/API_CreateThing.html#iot-CreateThing-request-thingName
 */
#define fpdemoMAX_THING_NAME_LENGTH                128

/**
 * @brief Size of CSR subject name buffer.
 *
 * See https://docs.aws.amazon.com/iot/latest/apireference/API_CreateThing.html#iot-CreateThing-request-thingName
 */
#define fpdemoMAX_CSR_SUBJECT_NAME_LENGTH          128

/**
 * @brief The maximum number of times to run the loop in this demo.
 *
 * @note The demo loop is attempted to re-run only if it fails in an iteration.
 * Once the demo loop succeeds in an iteration, the demo exits successfully.
 */
#ifndef fpdemoMAX_DEMO_LOOP_COUNT
    #define fpdemoMAX_DEMO_LOOP_COUNT    ( 3 )
#endif

/**
 * @brief Time in seconds to wait between retries of the demo loop if
 * demo loop fails.
 */
#define fpdemoDELAY_BETWEEN_DEMO_RETRY_ITERATIONS_SECONDS    ( 5 )

/**
 * @brief Size of buffer in which to hold the certificate signing request (CSR).
 */
#define fpdemoCSR_BUFFER_LENGTH                              2048

/**
 * @brief Size of buffer in which to hold the certificate.
 */
#define fpdemoCERT_BUFFER_LENGTH                             2048

/**
 * @brief Size of buffer in which to hold the certificate id.
 *
 * See https://docs.aws.amazon.com/iot/latest/apireference/API_Certificate.html#iot-Type-Certificate-certificateId
 */
#define fpdemoCERT_ID_BUFFER_LENGTH                          64

/**
 * @brief Size of buffer in which to hold the certificate ownership token.
 */
#define fpdemoOWNERSHIP_TOKEN_BUFFER_LENGTH                  512

/**
 * @brief Milliseconds per second.
 */
#define fpdemoMILLISECONDS_PER_SECOND                        ( 1000U )

/**
 * @brief Milliseconds per FreeRTOS tick.
 */
#define fpdemoMILLISECONDS_PER_TICK                          ( fpdemoMILLISECONDS_PER_SECOND / configTICK_RATE_HZ )

/**
 * @brief Status values of the Fleet Provisioning response.
 */
typedef enum
{
    ResponseNotReceived,
    ResponseAccepted,
    ResponseRejected
} ResponseStatus_t;


/**
 * @brief Each compilation unit that consumes the NetworkContext must define it.
 * It should contain a single pointer to the type of your desired transport.
 * When using multiple transports in the same compilation unit, define this pointer as void *.
 *
 * @note Transport stacks are defined in FreeRTOS-Plus/Source/Application-Protocols/network_transport.
 */
struct NetworkContext
{
    TlsTransportParams_t * pxParams;
};

/*-----------------------------------------------------------*/

/**
 * @brief Buffer to hold the fleet provisioning demo ID.
 */
static char pcDemoID[ fpdemoMAX_THING_NAME_LENGTH ] = { 0 };

/**
 * @brief Buffer to hold the provisioned AWS IoT Thing name.
 */
static char pcThingName[ fpdemoMAX_THING_NAME_LENGTH ] = { 0 };

/**
 * @brief Buffer to hold the CSR subject name.
 */
static char pcCSRSubjectName[ fpdemoMAX_CSR_SUBJECT_NAME_LENGTH ] = { 0 };

/*-----------------------------------------------------------*/

/**
 * @brief Status reported from the MQTT publish callback.
 */
static ResponseStatus_t xResponseStatus;

/**
 * @brief Length of the AWS IoT Thing name.
 */
static size_t xThingNameLength;

/**
 * @brief Buffer to hold responses received from the AWS IoT Fleet Provisioning
 * APIs. When the MQTT publish callback receives an expected Fleet Provisioning
 * accepted payload, it copies it into this buffer.
 */
static uint8_t pucPayloadBuffer[ democonfigNETWORK_BUFFER_SIZE ];

/**
 * @brief Length of the payload stored in #pucPayloadBuffer. This is set by the
 * MQTT publish callback when it copies a received payload into #pucPayloadBuffer.
 */
static size_t xPayloadLength;

/**
 * @brief The MQTT context used for MQTT operation.
 */
static MQTTContext_t xMqttContext;

/**
 * @brief The network context used for mbedTLS operation.
 */
static NetworkContext_t xNetworkContext;

/**
 * @brief The parameters for the network context using mbedTLS operation.
 */
static TlsTransportParams_t xTlsTransportParams;

/**
 * @brief Static buffer used to hold MQTT messages being sent and received.
 */
static uint8_t ucSharedBuffer[ democonfigNETWORK_BUFFER_SIZE ];

/**
 * @brief Static buffer used to hold MQTT messages being sent and received.
 */
static MQTTFixedBuffer_t xBuffer =
{
    ucSharedBuffer,
    democonfigNETWORK_BUFFER_SIZE
};

/**
 * @brief Accept topic/reject topic/publish topic
 */
static char * pcPublishTopic = NULL;
static uint16_t xPublishTopicLength;

static char * pcAcceptTopic = NULL;
static uint16_t xAcceptTopicLength;

static char * pcRejectTopic = NULL;
static uint16_t xRejectTopicLength;
/*-----------------------------------------------------------*/

/**
 * @brief Callback to receive the incoming publish messages from the MQTT
 * broker. Sets xResponseStatus if an expected CreateCertificateFromCsr or
 * RegisterThing response is received, and copies the response into
 * responseBuffer if the response is an accepted one.
 *
 * @param[in] pPublishInfo Pointer to publish info of the incoming publish.
 * @param[in] usPacketIdentifier Packet identifier of the incoming publish.
 */
static void prvProvisioningPublishCallback( MQTTContext_t * pxMqttContext,
                                            MQTTPacketInfo_t * pxPacketInfo,
                                            MQTTDeserializedInfo_t * pxDeserializedInfo );

/**
 * @brief Subscribe to the CreateCertificateFromCsr accepted and rejected topics.
 */
static bool prvSubscribeToCsrResponseTopics( void );

/**
 * @brief Unsubscribe from the CreateCertificateFromCsr accepted and rejected topics.
 */
static bool prvUnsubscribeFromCsrResponseTopics( void );

/**
 * @brief Subscribe to the RegisterThing accepted and rejected topics.
 */
static bool prvSubscribeToRegisterThingResponseTopics( void );

/**
 * @brief Unsubscribe from the RegisterThing accepted and rejected topics.
 */
static bool prvUnsubscribeFromRegisterThingResponseTopics( void );

/**
 * @brief The task used to demonstrate the FP API.
 *
 * This task uses the provided claim key and certificate files to connect to
 * AWS and use PKCS #11 to generate a new device key and certificate with a CSR.
 * The task then creates a new Thing with the Fleet Provisioning API using the
 * newly-created credentials. The task finishes by connecting to the newly-created
 * Thing to verify that it was successfully created and accessible using the key/cert.
 *
 * @param[in] pvParameters Parameters as passed at the time of task creation.
 * Not used in this example.
 */
static int prvFleetProvisioningTask( void * pvParameters );

void vStartFleetProvisioningDemo(void);

/*-----------------------------------------------------------*/

static void prvProvisioningPublishCallback( MQTTContext_t * pxMqttContext,
                                            MQTTPacketInfo_t * pxPacketInfo,
                                            MQTTDeserializedInfo_t * pxDeserializedInfo )
{
    FleetProvisioningStatus_t xStatus;
    FleetProvisioningTopic_t xApi;
    MQTTPublishInfo_t * pxPublishInfo;

    configASSERT( pxMqttContext != NULL );
    configASSERT( pxPacketInfo != NULL );
    configASSERT( pxDeserializedInfo != NULL );

    /* Suppress the unused parameter warning when asserts are disabled in
     * build. */
    ( void ) pxMqttContext;

    /* Handle an incoming publish. The lower 4 bits of the publish packet
     * type is used for the dup, QoS, and retain flags. Hence masking
     * out the lower bits to check if the packet is publish. */
    if( ( pxPacketInfo->type & 0xF0U ) == MQTT_PACKET_TYPE_PUBLISH )
    {
        configASSERT( pxDeserializedInfo->pPublishInfo != NULL );
        pxPublishInfo = pxDeserializedInfo->pPublishInfo;

        xStatus = FleetProvisioning_MatchTopic(pxPublishInfo->pTopicName,
                                               pxPublishInfo->topicNameLength,
                                               &xApi);

        if (xStatus != FleetProvisioningSuccess)
        {
            LogWarn( ( "Unexpected publish message received. Topic: %.*s.",
                       ( int ) pxPublishInfo->topicNameLength,
                       ( const char * ) pxPublishInfo->pTopicName ) );
        }
        else
        {
            if (xApi == FleetProvCborCreateCertFromCsrAccepted)
            {
                LogInfo( ( "Received accepted response from Fleet Provisioning CreateCertificateFromCsr API." ) );

                xResponseStatus = ResponseAccepted;

                /* Copy the payload from the MQTT library's buffer to #pucPayloadBuffer. */
                ( void ) memcpy( ( void * ) pucPayloadBuffer,
                                 ( const void * ) pxPublishInfo->pPayload,
                                 ( size_t ) pxPublishInfo->payloadLength );

                xPayloadLength = pxPublishInfo->payloadLength;
            }
            else if (xApi == FleetProvCborCreateCertFromCsrRejected)
            {
                LogError( ( "Received rejected response from Fleet Provisioning CreateCertificateFromCsr API." ) );

                xResponseStatus = ResponseRejected;
            }
            else if (xApi == FleetProvCborRegisterThingAccepted)
            {
                LogInfo( ( "Received accepted response from Fleet Provisioning RegisterThing API." ) );

                xResponseStatus = ResponseAccepted;

                /* Copy the payload from the MQTT library's buffer to #pucPayloadBuffer. */
                ( void ) memcpy( ( void * ) pucPayloadBuffer,
                                 ( const void * ) pxPublishInfo->pPayload,
                                 ( size_t ) pxPublishInfo->payloadLength );

                xPayloadLength = pxPublishInfo->payloadLength;
            }
            else if (xApi == FleetProvCborRegisterThingRejected)
            {
                LogError( ( "Received rejected response from Fleet Provisioning RegisterThing API." ) );

                xResponseStatus = ResponseRejected;
            }
            else
            {
                LogError( ( "Received message on unexpected Fleet Provisioning topic. Topic: %.*s.",
                            ( int ) pxPublishInfo->topicNameLength,
                            ( const char * ) pxPublishInfo->pTopicName ) );
            }
        }
    }
    else
    {
        vHandleOtherIncomingPacket( pxPacketInfo, pxDeserializedInfo->packetIdentifier );
        xResponseStatus = ResponseAccepted;
    }
}
/*-----------------------------------------------------------*/

static bool prvSubscribeToCsrResponseTopics( void )
{
    bool xStatus;

    xStatus = xSubscribeToTopic( &xMqttContext,
                                 FP_CBOR_CREATE_CERT_ACCEPTED_TOPIC,
                                 FP_CBOR_CREATE_CERT_ACCEPTED_LENGTH );

    if( xStatus == false )
    {
        LogError( ( "Failed to subscribe to fleet provisioning topic: %.*s.",
                    FP_CBOR_CREATE_CERT_ACCEPTED_LENGTH,
                    FP_CBOR_CREATE_CERT_ACCEPTED_TOPIC ) );
    }

    if( xStatus == true )
    {
        xStatus = xSubscribeToTopic( &xMqttContext,
                                     FP_CBOR_CREATE_CERT_REJECTED_TOPIC,
                                     FP_CBOR_CREATE_CERT_REJECTED_LENGTH );

        if( xStatus == false )
        {
            LogError( ( "Failed to subscribe to fleet provisioning topic: %.*s.",
                        FP_CBOR_CREATE_CERT_REJECTED_LENGTH,
                        FP_CBOR_CREATE_CERT_REJECTED_TOPIC ) );
        }
    }

    return xStatus;
}
/*-----------------------------------------------------------*/

static bool prvUnsubscribeFromCsrResponseTopics( void )
{
    bool xStatus;

    xStatus = xUnsubscribeFromTopic( &xMqttContext,
                                     FP_CBOR_CREATE_CERT_ACCEPTED_TOPIC,
                                     FP_CBOR_CREATE_CERT_ACCEPTED_LENGTH );

    if( xStatus == false )
    {
        LogError( ( "Failed to unsubscribe from fleet provisioning topic: %.*s.",
                    FP_CBOR_CREATE_CERT_ACCEPTED_LENGTH,
                    FP_CBOR_CREATE_CERT_ACCEPTED_TOPIC ) );
    }

    if( xStatus == true )
    {
        xStatus = xUnsubscribeFromTopic( &xMqttContext,
                                         FP_CBOR_CREATE_CERT_REJECTED_TOPIC,
                                         FP_CBOR_CREATE_CERT_REJECTED_LENGTH );

        if( xStatus == false )
        {
            LogError( ( "Failed to unsubscribe from fleet provisioning topic: %.*s.",
                        FP_CBOR_CREATE_CERT_REJECTED_LENGTH,
                        FP_CBOR_CREATE_CERT_REJECTED_TOPIC ) );
        }
    }

    return xStatus;
}
/*-----------------------------------------------------------*/

static bool prvSubscribeToRegisterThingResponseTopics( void )
{
    bool xStatus;

    xStatus = xSubscribeToTopic( &xMqttContext,
                                 pcAcceptTopic,
                                 xAcceptTopicLength );

    if( xStatus == false )
    {
        LogError( ( "Failed to subscribe to fleet provisioning topic: %.*s.",
                    xAcceptTopicLength,
                    pcAcceptTopic ) );
    }

    if( xStatus == true )
    {
        xStatus = xSubscribeToTopic( &xMqttContext,
                                     pcRejectTopic,
                                     xRejectTopicLength );

        if( xStatus == false )
        {
            LogError( ( "Failed to subscribe to fleet provisioning topic: %.*s.",
                        xRejectTopicLength,
                        pcRejectTopic ) );
        }
    }

    return xStatus;
}
/*-----------------------------------------------------------*/

static bool prvUnsubscribeFromRegisterThingResponseTopics( void )
{
    bool xStatus;

    xStatus = xUnsubscribeFromTopic( &xMqttContext,
                                     pcAcceptTopic,
                                     xAcceptTopicLength );

    if( xStatus == false )
    {
        LogError( ( "Failed to unsubscribe from fleet provisioning topic: %.*s.",
                    xAcceptTopicLength,
                    pcAcceptTopic ) );
    }

    if( xStatus == true )
    {
        xStatus = xUnsubscribeFromTopic( &xMqttContext,
                                         pcRejectTopic,
                                         xRejectTopicLength );

        if( xStatus == false )
        {
            LogError( ( "Failed to unsubscribe from fleet provisioning topic: %.*s.",
                        xRejectTopicLength,
                        pcRejectTopic ) );
        }
    }

    return xStatus;
}
/*-----------------------------------------------------------*/

/**
 * @brief Create the task that demonstrates the Fleet Provisioning library API
 */
void vStartFleetProvisioningDemo()
{
    /* This example uses a single application task, which shows that how to use
     * Fleet Provisioning library to generate and sign certificates with AWS IoT
     * and create new IoT Things using the AWS IoT Fleet Provisioning API */
    xTaskCreate( prvFleetProvisioningTask, /* Function that implements the task. */
                 "DemoTask",               /* Text name for the task - only used for debugging. */
                 democonfigDEMO_STACKSIZE, /* Size of stack (in words, not bytes) to allocate for the task. */
                 NULL,                     /* Task parameter - not used in this case. */
                 tskIDLE_PRIORITY,         /* Task priority, must be between 0 and configMAX_PRIORITIES - 1. */
                 NULL );                   /* Used to pass out a handle to the created task - not used in this case. */
}

/* This example uses a single application task, which shows that how to use
 * the Fleet Provisioning library to generate and validate AWS IoT Fleet
 * Provisioning MQTT topics, and use the coreMQTT library to communicate with
 * the AWS IoT Fleet Provisioning APIs. */
int prvFleetProvisioningTask( void * pvParameters )
{
    bool xStatus = false;
    /* Buffer for holding the CSR. */
    char pcCsr[ fpdemoCSR_BUFFER_LENGTH ] = { 0 };
    size_t xCsrLength = 0;
    /* Buffer for holding received certificate until it is saved. */
    char pcCertificate[ fpdemoCERT_BUFFER_LENGTH ];
    size_t xCertificateLength;
    /* Buffer for holding the certificate ID. */
    char pcCertificateId[ fpdemoCERT_ID_BUFFER_LENGTH ];
    size_t xCertificateIdLength;
    /* Buffer for holding the certificate ownership token. */
    char pcOwnershipToken[ fpdemoOWNERSHIP_TOKEN_BUFFER_LENGTH ];
    size_t xOwnershipTokenLength;
    bool xConnectionEstablished = false;
    CK_SESSION_HANDLE xP11Session;
    uint32_t ulDemoRunCount = 0U;
    CK_RV xPkcs11Ret = CKR_OK;
    CK_OBJECT_HANDLE xClientCertificate;
    CK_OBJECT_HANDLE xPrivateKey;
    uuid_param_t uuid;
    size_t templateLength;
    size_t thingnameLength;
    char *templateData = NULL;
    char *thingnameData = NULL;
    char *template_buff = NULL;
    char *thingname_buff = NULL;


    LogInfo( ( "---------Start Fleet Provisioning Task---------\r\n" ) );

    templateLength = prvGetCacheEntryLength(KVS_TEMPLATE_NAME);

#if defined(__TEST__)
    pcPublishTopic = FP_CBOR_REGISTER_PUBLISH_TOPIC( democonfigPROVISIONING_TEMPLATE_NAME );
    xPublishTopicLength = FP_CBOR_REGISTER_PUBLISH_LENGTH( fpdemoPROVISIONING_TEMPLATE_NAME_LENGTH );
    pcAcceptTopic = FP_CBOR_REGISTER_ACCEPTED_TOPIC( democonfigPROVISIONING_TEMPLATE_NAME );
    xAcceptTopicLength = FP_CBOR_REGISTER_ACCEPTED_LENGTH( fpdemoPROVISIONING_TEMPLATE_NAME_LENGTH );
    pcRejectTopic = FP_CBOR_REGISTER_REJECTED_TOPIC( democonfigPROVISIONING_TEMPLATE_NAME );
    xRejectTopicLength = FP_CBOR_REGISTER_REJECTED_LENGTH( fpdemoPROVISIONING_TEMPLATE_NAME_LENGTH ) ;
#else
    if (templateLength > 0)
    {

    	template_buff = pvPortMalloc(templateLength + 1);
    	xReadEntry(KVS_TEMPLATE_NAME, template_buff, templateLength );
    	template_buff[templateLength] = '\0';
    	templateData = template_buff;

    	xPublishTopicLength = FP_CBOR_REGISTER_PUBLISH_LENGTH(templateLength);
    	pcPublishTopic = pvPortMalloc(xPublishTopicLength + 1);
		snprintf( pcPublishTopic, xPublishTopicLength + 1, "%s%s%s%s", FP_REGISTER_API_PREFIX, templateData, FP_REGISTER_API_BRIDGE, FP_API_CBOR_FORMAT);

        xAcceptTopicLength = FP_CBOR_REGISTER_ACCEPTED_LENGTH(templateLength);
        pcAcceptTopic = pvPortMalloc(xAcceptTopicLength + 1);
        snprintf( pcAcceptTopic, xAcceptTopicLength + 1, "%s%s%s%s%s", FP_REGISTER_API_PREFIX, templateData, FP_REGISTER_API_BRIDGE, FP_API_CBOR_FORMAT,FP_API_ACCEPTED_SUFFIX);

        xRejectTopicLength = FP_CBOR_REGISTER_REJECTED_LENGTH(templateLength);
        pcRejectTopic = pvPortMalloc(xRejectTopicLength + 1);
        snprintf( pcRejectTopic, xRejectTopicLength + 1, "%s%s%s%s%s", FP_REGISTER_API_PREFIX, templateData, FP_REGISTER_API_BRIDGE, FP_API_CBOR_FORMAT,FP_API_REJECTED_SUFFIX);
    }

#endif


    /* Silence compiler warnings about unused variables. */
    ( void ) pvParameters;

    /* Generate unique fleet provisioning demo ID. */
    get_unique_id(&uuid);
    snprintf( pcDemoID, fpdemoMAX_THING_NAME_LENGTH, "%s_%08X_%08X_%08X_%08X", democonfigFP_DEMO_ID, uuid.uuid0, uuid.uuid1, uuid.uuid2, uuid.uuid3);
    snprintf( pcCSRSubjectName, fpdemoMAX_CSR_SUBJECT_NAME_LENGTH, "CN=%s", pcDemoID);

    /* Set the pParams member of the network context with desired transport. */
    xNetworkContext.pxParams = &xTlsTransportParams;

    do
    {
        /* Initialize the buffer lengths to their max lengths. */
        xCertificateLength = fpdemoCERT_BUFFER_LENGTH;
        xCertificateIdLength = fpdemoCERT_ID_BUFFER_LENGTH;
        xOwnershipTokenLength = fpdemoOWNERSHIP_TOKEN_BUFFER_LENGTH;

        /* Initialize the PKCS #11 module */
        xPkcs11Ret = xInitializePkcs11Session( &xP11Session );

        if( xPkcs11Ret != CKR_OK )
        {
            LogError( ( "Failed to initialize PKCS #11." ) );
            xStatus = false;
        }
        else
        {
            xPkcs11Ret = xGetCertificateAndKeyState( xP11Session,
                                                     &xClientCertificate,
                                                     &xPrivateKey );
            if( xPkcs11Ret != CKR_OK )
            {
                LogError( ( "Failed to get state of device certificate and private key." ) );
                xStatus = false;
            }
        }

        thingnameLength = prvGetCacheEntryLength(KVS_CORE_THING_NAME);

        if ( (xPkcs11Ret == CKR_OK) && ((xClientCertificate == CK_INVALID_HANDLE) || (xPrivateKey == CK_INVALID_HANDLE) || thingnameLength == 0) )
        {
            xPkcs11Ret = xDestroyCertificateAndKey ( xP11Session );
            if( xPkcs11Ret != CKR_OK )
            {
                LogError( ( "Failed to Destroy of device certificate and private key." ) );
                xStatus = false;
            }
            else
            {
                xStatus = xGenerateKeyAndCsr( xP11Session,
                                              pkcs11configLABEL_DEVICE_PRIVATE_KEY_FOR_TLS,
                                              pkcs11configLABEL_DEVICE_PUBLIC_KEY_FOR_TLS,
                                              pcCsr,
                                              fpdemoCSR_BUFFER_LENGTH,
                                              &xCsrLength,
                                              pcCSRSubjectName );

                if( xStatus == false )
                {
                    LogError( ( "Failed to generate Key and Certificate Signing Request." ) );
                }
            }
        }

        else if ( (xPkcs11Ret == CKR_OK) && (xClientCertificate != CK_INVALID_HANDLE) && (xPrivateKey != CK_INVALID_HANDLE) && (thingnameLength > 0) )
        {
            LogInfo( ( "It uses the device certificate, private key, thing name already stored." ) );
            thingname_buff = pvPortMalloc(thingnameLength + 1);
            xReadEntry(KVS_CORE_THING_NAME, thingname_buff, thingnameLength );
            thingname_buff[thingnameLength] = '\0';
            snprintf( pcThingName, fpdemoMAX_THING_NAME_LENGTH, "%s", thingname_buff);
            xStatus = true;
        }
        else
        {
            LogError( ( "Failed to initialize PKCS #11 or get state." ) );
            xStatus = false;
        }

        /* Skip fleet provisioning if you already have a device certificate, private key and thingname. */
        if ( (xClientCertificate == CK_INVALID_HANDLE) || (xPrivateKey == CK_INVALID_HANDLE) || thingnameLength == 0 )
        {
            /**** Connect to AWS IoT Core with provisioning claim credentials *****/

            /* We first use the claim credentials to connect to the broker. These
             * credentials should allow use of the RegisterThing API and one of the
             * CreateCertificatefromCsr or CreateKeysAndCertificate.
             * In this demo we use CreateCertificatefromCsr. */
            if( xStatus == true )
            {
                /* Attempts to connect to the AWS IoT MQTT broker. If the
                 * connection fails, retries after a timeout. Timeout value will
                 * exponentially increase until maximum attempts are reached. */
                LogInfo( ( "Establishing MQTT session with claim certificate..." ) );
                xStatus = xEstablishMqttSession( &xMqttContext,
                                                 &xNetworkContext,
                                                 &xBuffer,
                                                 prvProvisioningPublishCallback,
                                                 pkcs11configLABEL_CLAIM_CERTIFICATE,
                                                 pkcs11configLABEL_CLAIM_PRIVATE_KEY,
                                                 pcDemoID);

                if( xStatus == false )
                {
                    LogError( ( "Failed to establish MQTT session." ) );
                }
                else
                {
                    LogInfo( ( "Established connection with claim credentials." ) );
                    xConnectionEstablished = true;
                }
            }

            /**** Call the CreateCertificateFromCsr API ***************************/

            /* We use the CreateCertificatefromCsr API to obtain a client certificate
             * for a key on the device by means of sending a certificate signing
             * request (CSR). */
            if( xStatus == true )
            {
                /* Subscribe to the CreateCertificateFromCsr accepted and rejected
                 * topics. In this demo we use CBOR encoding for the payloads,
                 * so we use the CBOR variants of the topics. */
                xStatus = prvSubscribeToCsrResponseTopics();
            }

            if( xStatus == true )
            {
                /* Create the request payload containing the CSR to publish to the
                 * CreateCertificateFromCsr APIs. */
                xStatus = xGenerateCsrRequest( pucPayloadBuffer,
                                               democonfigNETWORK_BUFFER_SIZE,
                                               pcCsr,
                                               xCsrLength,
                                               &xPayloadLength );
            }

            if( xStatus == true )
            {
                /* Publish the CSR to the CreateCertificatefromCsr API. */
                xStatus = xPublishToTopic( &xMqttContext,
                                           FP_CBOR_CREATE_CERT_PUBLISH_TOPIC,
                                           FP_CBOR_CREATE_CERT_PUBLISH_LENGTH,
                                           ( char * ) pucPayloadBuffer,
                                           xPayloadLength );

                if( xStatus == false )
                {
                    LogError( ( "Failed to publish to fleet provisioning topic: %.*s.",
                                FP_CBOR_CREATE_CERT_PUBLISH_LENGTH,
                                FP_CBOR_CREATE_CERT_PUBLISH_TOPIC ) );
                }
            }

            if( xStatus == true )
            {
                /* From the response, extract the certificate, certificate ID, and
                 * certificate ownership token. */
                xStatus = xParseCsrResponse( pucPayloadBuffer,
                                             xPayloadLength,
                                             pcCertificate,
                                             &xCertificateLength,
                                             pcCertificateId,
                                             &xCertificateIdLength,
                                             pcOwnershipToken,
                                             &xOwnershipTokenLength );

                if( xStatus == true )
                {
                    LogInfo( ( "Received certificate with Id: %.*s", ( int ) xCertificateIdLength, pcCertificateId ) );
                }
            }

            if( xStatus == true )
            {
                /* Save the certificate into PKCS #11. */
                xStatus = xLoadCertificate( xP11Session,
                                            pcCertificate,
                                            pkcs11configLABEL_DEVICE_CERTIFICATE_FOR_TLS,
                                            xCertificateLength );
            }

            if( xStatus == true )
            {
                /* Unsubscribe from the CreateCertificateFromCsr topics. */
                xStatus = prvUnsubscribeFromCsrResponseTopics();
            }

            /**** Call the RegisterThing API **************************************/

            /* We then use the RegisterThing API to activate the received certificate,
             * provision AWS IoT resources according to the provisioning template, and
             * receive device configuration. */
            if( xStatus == true )
            {
                /* Create the request payload to publish to the RegisterThing API. */
                xStatus = xGenerateRegisterThingRequest( pucPayloadBuffer,
                                                         democonfigNETWORK_BUFFER_SIZE,
                                                         pcOwnershipToken,
                                                         xOwnershipTokenLength,
                                                         pcDemoID,
                                                         strlen ( pcDemoID ),
                                                         &xPayloadLength );
            }

            if( xStatus == true )
            {
                /* Subscribe to the RegisterThing response topics. */
                xStatus = prvSubscribeToRegisterThingResponseTopics();
            }

            if( xStatus == true )
            {
                /* Publish the RegisterThing request. */
                xStatus = xPublishToTopic( &xMqttContext,
                                           pcPublishTopic,
                                           xPublishTopicLength,
                                           ( char * ) pucPayloadBuffer,
                                           xPayloadLength );

                if( xStatus == false )
                {
                    LogError( ( "Failed to publish to fleet provisioning topic: %.*s.",
                    			xPublishTopicLength,
								pcPublishTopic ) );
                }
            }

            if( xStatus == true )
            {
                /* Extract the Thing name from the response. */
                xThingNameLength = fpdemoMAX_THING_NAME_LENGTH;
                xStatus = xParseRegisterThingResponse( pucPayloadBuffer,
                                                       xPayloadLength,
                                                       pcThingName,
                                                       &xThingNameLength );

                if( xStatus == true )
                {
                    LogInfo( ( "Received AWS IoT Thing name: %.*s", ( int ) xThingNameLength, pcThingName ) );
                    xprvWriteCacheEntry (strlen("thingname"), "thingname", xThingNameLength, pcThingName);
                    BaseType_t xSuccess = xprvWriteValueToImpl (KVS_CORE_THING_NAME, pcThingName, xThingNameLength);
                    if (xSuccess == pdTRUE)
                    {
                        LogInfo( ( "AWS IoT Thing name is saved to Data Flash") );
                    }
                }
            }

            if( xStatus == true )
            {
                /* Unsubscribe from the RegisterThing topics. */
                prvUnsubscribeFromRegisterThingResponseTopics();
            }

            /**** Disconnect from AWS IoT Core ************************************/

            /* As we have completed the provisioning workflow, we disconnect from
             * the connection using the provisioning claim credentials. We will
             * establish a new MQTT connection with the newly provisioned
             * credentials. */
            if( xConnectionEstablished == true )
            {
                xDisconnectMqttSession( &xMqttContext, &xNetworkContext );
                xConnectionEstablished = false;
            }
        }

        /**** Connect to AWS IoT Core with provisioned certificate ************/

        if( xStatus == true )
        {
            LogInfo( ( "Establishing MQTT session with provisioned certificate..." ) );
            xStatus = xEstablishMqttSession( &xMqttContext,
                                             &xNetworkContext,
                                             &xBuffer,
                                             prvProvisioningPublishCallback,
                                             pkcs11configLABEL_DEVICE_CERTIFICATE_FOR_TLS,
                                             pkcs11configLABEL_DEVICE_PRIVATE_KEY_FOR_TLS,
                                             pcThingName );

            if( xStatus != true )
            {
                LogError( ( "Failed to establish MQTT session with provisioned "
                            "credentials. Verify on your AWS account that the "
                            "new certificate is active and has an attached IoT "
                            "Policy that allows the \"iot:Connect\" action." ) );
            }
            else
            {
                LogInfo( ( "Sucessfully established connection with provisioned credentials." ) );
                xConnectionEstablished = true;
            }
        }

        /**** Finish **********************************************************/

        if( xConnectionEstablished == true )
        {
            /* Close the connection. */
            xDisconnectMqttSession( &xMqttContext, &xNetworkContext );
            xConnectionEstablished = false;
        }

        /**** Retry in case of failure ****************************************/

        xPkcs11CloseSession( xP11Session );

        /* Increment the demo run count. */
        ulDemoRunCount++;

        if( xStatus == true )
        {
            LogInfo( ( "Demo iteration %d is successful.", ulDemoRunCount ) );
        }
        /* Attempt to retry a failed iteration of demo for up to #fpdemoMAX_DEMO_LOOP_COUNT times. */
        else if( ulDemoRunCount < fpdemoMAX_DEMO_LOOP_COUNT )
        {
            LogWarn( ( "Demo iteration %d failed. Retrying...", ulDemoRunCount ) );
            vTaskDelay( fpdemoDELAY_BETWEEN_DEMO_RETRY_ITERATIONS_SECONDS );
        }
        /* Failed all #fpdemoMAX_DEMO_LOOP_COUNT demo iterations. */
        else
        {
            LogError( ( "All %d demo iterations failed.", fpdemoMAX_DEMO_LOOP_COUNT ) );
            break;
        }
    } while( xStatus != true );

    /* Log demo success. */
    if( xStatus == true )
    {
#if !defined(__TEST__)
        if (pcPublishTopic != NULL)
        {
            vPortFree (pcPublishTopic);
            pcPublishTopic = NULL;
        }

        if (pcAcceptTopic != NULL)
        {
            vPortFree (pcAcceptTopic);
            pcAcceptTopic = NULL;
        }
        if (pcRejectTopic != NULL)
        {
            vPortFree (pcRejectTopic);
            pcRejectTopic = NULL;
        }
#endif
        if (templateLength > 0)
        {
            vPortFree(template_buff);
        }

        if (thingnameLength > 0)
        {
            vPortFree(thingname_buff);
        }

        LogInfo( ( "Demo completed successfully." ) );
        LogInfo( ( "-------Fleet Provisioning Task Finished-------\r\n" ) );
    }

    xSetMQTTAgentState( MQTT_AGENT_STATE_INITIALIZED );

    /* Delete this task. */
    LogInfo( ( "Deleting Fleet Provisioning Demo task." ) );
    vTaskDelete( NULL );

    return ( xStatus == true ) ? EXIT_SUCCESS : EXIT_FAILURE;
}
/*-----------------------------------------------------------*/
