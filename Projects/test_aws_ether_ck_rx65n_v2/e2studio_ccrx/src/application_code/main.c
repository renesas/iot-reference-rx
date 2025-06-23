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
#include "dev_mode_key_provisioning.h"

/* FreeRTOS+TCP includes. */
#include "FreeRTOS_IP.h"

/* Demo includes */
#include "aws_clientcredential.h"
#include "mqtt_agent_task.h"
#include "test_execution_config.h"
#include "store.h"

bool ApplicationCounter (uint32_t xWaitTime);
signed char vISR_Routine (void);
BaseType_t OtaSelfTest(void);

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

#define appmainRUN_QUALIFICATION_TEST_SUITE       ( 1 )

#define appmainRUN_DEVICE_ADVISOR_TEST_SUITE      ( 0 )

#define appmainMQTT_OTA_UPDATE_TASK_STACK_SIZE    ( 4096 )

#define appmainTEST_TASK_STACK_SIZE               ( 4096 )
#define appmainTEST_TASK_PRIORITY                 ( tskIDLE_PRIORITY + 1 )

/**
 * @brief Stack size and priority for MQTT agent task.
 * Stack size is capped to an adequate value based on requirements from MbedTLS stack
 * for establishing a TLS connection. Task priority of MQTT agent is set to a priority
 * higher than other MQTT application tasks, so that the agent can drain the queue
 * as work is being produced.
 */
#define appmainMQTT_AGENT_TASK_STACK_SIZE         ( 6144 )
#define appmainMQTT_AGENT_TASK_PRIORITY           ( tskIDLE_PRIORITY + 2 )

#define mainLOGGING_TASK_STACK_SIZE               ( configMINIMAL_STACK_SIZE * 6 )
#define mainLOGGING_MESSAGE_QUEUE_LENGTH          ( 15 )
#define mainTEST_RUNNER_TASK_STACK_SIZE           ( configMINIMAL_STACK_SIZE * 8 )
#define UNSIGNED_SHORT_RANDOM_NUMBER_MASK         (0xFFFFUL)

#define mainUART_COMMAND_CONSOLE_STACK_SIZE ( configMINIMAL_STACK_SIZE * 6UL )
/* The priority used by the UART command console task. */
#define mainUART_COMMAND_CONSOLE_TASK_PRIORITY  ( 1 )

/* The MAC address array is not declared const as the MAC address will
normally be read from an EEPROM and not hard coded (in real deployed
applications).*/
static uint8_t ucMACAddress[6] =
{
    configMAC_ADDR0,
    configMAC_ADDR1,
    configMAC_ADDR2,
    configMAC_ADDR3,
    configMAC_ADDR4,
    configMAC_ADDR5
}; //XXX

/* Define the network addressing.  These parameters will be used if either
ipconfigUDE_DHCP is 0 or if ipconfigUSE_DHCP is 1 but DHCP auto configuration
failed. */
static const uint8_t ucIPAddress[4] =
{
    configIP_ADDR0,
    configIP_ADDR1,
    configIP_ADDR2,
    configIP_ADDR3
};
static const uint8_t ucNetMask[4] =
{
    configNET_MASK0,
    configNET_MASK1,
    configNET_MASK2,
    configNET_MASK3
};
static const uint8_t ucGatewayAddress[4] =
{
    configGATEWAY_ADDR0,
    configGATEWAY_ADDR1,
    configGATEWAY_ADDR2,
    configGATEWAY_ADDR3
};

/* The following is the address of an OpenDNS server. */
static const uint8_t ucDNSServerAddress[4] =
{
    configDNS_SERVER_ADDR0,
    configDNS_SERVER_ADDR1,
    configDNS_SERVER_ADDR2,
    configDNS_SERVER_ADDR3
};

extern int32_t littlFs_init (void);

/**
 * @brief Application task startup hook.
 */
void vApplicationDaemonTaskStartupHook (void);

/**
 * @brief Initializes the board.
 */
void prvMiscInitialization (void);

extern void prvQualificationTestTask (void * pvParameters);
extern void vSubscribePublishTestTask (void * pvParameters);
extern void prvQualificationTestTask (void * pvParameters);
extern void vSubscribePublishTestTask (void * pvParameters);
extern void vStartOtaDemo( void );

