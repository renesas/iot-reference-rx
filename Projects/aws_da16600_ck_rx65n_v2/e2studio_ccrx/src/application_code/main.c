/*
FreeRTOS
Copyright (C) 2020 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
Modifications Copyright (C) 2023-2025 Renesas Electronics Corporation or its affiliates.

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
the Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

 http://aws.amazon.com/freertos
 http://www.FreeRTOS.org
*/

/* FreeRTOS includes. */
#include "FreeRTOS.h"
#include "task.h"

/* Logging includes. */
#include "iot_logging_task.h"

/* Key provisioning includes. */
#include "aws_dev_mode_key_provisioning.h"

/* Demo includes */
#include "platform.h"
#include "aws_clientcredential.h"
#include "iot_wifi.h"
#include "demo_config.h"
#include "store.h"
#include "mqtt_agent_task.h"

#if (ENABLE_CREDENTIAL_BY_CLI == 0)
void vAssignCredentials(void);
extern int32_t xprvWriteCacheEntry(size_t KeyLength,
        char * Key,
        size_t ValueLength,
        char * pvNewValue );
extern BaseType_t KVStore_xCommitChanges( void );
#endif

#if (ENABLE_OTA_UPDATE_DEMO == 1)
    extern void vStartOtaDemo( void );
#endif

#if (ENABLE_FLEET_PROVISIONING_DEMO == 1)
    extern void vStartFleetProvisioningDemo(void);
#endif

/**
 * @brief Flag which enables OTA update task in background along with other demo tasks.
 * OTA update task polls regularly for firmware update jobs or acts on a new firmware update
 * available notification from OTA service.
 */
#define appmainINCLUDE_OTA_UPDATE_TASK            ( 1 )

/**
 * @brief Stack size and priority for OTA Update task.
 */
#define appmainMQTT_OTA_UPDATE_TASK_STACK_SIZE    ( 4096 )
#define appmainMQTT_OTA_UPDATE_TASK_PRIORITY      ( tskIDLE_PRIORITY )

/**
 * @brief Stack size and priority for MQTT agent task.
 * Stack size is capped to an adequate value based on requirements from MbedTLS stack
 * for establishing a TLS connection. Task priority of MQTT agent is set to a priority
 * higher than other MQTT application tasks, so that the agent can drain the queue
 * as work is being produced.
 */
#define appmainMQTT_AGENT_TASK_STACK_SIZE         ( 6144 )
#define appmainMQTT_AGENT_TASK_PRIORITY           ( tskIDLE_PRIORITY + 2 )

/**
 * @brief Stack size and priority for CLI task.
 */
#define appmainCLI_TASK_STACK_SIZE                ( 6144 )
#define appmainCLI_TASK_PRIORITY                  ( tskIDLE_PRIORITY + 1 )

#define mainLOGGING_TASK_STACK_SIZE               ( configMINIMAL_STACK_SIZE * 6 )
#define mainLOGGING_MESSAGE_QUEUE_LENGTH          ( 15 )
#define mainTEST_RUNNER_TASK_STACK_SIZE           ( configMINIMAL_STACK_SIZE * 8 )
#define UNSIGNED_SHORT_RANDOM_NUMBER_MASK         (0xFFFFUL)

#define mainUART_COMMAND_CONSOLE_STACK_SIZE ( configMINIMAL_STACK_SIZE * 6UL )
/* The priority used by the UART command console task. */
#define mainUART_COMMAND_CONSOLE_TASK_PRIORITY  ( 1 )

static bool _wifiEnable(void);
static bool _wifiConnectAccessPoint(void);

static bool ApplicationCounter (uint32_t xWaitTime);
signed char vISR_Routine (void);

/**
 * @brief Application task startup hook.
 */
void vApplicationDaemonTaskStartupHook(void);

/**
 * @brief Initializes the board.
 */
static void prvMiscInitialization(void);

