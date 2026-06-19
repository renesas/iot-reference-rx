/*
 * Copyright (c) 2025 Renesas Electronics Corporation and/or its affiliates
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/**********************************************************************************************************************
 * File Name    : cellular_byte_pool_control.c
 * Description  : Manages the memory byte pool.
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Includes   <System Includes> , "Project Includes"
 *********************************************************************************************************************/
#include "cellular_freertos.h"

#if BSP_CFG_RTOS_USED == (5)
/**********************************************************************************************************************
 * Macro definitions
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Typedef definitions
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Exported global variables
 *********************************************************************************************************************/
uint8_t g_recv_thread[CELLULAR_RECV_THREAD_SIZE];
uint8_t g_ring_thread[CELLULAR_RING_THREAD_SIZE];

TX_BLOCK_POOL g_cellular_socket_pool;
TX_BLOCK_POOL g_cellular_event_pool;
TX_BLOCK_POOL g_cellular_thread_pool;
TX_BLOCK_POOL g_cellular_semaphore_pool;

static uint8_t s_cellular_socket_pool[TOTAL_SOCKET_BLOCK_SIZE];
static uint8_t s_cellular_event_pool[TOTAL_EVENT_BLOCK_SIZE];
static uint8_t s_cellular_thread_pool[TOTAL_THREAD_BLOCK_SIZE];
static uint8_t s_cellular_semaphore_pool[TOTAL_SEMAPHORE_BLOCK_SIZE];

/**********************************************************************************************************************
 * Private (static) variables and functions
 *********************************************************************************************************************/

/*****************************************************************************************
 * Function Name  @fn            cellular_block_pool_create
 ****************************************************************************************/
e_cellular_err_t cellular_block_pool_create(void)
{
    UINT             rtos_ret;
    e_cellular_err_t ret = CELLULAR_SUCCESS;

    rtos_ret = tx_block_pool_create(&g_cellular_socket_pool, "socket pool",
            sizeof(st_cellular_socket_ctrl_t), s_cellular_socket_pool, TOTAL_SOCKET_BLOCK_SIZE);

    if (TX_SUCCESS == rtos_ret)
    {
        rtos_ret = tx_block_pool_create(&g_cellular_event_pool, "event pool",
                sizeof(TX_EVENT_FLAGS_GROUP), s_cellular_event_pool, TOTAL_EVENT_BLOCK_SIZE);
    }

    if (TX_SUCCESS == rtos_ret)
    {
        rtos_ret = tx_block_pool_create(&g_cellular_thread_pool, "thread pool",
                sizeof(TX_THREAD), s_cellular_thread_pool, TOTAL_THREAD_BLOCK_SIZE);
    }

    if (TX_SUCCESS == rtos_ret)
    {
        rtos_ret = tx_block_pool_create(&g_cellular_semaphore_pool, "semaphore pool",
                sizeof(TX_SEMAPHORE), s_cellular_semaphore_pool, TOTAL_SEMAPHORE_BLOCK_SIZE);
    }

    if (TX_SUCCESS != rtos_ret)
    {
        ret = CELLULAR_ERR_MEMORY_ALLOCATION;
    }

    return ret;
}
/**********************************************************************************************************************
 * End of function cellular_block_pool_create
 *********************************************************************************************************************/

/*****************************************************************************************
 * Function Name  @fn            cellular_block_pool_delete
 ****************************************************************************************/
e_cellular_err_t cellular_block_pool_delete(void)
{
    UINT             rtos_ret;
    e_cellular_err_t ret = CELLULAR_SUCCESS;

    rtos_ret = tx_block_pool_delete(&g_cellular_socket_pool);

    if (TX_SUCCESS == rtos_ret)
    {
        rtos_ret = tx_block_pool_delete(&g_cellular_event_pool);
    }

    if (TX_SUCCESS == rtos_ret)
    {
        rtos_ret = tx_block_pool_delete(&g_cellular_thread_pool);
    }

    if (TX_SUCCESS == rtos_ret)
    {
        rtos_ret = tx_block_pool_delete(&g_cellular_semaphore_pool);
    }

    if (TX_SUCCESS != rtos_ret)
    {
        ret = CELLULAR_ERR_MEMORY_ALLOCATION;
    }

    return ret;
}
/**********************************************************************************************************************
 * End of function cellular_block_pool_delete
 *********************************************************************************************************************/
#endif /* BSP_CFG_RTOS_USED == 5 */
