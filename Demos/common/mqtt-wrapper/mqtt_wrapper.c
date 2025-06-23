/*
 * Copyright Amazon.com, Inc. and its affiliates. All Rights Reserved.
 * Modifications Copyright (C) 2025 Renesas Electronics Corporation or its affiliates.
 * SPDX-License-Identifier: MIT
 *
 * Licensed under the MIT License. See the LICENSE accompanying this file
 * for the specific language governing permissions and limitations under
 * the License.
 */

#include <string.h>

#include "FreeRTOS.h"
#include "mqtt_wrapper.h"
#include "ota_demo.h"

#define MQTT_AGENT_NOTIFY_IDX    (2)

static MQTTContext_t * globalCoreMqttContext = NULL;

/**
 * @brief Static handle used for MQTT agent context.
 */
extern MQTTAgentContext_t xGlobalMqttAgentContext;

#define MAX_THING_NAME_SIZE    128U
static char globalThingName[MAX_THING_NAME_SIZE + 1];
static size_t globalThingNameLength = 0U;

/**
 * @brief Defines the structure to use as the command callback context in this
 * demo.
 */
struct MQTTAgentCommandContext
{
    MQTTStatus_t xReturnStatus;
    TaskHandle_t xTaskToNotify;
    void *       pArgs;
};

void mqttWrapper_setCoreMqttContext(MQTTContext_t * mqttContext)
{
    globalCoreMqttContext = mqttContext;
}

void mqttWrapper_setThingName(char * thingName,
                              size_t thingNameLength)
{
    configASSERT(thingNameLength <= MAX_THING_NAME_SIZE );
    memcpy(globalThingName, thingName, MAX_THING_NAME_SIZE);
    globalThingName[thingNameLength] = '\0';
    globalThingNameLength = thingNameLength;
}

void mqttWrapper_getThingName(char *   thingNameBuffer,
                              size_t * thingNameLength)
{
    configASSERT(globalThingName[ 0 ] != 0);

    memcpy(thingNameBuffer, globalThingName, globalThingNameLength);
    thingNameBuffer[globalThingNameLength] = '\0';
    *thingNameLength                       = globalThingNameLength;
}

bool mqttWrapper_isConnected(void)
{
    bool isConnected = false;

    configASSERT(globalCoreMqttContext != NULL);
    isConnected = (MQTTConnected == globalCoreMqttContext->connectStatus);
    return isConnected;
}

static void prvPublishCommandCallback(MQTTAgentCommandContext_t * pCmdCallbackContext,
                                      MQTTAgentReturnInfo_t *     pReturnInfo)
{
    TaskHandle_t xTaskHandle = (struct tskTaskControlBlock *) pCmdCallbackContext->xTaskToNotify;


    if (NULL != xTaskHandle)
    {
        uint32_t ulNotifyValue = MQTTSuccess; /* ( pxReturnInfo->returnCode & 0xFFFFFF ); */
/* */
/*      if( pxReturnInfo->pSubackCodes ) */
/*      { */
/*          ulNotifyValue += ( pxReturnInfo->pSubackCodes[ 0 ] << 24 ); */
/*      } */

        (void) xTaskNotifyIndexed(xTaskHandle,
                                  MQTT_AGENT_NOTIFY_IDX,
                                  ulNotifyValue,
                                  eSetValueWithOverwrite);
    }
}

/******************************************************************************
 * Function Name: prvUnsubscribeCommandCallback
 * Description  : Callback of UNSUBSCRIBE command issued by prvUnsubscribeCommandCallback
 * Arguments    : pCmdCallbackContext : The command context
 *              : pReturnInfo         : The return information
 * Return Value : None
 *****************************************************************************/
