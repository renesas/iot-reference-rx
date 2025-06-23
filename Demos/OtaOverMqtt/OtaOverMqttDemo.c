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

/**
 * @file OtaOverMqttDemo.c
 * @brief Over The Air Update demo using coreMQTT Agent.
 *
 * The file demonstrates how to perform Over The Air update using OTA agent and coreMQTT Agent
 * library. It creates an OTA agent task which manages the OTA firmware update
 * for the device. The example also provides implementations to subscribe, publish,
 * and receive data from an MQTT broker. The implementation uses coreMQTT agent which manages
 * thread safety of the MQTT operations and allows OTA agent to share the same MQTT
 * broker connection with other tasks. OTA agent invokes the callback implementations to
 * publish job related control information, as well as receive chunks
 * of pre-signed firmware image from the MQTT broker.
 *
 * See https://freertos.org/mqtt/mqtt-agent-demo.html
 * See https://freertos.org/ota/ota-mqtt-agent-demo.html
 */

/* Standard includes. */
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#if defined(__CCRX__)
#include "strnlen.h"
#endif
#include "ota_os_freertos.h"

/* Kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

#include "demo_config.h"

/* MQTT library includes. */
#include "core_mqtt_agent.h"

/* MQTT streams Library include. */
#include "MQTTFileDownloader.h"
#include "MQTTFileDownloader_base64.h"

/* jobs Library include. */
#include "jobs.h"

/* OTA job parser include. */
#include "job_parser.h"
#include "ota_job_processor.h"

/* Include firmware version struct definition. */
/*#include "ota_appversion32.h" */

#include "ota_demo.h"
#include "mqtt_wrapper.h"

/* Include platform abstraction header. */
#include "ota_pal.h"

/* Key Value store for fetching configuration info. */
#include "store.h"

#include "mqtt_agent_task.h"

/* JSON header */
#include "core_json.h"

/*------------- Demo configurations -------------------------*/

/**
 * @brief Task priority of OTA agent.
 */
#define AGENT_TASK_PRIORITY            (tskIDLE_PRIORITY + 2)

/**
 * @brief Maximum stack size of OTA agent task.
 */
#define AGENT_TASK_STACK_SIZE          (4096 * 2)

#define NUM_OF_BLOCKS_REQUESTED                  mqttFileDownloader_MAX_NUM_BLOCKS_REQUEST
#define MAX_THING_NAME_SIZE                      (128U)
#define MAX_JOB_ID_LENGTH                        (64U)
#define JOB_MSG_LENGTH                           (128U)
#define MAX_NUM_OF_OTA_DATA_BUFFERS              (2)
#define MAX_RETRY_ERASE_AREA                     (3)

/* Max bytes supported for a file signature (3072 bit RSA is 384 bytes). */
#define OTA_MAX_SIGNATURE_SIZE                   (384U)

 static const char asciiDigits[] =
 {
      '0', '1', '2', '3',
      '4', '5', '6', '7',
      '8', '9', 'a', 'b',
      'c', 'd', 'e', 'f',
 };

static MqttFileDownloaderContext_t mqttFileDownloaderContext = { 0 };
static uint32_t numOfBlocksRemaining = 0;
static uint32_t currentBlockOffset = 0;
static uint8_t currentFileId = 0;
static uint32_t totalBytesReceived = 0;
char globalJobId[ MAX_JOB_ID_LENGTH ] = { 0 };

/* The topic buffer to wait for job event */
char   jobEventTopicBuffer[TOPIC_BUFFER_SIZE+1] = { 0 };
size_t jobEventTopicBufferLength                = 0U;

static OtaDataEvent_t dataBuffers[MAX_NUM_OF_OTA_DATA_BUFFERS] = { 0 };
static OtaJobEventData_t jobDocBuffer = { 0 };
static AfrOtaJobDocumentFields_t jobFields = { 0 };
static uint8_t OtaImageSingatureDecoded[OTA_MAX_SIGNATURE_SIZE] = { 0 };
static SemaphoreHandle_t bufferSemaphore;

static OtaState_t otaAgentState = OtaAgentStateInit;

/**
 * @brief Create the task that demonstrates the OTA demo.
 */
void vStartOtaDemo (void);

/**
 * @brief The function which runs the OTA agent task.
 *
 * The function runs the OTA Agent Event processing loop, which waits for
 * any events for OTA agent and process them. The loop never returns until the OTA agent
 * is shutdown. The tasks exits gracefully by freeing up all resources in the event of an
 *  OTA agent shutdown.
 *
 * @param[in] pvParam Any parameters to be passed to OTA agent task.
 */
static void prvOTAAgentTask (void * pvParam);


/**
 * @brief The function which runs the OTA update task.
 *
 * The demo task initializes the OTA agent an loops until OTA agent is shutdown.
 * It reports OTA update statistics (which includes number of blocks received, processed and dropped),
 * at regular intervals.
 *
 * @param[in] pvParam Any parameters to be passed to OTA demo task.
 */
static void vOTAUpdateTask (void * pvParam);

/**
 * @brief This is in essence the OTA agent implementation.
 */
static void processOTAEvents (void);

/**
 * @brief
 */
static bool imageActivationHandler (void);

/**
 * @brief
 */
static bool closeFileHandler (void);

/**
 * @brief
 */
static OtaPalJobDocProcessingResult_t receivedJobDocumentHandler (OtaJobEventData_t * jobDoc);

/**
 * @brief
 */
static uint16_t getFreeOTABuffers (void);

/**
 * @brief
 */
static void freeOtaDataEventBuffer (OtaDataEvent_t * const pxBuffer);

/**
 * @brief
 */
static void resetOtaBuffers (void);

/**
 * @brief
 */
static OtaDataEvent_t * getOtaDataEventBuffer (void);

/**
 * @brief
 */
static void requestDataBlock (void);

/**
 * @brief
 */
static int16_t handleMqttStreamsBlockArrived (uint8_t * data,
                                              size_t dataLength);

/**
 * @brief
 */
static bool convertSignatureToDER (AfrOtaJobDocumentFields_t * jobFields);

/**
 * @brief
 */
static void initMqttDownloader (AfrOtaJobDocumentFields_t * jobFields);

/**
 * @brief
 */
static void requestJobDocumentHandler (void);

/**
 * @brief
 */
static bool jobDocumentParser (char * message,
                               size_t messageLength,
                               AfrOtaJobDocumentFields_t * jobFields);


/**
 * @brief
 */
static bool sendSuccessMessage (void);
static bool sendFailedMessage (void);
static bool saveInitVersion (void);

static size_t stringBuilderUInt32Hex (char * pBuffer,
                                      size_t bufferSizeBytes,
                                      uint32_t value);

static OtaPalJobDocProcessingResult_t verifyVersion (char * updaterVersion);

static void deleteBufferArea (void);

static void rollBackOldFirmware (void);

/*
 * @brief Handle self-test for OTA image
 */
extern BaseType_t OtaSelfTest (void);

/**
 * @brief Structure used for encoding firmware version.
 */
const AppVersion32_t appFirmwareVersion =
{
    .u.x.major = APP_VERSION_MAJOR,
    .u.x.minor = APP_VERSION_MINOR,
    .u.x.build = APP_VERSION_BUILD,
};

