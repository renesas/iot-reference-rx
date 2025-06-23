/**********************************************************************************************************************
 * DISCLAIMER
 * This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products. No
 * other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all
 * applicable laws, including copyright laws.
 * THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
 * THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED. TO THE MAXIMUM
 * EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES
 * SHALL BE LIABLE FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO
 * THIS SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
 * Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability of
 * this software. By using this software, you agree to the additional terms and conditions found by accessing the
 * following link:
 * http://www.renesas.com/disclaimer
 *
 * Copyright (C) 2024 Renesas Electronics Corporation. All rights reserved.
 *********************************************************************************************************************/
/**********************************************************************************************************************
 * File Name    : r_wifi_da16xxx_common.c
 * Description  : Common API functions definition for DA16XXX.
 *********************************************************************************************************************/

/**********************************************************************************************************************
 Includes   <System Includes> , "Project Includes"
 *********************************************************************************************************************/
#include "r_wifi_da16xxx_private.h"

/**********************************************************************************************************************
 Macro definitions
 *********************************************************************************************************************/
#if defined(__CCRX__) || defined(__ICCRX__) || defined(__RX__)
#define UART_BAUD_MAX_CNT    (4)
#elif defined(__CCRL__) || defined(__ICCRL78__) || defined(__RL)
#define UART_BAUD_MAX_CNT    (3)
#endif

#if defined(__CCRX__) || defined(__ICCRX__) || defined(__RX__)
#if WIFI_CFG_LOGGING_OPTION == 2
/* FreeRTOS CLI Command Console */
#if WIFI_CFG_LOG_TERM_CHANNEL == (0)
#define U_SCI_UART_CLI_PINSET()  R_SCI_PinSet_SCI0()
#elif WIFI_CFG_LOG_TERM_CHANNEL == (1)
#define U_SCI_UART_CLI_PINSET()  R_SCI_PinSet_SCI1()
#elif WIFI_CFG_LOG_TERM_CHANNEL == (2)
#define U_SCI_UART_CLI_PINSET()  R_SCI_PinSet_SCI2()
#elif WIFI_CFG_LOG_TERM_CHANNEL == (3)
#define U_SCI_UART_CLI_PINSET()  R_SCI_PinSet_SCI3()
#elif WIFI_CFG_LOG_TERM_CHANNEL == (4)
#define U_SCI_UART_CLI_PINSET()  R_SCI_PinSet_SCI4()
#elif WIFI_CFG_LOG_TERM_CHANNEL == (5)
#define U_SCI_UART_CLI_PINSET()  R_SCI_PinSet_SCI5()
#elif WIFI_CFG_LOG_TERM_CHANNEL == (6)
#define U_SCI_UART_CLI_PINSET()  R_SCI_PinSet_SCI6()
#elif WIFI_CFG_LOG_TERM_CHANNEL == (7)
#define U_SCI_UART_CLI_PINSET()  R_SCI_PinSet_SCI7()
#elif WIFI_CFG_LOG_TERM_CHANNEL == (8)
#define U_SCI_UART_CLI_PINSET()  R_SCI_PinSet_SCI8()
#elif WIFI_CFG_LOG_TERM_CHANNEL == (9)
#define U_SCI_UART_CLI_PINSET()  R_SCI_PinSet_SCI9()
#elif WIFI_CFG_LOG_TERM_CHANNEL == (10)
#define U_SCI_UART_CLI_PINSET()  R_SCI_PinSet_SCI10()
#elif WIFI_CFG_LOG_TERM_CHANNEL == (11)
#define U_SCI_UART_CLI_PINSET()  R_SCI_PinSet_SCI11()
#elif WIFI_CFG_LOG_TERM_CHANNEL == (12)
#define U_SCI_UART_CLI_PINSET()  R_SCI_PinSet_SCI12()
#else
#error "Error! Invalid setting for WIFI_CFG_LOG_TERM_CHANNEL in r_wifi_da16xxx_config.h"
#endif
#endif
#endif

/**********************************************************************************************************************
 Global Typedef definitions
 *********************************************************************************************************************/

/**********************************************************************************************************************
 Local Typedef definitions
 *********************************************************************************************************************/

/**********************************************************************************************************************
 Exported global variables
 *********************************************************************************************************************/
/* IP address */
static wifi_ip_configuration_t s_cur = {0};  /* Current IP configuration of the module */
static uint8_t g_rx_buff;

/* WIFI system state */
static volatile e_wifi_module_status_t s_wifi_system_state = MODULE_DISCONNECTED;
static volatile uint8_t g_rx_idx = 0;
#if WIFI_CFG_LOGGING_OPTION == 2
static sci_hdl_t g_sci_handle;
#endif

/**********************************************************************************************************************
 Private (static) variables and functions
 *********************************************************************************************************************/
/* System state control */
static void wifi_system_state_set (e_wifi_module_status_t state);
static e_wifi_module_status_t wifi_system_state_get (void);

/* Sub functions */
static wifi_err_t disconnect_ap_sub (void);

/* WIFI module control */
static void da16xxx_hw_reset (void);
static void da16xxx_close (void);
#if WIFI_CFG_SNTP_ENABLE == 1
static wifi_err_t da16xxx_sntp_service_init(void);
#endif

/* Log functions */
#if WIFI_CFG_LOGGING_OPTION != 0
static int16_t s_vsnprintf_safe (char * s, size_t n, const char * format, va_list arg);
static int16_t s_snprintf_safe (char * s, size_t n, const char * format, ...);
static void s_log_printf_common (uint8_t usLoggingLevel, const char WIFI_FAR * pcFormat, va_list args);
#endif
#if WIFI_CFG_LOGGING_OPTION == 2
static void s_uart_config (void);
static void s_uart_string_printf(char WIFI_FAR * pcString);
#endif

/**********************************************************************************************************************
 * Function Name: R_WIFI_DA16XXX_Open
 * Description  : Open WIFI Module.
 * Arguments    : none
 * Return Value : WIFI_SUCCESS
 *                WIFI_ERR_ALREADY_OPEN
 *                WIFI_ERR_SERIAL_OPEN
 *                WIFI_ERR_MODULE_COM
 *                WIFI_ERR_BYTEQ_OPEN
 *                WIFI_ERR_TAKE_MUTEX
 *********************************************************************************************************************/