extern void UserInitialization(void);
extern void CLI_Support_Settings(void);
extern void vUARTCommandConsoleStart(uint16_t usStackSize, UBaseType_t uxPriority);
extern void vRegisterSampleCLICommands(void);
extern void vStartSimplePubSubDemo (void);
BaseType_t OtaSelfTest(void);
extern int32_t littlFs_init (void);

/*-----------------------------------------------------------*/

/**
 * @brief The application entry point from a power on reset is PowerON_Reset_PC()
 * in resetprg.c.
 */
void main_task(void)
{
    int32_t xResults;
    int32_t Time2Wait = 10000;
    extern void vRegisterSampleCLICommands (void);
    extern void vUARTCommandConsoleStart (uint16_t usStackSize, UBaseType_t uxPriority);
    extern TaskHandle_t xCLIHandle;

    prvMiscInitialization();
    UserInitialization();

#if (ENABLE_CREDENTIAL_BY_CLI == 1)
    /* Register the standard CLI commands. */
    vRegisterSampleCLICommands();
    vUARTCommandConsoleStart(mainUART_COMMAND_CONSOLE_STACK_SIZE, mainUART_COMMAND_CONSOLE_TASK_PRIORITY);
#endif

    xResults = littlFs_init();

    xMQTTAgentInit();

    if (LFS_ERR_OK == xResults)
    {
        xResults = vprvCacheInit();
    }


#if (ENABLE_CREDENTIAL_BY_CLI == 0)
    vAssignCredentials();
#else

    if(ApplicationCounter(Time2Wait))
    {
        /* Remove CLI task before going to demo. */
        /* CLI and Log tasks use common resources but are not exclusively controlled. */
        /* For this reason, the CLI task must be deleted before executing the Demo. */
        vTaskDelete(xCLIHandle);
#endif

        if(!_wifiEnable())
        {
            configPRINTF(("Wi-Fi init failed"));
        }
        else
        {

            vTaskDelay(300);

            configPRINTF(("Initialise the RTOS's TCP/IP stack\n"));

            configPRINTF(("---------STARTING DEMO---------\r\n"));

        #if (ENABLE_FLEET_PROVISIONING_DEMO == 1)
            vStartFleetProvisioningDemo();
        #else
            xSetMQTTAgentState(MQTT_AGENT_STATE_INITIALIZED);
        #endif

        vStartMQTTAgent (appmainMQTT_AGENT_TASK_STACK_SIZE, appmainMQTT_AGENT_TASK_PRIORITY);

        vStartSimplePubSubDemo ();

        #if (ENABLE_OTA_UPDATE_DEMO == 1)
                    vStartOtaDemo();
        #endif
        }

#if (ENABLE_CREDENTIAL_BY_CLI == 1)
    }
#endif

    while(1)
    {
        vTaskSuspend(NULL);
    }
}

/*****************************************************************************************
 End of function main_task
 ****************************************************************************************/

/*-----------------------------------------------------------*/

static void prvMiscInitialization(void)
{
    /* Initialize UART for serial terminal. */
    CLI_Support_Settings();

    /* Start logging task. */
    xLoggingTaskInitialize( mainLOGGING_TASK_STACK_SIZE,
                            tskIDLE_PRIORITY + 2,
                            mainLOGGING_MESSAGE_QUEUE_LENGTH );

}

/*****************************************************************************************
 End of function prvMiscInitialization
 ****************************************************************************************/

/*-----------------------------------------------------------*/

void vApplicationDaemonTaskStartupHook(void)
{
    R_BSP_NOP();
}

/*****************************************************************************************
 End of function vApplicationDaemonTaskStartupHook
 ****************************************************************************************/

/*-----------------------------------------------------------*/

/* configUSE_STATIC_ALLOCATION is set to 1, so the application must provide an
 * implementation of vApplicationGetIdleTaskMemory() to provide the memory that is
 * used by the Idle task. */
