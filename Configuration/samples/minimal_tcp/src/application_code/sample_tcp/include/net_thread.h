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

/* Sets the length of the array of addresses of Domain Name Servers for each
 * endpoint.
 *
 * The field ucDNSIndex in the IP parameters of a NetworkEndPoint_t will point
 * to the DNS in use. When a DNS times out, ucDNSIndex will be moved to the
 * next available DNS.
 */
#define ipconfigENDPOINT_DNS_ADDRESS_COUNT    ( 1U )