/*
 * @brief The topic filter that AWS IoT Jobs service publishes to
 * when a job is canceled.
 */
const char * jobCancelTopic = "/cancellation_in_progress";

/*
 * @brief The thing name used for creating OTA job topics.
 * Thing name is retrieved from a key value store.
 */
static char * pcThingName      = NULL;
static size_t xThingNameLength = 0U;

/******************************************************************************
 * Function Name: vStartOtaDemo
 * Description  : Create the task that demonstrates the OTA demo.
 * Return Value : None
 *****************************************************************************/
void vStartOtaDemo(void)
{
    /*
     * vOtaDemoTask() connects to the MQTT broker, creates the
     * MQTT Agent task and calls the Ota demo loop prvRunOTADemo()
     * which creates the OTA Agent task.
     */

    xTaskCreate(vOTAUpdateTask,            /* Function that implements the task. */
                 "OTA Demo Task",          /* Text name for the task - only used for debugging. */
                 democonfigDEMO_STACKSIZE, /* Size of stack (in words, not bytes) to allocate for the task. */
                 NULL,                     /* Optional - task parameter - not used in this case. */
                 tskIDLE_PRIORITY + 1,     /* Task priority, must be between 0 and configMAX_PRIORITIES - 1. */
                 NULL);                    /* Optional - used to pass out a handle to the created task. */
}
/******************************************************************************
 End of function vStartOtaDemo
 *****************************************************************************/

/******************************************************************************
 * Function Name: prvOTAAgentTask
 * Description  : Waits for MQTT Agent to be connected and start the OTA demo
 * Argument     : pvParam
 * Return Value : None
 *****************************************************************************/
static void prvOTAAgentTask(void * pvParam)
{
    BaseType_t xResult;

    (void) pvParam;

    LogInfo(("Running OTA Agent task. Waiting..."));

    while (1)
    {
        xResult = xWaitForMQTTAgentState(MQTT_AGENT_STATE_CONNECTED,
                                          portMAX_DELAY);

        if (pdTRUE == xResult)
        {
            break;
        }
    }

    otaDemo_start();

    vTaskDelete(NULL);
}
/******************************************************************************
 End of function prvOTAAgentTask
 *****************************************************************************/

/******************************************************************************
 * Function Name: otaDemo_start
 * Description  : Starts the OTA demo
 * Return Value : None
 *****************************************************************************/
void otaDemo_start(void)
{
    OtaEventMsg_t initEvent = { 0 };

    if (!mqttWrapper_isConnected())
    {
        LogError(("MQTT not connected, exiting!"));
        return;
    }

    if (NULL != bufferSemaphore)
    {
        memset(dataBuffers, 0x00, sizeof(dataBuffers));
    }
    else
    {
        LogError(("Failed to create semaphore!\n"));
        return;
    }

    LogInfo(("Starting OTA thread."));

    OtaInitEvent_FreeRTOS();

    initEvent.eventId = OtaAgentEventRequestJobDocument;
    OtaSendEvent_FreeRTOS(&initEvent);

    while (OtaAgentStateStopped != otaAgentState)
    {
        processOTAEvents();
    }
}
/******************************************************************************
 End of function otaDemo_start
 *****************************************************************************/

/******************************************************************************
 * Function Name: requestJobDocumentHandler
 * Description  : Requests the OTA Jod document
 * Return Value : None
 *****************************************************************************/
static void requestJobDocumentHandler(void)
{
    char   thingName[MAX_THING_NAME_SIZE+1]    = { 0 };
    size_t thingNameLength                     = 0U;
    char   topicBuffer[TOPIC_BUFFER_SIZE+1]    = { 0 };
    char   messageBuffer[START_JOB_MSG_LENGTH] = { 0 };
    size_t topicLength                         = 0U;

    mqttWrapper_getThingName(thingName, &thingNameLength);

    /*
     * AWS IoT Jobs library:
     * Creates the topic string for a StartNextPendingJobExecution request.
     * It used to check if any pending jobs are available.
     */
    Jobs_StartNext(topicBuffer,
                   TOPIC_BUFFER_SIZE,
                   thingName,
                   (uint16_t) thingNameLength,
                   &topicLength);

    /*
     * AWS IoT Jobs library:
     * Creates the message string for a StartNextPendingJobExecution request.
     * It will be sent on the topic created in the previous step.
     */
    size_t messageLength = Jobs_StartNextMsg("test",
                                             4U,
                                             messageBuffer,
                                             START_JOB_MSG_LENGTH);

    mqttWrapper_publish(topicBuffer,
                        topicLength,
                        /* Cast to type "uint8_t *" to be compatible with parameter type */
                        (uint8_t *) messageBuffer,
                        messageLength);
}
/******************************************************************************
 End of function requestJobDocumentHandler
 *****************************************************************************/

/******************************************************************************
 * Function Name: initMqttDownloader
 * Description  : Initializes the MQTT File downloader library
 * Argument     : jobFields
 * Return Value : None
 *****************************************************************************/
static void initMqttDownloader(AfrOtaJobDocumentFields_t * jobFields)
{
    char   thingName[MAX_THING_NAME_SIZE+1] = { 0 };
    size_t thingNameLength                  = 0U;

    numOfBlocksRemaining = jobFields->fileSize /
                           mqttFileDownloader_CONFIG_BLOCK_SIZE;
    numOfBlocksRemaining += (((jobFields->fileSize %
                              mqttFileDownloader_CONFIG_BLOCK_SIZE) > 0) ? 1 : 0);

    currentFileId      = (uint8_t) jobFields->fileId; // cast the file ID
    currentBlockOffset = 0;
    totalBytesReceived = 0;

    mqttWrapper_getThingName(thingName, &thingNameLength);

    /*
     * MQTT streams Library:
     * Initializing the MQTT streams downloader. Passing the
     * parameters extracted from the AWS IoT OTA jobs document
     * using OTA jobs parser.
     */
    mqttDownloader_init(&mqttFileDownloaderContext,
                         jobFields->imageRef,
                         jobFields->imageRefLen,
                         thingName,
                         thingNameLength,
                         DATA_TYPE_JSON);

    /* SUBSCRIBE to MQTT Stream topic */
    mqttWrapper_subscribe(mqttFileDownloaderContext.topicStreamData,
                       mqttFileDownloaderContext.topicStreamDataLength);
    LogInfo(("SUBSCRIBE to topic %s\n", mqttFileDownloaderContext.topicStreamData));
}
/******************************************************************************
 End of function initMqttDownloader
 *****************************************************************************/

/******************************************************************************
 * Function Name: convertSignatureToDER
 * Description  : Converts the signature from PEM format to DER (binary)
 * Argument     : jobFields
 * Return Value : true   Conversion is successful
 *              : false  Conversion is failed
 *****************************************************************************/
static bool convertSignatureToDER(AfrOtaJobDocumentFields_t * jobFields)
{
    bool   returnVal              = true;
    size_t decodedSignatureLength = 0;


    Base64Status_t xResult = base64_Decode(OtaImageSingatureDecoded,
                                           sizeof(OtaImageSingatureDecoded),
                                           &decodedSignatureLength,
                                           (const uint8_t*)jobFields->signature, // cast the signature
                                           jobFields->signatureLen);

    if (Base64Success == xResult)
    {
        jobFields->signature    = (const char *)OtaImageSingatureDecoded; // cast the signature
        jobFields->signatureLen = decodedSignatureLength;
    }
    else
    {
        returnVal = false;
    }

    return returnVal;
}
/******************************************************************************
 End of function convertSignatureToDER
 *****************************************************************************/