wifi_err_t R_WIFI_DA16XXX_Open(void)
{
    wifi_err_t api_ret = WIFI_SUCCESS;
    uint8_t index = 0;
#if defined(__CCRX__) || defined(__ICCRX__) || defined(__RX__)
    uint32_t uart_baud_rates[UART_BAUD_MAX_CNT] = {115200, 230400, 460800, 921600};
#elif defined(__CCRL__) || defined(__ICCRL78__) || defined(__RL)
    uint32_t uart_baud_rates[UART_BAUD_MAX_CNT] = {115200, 230400, 460800};
#endif

#if WIFI_CFG_LOGGING_OPTION == 2
    /* Logging */
    s_uart_config();
#endif

    /* Disconnected WiFi module? */
    if (MODULE_DISCONNECTED != wifi_system_state_get())
    {
        WIFI_LOG_ERROR(("R_WIFI_DA16XXX_Open is called before!"));
        return WIFI_ERR_ALREADY_OPEN;
    }

    /* Mutex initialize */
    if (WIFI_SUCCESS != os_wrap_mutex_create())
    {
        api_ret = WIFI_ERR_TAKE_MUTEX;
        goto END_INITIALIZE;
    }

    /* Reset WIFI module */
    da16xxx_hw_reset();

    /* Open serial port */
    if (ATCMD_OK != uart_port_open())
    {
        api_ret = WIFI_ERR_SERIAL_OPEN;
        goto END_INITIALIZE;
    }

    for (index = 0; index < UART_BAUD_MAX_CNT; index++)
    {
        /* Set baud rate for serial port */
        uart_port_set_baudrate(uart_baud_rates[index]);

        /* Test basic communications with an AT command. */
        WIFI_LOG_INFO(("R_WIFI_DA16XXX_Open: Test with baud rate %lu!", uart_baud_rates[index]));
        if (AT_OK != at_exec("ATZ\r"))
        {
#if (WIFI_CFG_DA16600_SUPPORT)
            os_wrap_sleep(5000, UNIT_MSEC);
#else
            os_wrap_sleep(10, UNIT_MSEC);
#endif
            if (AT_OK != at_exec("ATZ\r"))
            {
                os_wrap_sleep(10, UNIT_MSEC);
            }
            else
            {
                break;
            }
        }
        else
        {
            break;
        }
    }

    if (index == UART_BAUD_MAX_CNT)
    {
        WIFI_LOG_ERROR(("R_WIFI_DA16XXX_Open: Cannot communicate with Wi-fi module!"));
        api_ret = WIFI_ERR_MODULE_COM;
        goto END_INITIALIZE;
    }

    /* Update the module baud rate in case if it doesn't match with user configured baud rate */
    WIFI_LOG_INFO(("R_WIFI_DA16XXX_Open: baud rate:%lu", WIFI_CFG_SCI_BAUDRATE));
    if (uart_baud_rates[index] != WIFI_CFG_SCI_BAUDRATE)
    {
        /* Configure UART parameters for UART port. */
        if (AT_OK != at_exec("ATB=%lu\r", WIFI_CFG_SCI_BAUDRATE))
        {
            WIFI_LOG_ERROR(("R_WIFI_DA16XXX_Open: Cannot update Wi-fi module baud rate!"));
            api_ret = WIFI_ERR_MODULE_COM;
            goto END_INITIALIZE;
        }

        /* Set baud rate for serial port */
        uart_port_set_baudrate(WIFI_CFG_SCI_BAUDRATE);
    }

    /* Show version info */
    at_exec("AT+VER\r");

#if defined(__CCRX__) || defined(__ICCRX__) || defined(__RX__)
#if WIFI_CFG_DEBUG_LOG == LOG_DEBUG
    /* Enable echo back */
    at_exec("ATE\r");
#endif
#endif

    /* Set AP mode */
    at_exec("AT+WFMODE=0\r");

    /* Set Wi-Fi STA profile */
    at_exec("AT+WFDIS=0\r");

    /* Set Country Code */
    at_exec("AT+WFCC=%s\r", WIFI_CFG_COUNTRY_CODE);

    /* Set WIFI State to "Connected WiFi module" */
    wifi_system_state_set(MODULE_CONNECTED);
#if WIFI_CFG_SNTP_ENABLE == 1
    api_ret = da16xxx_sntp_service_init();
    if (WIFI_SUCCESS != api_ret)
    {
        goto END_INITIALIZE;
    }
#endif

#if defined(__CCRX__) || defined(__ICCRX__) || defined(__RX__)
    /* Set flow control for serial port */
    flow_ctrl_init();
#endif

END_INITIALIZE:
    if (WIFI_SUCCESS != api_ret)
    {
        da16xxx_close();
    }
    return api_ret;
}
/**********************************************************************************************************************
 * End of function R_WIFI_DA16XXX_Open
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: R_WIFI_DA16XXX_IsOpened
 * Description  : Check wifi is opened.
 * Arguments    : none
 * Return Value : 0  - opened
 *                -1 - not opened
 *********************************************************************************************************************/
int32_t R_WIFI_DA16XXX_IsOpened(void)
{
    /* Connected access point? */
    if (MODULE_DISCONNECTED == wifi_system_state_get())
    {
        return -1;
    }
    else
    {
        return 0;
    }
}
/**********************************************************************************************************************
 * End of function R_WIFI_DA16XXX_IsOpened
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: R_WIFI_DA16XXX_Close
 * Description  : Close WIFI Module.
 * Arguments    : none
 * Return Value : WIFI_SUCCESS
 *********************************************************************************************************************/
wifi_err_t R_WIFI_DA16XXX_Close(void)
{
    /* Disconnected WiFi module? */
    if (MODULE_DISCONNECTED == wifi_system_state_get())
    {
        return WIFI_SUCCESS;
    }

    disconnect_ap_sub();

#if WIFI_CFG_LOGGING_OPTION == 2
    /* Logging */
    R_SCI_Close(g_sci_handle);
#endif

    /* Close module */
    da16xxx_close();

    return WIFI_SUCCESS;
}
/**********************************************************************************************************************
 * End of function R_WIFI_DA16XXX_Close
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: R_WIFI_DA16XXX_Ping
 * Description  : Execute Ping command.
 * Arguments    : ip_address
 *                count
 * Return Value : WIFI_SUCCESS
 *                WIFI_ERR_PARAMETER
 *                WIFI_ERR_MODULE_COM
 *                WIFI_ERR_NOT_CONNECT
 *                WIFI_ERR_TAKE_MUTEX
 *********************************************************************************************************************/
wifi_err_t R_WIFI_DA16XXX_Ping(uint32_t *ip_address, uint16_t count)
{
    wifi_err_t api_ret = WIFI_SUCCESS;
    uint16_t sent_count = 0;
    uint16_t recv_count = 0;

    /* Connected access point? */
    if (0 != R_WIFI_DA16XXX_IsConnected())
    {
        WIFI_LOG_ERROR(("R_WIFI_DA16XXX_Ping: R_WIFI_DA16XXX_Connect is not called!"));
        return WIFI_ERR_NOT_CONNECT;
    }

    /* Check parameters */
    if ((NULL == ip_address) || (0 == count))
    {
        return WIFI_ERR_PARAMETER;
    }

    /* Take mutex */
    if (OS_WRAP_SUCCESS != os_wrap_mutex_take(MUTEX_TX | MUTEX_RX))
    {
        return WIFI_ERR_TAKE_MUTEX;
    }

    /* Send ping to specified IP address. */
    at_set_timeout(5000 * count);
    if (AT_OK == at_exec_wo_mutex("AT+NWPING=0,%d.%d.%d.%d,%d\r",
                                  ip_address[0], ip_address[1], ip_address[2], ip_address[3], count))
    {
        at_read("+NWPING:%hu,%hu", &sent_count, &recv_count);
        if (sent_count != recv_count)
        {
            WIFI_LOG_ERROR(("R_WIFI_DA16XXX_Ping: Cannot reach to IP address (sent: %d, recv: %d)!",
                            sent_count, recv_count));
            api_ret = WIFI_ERR_MODULE_COM;
        }
    }
    else
    {
        api_ret = WIFI_ERR_MODULE_COM;
    }
    at_set_timeout(ATCMD_RESP_TIMEOUT);

    /* Give mutex */
    os_wrap_mutex_give(MUTEX_TX | MUTEX_RX);

    return api_ret;
}
/**********************************************************************************************************************
 * End of function R_WIFI_DA16XXX_Ping
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: R_WIFI_DA16XXX_Scan
 * Description  : Scan Access points.
 * Arguments    : ap_results
 *                max_networks
 * Return Value : WIFI_SUCCESS
 *                WIFI_ERR_PARAMETER
 *                WIFI_ERR_NOT_OPEN
 *                WIFI_ERR_MODULE_COM
 *                WIFI_ERR_TAKE_MUTEX
 *********************************************************************************************************************/