static void prvUnsubscribeCommandCallback(MQTTAgentCommandContext_t * pCmdCallbackContext,
                                          MQTTAgentReturnInfo_t *     pReturnInfo)
{
    TaskHandle_t xTaskHandle                  = (struct tskTaskControlBlock *) pCmdCallbackContext->xTaskToNotify;
    MQTTAgentSubscribeArgs_t * pSubscribeArgs = (MQTTAgentSubscribeArgs_t *) pCmdCallbackContext->pArgs;


    if(NULL != xTaskHandle)
    {
        uint32_t ulNotifyValue = MQTTSuccess; /* ( pxReturnInfo->returnCode & 0xFFFFFF ); */

        (void) xTaskNotifyIndexed(xTaskHandle,
                                  MQTT_AGENT_NOTIFY_IDX,
                                  ulNotifyValue,
                                  eSetValueWithOverwrite);

        /* Remove the topic filter callback */
        vRemoveMQTTTopicFilterCallback(pSubscribeArgs->pSubscribeInfo->pTopicFilter,
                                       pSubscribeArgs->pSubscribeInfo->topicFilterLength);
    }
}
/******************************************************************************
 End of function prvUnsubscribeCommandCallback
 *****************************************************************************/

bool mqttWrapper_publish(char *    topic,
                         size_t    topicLength,
                         uint8_t * message,
                         size_t    messageLength)
{
    bool success = false;

    configASSERT(NULL != globalCoreMqttContext);
    configASSERT(topicLength > 0);

    success = mqttWrapper_isConnected();

    if (success)
    {
        MQTTStatus_t mqttStatus = MQTTSuccess;

        /* TODO: This should be static or should we wait? */
        static MQTTPublishInfo_t pubInfo      = { 0 };
        MQTTAgentContext_t *     xAgentHandle = &xGlobalMqttAgentContext;

        pubInfo.qos             = MQTTQoS0;
        pubInfo.retain          = false;
        pubInfo.dup             = false;
        pubInfo.pTopicName      = topic;
        pubInfo.topicNameLength = topicLength;
        pubInfo.pPayload        = message;
        pubInfo.payloadLength   = messageLength;

        MQTTAgentCommandContext_t xCommandContext =
        {
            .xTaskToNotify = xTaskGetCurrentTaskHandle(),
            .xReturnStatus = MQTTIllegalState,
        };

        MQTTAgentCommandInfo_t xCommandParams =
        {
            .blockTimeMs                 = 1000,
            .cmdCompleteCallback         = prvPublishCommandCallback,
            .pCmdCompleteCallbackContext = &xCommandContext,
        };

        (void) xTaskNotifyStateClearIndexed(NULL, MQTT_AGENT_NOTIFY_IDX);

        mqttStatus = MQTTAgent_Publish(xAgentHandle,
                                       &pubInfo,
                                       &xCommandParams);

        if (MQTTSuccess == mqttStatus)
        {
            uint32_t ulNotifyValue = 0;

            if (xTaskNotifyWaitIndexed(MQTT_AGENT_NOTIFY_IDX,
                                       0x0,
                                       0xFFFFFFFF,
                                       &ulNotifyValue,
                                       portMAX_DELAY))
            {
                mqttStatus = (MQTTStatus_t) (ulNotifyValue & 0x00FFFFFF);
            }
            else
            {
                mqttStatus = MQTTKeepAliveTimeout;
            }
        }

        success = (MQTTSuccess == mqttStatus);
    }

    return success;
}

void handleReceivedPublish(void *              pvIncomingPublishCallbackContext,
                           MQTTPublishInfo_t * pxPublishInfo)
{
    (void) pvIncomingPublishCallbackContext;
    char *    topic          = NULL;
    size_t    topicLength    = 0U;
    uint8_t * message        = NULL;
    size_t    messageLength  = 0U;
    bool      messageHandled = false;

    topic         = (char *) pxPublishInfo->pTopicName;
    topicLength   = pxPublishInfo->topicNameLength;
    message       = (uint8_t *) pxPublishInfo->pPayload;
    messageLength = pxPublishInfo->payloadLength;

    configPRINTF(("[INFO] Incoming PUBLISH received on topic %.*s\n",
                 (unsigned int) topicLength, topic));

    messageHandled = otaDemo_handleIncomingMQTTMessage(topic, topicLength, message, messageLength);

    if (!messageHandled)
    {
        configPRINTF(("[WARN] Unhandled incoming PUBLISH received on topic %.*s\n, "
                     "message: %.*s\n",
                     (unsigned int) topicLength,
                     topic,
                     (unsigned int) messageLength,
                     (char *) message));
        configPRINTF(("\n"));
    }
}