/******************************************************************************
 * Function Name: handleMqttStreamsBlockArrived
 * Description  : Proccesses the received data block from MQTT File downloader
 * Arguments    : data
 *              : dataLength
 * Return Value : Size of received data block in bytes
 *****************************************************************************/
static int16_t handleMqttStreamsBlockArrived(uint8_t * data,
                                              size_t dataLength)
{
    int16_t writeblockRes = -1;

    LogInfo(("Downloaded block %u of %u. \n", currentBlockOffset, (currentBlockOffset + numOfBlocksRemaining)));

    writeblockRes = otaPal_WriteBlock(&jobFields,
                                       totalBytesReceived,
                                       data,
                                       dataLength);

    if (writeblockRes > 0)
    {
        totalBytesReceived += writeblockRes;
    }

    return writeblockRes;
}
/******************************************************************************
 End of function handleMqttStreamsBlockArrived
 *****************************************************************************/

/******************************************************************************
 * Function Name: requestDataBlock
 * Description  : Publishes the request for data block
 * Return Value : None
 *****************************************************************************/
static void requestDataBlock(void)
{
    char   getStreamRequest[GET_STREAM_REQUEST_BUFFER_SIZE];
    size_t getStreamRequestLength = 0U;

    /*
     * MQTT streams Library:
     * Creating the Get data block request. MQTT streams library only
     * creates the get block request. To publish the request, MQTT libraries
     * like coreMQTT are required.
     */
    getStreamRequestLength = mqttDownloader_createGetDataBlockRequest((DataType_t)mqttFileDownloaderContext.dataType,
                                                                       currentFileId,
                                                                       mqttFileDownloader_CONFIG_BLOCK_SIZE,
                                                                       (uint16_t) currentBlockOffset,
                                                                       NUM_OF_BLOCKS_REQUESTED,
                                                                       getStreamRequest,
                                                                       GET_STREAM_REQUEST_BUFFER_SIZE);

    mqttWrapper_publish(mqttFileDownloaderContext.topicGetStream,
                         mqttFileDownloaderContext.topicGetStreamLength,
                         (uint8_t *) getStreamRequest, // cast the parameter
                         getStreamRequestLength);
}
/******************************************************************************
 End of function requestDataBlock
 *****************************************************************************/

/******************************************************************************
 * Function Name: closeFileHandler
 * Description  : Closes the OTA file
 * Return Value : true   Successful to close file
 *              : false  Failed to close file
 *****************************************************************************/
static bool closeFileHandler(void)
{
    return (OtaPalSuccess == otaPal_CloseFile(&jobFields));
}
/******************************************************************************
 End of function closeFileHandler
 *****************************************************************************/

/******************************************************************************
 * Function Name: imageActivationHandler
 * Description  : Activates the new image
 * Return Value : true    Activation is successful
 *              : false   Activation is failed
 *****************************************************************************/
static bool imageActivationHandler(void)
{
    return (OtaPalSuccess == otaPal_ActivateNewImage(&jobFields));
}
/******************************************************************************
 End of function imageActivationHandler
 *****************************************************************************/

/******************************************************************************
 * Function Name: receivedJobDocumentHandler
 * Description  : Handles the received OTA Job document
 * Argument     : jobDoc
 * Return Value : OtaPalJobDocFileCreateFailed
 *              : OtaPalJobDocFileCreated
 *              : OtaPalJobDocProcessingStateInvalid
 *              : OtaPalJobDocParseFailed
 *              : OtaPalNewImageBootFailed
 *              : OtaPalNewImageBooted
 *              : OtaPalNewImageSameVersionSelfTestOK
 *              : OtaPalNewImageSameVersionSelfTestNG
 *****************************************************************************/
static OtaPalJobDocProcessingResult_t receivedJobDocumentHandler(OtaJobEventData_t * jobDoc)
{
    bool             parseJobDocument = false;
    bool             handled          = false;
    char *           jobId            = NULL;
    char **          jobIdptr         = &jobId;
    size_t           jobIdLength      = 0U;

    OtaPalJobDocProcessingResult_t xResult = OtaPalJobDocFileCreateFailed;

    OtaEventMsg_t nextEvent = { 0 };

    memset(&jobFields, 0, sizeof(jobFields));

    /*
     * AWS IoT Jobs library:
     * Extracting the job ID from the received OTA job document.
     */
    /* Cast to type "const char **" to be compatible with parameter type */
    jobIdLength = Jobs_GetJobId((char *)jobDoc->jobData, jobDoc->jobDataLength, (const char **) jobIdptr);

    if (jobIdLength)
    {
        if (strncmp(globalJobId, jobId, jobIdLength))
        {
            resetOtaBuffers();
            parseJobDocument = true;
            strncpy(globalJobId, jobId, jobIdLength);
            globalJobId[jobIdLength] = '\0';
        }
        else
        {
            xResult = OtaPalJobDocFileCreated;
        }
    }
    else /* the OTA job does not exist */
    {
        nextEvent.eventId = OtaAgentEventRequestJobDocument;
        OtaSendEvent_FreeRTOS(&nextEvent);
        xResult = OtaPalJobDocProcessingStateInvalid;
    }

    /* Verify the image version */
    char * updaterVersion = NULL;
    size_t versionLen;

    JSONStatus_t jsonResult = JSON_SearchConst((char *) jobDoc->jobData, // cast job data
                               jobDoc->jobDataLength,
                               "execution.statusDetails.updaterVersion",
                               strlen("execution.statusDetails.updaterVersion"),
							   /* Cast to type "const char **" to be compatible with parameter type */
                               (const char **)&updaterVersion,
                               &versionLen,
                               NULL);
    if ((JSONSuccess == jsonResult) && (NULL != updaterVersion))
        {
        /* Start version checking */
        return verifyVersion(updaterVersion);
    }

    if (parseJobDocument)
    {
        handled = jobDocumentParser((char *)jobDoc->jobData, jobDoc->jobDataLength, &jobFields); // cast the job data

        if (handled)
        {
            /* Populate the Jobs CANCELED event reserved topic ($aws/events/job/<jobId>/cancellation_in_progress) */
            JobsStatus_t ret = Jobs_Events(jobEventTopicBuffer,
                                            TOPIC_BUFFER_SIZE,
                                            jobId,
                                            (uint16_t) jobIdLength,
                                            &jobEventTopicBufferLength);
            if (JobsSuccess == ret)
            {
                /* SUBSCRIBE to the Jobs CANCELED event reserved topic ($aws/events/job/<jobId>/cancellation_in_progress) */
                /* Make sure to subscribe to Events topic before mqtt-file-stream as a precaution against Job cancellation. */
                mqttWrapper_subscribe(jobEventTopicBuffer, jobEventTopicBufferLength);
            }
            LogInfo(("SUBSCRIBE to topic %s\n", jobEventTopicBuffer));

            /* Initialize MQTT File Stream */
            initMqttDownloader(&jobFields);

            /* AWS IoT core returns the signature in a PEM format. We need to
             * convert it to DER format for image signature verification. */
            handled = convertSignatureToDER(&jobFields);

            if (handled)
            {
                xResult = otaPal_CreateFileForRx(&jobFields);
            }
            else
            {
                LogError(("Failed to decode the image signature to DER format."));
            }
        }
        else
        {
            xResult = OtaPalJobDocParseFailed;
            LogError(("Failed to parse the job document!"));
        }
    }

    return xResult;
}
/******************************************************************************
 End of function receivedJobDocumentHandler
 *****************************************************************************/