void vApplicationGetIdleTaskMemory(StaticTask_t ** ppxIdleTaskTCBBuffer,
                                    StackType_t ** ppxIdleTaskStackBuffer,
                                    uint32_t * pulIdleTaskStackSize)
{
    /* If the buffers to be provided to the Idle task are declared inside this
     * function then they must be declared static - otherwise they will be allocated on
     * the stack and so not exists after this function exits. */
    static StaticTask_t xIdleTaskTCB;
    static StackType_t uxIdleTaskStack[ configMINIMAL_STACK_SIZE ];

    /* Pass out a pointer to the StaticTask_t structure in which the Idle
     * task's state will be stored. */
    *ppxIdleTaskTCBBuffer = &xIdleTaskTCB;

    /* Pass out the array that will be used as the Idle task's stack. */
    *ppxIdleTaskStackBuffer = uxIdleTaskStack;

    /* Pass out the size of the array pointed to by *ppxIdleTaskStackBuffer.
     * Note that, as the array is necessarily of type StackType_t,
     * configMINIMAL_STACK_SIZE is specified in words, not bytes. */
    *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
}

/*****************************************************************************************
 End of function vApplicationGetIdleTaskMemory
 ****************************************************************************************/

/*-----------------------------------------------------------*/

/**
 * @brief This is to provide the memory that is used by the RTOS daemon/time task.
 *
 * If configUSE_STATIC_ALLOCATION is set to 1, so the application must provide an
 * implementation of vApplicationGetTimerTaskMemory() to provide the memory that is
 * used by the RTOS daemon/time task.
 */
void vApplicationGetTimerTaskMemory(StaticTask_t ** ppxTimerTaskTCBBuffer,
                                    StackType_t ** ppxTimerTaskStackBuffer,
                                    uint32_t * pulTimerTaskStackSize)
{
    /* If the buffers to be provided to the Timer task are declared inside this
     * function then they must be declared static - otherwise they will be allocated on
     * the stack and so not exists after this function exits. */
    static StaticTask_t xTimerTaskTCB;
    static StackType_t uxTimerTaskStack[ configTIMER_TASK_STACK_DEPTH ];

    /* Pass out a pointer to the StaticTask_t structure in which the Idle
     * task's state will be stored. */
    *ppxTimerTaskTCBBuffer = &xTimerTaskTCB;

    /* Pass out the array that will be used as the Timer task's stack. */
    *ppxTimerTaskStackBuffer = uxTimerTaskStack;

    /* Pass out the size of the array pointed to by *ppxTimerTaskStackBuffer.
     * Note that, as the array is necessarily of type StackType_t,
     * configMINIMAL_STACK_SIZE is specified in words, not bytes. */
    *pulTimerTaskStackSize = configTIMER_TASK_STACK_DEPTH;
}

/*****************************************************************************************
 End of function vApplicationGetTimerTaskMemory
 ****************************************************************************************/

/*-----------------------------------------------------------*/

#ifndef iotconfigUSE_PORT_SPECIFIC_HOOKS

/**
 * @brief Warn user if pvPortMalloc fails.
 *
 * Called if a call to pvPortMalloc() fails because there is insufficient
 * free memory available in the FreeRTOS heap.  pvPortMalloc() is called
 * internally by FreeRTOS API functions that create tasks, queues, software
 * timers, and semaphores.  The size of the FreeRTOS heap is set by the
 * configTOTAL_HEAP_SIZE configuration constant in FreeRTOSConfig.h.
 *
 */
    void vApplicationMallocFailedHook()
    {
        configPRINT_STRING(("ERROR: Malloc failed to allocate memory\r\n"));
        taskDISABLE_INTERRUPTS();

        for(; ;)
        {
            /* Loop forever */
            R_BSP_NOP();
        }
    }

/*****************************************************************************************
End of function vApplicationMallocFailedHook
****************************************************************************************/

/*-----------------------------------------------------------*/

/**
 * @brief Loop forever if stack overflow is detected.
 *
 * If configCHECK_FOR_STACK_OVERFLOW is set to 1,
 * this hook provides a location for applications to
 * define a response to a stack overflow.
 *
 * Use this hook to help identify that a stack overflow
 * has occurred.
 *
 */
    void vApplicationStackOverflowHook(TaskHandle_t xTask,
                                        char * pcTaskName)
    {
        configPRINT_STRING(("ERROR: stack overflow\r\n"));
        portDISABLE_INTERRUPTS();

        /* Unused Parameters */
        (void) xTask;
        (void) pcTaskName;

        for(; ;)
        {
            /* Loop forever */
            R_BSP_NOP();
        }
    }

