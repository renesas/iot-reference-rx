/*
FreeRTOS
Copyright (C) 2020 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
Modifications Copyright (C) 2023 Renesas Electronics Corporation. or its affiliates.

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

/* FreeRTOS+TCP includes. */
#include "FreeRTOSIPConfig.h"

/* Demo includes */
#include "aws_clientcredential.h"
#include "r_cellular_if.h"
#include "demo_config.h"
#include "store.h"
#include "mqtt_agent_task.h"

st_cellular_ctrl_t cellular_ctrl;
static bool _wifiEnable( void );
static bool _wifiConnectAccessPoint( void );

extern int32_t littlFs_init(void);
bool ApplicationCounter(uint32_t xWaitTime);
signed char vISR_Routine( void );

extern void vStartSimplePubSubDemo( void  );

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
 * @brief Subscribe Publish demo tasks configuration.
 * Subscribe publish demo task shows the basic functionality of connecting to an MQTT broker, subscribing
 * to a topic, publishing messages to a topic and reporting the incoming messages on subscribed topic.
 * Number of subscribe publish demo tasks to be spawned is configurable.
 */
#define appmainMQTT_NUM_PUBSUB_TASKS              ( 2 )
#define appmainMQTT_PUBSUB_TASK_STACK_SIZE        ( 2048 )
#define appmainMQTT_PUBSUB_TASK_PRIORITY          ( tskIDLE_PRIORITY +1 )

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

#define mainLOGGING_TASK_STACK_SIZE         ( configMINIMAL_STACK_SIZE * 6 )
#define mainLOGGING_MESSAGE_QUEUE_LENGTH    ( 15 )
#define mainTEST_RUNNER_TASK_STACK_SIZE    ( configMINIMAL_STACK_SIZE * 8 )
#define UNSIGNED_SHORT_RANDOM_NUMBER_MASK         (0xFFFFUL)

/**
 * @brief Band Select for Cellular connecting.
 * This is used for setting the band for cellular.
 * You can select below bands. Bands is related on Cellular carrier,
 * please must set bands your region and SIM acceptable.
 * ------------------------------------------------------
 * North America 2,4,5,12,13,25
 * EMEA 1,3,8,20,28
 * Japan 1,8,18,19,26
 * Australia 1,3,8,28
 * ------------------------------------------------------
 */
#define CELLULAR_BAND_CONFIG	"1,2,4,5,8,12,13,14,17,18,19,20,25,26,28,66"

/**
 * @brief Application task startup hook.
 */
void vApplicationDaemonTaskStartupHook( void );

/**
 * @brief Initializes the board.
 */
void prvMiscInitialization( void );

extern void CLI_Support_Settings(void);
extern void vUARTCommandConsoleStart( uint16_t usStackSize, UBaseType_t uxPriority );
extern void vRegisterSampleCLICommands( void );

/*-----------------------------------------------------------*/

/**
 * @brief The application entry point from a power on reset is PowerON_Reset_PC()
 * in resetprg.c.
 */
void main( void )
{
	int32_t xResults, Time2Wait = 10000;

	#define mainUART_COMMAND_CONSOLE_STACK_SIZE	( configMINIMAL_STACK_SIZE * 6UL )
	/* The priority used by the UART command console task. */
	#define mainUART_COMMAND_CONSOLE_TASK_PRIORITY	( 1 )

	extern void vRegisterSampleCLICommands( void );
	extern void vUARTCommandConsoleStart( uint16_t usStackSize, UBaseType_t uxPriority );
	extern TaskHandle_t xCLIHandle;

	prvMiscInitialization();

	/* Register the standard CLI commands. */
	vRegisterSampleCLICommands();
	vUARTCommandConsoleStart( mainUART_COMMAND_CONSOLE_STACK_SIZE, mainUART_COMMAND_CONSOLE_TASK_PRIORITY );

	xResults = littlFs_init();

	xMQTTAgentInit();

	if (xResults == LFS_ERR_OK)
	{
		xResults = vprvCacheInit();
	}

	if(ApplicationCounter(Time2Wait))
	{
		/* Remove CLI task before going to demo. */
		vTaskDelete(xCLIHandle);

		if(_wifiEnable())
		{
			vTaskDelay(300);
		}

		FreeRTOS_printf( ( "Initialise the RTOS's TCP/IP stack\n" ) );

		FreeRTOS_printf( ( "---------STARTING DEMO---------\r\n" ) );

        #if (ENABLE_FLEET_PROVISIONING_DEMO == 1)
           vStartFleetProvisioningDemo();
        #else
           xSetMQTTAgentState( MQTT_AGENT_STATE_INITIALIZED );
        #endif

        vStartMQTTAgent (appmainMQTT_AGENT_TASK_STACK_SIZE, appmainMQTT_AGENT_TASK_PRIORITY);

        vStartSimplePubSubDemo ();

        #if (ENABLE_OTA_UPDATE_DEMO == 1)
                  vStartOtaDemo();
        #endif
	}

	while( 1 )
	{
		vTaskSuspend( NULL );
	}
}
/*-----------------------------------------------------------*/

