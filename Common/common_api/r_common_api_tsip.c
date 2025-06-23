/*
* Copyright (c) 2024-2025 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/

/***********************************************************************************************************************
 * File Name    : r_common_api_tsip.c
 * Description  : This file provides the implementation of common API functions for managing TSIP (Trusted Secure IP)
 *                operations. It includes thread-safe resource management and ensures proper initialization and cleanup
 *                of TSIP-related resources.
 **********************************************************************************************************************/

/**********************************************************************************************************************
 Includes   <System Includes> , "Project Includes"
 *********************************************************************************************************************/
#include <stdio.h>

#include "FreeRTOS.h"
#include "queue.h"
#include "stddef.h"
#include "serial.h"
#include "semphr.h"
#include "r_common_api_tsip.h"

/**********************************************************************************************************************
 * Macro definitions
 *********************************************************************************************************************/

/**********************************************************************************************************************
 Exported global variables
 *********************************************************************************************************************/
xSemaphoreHandle xSemaphoreTSIPAccess;

/**********************************************************************************************************************
 Private (static) variables
 *********************************************************************************************************************/
static volatile CommonAPI_Status_t s_TSIP_COM_Status = COMAPI_STATE_CLOSE;/* Variables that manage commonAPI status */

/* Function Name: TSIP_COM_Resource_Create */
/******************************************************************************************************************//**
 * @brief Functions for creating OS resources.
 * @param[in] void
 * @return e_commonapi_err_t
 * @retval COMMONAPI_SUCCESS        success
 * @retval COMMONAPI_ERR            fail
 *********************************************************************************************************************/
static e_commonapi_err_t TSIP_COM_Resource_Create(void)
{
    xSemaphoreTSIPAccess = xSemaphoreCreateBinary();
    configASSERT(xSemaphoreTSIPAccess);

    /* Return an error if xSemaphoreTSIPAccess is NULL */
    if (NULL == xSemaphoreTSIPAccess)
    {
        return COMMONAPI_ERR;
    }

    xSemaphoreGive(xSemaphoreTSIPAccess);

    return COMMONAPI_SUCCESS;
}
/**********************************************************************************************************************
 End of function TSIP_COM_Resource_Create
 *********************************************************************************************************************/

/* Function Name: TSIP_COM_Resource_Release */
/******************************************************************************************************************//**
 * @brief Function to release OS resources.
 * @param[in] void
 * @return e_commonapi_err_t
 * @retval COMMONAPI_SUCCESS        success
 * @retval COMMONAPI_ERR            fail
 *********************************************************************************************************************/
static e_commonapi_err_t TSIP_COM_Resource_Release(void)
{
    if (NULL != xSemaphoreTSIPAccess)
    {
        vSemaphoreDelete(xSemaphoreTSIPAccess);
        xSemaphoreTSIPAccess = NULL;
    }

    return COMMONAPI_SUCCESS;
}
/**********************************************************************************************************************
 End of function TSIP_COM_Resource_Release
 *********************************************************************************************************************/

/* Function Name: R_Demo_Common_API_TSIP_Open */
/******************************************************************************************************************//**
 * @brief CommonAPI open function for TSIP.
 * @param[in] void
 * @return e_commonapi_err_t
 * @retval COMMONAPI_SUCCESS        success
 * @retval COMMONAPI_ERR            fail
 *********************************************************************************************************************/