/*****************************************************************************************
End of function vApplicationStackOverflowHook
****************************************************************************************/
#endif /* iotconfigUSE_PORT_SPECIFIC_HOOKS */
/*-----------------------------------------------------------*/

#if ( ipconfigUSE_LLMNR != 0 ) || ( ipconfigUSE_NBNS != 0 ) || ( ipconfigDHCP_REGISTER_HOSTNAME == 1 )
    /* This function will be called during the DHCP: the machine will be registered
     * with an IP address plus this name. 
     * Note: Please make sure vprvCacheInit() is called before this function, because
     * it retrieves thingname value from KeyValue table. */
    const char * pcApplicationHostnameHook( void )
    {
#if defined(__TEST__)
        return clientcredentialIOT_THING_NAME;
#else
    {
        /* The string returned by this API is stipulated to be a maximum of 32 characters. */
        static char s_buff[32];
        memset ( s_buff, 0x00, sizeof(s_buff) );

        size_t valueLength = prvGetCacheEntryLength(KVS_CORE_THING_NAME);
        /* Process for thing name input by CLI. */
        if (valueLength > 0)
        {
            if ( valueLength > 31 )
            {
                configPRINT_STRING( ( "Warning: thing name with null-terminate string is longer than 32 characters.\r\n" ) );
                valueLength = 31;
            }
            size_t xLength = xReadEntry( KVS_CORE_THING_NAME, s_buff, valueLength );
            if ( 0 != xLength )
            {
                s_buff[valueLength] = '\0';
                return s_buff;
            }
            else
            {
                valueLength = strlen(clientcredentialIOT_THING_NAME);
                if ( valueLength > 31 )
                {
                    configPRINT_STRING( ( "Warning: thing name with null-terminate string is longer than 32 characters.\r\n" ) );
                    valueLength = 31;
                }
                strncpy(s_buff, clientcredentialIOT_THING_NAME, valueLength);
                s_buff[valueLength] = '\0';
                return s_buff;
            }
        }
        /* Process for thing name in aws_clientcredential.h. */
        else
        {
            valueLength = strlen(clientcredentialIOT_THING_NAME);
            if ( valueLength > 31 )
            {
                configPRINT_STRING( ( "Warning: thing name with null-terminate string is longer than 32 characters.\r\n" ) );
                valueLength = 31;
            }
            strncpy(s_buff, clientcredentialIOT_THING_NAME, valueLength);
            s_buff[valueLength] = '\0';
            return s_buff;
        }
    }
#endif
    }
#endif
/*****************************************************************************************
 End of function pcApplicationHostnameHook
 ****************************************************************************************/

static bool ApplicationCounter(uint32_t xWaitTime)
{
    TickType_t xCurrent;
    bool DEMO_TEST = pdTRUE;
    const TickType_t xPrintFrequency = pdMS_TO_TICKS(xWaitTime);
    xCurrent = xTaskGetTickCount();
    signed char cRxChar;
    while(xCurrent < xPrintFrequency)
    {
        vTaskDelay(1);
        xCurrent = xTaskGetTickCount();

        cRxChar = vISR_Routine();
        if ((cRxChar != 0))
        {

            DEMO_TEST = pdFALSE;
            break;
        }
    }
    return DEMO_TEST;
}

/**********************************************************************************************************************
 End of function ApplicationCounter
 *********************************************************************************************************************/

signed char vISR_Routine(void)
{
    BaseType_t xTaskWokenByReceive = pdFALSE;
    extern signed char cRxedChar;
    return cRxedChar;
}

/*****************************************************************************************
End of function vISR_Routine
****************************************************************************************/

static bool _wifiEnable(void)
{
    bool ret = true;

    if(WIFI_On() != eWiFiSuccess)
    {
        return false;
    }

    ret = _wifiConnectAccessPoint();

    return ret;
}