/******************************************************************************
 * Function Name: getFreeOTABuffers
 * Description  : Gets the number of OTA free buffers
 * Return Value : Number of OTA free buffers
 *****************************************************************************/
static uint16_t getFreeOTABuffers(void)
{
    uint32_t ulIndex     = 0;
    uint16_t freeBuffers = 0;

    if (xSemaphoreTake(bufferSemaphore, portMAX_DELAY) == pdTRUE)
    {
        for (ulIndex = 0; ulIndex < MAX_NUM_OF_OTA_DATA_BUFFERS; ulIndex++)
        {
            if (false == dataBuffers[ulIndex].bufferUsed)
            {
                freeBuffers++;
            }
        }

        (void)xSemaphoreGive(bufferSemaphore);
    }
    else
    {
        LogInfo(("Failed to get buffer semaphore. \n"));
    }

    return freeBuffers;
}
/******************************************************************************
 End of function getFreeOTABuffers
 *****************************************************************************/

/******************************************************************************
 * Function Name: freeOtaDataEventBuffer
 * Description  : Release the OTA data event buffer
 * Argument     : pxBuffer
 * Return Value : None
 *****************************************************************************/
static void freeOtaDataEventBuffer(OtaDataEvent_t * const pxBuffer)
{
    if (xSemaphoreTake(bufferSemaphore, portMAX_DELAY) == pdTRUE)
    {
        pxBuffer->bufferUsed = false;
        (void)xSemaphoreGive(bufferSemaphore);
    }
    else
    {
        LogInfo(("Failed to get buffer semaphore.\n"));
    }
}
/******************************************************************************
 End of function freeOtaDataEventBuffer
 *****************************************************************************/

/******************************************************************************
 * Function Name: resetOtaBuffers
 * Description  : Release all the OTA data buffers
 * Argument     : None
 * Return Value : None
 *****************************************************************************/
static void resetOtaBuffers(void)
{
    if (xSemaphoreTake(bufferSemaphore, portMAX_DELAY) == pdTRUE)
    {
        for (uint8_t i = 0; i < MAX_NUM_OF_OTA_DATA_BUFFERS; i++)
        {
            dataBuffers[i].bufferUsed = false;
        }
        (void)xSemaphoreGive(bufferSemaphore);
    }
    else
    {
        LogInfo(("Failed to get buffer semaphore.\n"));
    }
}
/******************************************************************************
 End of function resetOtaBuffers
 *****************************************************************************/

/* Implemented for use by the MQTT library */
/******************************************************************************
 * Function Name: otaDemo_handleIncomingMQTTMessage
 * Description  : Handles the received MQTT message
 * Arguments    : topic
 *              : topicLength
 *              : message
 *              : messageLength
 * Return Value : true    Successful processing
 *              : false   Failure
 *****************************************************************************/
bool otaDemo_handleIncomingMQTTMessage(char * topic,
                                       size_t topicLength,
                                       uint8_t * message,
                                       size_t messageLength)
{
    OtaEventMsg_t nextEvent                        = { 0 };
    char          thingName[MAX_THING_NAME_SIZE+1] = { 0 };
    size_t        thingNameLength                  = 0U;

    /* Checks if a message comes from a topic with /rejected in the name. */
    if (NULL != strstr(topic, JOBS_API_FAILURE))
    {
        LogWarn(("The request is rejected!"));
    }

    /*
     * MQTT streams Library:
     * Checks if the incoming message contains the requested data block. It is performed by
     * comparing the incoming MQTT message topic with MQTT streams topics.
     */
    MQTTFileDownloaderStatus_t ret = mqttDownloader_isDataBlockReceived(&mqttFileDownloaderContext, topic, topicLength);

    if (MQTTFileDownloaderSuccess == ret)
    {
        LogInfo(("Data block is receiving from topic: %.*s\n", topicLength, topic));
        OtaDataEvent_t * dataBuf = getOtaDataEventBuffer();

        if (NULL != dataBuf)
        {
            memcpy(dataBuf->data, message, messageLength);
        }
        else
        {
            LogError(("Failed to get data event buffer!\n"));
            return false;
        }

        nextEvent.dataEvent = dataBuf;
        dataBuf->dataLength = messageLength;
        nextEvent.eventId   = OtaAgentEventReceivedFileBlock;
        OtaSendEvent_FreeRTOS(&nextEvent);
    }
    else /* not the callback from MQTT File Stream */
    {
        mqttWrapper_getThingName(thingName, &thingNameLength);
        if (NULL == thingName)
        {
            LogError(("Failed to get thing name!\n"));
            return false;
        }

        /*
         * AWS IoT Jobs library:
         * Checks if a message comes from the start-next/accepted reserved topic.
         */
        bool handled = Jobs_IsStartNextAccepted(topic,
                                            topicLength,
                                            thingName,
                                            thingNameLength);

        if (handled)
        {
            /* Copy the job document to process in OtaAgentEventReceivedJobDocument event handler */
            memcpy(jobDocBuffer.jobData, message, messageLength);
            nextEvent.jobEvent         = &jobDocBuffer;
            jobDocBuffer.jobDataLength = messageLength;
            nextEvent.eventId          = OtaAgentEventReceivedJobDocument;
            OtaSendEvent_FreeRTOS(&nextEvent);
        }
        else
        {
            /* The MQTT-agent gets an update in $aws/events topic. */
            /* Checks if a message comes from the $aws/events/job/<jobId>/cancellation_in_progress reserved topic. */
        	if ((NULL != strstr(topic, jobCancelTopic)) &&
        	    (NULL != strstr(topic, JOBS_EVENTS_PREFIX)) &&
        	    (NULL != strstr(topic, globalJobId)))
            {
        	    LogInfo(("The job is canceled\n"));
                
                /* Send 'Canceled' event to OTA-agent. */
                nextEvent.eventId   = OtaAgentEventJobCanceled;
                OtaSendEvent_FreeRTOS(&nextEvent);
            }
        }
        return handled;
    }
    return ret;
}
/******************************************************************************
 End of function otaDemo_handleIncomingMQTTMessage
 *****************************************************************************/

/******************************************************************************
 * Function Name: sendSuccessMessage
 * Description  : Sends notification to OTA Job as SUCCESS
 * Return Value : true    Success
 *              : false   Failure
 *****************************************************************************/
