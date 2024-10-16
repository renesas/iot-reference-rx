/* generated thread header file - do not edit */
#ifndef NET_THREAD_H_
#define NET_THREAD_H_
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

#ifdef __cplusplus
                extern "C" void net_thread_entry(void * pvParameters);
                #else

#define mainNET_THREAD_STACK_SIZE	( (unsigned short)512 )
extern void net_thread_entry(void *pvParameters);
#endif
#endif /* NET_THREAD_H_ */