e_commonapi_err_t R_Demo_Common_API_TSIP_Open(tsip_tls_ca_certification_public_key_index_t *ki1, tsip_update_key_ring_t *ki2)
{
    e_commonapi_err_t return_err_state = COMMONAPI_SUCCESS;
    e_tsip_err_t error_code = TSIP_SUCCESS;

    /* Enter critical section */
    taskENTER_CRITICAL();

    if (COMAPI_STATE_CLOSE == s_TSIP_COM_Status)
    {
        /* Status change */
        s_TSIP_COM_Status = COMAPI_STATE_RUNNING;

        /* Exit critical section */
        taskEXIT_CRITICAL();

        /* TSIP Driver Open */
        error_code = R_TSIP_Open(ki1, ki2);
        if (TSIP_SUCCESS != error_code)
        {
            s_TSIP_COM_Status = COMAPI_STATE_CLOSE;

            return COMMONAPI_ERR;
        }

        /* Resource Create */
        return_err_state = TSIP_COM_Resource_Create ();
        if (COMMONAPI_ERR == return_err_state)
        {
            s_TSIP_COM_Status = COMAPI_STATE_CLOSE;

            return COMMONAPI_ERR;
        }

        /* Status change */
        s_TSIP_COM_Status = COMAPI_STATE_OPEN;

    }
    else if (COMAPI_STATE_RUNNING == s_TSIP_COM_Status)
    {
        /* Exit critical section */
        taskEXIT_CRITICAL();

        /* Execute delay until running state ends */
        while (COMAPI_STATE_RUNNING == s_TSIP_COM_Status)
        {
            vTaskDelay (10);
        }

        /* Returns an error if status is not open */
        if (COMAPI_STATE_OPEN != s_TSIP_COM_Status)
        {
            return COMMONAPI_ERR;
        }
    }
    else if (COMAPI_STATE_OPEN == s_TSIP_COM_Status)
    {
        /* Exit critical section */
        taskEXIT_CRITICAL();
    }
    else
    {
        /* Exit critical section */
        taskEXIT_CRITICAL();

        /* Returns an error because it contains an unexpected status */
        return COMMONAPI_ERR;
    }

    return return_err_state;
}
/**********************************************************************************************************************
 End of function R_Demo_Common_API_TSIP_Open
 *********************************************************************************************************************/

/* Function Name: R_Demo_Common_API_TSIP_Close */
/******************************************************************************************************************//**
 * @brief CommonAPI close function for TSIP.
 * @param[in] void
 * @return e_commonapi_err_t
 * @retval COMMONAPI_SUCCESS        success
 * @retval COMMONAPI_ERR            fail
 *********************************************************************************************************************/
e_commonapi_err_t R_Demo_Common_API_TSIP_Close(void)
{
    e_commonapi_err_t return_err_state = COMMONAPI_SUCCESS;

    /* Enter critical section */
    taskENTER_CRITICAL();

    if (COMAPI_STATE_OPEN == s_TSIP_COM_Status)
    {
        /* Status change */
        s_TSIP_COM_Status = COMAPI_STATE_RUNNING;

        /* Exit critical section */
        taskEXIT_CRITICAL();

        /* TSIP Driver Close */
        (void)R_TSIP_Close();
        
        /* Resource Release */
        return_err_state = TSIP_COM_Resource_Release ();
        if (COMMONAPI_ERR == return_err_state)
        {
            s_TSIP_COM_Status = COMAPI_STATE_CLOSE;

            return COMMONAPI_ERR;
        }

        /* Status change */
        s_TSIP_COM_Status = COMAPI_STATE_CLOSE;

    }
    else if (COMAPI_STATE_RUNNING == s_TSIP_COM_Status)
    {
        /* Exit critical section */
        taskEXIT_CRITICAL();

        /* Execute delay until running state ends */
        while (COMAPI_STATE_RUNNING == s_TSIP_COM_Status)
        {
            vTaskDelay (10);
        }

        /* Returns an error if status is not close */
        if (COMAPI_STATE_CLOSE != s_TSIP_COM_Status)
        {
            return COMMONAPI_ERR;
        }
    }
    else if (COMAPI_STATE_CLOSE == s_TSIP_COM_Status)
    {
        /* Exit critical section */
        taskEXIT_CRITICAL();
    }
    else
    {
        /* Exit critical section */
        taskEXIT_CRITICAL();

        /* Returns an error because it contains an unexpected status */
        return COMMONAPI_ERR;
    }

    return return_err_state;
}
/**********************************************************************************************************************
 End of function R_Demo_Common_API_TSIP_Close
 *********************************************************************************************************************/