static bool sendSuccessMessage(void)
{
    char   thingName[MAX_THING_NAME_SIZE+1] = { 0 };
    size_t thingNameLength                  = 0U;
    char   topicBuffer[TOPIC_BUFFER_SIZE+1] = { 0 };
    size_t topicBufferLength                = 0U;
    char   messageBuffer[JOB_MSG_LENGTH]    = { 0 };

    bool result = true;

    JobsUpdateRequest_t jobUpdateRequest;
    JobsStatus_t        jobStatusResult;

    mqttWrapper_getThingName(thingName, &thingNameLength);
    if (NULL == thingName)
    {
        LogError(("Failed to get thing name!\n"));
        return false;
    }

    /*
     * AWS IoT Jobs library:
     * Creating the MQTT topic to update the status of OTA job.
     */
    jobStatusResult = Jobs_Update(topicBuffer,
                                   TOPIC_BUFFER_SIZE,
                                   thingName,
                                   (uint16_t)thingNameLength,
                                   globalJobId,
                                   (uint16_t)strnlen(globalJobId, 1000U),
                                   &topicBufferLength);

    if (JobsSuccess == jobStatusResult)
    {
        /*
         * AWS IoT Jobs library:
         * Creating the message which contains the status of OTA job.
         * It will be published on the topic created in the previous step.
         */
        jobUpdateRequest = (JobsUpdateRequest_t)
        {
            .status = Succeeded
        };
        size_t messageBufferLength = Jobs_UpdateMsg(jobUpdateRequest,
                                                     messageBuffer,
                                                     JOB_MSG_LENGTH);

        result = mqttWrapper_publish(topicBuffer,
                                      topicBufferLength,
                                      /* Cast to type "uint8_t *" to be compatible with parameter type */
                                      (uint8_t *) messageBuffer,
                                      messageBufferLength);

        LogInfo(("---OTA Completed successfully!---\n"));
        globalJobId[0] = 0U; /* mark the old job to be unavailable */

        /* Clean up the job doc buffer so that it is ready for when we
         * receive new job doc. */
        memset(&jobDocBuffer, 0, sizeof(jobDocBuffer));
    }
    else
    {
        result = false;
    }

    return result;
}
/******************************************************************************
 End of function sendSuccessMessage
 *****************************************************************************/

/******************************************************************************
 * Function Name: jobDocumentParser
 * Description  : Parses the OTA Job document
 * Arguments    : message
 *              : messageLength
 *              : jobFields
 * Return Value : true   Success
 *              : false  Failure
 *****************************************************************************/
static bool jobDocumentParser(char * message,
                               size_t messageLength,
                               AfrOtaJobDocumentFields_t * jobFields)
{
    const char * jobDoc;
    size_t       jobDocLength = 0U;
    int8_t       fileIndex    = 0;

    /*
     * AWS IoT Jobs library:
     * Extracting the OTA job document from the jobs message recevied from AWS IoT core.
     */
    jobDocLength = Jobs_GetJobDocument(message, messageLength, &jobDoc);

    if (0U != jobDocLength)
    {
        do
        {
            /*
             * AWS IoT Jobs library:
             * Parsing the OTA job document to extract all of the parameters needed to download
             * the new firmware.
             */
            fileIndex = otaParser_parseJobDocFile(jobDoc,
                                                   jobDocLength,
                                                   fileIndex,
                                                   jobFields);
        } while (fileIndex > 0);
    }
    else
    {
        LogError(("Failed to get job document!"));
    }

    /* File index will be -1 if an error occured, and 0 if all files were
     * processed. */
    return (0 == fileIndex);
}
/******************************************************************************
 End of function jobDocumentParser
 *****************************************************************************/

/******************************************************************************
 * Function Name: getOtaDataEventBuffer
 * Description  : Retrieves the OTA data event buffer
 * Return Value : the OTA data event buffer
 *****************************************************************************/
static OtaDataEvent_t * getOtaDataEventBuffer(void)
{
    uint32_t         ulIndex    = 0;
    OtaDataEvent_t * freeBuffer = NULL;

    if (xSemaphoreTake(bufferSemaphore, portMAX_DELAY) == pdTRUE)
    {
        for (ulIndex = 0; ulIndex < MAX_NUM_OF_OTA_DATA_BUFFERS; ulIndex++)
        {
            if (false == dataBuffers[ulIndex].bufferUsed)
            {
                dataBuffers[ulIndex].bufferUsed = true;
                freeBuffer                      = &dataBuffers[ulIndex];
                break;
            }
        }

        (void)xSemaphoreGive(bufferSemaphore);
    }
    else
    {
        LogInfo(("Failed to get buffer semaphore. \n"));
    }

    return freeBuffer;
}
/******************************************************************************
 End of function getOtaDataEventBuffer
 *****************************************************************************/

/******************************************************************************
 * Function Name: processOTAEvents
 * Description  : Receives and processes the OTA events
 * Return Value : None
 *****************************************************************************/