wifi_err_t R_WIFI_DA16XXX_Scan(wifi_scan_result_t * ap_results, uint8_t max_networks)
{
    wifi_err_t     api_ret = WIFI_SUCCESS;
    e_rslt_code_t  at_rslt;
    uint8_t        data_ret = DATA_NOT_FOUND;
    uint8_t        i;
    uint8_t        flag[50] = "\0";

    /* Disconnected WiFi module? */
    if (MODULE_DISCONNECTED == wifi_system_state_get())
    {
        WIFI_LOG_ERROR(("R_WIFI_DA16XXX_Scan: R_WIFI_DA16XXX_Open is not called!"));
        return WIFI_ERR_NOT_OPEN;
    }

    /* Check parameters */
    if (NULL == ap_results)
    {
        return WIFI_ERR_PARAMETER;
    }

    /* Take mutex */
    if (OS_WRAP_SUCCESS != os_wrap_mutex_take(MUTEX_TX | MUTEX_RX))
    {
        return WIFI_ERR_TAKE_MUTEX;
    }

    /* Show AP scan result. */
    at_set_timeout(60000);
    at_rslt = at_exec_wo_mutex("AT+WFSCAN\r");
    if (AT_OK != at_rslt)
    {
        api_ret = WIFI_ERR_MODULE_COM;
        goto RETURN_ERROR;
    }

    /* set timeout to default */
    at_set_timeout(ATCMD_RESP_TIMEOUT);

    /* clear tmp variables */
    memset(flag, 0, 50);
    i = 0;
    while (i != max_networks)
    {
        if (0 == strncmp((const char *)at_get_current_line(), "OK\r\n", 4))
        {
            break;
        }
        if (i == 0)
        {
            data_ret = at_read("+WFSCAN:%x:%x:%x:%x:%x:%x\t%*d\t%d\t%s%*[\t]%[^\n]\n",
                               &ap_results[i].bssid[0], &ap_results[i].bssid[1], &ap_results[i].bssid[2],
                               &ap_results[i].bssid[3], &ap_results[i].bssid[4], &ap_results[i].bssid[5],
                               &ap_results[i].rssi, flag, ap_results[i].ssid);
        }
        else
        {
            data_ret = at_read_wo_prefix("%x:%x:%x:%x:%x:%x\t%*d\t%d\t%s%*[\t]%[^\n]\n",
                                         &ap_results[i].bssid[0], &ap_results[i].bssid[1], &ap_results[i].bssid[2],
                                         &ap_results[i].bssid[3], &ap_results[i].bssid[4], &ap_results[i].bssid[5],
                                         &ap_results[i].rssi, flag, ap_results[i].ssid);
            at_move_to_next_line();
        }
        if (DATA_FOUND == data_ret)
        {

            /* security */
            if (NULL != strstr((const char *)flag, "[WPA2-PSK"))
            {
                ap_results[i].security = WIFI_SECURITY_WPA2;
            }
            else if (NULL != strstr((const char *)flag, "[WPA-PSK"))
            {
                ap_results[i].security = WIFI_SECURITY_WPA;
            }
            else if (NULL != strstr((const char *)flag, "[WPS]"))
            {
                ap_results[i].security = WIFI_SECURITY_UNDEFINED;
            }
            else
            {
                ap_results[i].security = WIFI_SECURITY_OPEN;
            }

            /* encryption */
            if (NULL != strstr((const char *)flag, "CCMP+TKIP"))
            {
                ap_results[i].encryption = WIFI_ENCRYPTION_TKIP_AES;
            }
            else if (NULL != strstr((const char *)flag, "CCMP"))
            {
                ap_results[i].encryption = WIFI_ENCRYPTION_AES;
            }
            else if (NULL != strstr((const char *)flag, "TKIP"))
            {
                ap_results[i].encryption = WIFI_ENCRYPTION_TKIP;
            }
            else
            {
                ap_results[i].encryption = WIFI_ENCRYPTION_UNDEFINED;
            }

            /* ssid */
            WIFI_LOG_INFO(("R_WIFI_DA16XXX_Scan: AP result:"));
            if (ap_results[i].ssid[0] == '\0')
            {
                ap_results[i].hidden = 1;
            }
            else
            {
                ap_results[i].hidden = 0;
                WIFI_LOG_INFO(("SSID: %s", ap_results[i].ssid));
            }
            WIFI_LOG_INFO(("BSSID: %X.%X.%X.%X.%X.%X", ap_results[i].bssid[0],
                                                       ap_results[i].bssid[1],
                                                       ap_results[i].bssid[2],
                                                       ap_results[i].bssid[3],
                                                       ap_results[i].bssid[4],
                                                       ap_results[i].bssid[5]));
            WIFI_LOG_INFO(("Security: %d", ap_results[i].security));
            WIFI_LOG_INFO(("Encryption: %d", ap_results[i].encryption));

            /* clear tmp variables */
            memset(flag, 0, 50);
            i++;
        }
    }

RETURN_ERROR:
    /* Give mutex */
    os_wrap_mutex_give(MUTEX_TX | MUTEX_RX);
    return api_ret;
}
/**********************************************************************************************************************
 * End of function R_WIFI_DA16XXX_Scan
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: R_WIFI_DA16XXX_Connect
 * Description  : Connect to Access Point.
 * Arguments    : ssid
 *                pass
 *                security
 *                enc_type
 * Return Value : WIFI_SUCCESS
 *                WIFI_ERR_PARAMETER
 *                WIFI_ERR_NOT_OPEN
 *                WIFI_ERR_MODULE_COM
 *                WIFI_ERR_NOT_CONNECT
 *                WIFI_ERR_TAKE_MUTEX
 *********************************************************************************************************************/