bool mqttWrapper_subscribe(char * topic,
                           size_t topicLength)
{
    bool success = false;

    configASSERT(NULL != globalCoreMqttContext);

    configASSERT(topic);
    configASSERT(topicLength > 0);

    success = mqttWrapper_isConnected();

    if (success)
    {
        MQTTStatus_t mqttStatus = MQTTSuccess;

        mqttStatus = MqttAgent_SubscribeSync(topic,
                                             topicLength,
                                             MQTTQoS0,
                                             handleReceivedPublish,
                                             NULL);

        configASSERT(MQTTSuccess == mqttStatus);

        success = (MQTTSuccess == mqttStatus);
    }

    return success;
}

/******************************************************************************
 * Function Name: mqttWrapper_unsubscribe
 * Description  : UNSUBSCRIBE from a topic filter
 * Arguments    : topic       : The topic filter to be unsubscribed
 *              : topicLength : Length of the topic filter
 * Return Value : bool        : true if successful, false otherwise.
 *****************************************************************************/
bool mqttWrapper_unsubscribe(char * topic,
                             size_t topicLength)
{

    configASSERT(topic);
    configASSERT(topicLength > 0);

    MQTTAgentSubscribeArgs_t  xSubscribeArgs  = {0};
    MQTTSubscribeInfo_t       xSubscribeInfo  = {0};
    MQTTAgentCommandContext_t xCommandContext = {0UL};
    MQTTAgentCommandInfo_t    xCommandParams  = {0UL};

    /* Complete the subscribe information.  The topic string must persist for
     * duration of subscription! */
    xSubscribeInfo.pTopicFilter      = topic;
    xSubscribeInfo.topicFilterLength = (uint16_t)topicLength;
    xSubscribeInfo.qos               = MQTTQoS0;

    xSubscribeArgs.pSubscribeInfo   = &xSubscribeInfo;
    xSubscribeArgs.numSubscriptions = 1;

    /* Complete an application defined context associated with this subscribe message.
     * This gets updated in the callback function so the variable must persist until
     * the callback executes. */
    xCommandContext.xTaskToNotify = xTaskGetCurrentTaskHandle();
    xCommandContext.xReturnStatus = MQTTIllegalState;
    xCommandContext.pArgs         = (void *)&xSubscribeArgs;

    xCommandParams = (MQTTAgentCommandInfo_t)
    {
        .blockTimeMs                 = 1000,
        .cmdCompleteCallback         = prvUnsubscribeCommandCallback,
        .pCmdCompleteCallbackContext = &xCommandContext,
    };

    MQTTStatus_t xCommandStatus = MQTTAgent_Unsubscribe(&xGlobalMqttAgentContext,
                                                        &xSubscribeArgs,
                                                        &xCommandParams);

    if (MQTTSuccess == xCommandStatus)
    {
        uint32_t ulNotifyValue = 0;

        if (xTaskNotifyWaitIndexed(MQTT_AGENT_NOTIFY_IDX,
                                   0x0,
                                   0xFFFFFFFF,
                                   &ulNotifyValue,
                                   portMAX_DELAY))
        {
            xCommandStatus = (MQTTStatus_t) (ulNotifyValue & 0x00FFFFFF);
        }
        else
        {
            xCommandStatus = MQTTKeepAliveTimeout;
        }
    }

    return (MQTTSuccess == xCommandStatus);
}
/******************************************************************************
 End of function mqttWrapper_unsubscribe
 *****************************************************************************/