static void processOTAEvents(void)
{
    OtaEventMsg_t     recvEvent       = { 0 };
    OtaEvent_t        recvEventId     = OtaAgentEventStart;
    static OtaEvent_t lastRecvEventId = OtaAgentEventStart;
    OtaEventMsg_t     nextEvent       = { 0 };

    OtaReceiveEvent_FreeRTOS(&recvEvent);
    recvEventId = recvEvent.eventId;

    if (OtaAgentEventStart != recvEventId)
    {
        lastRecvEventId = recvEventId;
    }
    else
    {
        if (OtaAgentEventRequestFileBlock == lastRecvEventId)
        {
            /* No current event and we have not received the new block
             * since last timeout, try sending the request for block again. */
            recvEventId = lastRecvEventId;

            /* It is likely that the network was disconnected and reconnected,
             * we should wait for the MQTT connection to go up. */
            while (!mqttWrapper_isConnected())
            {
                vTaskDelay(pdMS_TO_TICKS(100));
            }
        }
    }

    switch (recvEventId)
    {
        case OtaAgentEventRequestJobDocument:
            LogInfo(("Request Job Document event Received \n"));
            LogInfo(("-------------------------------------\n"));
            requestJobDocumentHandler();
            otaAgentState = OtaAgentStateRequestingJob;
            break;

        case OtaAgentEventReceivedJobDocument:
            LogInfo(("Received Job Document event Received \n"));
            LogInfo(("-------------------------------------\n"));

            if ((OtaAgentStateSuspended == otaAgentState) ||
                (OtaAgentStateJobCanceled == otaAgentState))
            {
                LogInfo(("OTA-Agent is in Suspend State or OTA job is canceled. Hence dropping Job Document.\n"));
                break;
            }

            OtaPalJobDocProcessingResult_t JobResult = receivedJobDocumentHandler(recvEvent.jobEvent);
            switch (JobResult)
            {
                case OtaPalJobDocFileCreated:
                    LogInfo(("Received OTA Job. \n"));
                    nextEvent.eventId = OtaAgentEventRequestFileBlock;
                    OtaSendEvent_FreeRTOS(&nextEvent);
                    otaAgentState = OtaAgentStateCreatingFile;
                    break;

                case OtaPalJobDocFileCreateFailed:
                    LogInfo(("Unable to create the OTA file\n"));
                    break;

                case OtaPalNewImageBootFailed:
                    LogError(("The new image cannot be booted, rollback to old image... \n"));
                    sendFailedMessage();
                    vTaskDelay(pdMS_TO_TICKS(5000));
                    rollBackOldFirmware();
                    break;

                case OtaPalNewImageSameVersionSelfTestOK:
                    nextEvent.eventId = OtaAgentEventRequestJobDocument;
                    OtaSendEvent_FreeRTOS(&nextEvent);

                    /*Set OTA Job status to FAILED */
                    sendFailedMessage();
                    vTaskDelay(pdMS_TO_TICKS(5000));

                    /* Delete buffer area */
                    deleteBufferArea();
                    break;

                case OtaPalNewImageSameVersionSelfTestNG:
                    LogInfo(("New image has same version, and self-test is failed. Resetting...\n"));
                    vTaskDelay(pdMS_TO_TICKS(500));
                    otaPal_ResetDevice(&jobFields);
                    break;

                case OtaPalJobDocParseFailed:

                    /* Change job status to FAILED */
                    sendFailedMessage();
                    vTaskDelay(pdMS_TO_TICKS(5000));
                    LogError(("The job document cannot be parsed or corrupted!\n"));
                    break;

                case OtaPalJobDocProcessingStateInvalid:
                    LogInfo(("Waiting for OTA job...\n"));
                    break;

                case OtaPalNewImageBooted:

                    /* Get ready for new OTA job. */
                    nextEvent.eventId = OtaAgentEventRequestJobDocument;
                    OtaSendEvent_FreeRTOS(&nextEvent);

                    /* Delete buffer area */
                    deleteBufferArea();
                    sendSuccessMessage();
                    vTaskDelay(pdMS_TO_TICKS(5000));
                    break;

                default:
                    break;
            }

            break;

        case OtaAgentEventRequestFileBlock:
            otaAgentState = OtaAgentStateRequestingFileBlock;
            LogInfo(("Request File Block event Received \n"));
            LogInfo(("-----------------------------------\n"));

            if (0 == currentBlockOffset)
            {
                LogInfo(("Starting The Download. \n"));
            }

            requestDataBlock();
            LogInfo(("ReqSent----------------------------\n"));
            break;

        case OtaAgentEventReceivedFileBlock:
            LogInfo(("Received File Block event Received \n"));
            LogInfo(("---------------------------------------\n"));

            /* Skip remaining PUBLISH to MQTT Stream callback */
            if ((OtaAgentStateSuspended == otaAgentState) ||
                (OtaAgentStateJobCanceled == otaAgentState))
            {
                LogInfo(("OTA-Agent is in Suspend State or OTA Job is canceled. Hence dropping File Block. \n"));
                freeOtaDataEventBuffer(recvEvent.dataEvent);
                break;
            }

            uint8_t decodedData[mqttFileDownloader_CONFIG_BLOCK_SIZE];
            size_t decodedDataLength = 0;
            MQTTFileDownloaderStatus_t xReturnStatus;
            int16_t result = -1;
            int32_t fileId;
            int32_t blockId;
            int32_t blockSize;
            static int32_t lastReceivedblockId = -1;

            /*
             * MQTT streams Library:
             * Extracting and decoding the received data block from the incoming MQTT message.
             */
            xReturnStatus = mqttDownloader_processReceivedDataBlock(
                &mqttFileDownloaderContext,
                recvEvent.dataEvent->data,
                recvEvent.dataEvent->dataLength,
                &fileId,
                &blockId,
                &blockSize,
                decodedData,
                &decodedDataLength);

            if (MQTTFileDownloaderSuccess != xReturnStatus)
            {
                /* There was some failure in trying to decode the block. */
                LogError(("File block decoding error\n"));
            }
            else if (fileId != jobFields.fileId)
            {
                /* Error - the file ID doesn't match with the one we received in the job document. */
                LogError(("File ID mismatched\n"));
            }
            else if (blockSize > mqttFileDownloader_CONFIG_BLOCK_SIZE)
            {
                /* Error - the block size doesn't match with what we requested. It can be smaller as
                 * the last block may or may not be of exact size. */
                LogError(("File block size mismatched\n"));
            }
            else if (blockId <= lastReceivedblockId)
            {
                /* Ignore this block. */
                LogInfo(("Received file block #%d that received before (%d). Ignored this block\n", blockId, lastReceivedblockId));
            }
            else
            {
                result = handleMqttStreamsBlockArrived(decodedData, decodedDataLength);
                lastReceivedblockId = blockId;
            }

            freeOtaDataEventBuffer(recvEvent.dataEvent);

            if (result > 0)
            {
                numOfBlocksRemaining--;
                currentBlockOffset++;
            }

            if ((numOfBlocksRemaining % 10) == 0)
            {
                LogInfo(("Free OTA buffers %u", getFreeOTABuffers()));
            }

            if (0 == numOfBlocksRemaining)
            {
                nextEvent.eventId = OtaAgentEventCloseFile;
                OtaSendEvent_FreeRTOS(&nextEvent);
            }
            else
            {
                if ((currentBlockOffset % NUM_OF_BLOCKS_REQUESTED) == 0)
                {
                    nextEvent.eventId = OtaAgentEventRequestFileBlock;
                    OtaSendEvent_FreeRTOS(&nextEvent);
                }
            }

            break;

        case OtaAgentEventCloseFile:
            LogInfo(("Close file event Received \n"));
            LogInfo(("-----------------------\n"));

            if (true == closeFileHandler())
            {
                nextEvent.eventId = OtaAgentEventActivateImage;
                OtaSendEvent_FreeRTOS(&nextEvent);
            }
            else
            {
                LogError(("OTA job failed at OtaAgentEventCloseFile.\n"));

                /* Change job status to FAILED */
                sendFailedMessage();
                vTaskDelay(pdMS_TO_TICKS(5000));
                otaPal_ResetDevice(&jobFields);
            }

            break;

        case OtaAgentEventActivateImage:
            LogInfo(("Activate Image event Received \n"));
            LogInfo(("-----------------------\n"));

            if (true == saveInitVersion())
            {
                vTaskDelay(pdMS_TO_TICKS(5000));
                nextEvent.eventId = OtaAgentEventVersionCheck;
                OtaSendEvent_FreeRTOS(&nextEvent);
            }

            /* Start the new image */
            imageActivationHandler();

            otaAgentState = OtaAgentStateStopped;
            break;

        case OtaAgentEventSuspend:
            LogInfo(("Suspend Event Received \n"));
            LogInfo(("-----------------------\n"));
            otaAgentState = OtaAgentStateSuspended;
            break;

        /* New enum to handle CANCELED event */
        case OtaAgentEventJobCanceled:
            LogInfo(("Job canceled Event Received \n"));
            LogInfo(("-----------------------\n"));
            otaAgentState = OtaAgentStateJobCanceled;

            /* Unsubscribe to the MQTT Stream topic of the canceled job */
            LogInfo(("UNSUBSCRIBE to topic %s\n", mqttFileDownloaderContext.topicStreamData));
            bool bResult = mqttWrapper_unsubscribe(mqttFileDownloaderContext.topicStreamData,
                    mqttFileDownloaderContext.topicStreamDataLength);

            if (false == bResult)
            {
                LogError(("Failed to UNSUBSCRIBE to data stream!\n"));
            }

            /*Unsubscribe from the Jobs event topic (use wildcard to unsubscribe from all Jobs Event topic) */
            bResult = mqttWrapper_unsubscribe(JOBS_EVENTS_PREFIX "/#", JOBS_EVENTS_PREFIX_LENGTH + 2);

            if (false == bResult)
            {
                LogError(("Failed to UNSUBSCRIBE to job event!\n"));
            }

            /* Reset the OTA Event queue */
            OtaDeinitEvent_FreeRTOS();
            OtaInitEvent_FreeRTOS();

            LogInfo(("Waiting for job cancellation to finalize in Jobs server... \n"));

            vTaskDelay(pdMS_TO_TICKS(5000));

            /* Reset Job document buffer, erase buffer flash bank, reset OtaFileContext */
            memset(&jobDocBuffer, 0, sizeof(jobDocBuffer));
            otaPal_EraseArea(1);
            otaPal_Abort(&jobFields);
            globalJobId[0] = 0U;
            resetOtaBuffers();

            /* Reset the block count */
            currentBlockOffset   = 0;
            totalBytesReceived   = 0;
            lastReceivedblockId  = -1;

            /* Start requesting for new job after suspended for some time (reset OTA-agent to initial state) */
            nextEvent.eventId = OtaAgentEventRequestJobDocument;
            OtaSendEvent_FreeRTOS(&nextEvent);

            break;

        case OtaAgentEventResume:
            LogInfo(("Resume Event Received \n"));
            LogInfo(("---------------------\n"));
            otaAgentState     = OtaAgentStateRequestingJob;
            nextEvent.eventId = OtaAgentEventRequestJobDocument;
            OtaSendEvent_FreeRTOS(&nextEvent);
            break;

        default:
            LogInfo(("Invalid event!\n"));
            break;
    }
}
/******************************************************************************
 End of function processOTAEvents
 *****************************************************************************/

