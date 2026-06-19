/* generated thread source file - do not edit */
#include "net_thread.h"

static StaticTask_t net_thread_memory;

TaskHandle_t net_thread;
void net_thread_create(void);
static void net_thread_func(void *pvParameters);
void rtos_startup_err_callback(void *p_instance, void *p_data);
void rtos_startup_common_init(void);
extern uint32_t g_fsp_common_thread_count;
static StackType_t net_thread_stack[1024];

void net_thread_create(void)
{
    /* Increment count so we will know the number of threads created in the RX Configuration editor. */
    g_fsp_common_thread_count++;

    /* Initialize each kernel object. */

    net_thread = xTaskCreateStatic (net_thread_func,
                                    (const char*) "Net Thread", 1024, // In words, not bytes
                                    NULL, //pvParameters
                                    1,
                                    (StackType_t*) &net_thread_stack,
                                    (StaticTask_t*) &net_thread_memory
                                    );

    if (NULL == net_thread)
    {
        rtos_startup_err_callback (net_thread, 0);
    }
}
static void net_thread_func(void *pvParameters)
{
    /* Initialize common components */
    rtos_startup_common_init ();

    /* Enter user code for this thread. Pass task handle. */
    net_thread_entry (pvParameters);
}