wifi_err_t R_WIFI_DA16XXX_Connect(const uint8_t * ssid, const uint8_t * pass,
                                  wifi_security_t security, wifi_encryption_t enc_type)
{
    uint8_t retry_count = 0;
    uint8_t retry_max = 3;
    wifi_err_t api_ret = WIFI_SUCCESS;
    uint8_t at_cmd[WIFI_CFG_MAX_SSID_LEN + WIFI_CFG_MAX_PASS_LEN + 20];

    /* Disconnected WiFi module? */
    if (MODULE_DISCONNECTED == wifi_system_state_get())
    {
        WIFI_LOG_ERROR(("R_WIFI_DA16XXX_Connect: R_WIFI_DA16XXX_Open is not called!"));
        return WIFI_ERR_NOT_OPEN;
    }

    /* Already connected access point? */
    if (0 == R_WIFI_DA16XXX_IsConnected())
    {
        WIFI_LOG_ERROR(("R_WIFI_DA16XXX_Connect: Already connected to access point!"));
        return WIFI_ERR_NOT_CONNECT;
    }

    /* Check ssid */
    if (NULL == ssid)
    {
        return WIFI_ERR_PARAMETER;
    }

    /* Connect to an open security AP */
    if (WIFI_SECURITY_OPEN == security)
    {
        snprintf((char *) at_cmd, sizeof(at_cmd), "AT+WFJAP='%s',%d\r", ssid, security);
    }
    else if (WIFI_SECURITY_WPA == security || WIFI_SECURITY_WPA2 == security)
    {
        snprintf((char *) at_cmd, sizeof(at_cmd), "AT+WFJAP='%s',%d,%d,'%s'\r", ssid, security, enc_type, pass);
    }
    else if (WIFI_SECURITY_WPA3 == security)
    {
        snprintf((char *) at_cmd, sizeof(at_cmd), "AT+WFJAPA='%s','%s'\r", ssid, pass);
    }
    else
    {
        WIFI_LOG_ERROR(("R_WIFI_DA16XXX_Connect: Unsupported security type (%d)!", security));
        api_ret = WIFI_ERR_PARAMETER;
        goto RETURN_ERROR;
    }

    /* Take mutex */
    if (OS_WRAP_SUCCESS != os_wrap_mutex_take(MUTEX_TX | MUTEX_RX))
    {
        return WIFI_ERR_TAKE_MUTEX;
    }

    /* Call to get WiFi connection does not always work the first time */
    at_set_timeout(30000);
    api_ret = WIFI_ERR_MODULE_COM;
    for (retry_count = 0; retry_count < retry_max; retry_count++)
    {
        if (AT_OK == at_exec_wo_mutex((char *) at_cmd))
        {
            if (AT_OK == at_exec_wo_mutex("AT+WFSTA\r"))
            {
                api_ret = WIFI_SUCCESS;
                break;
            }
        }
        os_wrap_sleep(1000, UNIT_MSEC);
    }
    at_set_timeout(ATCMD_RESP_TIMEOUT);

    /* flush SSID, password AP information */
    memset(at_cmd, 0, sizeof(at_cmd));

    if (api_ret == WIFI_SUCCESS)
    {
        if (DATA_FOUND == at_read("+WFSTA:1\r"))
        {
            /* Set WIFI State to "Connected access point" */
            wifi_system_state_set(MODULE_ACCESSPOINT);

            /* DHCP setting */
            if (AT_OK != at_exec_wo_mutex("AT+NWDHC=1\r"))
            {
                api_ret = WIFI_ERR_MODULE_COM;
                goto GIVE_MUTEX;
            }

            /* Give mutex */
            os_wrap_mutex_give(MUTEX_TX | MUTEX_RX);

            /* Get IP address */
            api_ret = R_WIFI_DA16XXX_GetIpAddress(&s_cur);
            return api_ret;
        }
        else
        {
            WIFI_LOG_ERROR(("R_WIFI_DA16XXX_Connect: Cannot connect to access point!"));
            api_ret = WIFI_ERR_NOT_CONNECT;
        }
    }

GIVE_MUTEX:
    /* Give mutex */
    os_wrap_mutex_give(MUTEX_TX | MUTEX_RX);

RETURN_ERROR:
    return api_ret;
}
/**********************************************************************************************************************
 * End of function R_WIFI_DA16XXX_Connect
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: R_WIFI_DA16XXX_Disconnect
 * Description  : Disconnect from Access Point.
 * Arguments    : none
 * Return Value : WIFI_SUCCESS
 *                WIFI_ERR_NOT_OPEN
 *                WIFI_ERR_MODULE_COM
 *********************************************************************************************************************/
wifi_err_t R_WIFI_DA16XXX_Disconnect(void)
{
    wifi_err_t api_ret = WIFI_SUCCESS;

    /* Disconnected WiFi module? */
    if (MODULE_DISCONNECTED == wifi_system_state_get())
    {
        WIFI_LOG_ERROR(("R_WIFI_DA16XXX_Disconnect: R_WIFI_DA16XXX_Open is not called!"));
        return WIFI_ERR_NOT_OPEN;
    }

    if (WIFI_SUCCESS != disconnect_ap_sub())
    {
        api_ret = WIFI_ERR_MODULE_COM;
        goto RETURN_ERROR;
    }

    /* Set WIFI State to "Connected WiFi module" */
    wifi_system_state_set(MODULE_CONNECTED);

RETURN_ERROR:
    return api_ret;
}
/**********************************************************************************************************************
 * End of function R_WIFI_DA16XXX_Disconnect
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: R_WIFI_DA16XXX_IsConnected
 * Description  : Check connected access point.
 * Arguments    : none
 * Return Value : 0  - connected
 *                -1 - disconnected
 *********************************************************************************************************************/
int32_t R_WIFI_DA16XXX_IsConnected(void)
{
    /* Connected access point? */
    if (MODULE_ACCESSPOINT == wifi_system_state_get())
    {
        return 0;
    }
    else
    {
        return (-1);
    }
}
/**********************************************************************************************************************
 * End of function R_WIFI_DA16XXX_IsConnected
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: R_WIFI_DA16XXX_DnsQuery
 * Description  : Execute DNS query.
 * Arguments    : domain_name
 *                ip_address
 * Return Value : WIFI_SUCCESS
 *                WIFI_ERR_PARAMETER
 *                WIFI_ERR_NOT_CONNECT
 *                WIFI_ERR_MODULE_COM
 *                WIFI_ERR_TAKE_MUTEX
 *********************************************************************************************************************/
wifi_err_t R_WIFI_DA16XXX_DnsQuery(uint8_t WIFI_FAR * domain_name, uint32_t * ip_address)
{
    wifi_err_t api_ret = WIFI_ERR_MODULE_COM;

    /* Connected access point? */
    if (0 != R_WIFI_DA16XXX_IsConnected())
    {
        WIFI_LOG_ERROR(("R_WIFI_DA16XXX_DnsQuery: Not connected to access point!"));
        return WIFI_ERR_NOT_CONNECT;
    }

    /* Check parameters */
    if ((NULL == domain_name) || (NULL == ip_address))
    {
        return WIFI_ERR_PARAMETER;
    }

    /* Take mutex */
    if (OS_WRAP_SUCCESS != os_wrap_mutex_take(MUTEX_TX | MUTEX_RX))
    {
        return WIFI_ERR_TAKE_MUTEX;
    }

    at_set_timeout(10000);
    if (AT_OK == at_exec_wo_mutex("AT+NWHOST=%s\r", domain_name))
    {
        if (DATA_FOUND == at_read("+NWHOST:%d.%d.%d.%d",
                                  &ip_address[0], &ip_address[1],
                                  &ip_address[2],  &ip_address[3]))
        {
            api_ret = WIFI_SUCCESS;
        }
    }
    at_set_timeout(ATCMD_RESP_TIMEOUT);

    /* Give mutex */
    os_wrap_mutex_give(MUTEX_TX | MUTEX_RX);

    return api_ret;
}
/**********************************************************************************************************************
 * End of function R_WIFI_DA16XXX_DnsQuery
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: R_WIFI_DA16XXX_SntpServerIpAddressSet
 * Description  : Set SNTP server IP address.
 * Arguments    : ip_address
 * Return Value : WIFI_SUCCESS
 *                WIFI_ERR_PARAMETER
 *                WIFI_ERR_NOT_OPEN
 *                WIFI_ERR_MODULE_COM
 *********************************************************************************************************************/
wifi_err_t R_WIFI_DA16XXX_SntpServerIpAddressSet (uint32_t * ip_address)
{
    wifi_err_t api_ret = WIFI_SUCCESS;

    /* Disconnected WiFi module? */
    if (MODULE_DISCONNECTED == wifi_system_state_get())
    {
        WIFI_LOG_ERROR(("R_WIFI_DA16XXX_SntpServerIpAddressSet: R_WIFI_DA16XXX_Open is not called!"));
        return WIFI_ERR_NOT_OPEN;
    }

    /* Check parameters */
    if (NULL == ip_address)
    {
        return WIFI_ERR_PARAMETER;
    }

    if (AT_OK != at_exec("AT+NWSNS=%d.%d.%d.%d\r", ip_address[0], ip_address[1], ip_address[2], ip_address[3]))
    {
        api_ret = WIFI_ERR_MODULE_COM;
    }

    return api_ret;
}
/**********************************************************************************************************************
 * End of function R_WIFI_DA16XXX_SntpServerIpAddressSet
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: R_WIFI_DA16XXX_SntpEnableSet
 * Description  : Enable or disable SNTP client service.
 * Arguments    : enable
 * Return Value : WIFI_SUCCESS
 *                WIFI_ERR_PARAMETER
 *                WIFI_ERR_NOT_OPEN
 *                WIFI_ERR_MODULE_COM
 *********************************************************************************************************************/