/******************************************************************************
 * Function Name: vOTAUpdateTask
 * Description  : Creates the OTA Agent task
 * Argument     : pvParam
 * Return Value : None
 *****************************************************************************/
static void vOTAUpdateTask(void * pvParam)
{
    /* FreeRTOS APIs return status. */
    BaseType_t xResult          = pdPASS;

    (void) pvParam;

    LogInfo(("---------Start OTA Update Task---------\r\n"));

    LogInfo(("OTA over MQTT demo, Application version %u.%u.%u",
               appFirmwareVersion.u.x.major,
               appFirmwareVersion.u.x.minor,
               appFirmwareVersion.u.x.build));

    /****************************** Init OTA Library. ******************************/

    bufferSemaphore = xSemaphoreCreateMutex();
    if (NULL == bufferSemaphore)
    {
        xResult = pdFAIL;
    }

#if defined(__TEST__)
    pcThingName = clientcredentialIOT_THING_NAME;
    xThingNameLength = strlen(pcThingName);
#else
    xThingNameLength = prvGetCacheEntryLength(KVS_CORE_THING_NAME);

    if (xThingNameLength > 0)
    {
        pcThingName = GetStringValue(KVS_CORE_THING_NAME, xThingNameLength + 1);
    }
    else
    {
        LogError(("Thing name is not stored in Data Flash.\n"));
        xResult = pdFAIL;
    }

#endif
    if (NULL != pcThingName)
    {
        mqttWrapper_setThingName(pcThingName, xThingNameLength);
    }

    if (pdPASS == xResult)
    {
        if (pdPASS != (xResult = xTaskCreate(prvOTAAgentTask,
                                     "OTA_Agent",
                                     AGENT_TASK_STACK_SIZE,
                                     NULL,
                                     AGENT_TASK_PRIORITY,
                                     NULL)))
        {
            LogError(("Failed to start OTA Agent task: "
                        ", errno=%d",
                        xResult));
        }
    }

    vTaskDelay(portMAX_DELAY);

    LogInfo(("OTA agent task stopped. Exiting OTA demo."));

    /****************************** Cleanup ******************************/
    if (NULL != bufferSemaphore)
    {
        /* Cleanup semaphore created for buffer operations. */
        vSemaphoreDelete(bufferSemaphore);
    }

    if (NULL != pcThingName)
    {
#if !defined(__TEST__)
        vPortFree(pcThingName);
#endif
        pcThingName      = NULL;
        xThingNameLength = 0U;
    }

    vTaskDelete(NULL);
}
/******************************************************************************
 End of function vOTAUpdateTask
 *****************************************************************************/

/******************************************************************************
 * Function Name: saveInitVersion
 * Description  : Stores the current version of OTA image to OTA Job details
 * Return Value : true   Success
 *              : false  Failure
 *****************************************************************************/
static bool saveInitVersion(void)
{
    char   thingName[MAX_THING_NAME_SIZE+1]   = { 0 };
    size_t thingNameLength                    = 0U;
    char   topicBuffer[TOPIC_BUFFER_SIZE+1]   = { 0 };
    size_t topicBufferLength                  = 0U;
    char   messageBuffer[JOB_MSG_LENGTH]      = { 0 };
    bool   result                             = true;

    JobsUpdateRequest_t jobUpdateRequest;
    JobsStatus_t        jobStatusResult;

    char * UPDATER_VERISON_PREFIX = "{\"updaterVersion\":\"0x";

    mqttWrapper_getThingName(thingName, &thingNameLength);

    /*
     * AWS IoT Jobs library:
     * Creating the MQTT topic to update the status of OTA job.
     */
    jobStatusResult = Jobs_Update(topicBuffer,
                                   TOPIC_BUFFER_SIZE,
                                   thingName,
                                   (uint16_t) thingNameLength,
                                   globalJobId,
                                   (uint16_t) strnlen(globalJobId, 1000U),
                                   &topicBufferLength);

    if (JobsSuccess == jobStatusResult)
    {
        char updaterVersion[11] = { 0 }; // size of the version string with major+minor+build
        (void)stringBuilderUInt32Hex(updaterVersion, 11, appFirmwareVersion.u.unsignedVersion32);
        char statusDetails[40] = { 0 };
        strcat(statusDetails, UPDATER_VERISON_PREFIX);
        strcat(statusDetails, updaterVersion);
        strcat(statusDetails, "\"}");

        jobUpdateRequest = (JobsUpdateRequest_t)
        {
            .status = InProgress,
            .statusDetails = statusDetails,
            .statusDetailsLength = strlen(statusDetails)
        };

        /*
         * AWS IoT Jobs library:
         * Creating the message which contains the status of OTA job.
         * It will be published on the topic created in the previous step.
         */
        size_t messageBufferLength = Jobs_UpdateMsg(jobUpdateRequest,
                                                     messageBuffer,
                                                     JOB_MSG_LENGTH);

        result = mqttWrapper_publish(topicBuffer,
                                      topicBufferLength,
                                      (uint8_t *) messageBuffer, // cast the publish message
                                      messageBufferLength);

        LogInfo(("OTA image is in selfcheck mode.\n"));

        /* Clean up the job doc buffer so that it is ready for when we
         * receive new job doc. */
        memset(&jobDocBuffer, 0, sizeof(jobDocBuffer));

    }
    else
    {
        result = false;
    }

    return result;
}
/******************************************************************************
 End of function saveInitVersion
 *****************************************************************************/

/******************************************************************************
 * Function Name: stringBuilderUInt32Hex
 * Description  : Converts a hex value to a string of hex number
 * Arguments    : pBuffer
 *              : bufferSizeBytes
 *              : value
 * Return Value : Size of the converted string
 *****************************************************************************/