/*-----------------------------------------------------------*/

extern void vStartOtaDemo( void );

/**********************************************************************************************************************
 * Function Name: RunDeviceAdvisorDemo
 * Description  : .
 * Return Value : .
 *********************************************************************************************************************/
int RunDeviceAdvisorDemo(void)
{
    BaseType_t xResult = pdFAIL;

    xResult = xMQTTAgentInit();
    xSetMQTTAgentState(MQTT_AGENT_STATE_INITIALIZED);
    vStartMQTTAgent (appmainMQTT_AGENT_TASK_STACK_SIZE, appmainMQTT_AGENT_TASK_PRIORITY);

    if (pdPASS == xResult)
    {
        xResult = xTaskCreate(vSubscribePublishTestTask,
                                "TEST",
                                appmainTEST_TASK_STACK_SIZE,
                                NULL,
                                appmainTEST_TASK_PRIORITY,
                                NULL);

    }
    return (pdPASS == xResult) ? 0 : -1;
}
/*****************************************************************************************
End of function RunDeviceAdvisorDemo
****************************************************************************************/


int RunOtaE2eDemo( void )
{
    xMQTTAgentInit();
    xSetMQTTAgentState( MQTT_AGENT_STATE_INITIALIZED );
    vStartMQTTAgent (appmainMQTT_AGENT_TASK_STACK_SIZE, appmainMQTT_AGENT_TASK_PRIORITY);

    vStartOtaDemo();
    return 0;
}

/**
 * @brief The application entry point from a power on reset is PowerON_Reset_PC()
 * in resetprg.c.
 */

/**********************************************************************************************************************
 * Function Name: main_task
 * Description  : The application entry point from a power on reset is PowerON_Reset_PC()
 *                in resetprg.c.
 * Return Value : .
 *********************************************************************************************************************/
void main_task(void)
{
    int32_t xResults;
    int32_t Time2Wait = 10000;
    extern void vRegisterSampleCLICommands (void);
    extern void vUARTCommandConsoleStart (uint16_t usStackSize, UBaseType_t uxPriority);
    extern TaskHandle_t xCLIHandle;

    /* Initialize UART for serial terminal. */
    prvMiscInitialization();

    /* Register the standard CLI commands. */
    vRegisterSampleCLICommands();
    vUARTCommandConsoleStart(mainUART_COMMAND_CONSOLE_STACK_SIZE, mainUART_COMMAND_CONSOLE_TASK_PRIORITY);

    xResults = littlFs_init();

    if (LFS_ERR_OK == xResults)
    {
    xResults = vprvCacheInit();
    }


    if (ApplicationCounter(Time2Wait))
    {
        /* Remove CLI task before going to demo. */
        /* CLI and Log tasks use common resources but are not exclusively controlled. */
        /* For this reason, the CLI task must be deleted before executing the Demo. */
        vTaskDelete(xCLIHandle);

        /* Initialise the RTOS's TCP/IP stack.  The tasks that use the network
        are created in the vApplicationIPNetworkEventHook() hook function
        below.  The hook function is called when the network connects. */

        FreeRTOS_IPInit(ucIPAddress,
                        ucNetMask,
                        ucGatewayAddress,
                        ucDNSServerAddress,
                        ucMACAddress);

        /* We should wait for the network to be up before we run any demos. */
        while (FreeRTOS_IsNetworkUp() == pdFALSE)
        {
            vTaskDelay(300);
        }

        FreeRTOS_printf(("Initialise the RTOS's TCP/IP stack\n"));


        #if ( OTA_E2E_TEST_ENABLED == 1 )

        RunOtaE2eDemo();

        #else
        xResults = xTaskCreate(prvQualificationTestTask,
                                "TEST",
                                appmainTEST_TASK_STACK_SIZE,
                                NULL,
                                appmainTEST_TASK_PRIORITY,
                                NULL);

        #endif
    }

    while (1)
    {
        vTaskSuspend(NULL);
    }
}
/*****************************************************************************************
End of function main_task
****************************************************************************************/
/*-----------------------------------------------------------*/

/**********************************************************************************************************************
 * Function Name: prvMiscInitialization
 * Description  : .
 * Return Value : .
 *********************************************************************************************************************/