wifi_err_t R_WIFI_DA16XXX_SntpEnableSet (wifi_sntp_enable_t enable)
{
    wifi_err_t api_ret = WIFI_SUCCESS;

    /* Disconnected WiFi module? */
    if (MODULE_DISCONNECTED == wifi_system_state_get())
    {
        WIFI_LOG_ERROR(("R_WIFI_DA16XXX_SntpEnableSet: R_WIFI_DA16XXX_Open is not called!"));
        return WIFI_ERR_NOT_OPEN;
    }

    /* Check parameters */
    if (enable != WIFI_SNTP_ENABLE && enable != WIFI_SNTP_DISABLE)
    {
        return WIFI_ERR_PARAMETER;
    }

    if (AT_OK != at_exec("AT+NWSNTP=%d\r", enable))
    {
        api_ret = WIFI_ERR_MODULE_COM;
    }

    return api_ret;
}
/**********************************************************************************************************************
 * End of function R_WIFI_DA16XXX_SntpEnableSet
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: R_WIFI_DA16XXX_SntpTimeZoneSet
 * Description  : Set SNTP timezone.
 * Arguments    : utc_offset_in_hours
 * Return Value : WIFI_SUCCESS
 *                WIFI_ERR_PARAMETER
 *                WIFI_ERR_NOT_OPEN
 *                WIFI_ERR_MODULE_COM
 *********************************************************************************************************************/
wifi_err_t R_WIFI_DA16XXX_SntpTimeZoneSet (int8_t utc_offset_in_hours)
{
    wifi_err_t api_ret = WIFI_SUCCESS;

    /* Disconnected WiFi module? */
    if (MODULE_DISCONNECTED == wifi_system_state_get())
    {
        WIFI_LOG_ERROR(("R_WIFI_DA16XXX_SntpTimeZoneSet: R_WIFI_DA16XXX_Open is not called!"));
        return WIFI_ERR_NOT_OPEN;
    }

    /* Check parameters */
    if (utc_offset_in_hours < -12 || utc_offset_in_hours > 12)
    {
        return WIFI_ERR_PARAMETER;
    }

    if (AT_OK != at_exec("AT+TZONE=%d\r", utc_offset_in_hours * DA16XXX_HOURS_IN_SECONDS))
    {
        api_ret = WIFI_ERR_MODULE_COM;
    }

    return api_ret;
}
/**********************************************************************************************************************
 * End of function R_WIFI_DA16XXX_SntpTimeZoneSet
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: R_WIFI_DA16XXX_LocalTimeGet
 * Description  : Get the current local time based on current timezone in a string . Exp: YYYY-MM-DD,HOUR:MIN:SECS.
 * Arguments    : local_time
 *                size_string
 * Return Value : WIFI_SUCCESS
 *                WIFI_ERR_PARAMETER
 *                WIFI_ERR_NOT_OPEN
 *                WIFI_ERR_MODULE_COM
 *                WIFI_ERR_TAKE_MUTEX
 *********************************************************************************************************************/
wifi_err_t R_WIFI_DA16XXX_LocalTimeGet (uint8_t * local_time, uint8_t size_string)
{
    wifi_err_t api_ret = WIFI_ERR_MODULE_COM;

    /* Disconnected WiFi module? */
    if (MODULE_DISCONNECTED == wifi_system_state_get())
    {
        WIFI_LOG_ERROR(("R_WIFI_DA16XXX_LocalTimeGet: R_WIFI_DA16XXX_Open is not called!"));
        return WIFI_ERR_NOT_OPEN;
    }

    /* Check parameters */
    if (NULL == local_time || DA16XXX_LOCAL_TIME_STR_SIZE > size_string)
    {
        return WIFI_ERR_PARAMETER;
    }

    /* Take mutex */
    if (OS_WRAP_SUCCESS != os_wrap_mutex_take(MUTEX_TX | MUTEX_RX))
    {
        return WIFI_ERR_TAKE_MUTEX;
    }

    if (AT_OK == at_exec_wo_mutex("AT+TIME=?\r"))
    {
        if (DATA_FOUND == at_read("+TIME:%s", local_time))
        {
            api_ret = WIFI_SUCCESS;
        }
    }

    /* Give mutex */
    os_wrap_mutex_give(MUTEX_TX | MUTEX_RX);

    return api_ret;
}
/**********************************************************************************************************************
 * End of function R_WIFI_DA16XXX_LocalTimeGet
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: R_WIFI_DA16XXX_SetDnsServerAddress
 * Description  : Set DNS Server Address.
 * Arguments    : dns_address
 * Return Value : WIFI_SUCCESS
 *                WIFI_ERR_PARAMETER
 *                WIFI_ERR_NOT_CONNECT
 *                WIFI_ERR_MODULE_COM
 *********************************************************************************************************************/
wifi_err_t R_WIFI_DA16XXX_SetDnsServerAddress (uint8_t * dns_address)
{
    wifi_err_t api_ret = WIFI_SUCCESS;

    /* Connected access point? */
    if (0 != R_WIFI_DA16XXX_IsConnected())
    {
        WIFI_LOG_ERROR(("R_WIFI_DA16XXX_SetDnsServerAddress: Not connected to access point!"));
        return WIFI_ERR_NOT_CONNECT;
    }

    /* Check parameters */
    if (NULL == dns_address)
    {
        return WIFI_ERR_PARAMETER;
    }


    if (AT_OK != at_exec("AT+NWDNS=%d.%d.%d.%d\r", dns_address[0], dns_address[1], dns_address[2], dns_address[3]))
    {
        api_ret = WIFI_ERR_MODULE_COM;
    }

    return api_ret;
}
/**********************************************************************************************************************
 * End of function R_WIFI_DA16XXX_SetDnsServerAddress
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: R_WIFI_DA16XXX_GetMacAddress
 * Description  : Get MAC Address.
 * Arguments    : mac_address.
 * Return Value : WIFI_SUCCESS
 *                WIFI_ERR_PARAMETER
 *                WIFI_ERR_NOT_CONNECT
 *                WIFI_ERR_MODULE_COM
 *                WIFI_ERR_TAKE_MUTEX
 *********************************************************************************************************************/
wifi_err_t R_WIFI_DA16XXX_GetMacAddress (uint32_t * mac_address)
{
    wifi_err_t api_ret = WIFI_ERR_MODULE_COM;

    /* Connected access point? */
    if (0 != R_WIFI_DA16XXX_IsConnected())
    {
        WIFI_LOG_ERROR(("R_WIFI_DA16XXX_GetMacAddress: Not connected to access point!"));
        return WIFI_ERR_NOT_CONNECT;
    }

    /* Check parameters */
    if (NULL == mac_address)
    {
        return WIFI_ERR_PARAMETER;
    }

    /* Take mutex */
    if (OS_WRAP_SUCCESS != os_wrap_mutex_take(MUTEX_TX | MUTEX_RX))
    {
        return WIFI_ERR_TAKE_MUTEX;
    }

    if (AT_OK == at_exec_wo_mutex("AT+WFMAC=?\r"))
    {
        if (DATA_FOUND == at_read("+WFMAC:%X:%X:%X:%X:%X:%X",
                                  &mac_address[0], &mac_address[1], &mac_address[2],
                                  &mac_address[3], &mac_address[4], &mac_address[5]))
        {
            api_ret = WIFI_SUCCESS;
        }
    }

    /* Give mutex */
    os_wrap_mutex_give(MUTEX_TX | MUTEX_RX);

    return api_ret;
}
/**********************************************************************************************************************
 * End of function R_WIFI_DA16XXX_GetMacAddress
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: R_WIFI_DA16XXX_GetIpAddress
 * Description  : Get IP Address.
 * Arguments    : ip_config.
 * Return Value : WIFI_SUCCESS
 *                WIFI_ERR_PARAMETER
 *                WIFI_ERR_NOT_CONNECT
 *                WIFI_ERR_MODULE_COM
 *                WIFI_ERR_TAKE_MUTEX
 *********************************************************************************************************************/