/**********************************************************************************************************************
 End of function _wifiEnable
 *********************************************************************************************************************/

static bool _wifiConnectAccessPoint(void)
{
    bool status = true;
    WIFINetworkParams_t xConnectParams;
    size_t xSSIDLength;
    size_t xPasswordLength;
    const char * pcSSID = clientcredentialWIFI_SSID;
    const char * pcPassword = clientcredentialWIFI_PASSWORD;
    WIFISecurity_t xSecurity = clientcredentialWIFI_SECURITY;


    xSSIDLength = strlen(pcSSID);

    if((0 < xSSIDLength) && (wificonfigMAX_SSID_LEN > xSSIDLength))
    {
        xConnectParams.ucSSIDLength = xSSIDLength;
        memcpy(xConnectParams.ucSSID, clientcredentialWIFI_SSID, xSSIDLength);
    }
    else
    {
        status = false;
    }

    xConnectParams.xSecurity = xSecurity;
    switch(xSecurity)
    {
        case eWiFiSecurityWPA:
        case eWiFiSecurityWPA2:
            xPasswordLength = strlen(clientcredentialWIFI_PASSWORD);

            if((0 < xPasswordLength) && (wificonfigMAX_PASSPHRASE_LEN > xPasswordLength))
            {
                xConnectParams.xPassword.xWPA.ucLength = xPasswordLength;
                memcpy(xConnectParams.xPassword.xWPA.cPassphrase, clientcredentialWIFI_PASSWORD, xPasswordLength);
            }
            else
            {
                status = false;
            }
            break;
        case eWiFiSecurityOpen:
            break; /* Nothing to do. */
        case eWiFiSecurityWPA3:
        case eWiFiSecurityWPA2_ent:
        case eWiFiSecurityWEP:
        default:
            configPRINT_STRING("The configured WiFi security option is not supported.");
            status = false;
            break;
    }

    if(status == true)
    {
        if(eWiFiSuccess != WIFI_ConnectAP(&(xConnectParams)))
        {
            status = false;
        }
    }

    return status;
}

/**********************************************************************************************************************
 End of function _wifiConnectAccessPoint
 *********************************************************************************************************************/

#if (ENABLE_CREDENTIAL_BY_CLI == 0)
void vAssignCredentials(void)
{

    /* Write thing name */
    char *pValue = democonfigCLIENT_IDENTIFIER;
    xprvWriteCacheEntry( strlen("thingname"), "thingname", strlen(pValue), pValue );

    /* Write endpoint */
    pValue = democonfigMQTT_BROKER_ENDPOINT;
    xprvWriteCacheEntry( strlen("endpoint"), "endpoint", strlen(pValue), pValue );

    /* Write certificate */
    pValue = keyCLIENT_CERTIFICATE_PEM;
    xprvWriteCacheEntry( strlen("cert"), "cert", strlen(pValue), pValue );

    /* Write private key */
    pValue = keyCLIENT_PRIVATE_KEY_PEM;
    xprvWriteCacheEntry( strlen("key"), "key", strlen(pValue), pValue );

    /* Write code signing certificate */
    pValue = otapalconfigCODE_SIGNING_CERTIFICATE;
    xprvWriteCacheEntry( strlen("codesigncert"), "codesigncert", strlen(pValue), pValue );

    /* Write root CA */
    pValue = democonfigROOT_CA_PEM;
    xprvWriteCacheEntry( strlen("rootca"), "rootca", strlen(pValue), pValue );

    /* Write cache to DF */
    KVStore_xCommitChanges();
}
#endif

/**********************************************************************************************************************
 * Function Name: OtaSelfTest
 * Description  : The test function executed during the self-check process during an OTA update
 * Return Value : pdTRUE (if the self-test was successful)
 *                pdFALSE (if the self-test was failed)
 *                Always return pdTRUE since the initial firmware was evaluated during the manufacturing process.
 *********************************************************************************************************************/
BaseType_t OtaSelfTest(void)
{
	return pdTRUE;
}