void prvMiscInitialization( void )
{
    /* Initialize UART for serial terminal. */
	CLI_Support_Settings();

    /* Start logging task. */
    xLoggingTaskInitialize( mainLOGGING_TASK_STACK_SIZE,
                            tskIDLE_PRIORITY + 2,
                            mainLOGGING_MESSAGE_QUEUE_LENGTH );

}
/*-----------------------------------------------------------*/

void vApplicationDaemonTaskStartupHook( void )
{

}

/*-----------------------------------------------------------*/

/* configUSE_STATIC_ALLOCATION is set to 1, so the application must provide an
 * implementation of vApplicationGetIdleTaskMemory() to provide the memory that is
 * used by the Idle task. */
void vApplicationGetIdleTaskMemory( StaticTask_t ** ppxIdleTaskTCBBuffer,
                                    StackType_t ** ppxIdleTaskStackBuffer,
                                    uint32_t * pulIdleTaskStackSize )
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
/*-----------------------------------------------------------*/

/**
 * @brief This is to provide the memory that is used by the RTOS daemon/time task.
 *
 * If configUSE_STATIC_ALLOCATION is set to 1, so the application must provide an
 * implementation of vApplicationGetTimerTaskMemory() to provide the memory that is
 * used by the RTOS daemon/time task.
 */
void vApplicationGetTimerTaskMemory( StaticTask_t ** ppxTimerTaskTCBBuffer,
                                     StackType_t ** ppxTimerTaskStackBuffer,
                                     uint32_t * pulTimerTaskStackSize )
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
        configPRINT_STRING( ( "ERROR: Malloc failed to allocate memory\r\n" ) );
        taskDISABLE_INTERRUPTS();

        /* Loop forever */
        for( ; ; )
        {
        }
    }

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
    void vApplicationStackOverflowHook( TaskHandle_t xTask,
                                        char * pcTaskName )
    {
        configPRINT_STRING( ( "ERROR: stack overflow\r\n" ) );
        portDISABLE_INTERRUPTS();

        /* Unused Parameters */
        ( void ) xTask;
        ( void ) pcTaskName;

        /* Loop forever */
        for( ; ; )
        {
        }
    }
#endif /* iotconfigUSE_PORT_SPECIFIC_HOOKS */
/*-----------------------------------------------------------*/

#if ( ipconfigUSE_LLMNR != 0 ) || ( ipconfigUSE_NBNS != 0 ) || ( ipconfigDHCP_REGISTER_HOSTNAME == 1 )

    const char * pcApplicationHostnameHook( void )
    {
        /* This function will be called during the DHCP: the machine will be registered
         * with an IP address plus this name. */
        return clientcredentialIOT_THING_NAME;
    }

#endif

bool ApplicationCounter(uint32_t xWaitTime)
{
    TickType_t xCurrent;
    bool DEMO_TEST = pdTRUE;
    const TickType_t xPrintFrequency = pdMS_TO_TICKS( xWaitTime );
    xCurrent = xTaskGetTickCount();
    signed char cRxChar;
    while( xCurrent < xPrintFrequency )
    {
    	vTaskDelay(1);
    	xCurrent = xTaskGetTickCount();

    	cRxChar = vISR_Routine();
    	if ((cRxChar != 0) )
    	{

    		DEMO_TEST = pdFALSE;
    		break;
    	}
    }
    return DEMO_TEST;
}

signed char vISR_Routine( void )
{
	BaseType_t xTaskWokenByReceive = pdFALSE;
	extern signed char cRxedChar;
    return cRxedChar;
}

static bool _wifiConnectAccessPoint( void )
{
	configPRINTF(("Connect to AccessPoint \r\n "));
	e_cellular_err_t ret = R_CELLULAR_APConnect(&cellular_ctrl, NULL);

	if(CELLULAR_SUCCESS != ret)
	{
		LogError(("Error: AccessPoint connect time out. Please set more long period for waiting Connection."));
	}

	return (ret == CELLULAR_SUCCESS);
}


static bool _wifiEnable( void )
{
	bool result = pdFALSE;
	e_cellular_err_t ret = R_CELLULAR_Open(&cellular_ctrl, NULL);


#if 1 /* This is enable from R_Cellular Driver rev1.10 */
	if(CELLULAR_SUCCESS == ret )
	{
		/* Set SIM Operator */
		ret = R_CELLULAR_SetOperator(&cellular_ctrl, "standard");
	}
#endif /* This is enable from R_Cellular Driver rev1.10 */

	if(CELLULAR_SUCCESS == ret )
	{
		/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!  */
		/* !! Please must set your cellular band not to connect band that not support your region  !!  */
		/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!  */
		/* Show band setting for cellular */
		configPRINTF(("Set the band of Cellular. Set bands %s:" CELLULAR_BAND_CONFIG ));
		/* Set cellular bands */
		ret = R_CELLULAR_SetBand(&cellular_ctrl, CELLULAR_BAND_CONFIG);
	}

	if(CELLULAR_SUCCESS == ret )
	{
		configPRINTF(("Connect to AccessPoint. \r\n"));
		configPRINTF(("It takes around 3 or 5 minute when you connect to it first time. \r\n "));
		result = _wifiConnectAccessPoint();
	}

	return result;
}