wifi_err_t R_WIFI_DA16XXX_GetIpAddress (wifi_ip_configuration_t * ip_config)
{
    wifi_err_t api_ret = WIFI_ERR_MODULE_COM;

    /* Connected access point? */
    if (0 != R_WIFI_DA16XXX_IsConnected())
    {
        WIFI_LOG_ERROR(("R_WIFI_DA16XXX_GetIpAddress: Not connected to access point!"));
        return WIFI_ERR_NOT_CONNECT;
    }

    /* Check parameters */
    if (NULL == ip_config)
    {
        return WIFI_ERR_PARAMETER;
    }

    /* Take mutex */
    if (OS_WRAP_SUCCESS != os_wrap_mutex_take(MUTEX_TX | MUTEX_RX))
    {
        return WIFI_ERR_TAKE_MUTEX;
    }

    if (AT_OK == at_exec_wo_mutex("AT+NWIP=?\r"))
    {
        if (DATA_FOUND == at_read(
                "+NWIP:0,%d.%d.%d.%d,%d.%d.%d.%d,%d.%d.%d.%d",
                &ip_config->ipaddress[0], &ip_config->ipaddress[1],
                &ip_config->ipaddress[2], &ip_config->ipaddress[3],
                &ip_config->subnetmask[0], &ip_config->subnetmask[1],
                &ip_config->subnetmask[2], &ip_config->subnetmask[3],
                &ip_config->gateway[0], &ip_config->gateway[1],
                &ip_config->gateway[2], &ip_config->gateway[3]
            )
        )
        {
            api_ret = WIFI_SUCCESS;
        }
    }

    /* Give mutex */
    os_wrap_mutex_give(MUTEX_TX | MUTEX_RX);

    return api_ret;
}
/**********************************************************************************************************************
 * End of function R_WIFI_DA16XXX_GetIpAddress
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: R_WIFI_DA16XXX_HardwareReset
 * Description  : Reset the module.
 * Arguments    : none
 * Return Value : WIFI_SUCCESS
 *                WIFI_ERR_SERIAL_OPEN
 *                WIFI_ERR_MODULE_COM
 *                WIFI_ERR_NOT_CONNECT
 *                WIFI_ERR_BYTEQ_OPEN
 *                WIFI_ERR_TAKE_MUTEX
 *                WIFI_ERR_SOCKET_CREATE
 *********************************************************************************************************************/
wifi_err_t R_WIFI_DA16XXX_HardwareReset (void)
{
    uint8_t retry_count;
    uint8_t retry_max = 30;

    /* Disconnected WiFi module? */
    if (MODULE_DISCONNECTED == wifi_system_state_get())
    {
        WIFI_LOG_ERROR(("R_WIFI_DA16XXX_HardwareReset: R_WIFI_DA16XXX_Open is not called!"));
        return WIFI_ERR_NOT_OPEN;
    }

    /* Take mutex */
    if (OS_WRAP_SUCCESS != os_wrap_mutex_take(MUTEX_TX))
    {
        WIFI_LOG_ERROR(("R_WIFI_DA16XXX_HardwareReset: Failed to take mutex!"));
        return WIFI_ERR_TAKE_MUTEX;
    }

    /* Set WIFI State to "Connected WiFi module" */
    wifi_system_state_set(MODULE_CONNECTED);

    /* Close UART port */
    uart_port_close();

    /* Reset WIFI module */
    da16xxx_hw_reset();

    /* Open UART port */
    if (ATCMD_OK != uart_port_open())
    {
        WIFI_LOG_ERROR(("R_WIFI_DA16XXX_HardwareReset: Failed to open SCI port!"));
        os_wrap_mutex_give(MUTEX_TX);
        return WIFI_ERR_SERIAL_OPEN;
    }

#if defined(__CCRX__) || defined(__ICCRX__) || defined(__RX__)
    /* Set flow control for serial port */
    flow_ctrl_init();
#endif

    /* Give mutex */
    os_wrap_mutex_give(MUTEX_TX);

#if WIFI_CFG_DEBUG_LOG == LOG_DEBUG
    /* Enable echo back */
    at_exec("ATE\r");
#endif

    // check if R_WIFI_DA16XXX_Connect() is not called
    if (s_cur.ipaddress[0] == 0)
    {
        return WIFI_SUCCESS;
    }
    else
    {
        /* Reconnect AP */
        at_exec("AT+WFCAP\r");
    }

    WIFI_LOG_DEBUG(("Connecting to AccessPoint... \r\n"));
    for (retry_count = 0; retry_count < retry_max; retry_count++)
    {
        if (MODULE_ACCESSPOINT == wifi_system_state_get())
        {
            break;
        }
        else
        {
            os_wrap_sleep(1000, UNIT_MSEC);
        }
    }

    if (retry_count == retry_max)
    {
        WIFI_LOG_ERROR(("Cannot connect to AccessPoint. \r\n"));
        return WIFI_ERR_NOT_CONNECT;
    }

    /* Get IP address */
    R_WIFI_DA16XXX_GetIpAddress(&s_cur);
    WIFI_LOG_INFO(("Connected to AccessPoint\r\n"));

#if WIFI_CFG_TCP_SUPPORT == 1
    /* reconnect sockets */
    R_WIFI_DA16XXX_TcpReconnect(UINT8_MAX);
#endif

#if WIFI_CFG_TLS_SUPPORT == 1
    /* reconnect secure sockets */
    R_WIFI_DA16XXX_TlsReconnect(UINT8_MAX);
#endif

    return WIFI_SUCCESS;
}
/**********************************************************************************************************************
 * End of function R_WIFI_DA16XXX_HardwareReset
 *********************************************************************************************************************/

/******************************************************************************************************************//**
 * Function Name: R_WIFI_DA16XXX_GetVersion
 * Description  : Get the Wi-Fi module version.
 * Arguments    : none
 * Return Value : The version of Wi-Fi module. The version number is encoded such that the top 2 bytes are the major
 *                version number and the bottom 2 bytes are the minor version number.
 **********************************************************************************************************************/
uint32_t R_WIFI_DA16XXX_GetVersion(void)
{
    uint32_t version = (((uint32_t) WIFI_VERSION_MAJOR) << 16) | (uint32_t) WIFI_VERSION_MINOR;

    return version;
}
/**********************************************************************************************************************
 * End of function R_WIFI_DA16XXX_GetVersion
 *********************************************************************************************************************/

/*
 * System state control
 */
/**********************************************************************************************************************
 * Function Name: wifi_system_state_set
 * Description  : Set WIFI system state.
 * Arguments    : state
 * Return Value : none
 *********************************************************************************************************************/
static void wifi_system_state_set(e_wifi_module_status_t state)
{
    s_wifi_system_state = state;
}
/**********************************************************************************************************************
 * End of function wifi_system_state_set
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: wifi_system_state_get
 * Description  : Get WiFi system state.
 * Arguments    : none
 * Return Value : wifi_system_status_t WIFI system state
 *********************************************************************************************************************/
static e_wifi_module_status_t wifi_system_state_get(void)
{
    return s_wifi_system_state;
}
/**********************************************************************************************************************
 * End of function wifi_system_state_get
 *********************************************************************************************************************/

/*
 * Sub functions for API
 */