static size_t stringBuilderUInt32Hex(char * pBuffer,
                                      size_t bufferSizeBytes,
                                      uint32_t value)
{
    char workBuf[10];

    char  * pCur  = workBuf;
    char  * pDest = pBuffer;

    size_t i;
    size_t size = 0;

    uint32_t valueCopy = value;

    /* Assert if there is not enough buffer space. */

    assert(bufferSizeBytes >= 10);
    (void) bufferSizeBytes;

    /* Render all 8 digits, including leading zeros. */
    for (i = 0U; i < 8U; i++)
    {
        *pCur = asciiDigits[valueCopy & 15U]; /* 15U = 0xF*/
        pCur++;
        valueCopy >>= 4;
    }

    while (pCur > workBuf)
    {
        pCur--;
        pDest[size] = *pCur;
        size++;
    }

    pDest[size] = '\0';
    size++;
    return size;
}
/******************************************************************************
 End of function stringBuilderUInt32Hex
 *****************************************************************************/

/******************************************************************************
 * Function Name: sendFailedMessage
 * Description  : Sends notification to OTA Job as FAILED
 * Return Value : true   Success
 *              : false  Failure
 *****************************************************************************/
static bool sendFailedMessage(void)
{
    char   thingName[MAX_THING_NAME_SIZE+1] = { 0 };
    size_t thingNameLength                  = 0U;
    char   topicBuffer[TOPIC_BUFFER_SIZE+1] = { 0 };
    size_t topicBufferLength                = 0U;
    char   messageBuffer[JOB_MSG_LENGTH]    = { 0 };

    bool result = true;

    JobsUpdateRequest_t jobUpdateRequest;
    JobsStatus_t        jobStatusResult;

    mqttWrapper_getThingName(thingName, &thingNameLength);

    /*
     * AWS IoT Jobs library:
     * Creating the MQTT topic to update the status of OTA job.
     */
    jobStatusResult = Jobs_Update(topicBuffer,
                                   TOPIC_BUFFER_SIZE,
                                   thingName,
                                   (uint16_t) thingNameLength,
                                   globalJobId,
                                   (uint16_t) strnlen(globalJobId, 1000U),
                                   &topicBufferLength);

    if (JobsSuccess == jobStatusResult)
    {
        /*
         * AWS IoT Jobs library:
         * Creating the message which contains the status of OTA job.
         * It will be published on the topic created in the previous step.
         */
        jobUpdateRequest = (JobsUpdateRequest_t)
        {
            .status = Failed
        };
        size_t messageBufferLength = Jobs_UpdateMsg(jobUpdateRequest,
                                                     messageBuffer,
                                                     JOB_MSG_LENGTH);

        result = mqttWrapper_publish(topicBuffer,
                                      topicBufferLength,
                                      /* Cast to type "uint8_t *" to be compatible with parameter type */
                                      (uint8_t *) messageBuffer, // cast the publish message
                                      messageBufferLength);

        LogError(("OTA is failed!"));
        globalJobId[0] = 0U; /* mark the old job to be unavailable */

        /* Clean up the job doc buffer so that it is ready for when we
         * receive new job doc. */
        memset(&jobDocBuffer, 0, sizeof(jobDocBuffer));
    }
    else
    {
        result = false;
    }

    return result;
}
/******************************************************************************
 End of function sendFailedMessage
 *****************************************************************************/

/******************************************************************************
 * Function Name: verifyVersion
 * Description  : Retrieves the old image version from OTA Job details and
 *              : compares with current version
 * Argument     : updaterVersion
 * Return Value : OtaPalNewImageBooted
 *              : OtaPalNewImageBootFailed
 *              : OtaPalNewImageSameVersionSelfTestOK
 *              : OtaPalNewImageSameVersionSelfTestNG
 *****************************************************************************/
static OtaPalJobDocProcessingResult_t verifyVersion(char * updaterVersion)
{
    /* Converting the version hex text to value */
    char *   sEnd            = NULL;
    uint32_t unsignedVersion = 0;
    
    unsignedVersion = strtol(updaterVersion, &sEnd, 16);

    if (appFirmwareVersion.u.unsignedVersion32 > unsignedVersion)

    /* Current version is greater than retrieved version from OTA Job */
    {
        if (pdTRUE == OtaSelfTest())
        {
            LogInfo(("New image has higher version than current image, accepted!\n"));
            return OtaPalNewImageBooted;
        }
        else /* Self-test is failed */
        {
            LogInfo(("New image cannot pass self-test...\n"));
            return OtaPalNewImageBootFailed;
        }
    }

    else if (appFirmwareVersion.u.unsignedVersion32 == unsignedVersion)
    {
        LogInfo(("Version of new image is identical, or application has rolled-back to initial version.\n"));
        if (pdTRUE == OtaSelfTest())
        {
            LogInfo(("Image self-test passed!\n"));
            return OtaPalNewImageSameVersionSelfTestOK;
        }
        else /* Self-test is failed */
        {
            LogInfo(("New image cannot pass self-test...\n"));
            return OtaPalNewImageSameVersionSelfTestNG;
        }
    }

    else /* Current version is less than retrieved version from OTA Job */
    {
        LogError(("The image version is invalid\n"));
        return OtaPalNewImageBootFailed;
    }
}
/******************************************************************************
 End of function verifyVersion
 *****************************************************************************/

/******************************************************************************
 * Function Name: deleteBufferArea
 * Description  : Deletes the FWUP buffer area
 * Return Value : None
 *****************************************************************************/
static void deleteBufferArea(void) 
{
    OtaPalJobDocProcessingResult_t xResult;
    OtaPalStatus_t                 eRet;
    uint8_t                        retry_cnt;

    /* Call otaPal_CreateFileForRx() to open FWUP then open flash */
    xResult = otaPal_CreateFileForRx(&jobFields);

    if (OtaPalJobDocFileCreated == xResult)
    {
        for (retry_cnt = 0; retry_cnt < MAX_RETRY_ERASE_AREA; retry_cnt++)
        {
            eRet = otaPal_EraseArea(1);
            if (OtaPalSuccess == eRet) /* Erase buffer area successfully */
            {
                LogInfo(("Successful to erase the buffer area\n"));
                break;
            }
        }

        if (MAX_RETRY_ERASE_AREA <= retry_cnt)
        {
            LogError(("Failed to erase the buffer area\n"));
        }

    otaPal_CloseFileNoSignatureCheck(&jobFields);

    }
    
}
/******************************************************************************
 End of function deleteBufferArea
 *****************************************************************************/

/******************************************************************************
 * Function Name: rollBackOldFirmware
 * Description  : Reverts back to old firmware
 * Return Value : None
 *****************************************************************************/
static void rollBackOldFirmware(void)
{
    /* Call otaPal_CreateFileForRx() to open FWUP then open flash */
    if (OtaPalJobDocFileCreated == otaPal_CreateFileForRx(&jobFields))
    {
        otaPal_ActivateNewImage(&jobFields);
        otaPal_CloseFileNoSignatureCheck(&jobFields);
    }
}
/******************************************************************************
 End of function rollBackOldFirmware
 *****************************************************************************/