void prvMiscInitialization(void)
{
    /* Initialize UART for serial terminal. */
    extern void CLI_Support_Settings (void);
    CLI_Support_Settings();

    /* Start logging task. */
    xLoggingTaskInitialize(mainLOGGING_TASK_STACK_SIZE,
                            tskIDLE_PRIORITY + 2,
                            mainLOGGING_MESSAGE_QUEUE_LENGTH);

}
/*****************************************************************************************
End of function prvMiscInitialization
****************************************************************************************/
/*-----------------------------------------------------------*/

/**********************************************************************************************************************
 * Function Name: vApplicationDaemonTaskStartupHook
 * Description  : .
 * Return Value : .
 *********************************************************************************************************************/
void vApplicationDaemonTaskStartupHook(void)
{

}
/*****************************************************************************************
End of function vApplicationDaemonTaskStartupHook
****************************************************************************************/

/**********************************************************************************************************************
 * Function Name: ApplicationCounter
 * Description  : .
 * Argument     : xWaitTime
 * Return Value : .
 *********************************************************************************************************************/
bool ApplicationCounter(uint32_t xWaitTime)
{
    TickType_t xCurrent;
    bool DEMO_TEST = pdTRUE;
    const TickType_t xPrintFrequency = pdMS_TO_TICKS(xWaitTime);
    xCurrent = xTaskGetTickCount();
    signed char cRxChar;
    while (xCurrent < xPrintFrequency)
    {
        vTaskDelay(1);
        xCurrent = xTaskGetTickCount();

        cRxChar = vISR_Routine();
        if ((0 != cRxChar))
        {

            DEMO_TEST = pdFALSE;
            break;
        }
    }
    return DEMO_TEST;
}
/*****************************************************************************************
End of function ApplicationCounter
****************************************************************************************/

/**********************************************************************************************************************
 * Function Name: vISR_Routine
 * Description  : .
 * Return Value : .
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

/*-----------------------------------------------------------*/

/**********************************************************************************************************************
 * Function Name: vApplicationGetIdleTaskMemory
 * Description  : configUSE_STATIC_ALLOCATION is set to 1, so the application must provide an
 *                implementation of vApplicationGetIdleTaskMemory() to provide the memory that is
 *                used by the Idle task.
 * Arguments    : ppxIdleTaskTCBBuffer
 *              : ppxIdleTaskStackBuffer
 *              : pulIdleTaskStackSize
 * Return Value : .
 *********************************************************************************************************************/