/**********************************************************************************************************************
 * Function Name: disconnect_ap_sub
 * Description  : Disconnect access point (sub function).
 * Arguments    : none
 * Return Value : WIFI_SUCCESS
 *                WIFI_ERR_MODULE_COM
 *********************************************************************************************************************/
static wifi_err_t disconnect_ap_sub(void)
{
    /* Not connected access point? */
    if (0 != R_WIFI_DA16XXX_IsConnected())
    {
        return WIFI_SUCCESS;
    }

    /* Disconnect from currently connected Access Point */
    if (AT_OK != at_exec("AT+WFQAP\r"))
    {
        return WIFI_ERR_MODULE_COM;
    }

    return WIFI_SUCCESS;
}
/**********************************************************************************************************************
 * End of function disconnect_ap_sub
 *********************************************************************************************************************/

/*
 * WIFI module control
 */
/**********************************************************************************************************************
 * Function Name: da16xxx_hw_reset
 * Description  : Reset DA16XXX.
 * Arguments    : none
 * Return Value : none
 *********************************************************************************************************************/
static void da16xxx_hw_reset(void)
{
    /* WIFI Module hardware reset   */
#if defined(__CCRX__) || defined(__ICCRX__) || defined(__RX__)
    WIFI_RESET_DDR(WIFI_CFG_RESET_PORT, WIFI_CFG_RESET_PIN) = 1;  /* output */
#endif
    WIFI_RESET_DR(WIFI_CFG_RESET_PORT, WIFI_CFG_RESET_PIN)  = 0;  /* low    */
#if (WIFI_CFG_DA16600_SUPPORT)
    os_wrap_swdelay(500, UNIT_MSEC);
#else
    os_wrap_swdelay(30, UNIT_MSEC);
#endif
    WIFI_RESET_DR(WIFI_CFG_RESET_PORT, WIFI_CFG_RESET_PIN)  = 1;  /* high   */
    os_wrap_swdelay(250, UNIT_MSEC);
}
/**********************************************************************************************************************
 * End of function da16xxx_hw_reset
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: da16xxx_close
 * Description  : Close WIFI module.
 * Arguments    : none
 * Return Value : none
 *********************************************************************************************************************/
static void da16xxx_close(void)
{
    uart_port_close();
    os_wrap_mutex_delete();
    wifi_system_state_set(MODULE_DISCONNECTED);
}
/**********************************************************************************************************************
 * End of function da16xxx_close
 *********************************************************************************************************************/

#if WIFI_CFG_SNTP_ENABLE == 1
/**********************************************************************************************************************
 * Function Name: da16xxx_sntp_service_init
 * Description  : Initialize SNTP client service.
 * Arguments    : none
 * Return Value : WIFI_SUCCESS
 *                WIFI_ERR_PARAMETER
 *                WIFI_ERR_MODULE_COM
 *                WIFI_ERR_TAKE_MUTEX
 *********************************************************************************************************************/
static wifi_err_t da16xxx_sntp_service_init(void)
{
    wifi_err_t api_ret = WIFI_SUCCESS;
    uint32_t    ip_address_sntp_server[4];

    /* Set the SNTP server IP address */
    if (4 != sscanf(WIFI_CFG_SNTP_SERVER_IP, "%d.%d.%d.%d",
                    &ip_address_sntp_server[0], &ip_address_sntp_server[1],
                    &ip_address_sntp_server[2], &ip_address_sntp_server[3]))
    {
        WIFI_LOG_ERROR(("da16xxx_sntp_service_init: Wrong format of SNTP server IP address!"));
        return WIFI_ERR_PARAMETER;
    }

    /* Configure the SNTP Server Address */
    api_ret = R_WIFI_DA16XXX_SntpServerIpAddressSet(ip_address_sntp_server);
    if (api_ret != WIFI_SUCCESS)
    {
        return api_ret;
    }

    /* Enable the SNTP client */
    api_ret = R_WIFI_DA16XXX_SntpEnableSet((wifi_sntp_enable_t) WIFI_CFG_SNTP_ENABLE);
    if (api_ret != WIFI_SUCCESS)
    {
        return api_ret;
    }

    /* Set the SNTP timezone configuration string */
    api_ret = R_WIFI_DA16XXX_SntpTimeZoneSet(WIFI_CFG_SNTP_UTC_OFFSET);
    if (api_ret != WIFI_SUCCESS)
    {
        return api_ret;
    }

    return api_ret;
}
/**********************************************************************************************************************
 * End of function da16xxx_sntp_service_init
 *********************************************************************************************************************/
#endif

/*
 * Callback functions
 */
/**********************************************************************************************************************
 * Function Name: da16xxx_handle_incoming_common_data
 * Description  : Handles incoming common data.
 * Arguments    : type
 *                state
 *                data
 * Return Value : none
 *********************************************************************************************************************/
void da16xxx_handle_incoming_common_data(wifi_resp_type_t *type, wifi_recv_state_t *state, uint8_t data)
{
    // check if R_WIFI_DA16XXX_Connect() is not called
    if (s_cur.ipaddress[0] == 0)
    {
        *state = WIFI_RECV_PREFIX;
    }
    else if (*type == WIFI_RESP_WFDAP)
    {
        s_wifi_system_state = MODULE_CONNECTED;
        *state = WIFI_RECV_PREFIX;
    }

    // checking +WFJAP status
    switch (*state)
    {
        case WIFI_RECV_PARAM_CID:
        {
            if (g_rx_idx == 0)
            {
                g_rx_buff = data;
                g_rx_idx++;
            }
            else if (',' == data)
            {
                if ('1' == g_rx_buff)
                {
                    s_wifi_system_state = MODULE_ACCESSPOINT;
                }
                g_rx_idx = 0;
                *type = WIFI_RESP_NONE;
                *state = WIFI_RECV_PREFIX;
            }
            else
            {
                g_rx_idx = 0;
                *type = WIFI_RESP_NONE;
                *state = WIFI_RECV_PREFIX;
            }
            break;
        }

        default:
        {
            *type = WIFI_RESP_NONE;
            *state = WIFI_RECV_PREFIX;
            break;
        }
    }
}
/**********************************************************************************************************************
 * End of function da16xxx_handle_incoming_common_data
 *********************************************************************************************************************/

/*
 * Logging functions
 */
#if WIFI_CFG_LOGGING_OPTION >= 2
/**********************************************************************************************************************
 * Function Name: s_vsnprintf_safe
 * Description  : vsnprintf.
 * Arguments    : s - string buffer
 *                n - length
 *                format - format message
 *                varg
 * Return Value : None
 *********************************************************************************************************************/
static int16_t s_vsnprintf_safe(char * s,
                                size_t n,
                                const char * format,
                                va_list arg)
{
    int16_t ret;

    ret = vsnprintf(s, n, format, arg);

    /* Check if the string was truncated and if so, update the return value
     * to reflect the number of characters actually written. */
    if( ret >= (int16_t) n )
    {
        /* Do not include the terminating NULL character to keep the behaviour
         * same as the standard. */
        ret = n - 1;
    }
    else if( ret < 0 )
    {
        /* Encoding error - Return 0 to indicate that nothing was written to the
         * buffer. */
        ret = 0;
    }
    else
    {
        /* Complete string was written to the buffer. */
    }

    return ret;
}
/**********************************************************************************************************************
 * End of function s_vsnprintf_safe
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: s_snprintf_safe
 * Description  : snprintf.
 * Arguments    : s - string buffer
 *                n - length
 *                format - format message
 * Return Value : None
 *********************************************************************************************************************/
static int16_t s_snprintf_safe(char * s,
                               size_t n,
                               const char * format,
                               ... )
{
    int16_t ret;
    va_list args = {0};

    va_start( args, format );
    ret = s_vsnprintf_safe( s, n, format, args );
    va_end( args );

    return ret;
}
/**********************************************************************************************************************
 * End of function s_snprintf_safe
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: s_log_printf_common
 * Description  : Print common log without freeRTOS logging module.
 * Arguments    : usLoggingLevel - log level
 *                pcFormat - format message
 *                varg
 * Return Value : None
 *********************************************************************************************************************/
static void s_log_printf_common(uint8_t usLoggingLevel,
                                const char WIFI_FAR * pcFormat,
                                va_list args)
{
    size_t xLength = 0;
    char pcPrintString[WIFI_LOGGING_MAX_MESSAGE_LENGTH];

    if (NULL != pcPrintString)
    {
        const char * pcLevelString = NULL;
        size_t ulFormatLen = 0UL;

        /* Choose the string for the log level meta data for the log message. */
        switch (usLoggingLevel)
        {
            case LOG_ERROR:
                pcLevelString = "ERROR";
                break;

            case LOG_WARN:
                pcLevelString = "WARN";
                break;

            case LOG_INFO:
                pcLevelString = "INFO";
                break;

            case LOG_DEBUG:
                pcLevelString = "DEBUG";
                break;

            default:
                pcLevelString = NULL;
                break;
        }

        /* Add the chosen log level information as prefix for the message. */
        if( ( pcLevelString != NULL ) && ( xLength < WIFI_LOGGING_MAX_MESSAGE_LENGTH ) )
        {
            xLength += s_snprintf_safe( pcPrintString + xLength,
                                        WIFI_LOGGING_MAX_MESSAGE_LENGTH - xLength, "[%s] ", pcLevelString );
        }

        if( xLength < WIFI_LOGGING_MAX_MESSAGE_LENGTH )
        {
            xLength += s_vsnprintf_safe( pcPrintString + xLength,
                                         WIFI_LOGGING_MAX_MESSAGE_LENGTH - xLength, pcFormat, args );
        }

        /* Add newline characters if the message does not end with them.*/
        ulFormatLen = strlen( (char WIFI_FAR *)pcFormat );

        if( ( ulFormatLen >= 2 ) &&
            ( strncmp( (char WIFI_FAR *)(pcFormat + ulFormatLen), "\r\n", 2 ) != 0 ) &&
            ( xLength < WIFI_LOGGING_MAX_MESSAGE_LENGTH ) )
        {
            xLength += s_snprintf_safe(pcPrintString + xLength,
                                       WIFI_LOGGING_MAX_MESSAGE_LENGTH - xLength, "%s", "\r\n" );
        }

        pcPrintString[xLength] = '\0';

        /* Only send the buffer to the logging task if it is
         * not empty. */
        if (xLength > 0)
        {
#if WIFI_CFG_LOGGING_OPTION == 3
            printf ("%s", pcPrintString);
#elif WIFI_CFG_LOGGING_OPTION == 2
            s_uart_string_printf(pcPrintString);
#endif
        }
    }
}
/**********************************************************************************************************************
 * End of function s_log_printf_common
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: printf_log_error
 * Description  : Print error log without freeRTOS logging module.
 * Arguments    : format - format message
 * Return Value : None
 *********************************************************************************************************************/
void printf_log_error (const char WIFI_FAR * format, ...)
{
    va_list args = {0};

    va_start(args, format);
    s_log_printf_common(LOG_ERROR, format, args);

    va_end(args);
}
/**********************************************************************************************************************
 * End of function printf_log_error
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: printf_log_warn
 * Description  : Print warning log without freeRTOS logging module.
 * Arguments    : format - format message
 * Return Value : None
 *********************************************************************************************************************/
void printf_log_warn (const char WIFI_FAR * format, ...)
{
    va_list args = {0};

    va_start(args, format);
    s_log_printf_common(LOG_WARN, format, args);

    va_end(args);
}
/**********************************************************************************************************************
 * End of function printf_log_warn
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: printf_log_info
 * Description  : Print information log without freeRTOS logging module.
 * Arguments    : format - format message
 * Return Value : None
 *********************************************************************************************************************/
void printf_log_info (const char WIFI_FAR * format, ...)
{
    va_list args = {0};

    va_start(args, format);
    s_log_printf_common(LOG_INFO, format, args);

    va_end(args);
}
/**********************************************************************************************************************
 * End of function printf_log_info
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: printf_log_debug
 * Description  : Print debug (AT command) log without freeRTOS logging module.
 * Arguments    : format - format message
 * Return Value : None
 *********************************************************************************************************************/
void printf_log_debug (const char WIFI_FAR * format, ...)
{
    va_list args = {0};

    va_start(args, format);
    s_log_printf_common(LOG_DEBUG, format, args);

    va_end(args);
}
/**********************************************************************************************************************
 * End of function printf_log_debug
 *********************************************************************************************************************/
#endif

#if WIFI_CFG_LOGGING_OPTION == 2
/**********************************************************************************************************************
 * Function Name: s_uart_config
 * Description  : Peripheral UART operation for log output.
 * Arguments    : none
 * Return Value : none
 *********************************************************************************************************************/
static void s_uart_config(void)
{
    sci_cfg_t sci_config;
#if defined(__CCRX__) || defined(__ICCRX__) || defined(__RX__)
    U_SCI_UART_CLI_PINSET();
#endif
    sci_config.async.baud_rate    = WIFI_CFG_SCI_UART_TERMINAL_BAUDRATE;
    sci_config.async.clk_src      = SCI_CLK_INT;
    sci_config.async.data_size    = SCI_DATA_8BIT;
    sci_config.async.parity_en    = SCI_PARITY_OFF;
    sci_config.async.parity_type  = SCI_EVEN_PARITY;
    sci_config.async.stop_bits    = SCI_STOPBITS_1;
    sci_config.async.int_priority = WIFI_CFG_SCI_UART_INTERRUPT_PRIORITY;

    if (SCI_SUCCESS != R_SCI_Open(WIFI_CFG_LOG_TERM_CHANNEL, SCI_MODE_ASYNC, &sci_config, NULL, &g_sci_handle))
    {
        R_BSP_NOP();
    }
}
/**********************************************************************************************************************
 * End of function s_uart_config
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: s_uart_string_printf
 * Description  : Sends the specified character string on the set UART channel.
 * Arguments    : pString-
 *                  Pointer type log output character string.
 * Return Value : none
 *********************************************************************************************************************/
static void s_uart_string_printf(char WIFI_FAR * pcString)
{
    sci_err_t sci_error = SCI_SUCCESS;
    uint16_t retry = 0xffff;
    uint16_t str_length = (uint16_t) strlen(pcString);
    uint16_t transmit_length;

    while ((retry > 0) && (str_length > 0))
    {

        R_SCI_Control(g_sci_handle, SCI_CMD_TX_Q_BYTES_FREE, &transmit_length);

        if (transmit_length > str_length)
        {
            transmit_length = str_length;
        }

        sci_error = R_SCI_Send(g_sci_handle, (uint8_t WIFI_FAR *) pcString, transmit_length);

        if ((SCI_ERR_XCVR_BUSY == sci_error) || (SCI_ERR_INSUFFICIENT_SPACE == sci_error))
        {
            /* retry if previous transmission still in progress or tx buffer is insufficient. */
            retry--;
        }
        else
        {
            str_length -= transmit_length;
            pcString += transmit_length;
        }
    }

    if (SCI_SUCCESS != sci_error)
    {
        R_BSP_NOP();
    }
}
/**********************************************************************************************************************
 * End of function s_uart_string_printf
 *********************************************************************************************************************/
#endif