void vApplicationGetIdleTaskMemory(StaticTask_t ** ppxIdleTaskTCBBuffer,
                                    StackType_t ** ppxIdleTaskStackBuffer,
                                    uint32_t * pulIdleTaskStackSize)
{
    /* If the buffers to be provided to the Idle task are declared inside this
     * function then they must be declared static - otherwise they will be allocated on
     * the stack and so not exists after this function exits. */
    static StaticTask_t xIdleTaskTCB;
    static StackType_t uxIdleTaskStack[configMINIMAL_STACK_SIZE];

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

/**********************************************************************************************************************
 * Function Name: vApplicationGetTimerTaskMemory
 * Description  : This is to provide the memory that is used by the RTOS daemon/time task.
 *
 *                If configUSE_STATIC_ALLOCATION is set to 1, so the application must provide an
 *                implementation of vApplicationGetTimerTaskMemory() to provide the memory that is
 *                used by the RTOS daemon/time task.
 * Arguments    : ppxTimerTaskTCBBuffer
 *              : ppxTimerTaskStackBuffer
 *              : pulTimerTaskStackSize
 * Return Value : .
 *********************************************************************************************************************/
void vApplicationGetTimerTaskMemory(StaticTask_t ** ppxTimerTaskTCBBuffer,
                                    StackType_t ** ppxTimerTaskStackBuffer,
                                    uint32_t * pulTimerTaskStackSize)
{
    /* If the buffers to be provided to the Timer task are declared inside this
     * function then they must be declared static - otherwise they will be allocated on
     * the stack and so not exists after this function exits. */
    static StaticTask_t xTimerTaskTCB;
    static StackType_t uxTimerTaskStack[configTIMER_TASK_STACK_DEPTH];

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

/**********************************************************************************************************************
 * Function Name: vApplicationMallocFailedHook
 * Description  : Warn user if pvPortMalloc fails.
 *
 *                Called if a call to pvPortMalloc() fails because there is insufficient
 *                free memory available in the FreeRTOS heap.  pvPortMalloc() is called
 *                internally by FreeRTOS API functions that create tasks, queues, software
 *                timers, and semaphores.  The size of the FreeRTOS heap is set by the
 *                configTOTAL_HEAP_SIZE configuration constant in FreeRTOSConfig.h.
 * Argument     :
 * Return Value : .
 *********************************************************************************************************************/
void vApplicationMallocFailedHook()
    {
        configPRINT_STRING(("ERROR: Malloc failed to allocate memory\r\n"));
        taskDISABLE_INTERRUPTS();

        for (;;)
        {
            /* Loop forever */
        }
    }
/*****************************************************************************************
End of function vApplicationMallocFailedHook
****************************************************************************************/

/*-----------------------------------------------------------*/

/**********************************************************************************************************************
 * Function Name: vApplicationStackOverflowHook
 * Description  : Loop forever if stack overflow is detected.
 *                If configCHECK_FOR_STACK_OVERFLOW is set to 1,
 *                this hook provides a location for applications to
 *                define a response to a stack overflow.
 *
 *                Use this hook to help identify that a stack overflow
 *                has occurred.
 * Arguments    : xTask
 *              : pcTaskName
 * Return Value : .
 *********************************************************************************************************************/
void vApplicationStackOverflowHook(TaskHandle_t xTask,
                                        char * pcTaskName)
    {
        configPRINT_STRING(("ERROR: stack overflow\r\n"));
        portDISABLE_INTERRUPTS();

        /* Unused Parameters */
        (void) xTask;
        (void) pcTaskName;

        for (;;)
        {
            /* Loop forever */
        }
    }
/*****************************************************************************************
End of function vApplicationStackOverflowHook
****************************************************************************************/

#endif /* iotconfigUSE_PORT_SPECIFIC_HOOKS */
/*-----------------------------------------------------------*/

#if ( ipconfigUSE_LLMNR != 0 ) || ( ipconfigUSE_NBNS != 0 ) || ( ipconfigDHCP_REGISTER_HOSTNAME == 1 )

/**********************************************************************************************************************
 * Function Name: pcApplicationHostnameHook
 * Description  : This function will be called during the DHCP: the machine will be registered
 *                 with an IP address plus this name.
 *                 Note: Please make sure vprvCacheInit() is called before this function, because
 *                 it retrieves thingname value from KeyValue table.
 * Return Value : .
 *********************************************************************************************************************/
const char * pcApplicationHostnameHook(void)
    {
#if defined(__TEST__)
        return clientcredentialIOT_THING_NAME;
#else
    {
        /* The string returned by this API is stipulated to be a maximum of 32 characters. */
        static char s_buff[32];
        memset (s_buff, 0x00, sizeof(s_buff));

        size_t valueLength = prvGetCacheEntryLength(KVS_CORE_THING_NAME);

        /* Process for thing name input by CLI. */
        if (valueLength > 0)
        {
            if (valueLength > 31)
            {
                configPRINT_STRING(("Warning: thing name with null-terminate string is longer than 32 characters.\r\n"));
                valueLength = 31;
            }
            size_t xLength = xReadEntry(KVS_CORE_THING_NAME, s_buff, valueLength);
            if (0 != xLength)
            {
                s_buff[valueLength] = '\0';
                return s_buff;
            }
            else
            {
                valueLength = strlen(clientcredentialIOT_THING_NAME);
                if (valueLength > 31)
                {
                    configPRINT_STRING(("Warning: thing name with null-terminate string is longer than 32 characters.\r\n"));
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
            if (valueLength > 31)
            {
                configPRINT_STRING(("Warning: thing name with null-terminate string is longer than 32 characters.\r\n"));
                valueLength = 31;
            }
            strncpy(s_buff, clientcredentialIOT_THING_NAME, valueLength);
            s_buff[valueLength] = '\0';
            return s_buff;
        }
    }
#endif
    }
/*****************************************************************************************
End of function pcApplicationHostnameHook
****************************************************************************************/
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

