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
 * File Name    : r_wifi_da16xxx_tcp_tls.c
 * Description  : TLS client API functions definition for DA16XXX.
 *********************************************************************************************************************/

/**********************************************************************************************************************
 Includes   <System Includes> , "Project Includes"
 *********************************************************************************************************************/
#include "r_wifi_da16xxx_private.h"

/**********************************************************************************************************************
 Macro definitions
 *********************************************************************************************************************/
#if WIFI_CFG_TLS_SUPPORT == 1
#define TLS_TBL_MAX     (WIFI_CFG_TLS_CREATABLE_SOCKETS)            // Socket table
#define TLS_BUF_MAX     (WIFI_CFG_TLS_SOCKET_RECEIVE_BUFFER_SIZE)   // Socket buffer

/**********************************************************************************************************************
 Local Typedef definitions
 *********************************************************************************************************************/
/* Socket table information */
typedef struct st_sock_tbl
{
    uint32_t                ipaddr[4];
    uint16_t                port;
    wifi_socket_status_t    status;
    uint8_t                 ipver;
    wifi_socket_type_t      type;
    byteq_hdl_t             byteq_hdl;
    uint16_t                put_err_cnt;
    uint8_t                 recv_buf[TLS_BUF_MAX];
    int16_t                 recv_len;
    st_wifi_timer           timer_tx;
    st_wifi_timer           timer_rx;
} st_sock_tbl_t;

/**********************************************************************************************************************
 Exported global variables
 *********************************************************************************************************************/
static st_sock_tbl_t g_sock_tbl[TLS_TBL_MAX];
static uint8_t g_sock_list[TLS_TBL_MAX];
static volatile uint8_t g_cur_sock_idx = UINT8_MAX;
static volatile uint8_t g_rx_idx = 0;

/**********************************************************************************************************************
 Private (static) variables and functions
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: R_WIFI_DA16XXX_GetAvailableTlsSocket
 * Description  : Get the next available socket ID.
 * Arguments    : socket_id
 * Return Value : WIFI_SUCCESS
 *                WIFI_ERR_PARAMETER
 *                WIFI_ERR_NOT_CONNECT
 *                WIFI_ERR_SOCKET_NUM
 *********************************************************************************************************************/
wifi_err_t R_WIFI_DA16XXX_GetAvailableTlsSocket (uint32_t * socket_id)
{
    uint8_t i = 0;

    /* Connected access point? */
    if (0 != R_WIFI_DA16XXX_IsConnected())
    {
        WIFI_LOG_ERROR(("R_WIFI_DA16XXX_GetAvailableTlsSocket: Not connected to access point!"));
        return WIFI_ERR_NOT_CONNECT;
    }

    /* Check parameters */
    if (NULL == socket_id)
    {
        return WIFI_ERR_PARAMETER;
    }

    for (i = 0; i < TLS_TBL_MAX; i++)
    {
        if (g_sock_tbl[i].status == WIFI_SOCKET_STATUS_CLOSED)
        {
            *socket_id = i;
            return WIFI_SUCCESS;
        }
    }

    *socket_id = UINT8_MAX;
    return WIFI_ERR_SOCKET_NUM;
}
/**********************************************************************************************************************
 * End of function R_WIFI_DA16XXX_GetAvailableTlsSocket
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: R_WIFI_DA16XXX_GetTlsSocketStatus
 * Description  : Get the socket status.
 * Arguments    : socket_number
 *                socket_status
 * Return Value : WIFI_SUCCESS
 *                WIFI_ERR_PARAMETER
 *                WIFI_ERR_NOT_OPEN
 *                WIFI_ERR_SOCKET_NUM
 *********************************************************************************************************************/
wifi_err_t R_WIFI_DA16XXX_GetTlsSocketStatus (uint32_t socket_number, wifi_socket_status_t *socket_status)
{
    /* Disconnected WiFi module? */
    if (0 != R_WIFI_DA16XXX_IsOpened())
    {
        WIFI_LOG_ERROR(("R_WIFI_DA16XXX_GetTlsSocketStatus: R_WIFI_DA16XXX_Open is not called!"));
        return WIFI_ERR_NOT_OPEN;
    }

    /* Check parameters */
    if (NULL == socket_status)
    {
        return WIFI_ERR_PARAMETER;
    }

    if (socket_number >= TLS_TBL_MAX)
    {
        return WIFI_ERR_SOCKET_NUM;
    }

    *socket_status = g_sock_tbl[socket_number].status;

    return WIFI_SUCCESS;
}
/**********************************************************************************************************************
 * End of function R_WIFI_DA16XXX_GetTlsSocketStatus
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: R_WIFI_DA16XXX_CreateTlsSocket
 * Description  : Create a new socket instance.
 * Arguments    : socket_number
 *                type
 *                ip_version
 * Return Value : WIFI_SUCCESS
 *                WIFI_ERR_PARAMETER
 *                WIFI_ERR_NOT_CONNECT
 *                WIFI_ERR_SOCKET_CREATE
 *********************************************************************************************************************/
wifi_err_t R_WIFI_DA16XXX_CreateTlsSocket (uint32_t socket_number, wifi_socket_type_t type, uint8_t ip_version)
{
    static bool socket_init = false;
    wifi_err_t ret = WIFI_ERR_SOCKET_CREATE;

    /* Connected access point? */
    if (0 != R_WIFI_DA16XXX_IsConnected())
    {
        WIFI_LOG_ERROR(("R_WIFI_DA16XXX_CreateTlsSocket: Not connected to access point!"));
        return WIFI_ERR_NOT_CONNECT;
    }

    /* Check parameters */
    if (socket_number >= TLS_TBL_MAX || WIFI_SOCKET_TYPE_TLS != type)
    {
        return WIFI_ERR_PARAMETER;
    }

    /* init socket table */
    if (socket_init == false)
    {
        memset(g_sock_list, UINT8_MAX, sizeof(g_sock_list));
        memset(g_sock_tbl, 0, sizeof(g_sock_tbl));
        socket_init = true;
    }

    if (WIFI_SOCKET_STATUS_CLOSED == g_sock_tbl[socket_number].status)
    {
        g_sock_tbl[socket_number].status = WIFI_SOCKET_STATUS_SOCKET;    /* socket status   */
        g_sock_tbl[socket_number].ipver = ip_version;                       /* ip_version      */
        g_sock_tbl[socket_number].type = type;                              /* type            */
        g_sock_tbl[socket_number].put_err_cnt = 0;
        if (BYTEQ_SUCCESS == R_BYTEQ_Open(g_sock_tbl[socket_number].recv_buf,
                                          TLS_BUF_MAX,
                                          &g_sock_tbl[socket_number].byteq_hdl))
        {
            WIFI_LOG_INFO(("R_WIFI_DA16XXX_CreateTlsSocket: Creating socket %d!", socket_number));
            ret = WIFI_SUCCESS;
        }
        else
        {
            WIFI_LOG_ERROR(("R_WIFI_DA16XXX_CreateTlsSocket: Cannot open BYTEQ for socket %d!", socket_number));
            ret = WIFI_ERR_BYTEQ_OPEN;
        }
    }
    else
    {
        WIFI_LOG_WARN(("R_WIFI_DA16XXX_CreateTlsSocket: socket %d has already created!", socket_number));
    }

    return ret;
}
/**********************************************************************************************************************
 * End of function R_WIFI_DA16XXX_CreateTlsSocket
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: R_WIFI_DA16XXX_TlsConnect
 * Description  : Connect to a specific IP and Port using socket.
 * Arguments    : socket_number
 *                ip_address
 *                port
 * Return Value : WIFI_SUCCESS
 *                WIFI_ERR_PARAMETER
 *                WIFI_ERR_MODULE_COM
 *                WIFI_ERR_NOT_CONNECT
 *                WIFI_ERR_SOCKET_NUM
 *                WIFI_ERR_TAKE_MUTEX
 *********************************************************************************************************************/
wifi_err_t R_WIFI_DA16XXX_TlsConnect (uint32_t socket_number, uint32_t WIFI_FAR * ip_address, uint16_t port)
{
    wifi_err_t  api_ret = WIFI_ERR_MODULE_COM;

    /* Connected access point? */
    if (0 != R_WIFI_DA16XXX_IsConnected())
    {
        WIFI_LOG_ERROR(("R_WIFI_DA16XXX_TlsConnect: Not connected to access point!"));
        return WIFI_ERR_NOT_CONNECT;
    }

    /* Check parameters */
    if (socket_number >= TLS_TBL_MAX || NULL == ip_address || 0 == port)
    {
        return WIFI_ERR_PARAMETER;
    }

    /* socket created? */
    if (WIFI_SOCKET_STATUS_SOCKET != g_sock_tbl[socket_number].status)
    {
        WIFI_LOG_ERROR(("R_WIFI_DA16XXX_TlsConnect: socket %d is not created!", socket_number));
        return WIFI_ERR_SOCKET_NUM;
    }

    at_set_timeout(DA16XXX_AT_TLS_CON_TIMEOUT);

    if (AT_OK != at_exec("AT+TRSSLCO=%d,%d.%d.%d.%d,%d\r",
                         socket_number,ip_address[0], ip_address[1], ip_address[2], ip_address[3], port))
    {
        WIFI_LOG_ERROR(("R_WIFI_DA16XXX_TlsConnect: cannot connect to SSL server!"));
        api_ret = WIFI_ERR_MODULE_COM;
        goto RETURN_ERROR;
    }
    memcpy(g_sock_tbl[socket_number].ipaddr, ip_address, sizeof(g_sock_tbl[socket_number].ipaddr));
    g_sock_tbl[socket_number].port = port;
    g_sock_tbl[socket_number].status = WIFI_SOCKET_STATUS_CONNECTED;
    g_sock_list[socket_number] = socket_number;
    api_ret = WIFI_SUCCESS;
    WIFI_LOG_INFO(("R_WIFI_DA16XXX_TlsConnect: connected socket %d to SSL server.", socket_number));

RETURN_ERROR:
    at_set_timeout(ATCMD_RESP_TIMEOUT);
    return api_ret;
}
/**********************************************************************************************************************
 * End of function R_WIFI_DA16XXX_TcpTlsConnect
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: R_WIFI_DA16XXX_SendTlsSocket
 * Description  : Send data on connecting socket.
 * Arguments    : socket_number
 *                data
 *                length
 *                timeout_ms
 * Return Value : Positive number - number of sent data
 *                WIFI_ERR_PARAMETER
 *                WIFI_ERR_MODULE_COM
 *                WIFI_ERR_MODULE_TIMEOUT
 *                WIFI_ERR_NOT_CONNECT
 *                WIFI_ERR_SOCKET_NUM
 *                WIFI_ERR_TAKE_MUTEX
 *********************************************************************************************************************/
int16_t R_WIFI_DA16XXX_SendTlsSocket (uint32_t socket_number, uint8_t WIFI_FAR * data,
                                      uint16_t length, uint32_t timeout_ms)
{
    uint16_t    send_idx = 0;
    uint8_t     send_data[DA16XXX_AT_CMD_BUF_MAX] = {0};
    uint16_t    send_length;
    int16_t     api_ret = WIFI_SUCCESS;
    e_atcmd_err_t at_ret = ATCMD_OK;
    e_rslt_code_t rslt_ret = AT_OK;
    OS_TICK     tick_tmp;
    uint16_t    tx_length;

    /* Connect access point? */
    if (0 != R_WIFI_DA16XXX_IsConnected())
    {
        WIFI_LOG_ERROR(("R_WIFI_DA16XXX_SendTlsSocket: Not connected to access point!"));
        return WIFI_ERR_NOT_CONNECT;
    }

    /* Check parameters */
    if ((socket_number >= TLS_TBL_MAX) || (NULL == data))
    {
        return WIFI_ERR_PARAMETER;
    }

    /* Not connect? */
    if (WIFI_SOCKET_STATUS_CONNECTED != g_sock_tbl[socket_number].status)
    {
        WIFI_LOG_ERROR(("R_WIFI_DA16XXX_SendTlsSocket: socket #%d is not connected!", socket_number));
        return WIFI_ERR_SOCKET_NUM;
    }

    /* Take mutex */
    if (OS_WRAP_SUCCESS != os_wrap_mutex_take(MUTEX_TX))
    {
        return WIFI_ERR_TAKE_MUTEX;
    }

    /* Set timeout */
    at_set_timeout(timeout_ms);
    tick_tmp = os_wrap_tickcount_get();
    while (send_idx < length)
    {
        /* get length of data sent */
        if (length - send_idx > (uint32_t)(DA16XXX_AT_SOCK_TX_MAX))
        {
            tx_length = (uint32_t)(DA16XXX_AT_SOCK_TX_MAX);
        }
        else
        {
            tx_length = length - send_idx;
        }
        /* get prefix length */
        send_length = snprintf((char WIFI_FAR *) send_data,
                               DA16XXX_AT_CMD_BUF_MAX,
                               "AT+TRSSLWR=%d,%d.%d.%d.%d,%lu,r,%d,",
                               socket_number,
                               g_sock_tbl[socket_number].ipaddr[0],
                               g_sock_tbl[socket_number].ipaddr[1],
                               g_sock_tbl[socket_number].ipaddr[2],
                               g_sock_tbl[socket_number].ipaddr[3],
                               g_sock_tbl[socket_number].port,
                               (int) tx_length );

        WIFI_LOG_DEBUG(("SendTlsSocket: %s", send_data));
        at_ret = at_send_raw((uint8_t WIFI_FAR *) send_data, (uint32_t) send_length);
        if (ATCMD_ERR_TIMEOUT == at_ret)
        {
            WIFI_LOG_ERROR(("SendTlsSocket: at_send_raw() timed out (ret=%ld)!", at_ret));
            api_ret = (int32_t) WIFI_ERR_MODULE_TIMEOUT;
            break;
        }
        else if (ATCMD_OK != at_ret)
        {
            WIFI_LOG_ERROR(("SendTlsSocket: at_send_raw() failed (ret=%ld)!", at_ret));
            api_ret = (int32_t) WIFI_ERR_MODULE_COM;
            break;
        }

        at_ret = at_send_raw((uint8_t WIFI_FAR *) data + send_idx, tx_length);
        if (ATCMD_ERR_TIMEOUT == at_ret)
        {
            WIFI_LOG_ERROR(("SendTlsSocket: at_send_raw() timed out (ret=%ld)!", at_ret));
            api_ret = (int32_t) WIFI_ERR_MODULE_TIMEOUT;
            break;
        }
        else if (ATCMD_OK != at_ret)
        {
            WIFI_LOG_ERROR(("SendTlsSocket: at_send_raw() failed (ret=%ld)!", at_ret));
            api_ret = (int32_t) WIFI_ERR_MODULE_COM;
            break;
        }

        rslt_ret = at_recv();
        if (AT_OK != rslt_ret)
        {
            if (AT_UNKNOWN == rslt_ret)
            {
                WIFI_LOG_ERROR(("SendTlsSocket: socket %d is disconnected (ret=%ld)!", socket_number, rslt_ret));
                g_sock_tbl[socket_number].put_err_cnt = 0;
                g_sock_tbl[socket_number].status = WIFI_SOCKET_STATUS_SOCKET;
                api_ret = (int32_t) WIFI_ERR_SOCKET_NUM;
            }
            else if (AT_INTERNAL_TIMEOUT == rslt_ret)
            {
                WIFI_LOG_ERROR(("SendTlsSocket: at_recv() timed out (ret=%ld)!", rslt_ret));
                api_ret = (int32_t) WIFI_ERR_MODULE_TIMEOUT;
            }
            else
            {
                WIFI_LOG_ERROR(("SendTlsSocket: at_recv() failed (ret=%ld)!", rslt_ret));
                api_ret = (int32_t) WIFI_ERR_MODULE_COM;
            }
            break;
        }

        send_idx += tx_length;
    }
    if (api_ret >= WIFI_SUCCESS)
    {
        api_ret = send_idx;
    }

    at_set_timeout(ATCMD_RESP_TIMEOUT);
    os_wrap_mutex_give(MUTEX_TX);
    tick_tmp = os_wrap_tickcount_get() - tick_tmp;
    WIFI_LOG_INFO(("R_WIFI_DA16XXX_SendTlsSocket: socket %d ret=%ld (%lu).", socket_number, api_ret, tick_tmp));
    return api_ret;
}
/**********************************************************************************************************************
 * End of function R_WIFI_DA16XXX_SendTlsSocket
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: R_WIFI_DA16XXX_ReceiveTlsSocket
 * Description  : Receive data on connecting socket.
 * Arguments    : socket_number
 *                data
 *                length
 *                timeout_ms
 * Return Value : Positive number - number of received data
 *                WIFI_ERR_PARAMETER
 *                WIFI_ERR_NOT_CONNECT
 *                WIFI_ERR_SOCKET_NUM
 *********************************************************************************************************************/
int16_t R_WIFI_DA16XXX_ReceiveTlsSocket (uint32_t socket_number, uint8_t WIFI_FAR * data,
                                         uint16_t length, uint32_t timeout_ms)
{
    int16_t     api_ret = WIFI_SUCCESS;
    uint16_t    recv_cnt = 0;
    byteq_err_t byteq_ret;
    OS_TICK     tick_tmp = 0;

    /* Connect access point? */
    if (0 != R_WIFI_DA16XXX_IsConnected())
    {
        WIFI_LOG_ERROR(("R_WIFI_DA16XXX_ReceiveTlsSocket: Not connected to access point!"));
        return WIFI_ERR_NOT_CONNECT;
    }

    /* Check parameters */
    if ((socket_number >= TLS_TBL_MAX) || (NULL == data))
    {
        return WIFI_ERR_PARAMETER;
    }

    g_sock_tbl[socket_number].timer_rx.threshold = OS_WRAP_MS_TO_TICKS(timeout_ms);
    if (0 < timeout_ms)
    {
        g_sock_tbl[socket_number].timer_rx.tick_sta = os_wrap_tickcount_get();
    }
    while (recv_cnt < length)
    {
        R_BSP_InterruptsDisable();
        byteq_ret = R_BYTEQ_Get(g_sock_tbl[socket_number].byteq_hdl, (uint8_t *) (data + recv_cnt));
        R_BSP_InterruptsEnable();

        if (BYTEQ_SUCCESS == byteq_ret)
        {
            recv_cnt++;
            continue;
        }

        /* timeout? */
        if (0 < timeout_ms)
        {
            tick_tmp = os_wrap_tickcount_get() - g_sock_tbl[socket_number].timer_rx.tick_sta;
            if (g_sock_tbl[socket_number].timer_rx.threshold <= tick_tmp)
            {
                WIFI_LOG_WARN(("R_WIFI_DA16XXX_ReceiveTlsSocket: timeout!"));
                break;
            }
        }
        else if (0 == timeout_ms)
        {
            /* return immediately when no data in buffer */
            break;
        }
        os_wrap_sleep(10, UNIT_MSEC);
    }

    /* Not connect? */
    if ((recv_cnt == 0) && (WIFI_SOCKET_STATUS_CONNECTED != g_sock_tbl[socket_number].status))
    {
        WIFI_LOG_ERROR(("R_WIFI_DA16XXX_ReceiveTlsSocket: socket %d is not connected!", socket_number));
        return WIFI_ERR_SOCKET_NUM;
    }

    WIFI_LOG_INFO(("R_WIFI_DA16XXX_ReceiveTlsSocket: socket %d recv_cnt=%ld (%lu).", socket_number, recv_cnt, tick_tmp));
    api_ret = recv_cnt;
    return api_ret;
}
/**********************************************************************************************************************
 * End of function R_WIFI_DA16XXX_ReceiveTlsSocket
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: R_WIFI_DA16XXX_CloseTlsSocket
 * Description  : Disconnect a specific socket connection.
 * Arguments    : socket_number
 * Return Value : WIFI_SUCCESS
 *                WIFI_ERR_NOT_OPEN
 *                WIFI_ERR_MODULE_COM
 *                WIFI_ERR_MODULE_TIMEOUT
 *                WIFI_ERR_SOCKET_NUM
 *********************************************************************************************************************/
wifi_err_t R_WIFI_DA16XXX_CloseTlsSocket (uint32_t socket_number)
{
    wifi_err_t api_ret = WIFI_SUCCESS;
    e_rslt_code_t at_rep = AT_OK;

    /* Check parameters */
    if (socket_number >= TLS_TBL_MAX)
    {
        return WIFI_ERR_SOCKET_NUM;
    }

    /* Disconnected WiFi module? */
    if (0 != R_WIFI_DA16XXX_IsOpened())
    {
        return WIFI_ERR_NOT_OPEN;
    }

    /* Connected access point? */
    if (0 != R_WIFI_DA16XXX_IsConnected())
    {
        goto CLOSE_SOCKET;
    }

    at_set_timeout(5000);
    at_rep = at_exec("AT+TRSSLCL=%d\r", socket_number);
    if (AT_INTERNAL_TIMEOUT == at_rep)
    {
        api_ret = WIFI_ERR_MODULE_TIMEOUT;
        goto RETURN_ERROR;
    }
    else if (-762 == at_rep) //No assigned CID of TLS session
    {
        goto RETURN_ERROR;
    }
    else if (AT_OK != at_rep)
    {
        api_ret = WIFI_ERR_MODULE_COM;
        goto RETURN_ERROR;
    }

CLOSE_SOCKET:
    R_BYTEQ_Close(g_sock_tbl[socket_number].byteq_hdl);
    g_sock_tbl[socket_number].put_err_cnt = 0;
    g_sock_tbl[socket_number].status = WIFI_SOCKET_STATUS_CLOSED;
    g_sock_list[socket_number] = UINT8_MAX;
    WIFI_LOG_INFO(("R_WIFI_DA16XXX_CloseTlsSocket: socket %d is closed!", socket_number));

RETURN_ERROR:
    at_set_timeout(ATCMD_RESP_TIMEOUT);
    return api_ret;
}
/**********************************************************************************************************************
 * End of function R_WIFI_DA16XXX_CloseTlsSocket
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: R_WIFI_DA16XXX_TlsReconnect
 * Description  : Reconnect TLS socket.
 * Arguments    : socket_number - UINT8_MAX for reconnect all disconnected sockets.
 * Return Value : WIFI_SUCCESS
 *                WIFI_ERR_PARAMETER
 *                WIFI_ERR_MODULE_COM
 *                WIFI_ERR_NOT_CONNECT
 *                WIFI_ERR_SOCKET_NUM
 *                WIFI_ERR_TAKE_MUTEX
 *********************************************************************************************************************/
wifi_err_t R_WIFI_DA16XXX_TlsReconnect (uint32_t socket_number)
{
    uint32_t cid;

    if (UINT8_MAX == socket_number)
    {
        for (cid = 0; cid < TLS_TBL_MAX; cid++)
        {
            if (UINT8_MAX != g_sock_list[cid])
            {
                R_WIFI_DA16XXX_TlsConnect(cid, g_sock_tbl[cid].ipaddr, g_sock_tbl[cid].port);
            }
        }
        return WIFI_SUCCESS;
    }
    else
    {
        return R_WIFI_DA16XXX_TlsConnect(socket_number,
                                         g_sock_tbl[socket_number].ipaddr,
                                         g_sock_tbl[socket_number].port);
    }
}
/**********************************************************************************************************************
 * End of function R_WIFI_DA16XXX_TlsReconnect
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: R_WIFI_DA16XXX_ConfigTlsSocket
 * Description  : Configure SSL Connection on WiFi module.
 * Arguments    : socket_num
 *                cert_info
 *                trans_buf_size
 *                recv_buf_size
 *                timeout
 * Return Value : WIFI_SUCCESS
 *                WIFI_ERR_PARAMETER
 *                WIFI_ERR_NOT_CONNECT
 *                WIFI_ERR_MODULE_COM
 *********************************************************************************************************************/
wifi_err_t R_WIFI_DA16XXX_ConfigTlsSocket(uint32_t * socket_num,
                                          wifi_tls_cert_info_t * cert_info,
                                          uint8_t WIFI_FAR * sni_name,
                                          uint8_t ser_valid,
                                          uint16_t trans_buf_size,
                                          uint16_t recv_buf_size,
                                          uint32_t timeout)
{
    wifi_err_t api_ret = WIFI_SUCCESS;

    /* Connected access point? */
    if (0 != R_WIFI_DA16XXX_IsConnected())
    {
        WIFI_LOG_ERROR(("R_WIFI_DA16200_RequestTlsSocket: Not connected to access point!"));
        return WIFI_ERR_NOT_CONNECT;
    }

    /* Check parameters */
    if ((0 == trans_buf_size) || (0 == recv_buf_size) || (NULL == socket_num))
    {
        return WIFI_ERR_PARAMETER;
    }

    /* Initialize the SSL module. */
    if (AT_OK == at_exec("AT+TRSSLINIT=1\r"))
    {
        if (DATA_FOUND == at_read("+TRSSLINIT:%d\r", socket_num))
        {
            WIFI_LOG_INFO(("R_WIFI_DA16XXX_RequestTlsSocket: Initialize the SSL module. Socket Section %d.", *socket_num));
        }
        else
        {
            return WIFI_ERR_MODULE_COM;
        }
    }
    else
    {
        return WIFI_ERR_MODULE_COM;
    }

    /* Configure SSL connection */
    /* set SSL Certificate */
    if (AT_OK != at_exec("AT+TRSSLCFG=%d,2,%s\r", *socket_num, cert_info->cert_ca))
    {
    	return WIFI_ERR_MODULE_COM;
    }

    if (AT_OK != at_exec("AT+TRSSLCFG=%d,3,%s\r", *socket_num, cert_info->cert_name))
    {
    	return WIFI_ERR_MODULE_COM;
    }

    /* set the SNI (supported only for TLS client) */
    if (NULL != sni_name)
    {
        if (AT_OK != at_exec("AT+TRSSLCFG=%d,6,%s\r", *socket_num, (char WIFI_FAR *) sni_name))
        {
            return WIFI_ERR_MODULE_COM;
        }
    }

    /* enable server validation */
    if (AT_OK != at_exec("AT+TRSSLCFG=%d,9,%d\r", *socket_num, ser_valid))
    {
        return WIFI_ERR_MODULE_COM;
    }

    /* set the Incoming buffer length */
    if (AT_OK != at_exec("AT+TRSSLCFG=%d,10,%lu\r", *socket_num, recv_buf_size))
    {
        return WIFI_ERR_MODULE_COM;
    }

    /* set the outgoing buffer length */
    if (AT_OK != at_exec("AT+TRSSLCFG=%d,11,%lu\r", *socket_num, trans_buf_size))
    {
        return WIFI_ERR_MODULE_COM;
    }

    /* set the DA TLS connection timeout */
    if (0 != timeout)
    {
        if (AT_OK != at_exec("AT+TRSSLCFG=%d,12,%lu\r", *socket_num, timeout))
        {
            return WIFI_ERR_MODULE_COM;
        }
    }

    return api_ret;
}
/**********************************************************************************************************************
 * End of function R_WIFI_DA16XXX_ConfigTlsSocket
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: R_WIFI_DA16XXX_RegistServerCertificate
 * Description  : Register server certificate on WiFi module.
 * Arguments    : socket_num
 *                cert_info
 *                trans_buf_size
 *                recv_buf_size
 * Return Value : WIFI_SUCCESS
 *                WIFI_ERR_PARAMETER
 *                WIFI_ERR_NOT_OPEN
 *                WIFI_ERR_MODULE_COM
 *********************************************************************************************************************/
wifi_err_t R_WIFI_DA16XXX_RegistServerCertificate(uint32_t socket_num,
                                                  wifi_tls_cert_info_t * cert_info,
                                                  uint8_t WIFI_FAR * sni_name,
                                                  uint8_t ser_valid,
                                                  uint16_t trans_buf_size,
                                                  uint16_t recv_buf_size)
{
    wifi_err_t api_ret = WIFI_SUCCESS;

    /* Disconnected WiFi module? */
    if (0 != R_WIFI_DA16XXX_IsOpened())
    {
        WIFI_LOG_ERROR(("R_WIFI_DA16XXX_RegistServerCertificate: R_WIFI_DA16XXX_Open is not called!"));
        return WIFI_ERR_NOT_OPEN;
    }

    /* Check parameters */
    if ((0 == trans_buf_size) || (0 == recv_buf_size))
    {
        return WIFI_ERR_PARAMETER;
    }

    if (socket_num >= TLS_TBL_MAX)
    {
        return WIFI_ERR_SOCKET_NUM;
    }

    /* Configure SSL connection */
    /* set SSL Certificate */
    if (AT_OK != at_exec("AT+TRSSLCFG=%d,2,%s\r", socket_num, cert_info->cert_ca))
    {
    	return WIFI_ERR_MODULE_COM;
    }

    if (AT_OK != at_exec("AT+TRSSLCFG=%d,3,%s\r", socket_num, cert_info->cert_name))
    {
    	return WIFI_ERR_MODULE_COM;
    }

    /* set the SNI (supported only for TLS client) */
    if (NULL != sni_name)
    {
        if (AT_OK != at_exec("AT+TRSSLCFG=%d,6,%s\r", socket_num, (char WIFI_FAR *) sni_name))
        {
            return WIFI_ERR_MODULE_COM;
        }
    }

    /* enable server validation */
    if (AT_OK != at_exec("AT+TRSSLCFG=%d,9,%d\r", socket_num, ser_valid))
    {
        return WIFI_ERR_MODULE_COM;
    }

    /* set the Incoming buffer length */
    if (AT_OK != at_exec("AT+TRSSLCFG=%d,10,%ld\r", socket_num, recv_buf_size))
    {
        return WIFI_ERR_MODULE_COM;
    }

    /* set the outgoing buffer length */
    if (AT_OK != at_exec("AT+TRSSLCFG=%d,11,%ld\r", socket_num, trans_buf_size))
    {
        return WIFI_ERR_MODULE_COM;
    }

    return api_ret;
}
/**********************************************************************************************************************
 * End of function R_WIFI_DA16XXX_RegistServerCertificate
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: R_WIFI_DA16XXX_RequestTlsSocket
 * Description  : Request TLS socket communication.
 * Arguments    : socket_number
 * Return Value : WIFI_SUCCESS
 *                WIFI_ERR_NOT_CONNECT
 *                WIFI_ERR_SOCKET_CREATE
 *********************************************************************************************************************/
wifi_err_t R_WIFI_DA16XXX_RequestTlsSocket(uint32_t socket_number)
{
    /* Connected access point? */
    if (0 != R_WIFI_DA16XXX_IsConnected())
    {
        WIFI_LOG_ERROR(("R_WIFI_DA16200_RequestTlsSocket: Not connected to access point!"));
        return WIFI_ERR_NOT_CONNECT;
    }

    /* Initialize the SSL module. */
    switch (socket_number)
    {
        case 0:
            if (AT_OK == at_exec("AT+TRSSLINIT=1\r"))
            {
                if (DATA_FOUND == at_read("+TRSSLINIT:0\r"))
                {
                    WIFI_LOG_INFO(("R_WIFI_DA16XXX_RequestTlsSocket: Initialize the SSL module. Socket Section 0."));
                    return WIFI_SUCCESS;
                }
            }
            break;
        case 1:
            if (AT_OK == at_exec("AT+TRSSLINIT=1\r"))
            {
                if (DATA_FOUND == at_read("+TRSSLINIT:0\r"))
                {
                    WIFI_LOG_INFO(("R_WIFI_DA16XXX_RequestTlsSocket: Initialize the SSL module. Socket Section 0."));
                }
            }

            if (AT_OK == at_exec("AT+TRSSLINIT=1\r"))
            {
                if (DATA_FOUND == at_read("+TRSSLINIT:1\r"))
                {
                    WIFI_LOG_INFO(("R_WIFI_DA16XXX_RequestTlsSocket: Initialize the SSL module. Socket Section 1."));
                    return WIFI_SUCCESS;
                }
            }
            break;
    }

    return WIFI_ERR_SOCKET_CREATE;
}
/**********************************************************************************************************************
 * End of function R_WIFI_DA16XXX_RequestTlsSocket
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: R_WIFI_DA16XXX_GetServerCertificate
 * Description  : Get stored server certificate on WiFi module.
 * Arguments    : cert_info
 * Return Value : WIFI_SUCCESS
 *                WIFI_ERR_PARAMETER
 *                WIFI_ERR_NOT_OPEN
 *                WIFI_ERR_MODULE_COM
 *********************************************************************************************************************/
wifi_err_t R_WIFI_DA16XXX_GetServerCertificate(wifi_tls_cert_info_t * cert_info)
{
    wifi_err_t api_ret = WIFI_SUCCESS;
    uint8_t certificate_key_tmp[WIFI_CFG_TLS_CERT_MAX_NAME] = {0};

    /* Disconnected WiFi module? */
    if (0 != R_WIFI_DA16XXX_IsOpened())
    {
        WIFI_LOG_ERROR(("R_WIFI_DA16XXX_GetServerCertificate: R_WIFI_DA16XXX_Open is not called!"));
        return WIFI_ERR_NOT_OPEN;
    }

    /* Check parameters */
    if (NULL == cert_info)
    {
        return WIFI_ERR_PARAMETER;
    }

    at_set_timeout(8000);

    /* CA Certificate */
#if WIFI_CFG_TLS_USE_CA_CERT
    if (AT_OK == at_exec("AT+TRSSLCERTLIST=0\r"))
    {
        if (DATA_NOT_FOUND == at_read("+TRSSLCERTLIST:0,%s\r", cert_info->cert_ca))
        {
            memset(cert_info->cert_ca, 0, sizeof(cert_info->cert_ca));
        }
    }
    else
    {
        return WIFI_ERR_MODULE_COM;
    }
#else
    if (AT_OK == at_exec("AT+TRSSLCERTLIST=0\r"))
    {
        if (DATA_FOUND == at_read("+TRSSLCERTLIST:\r"))
        {
            memset(cert_info->cert_ca, 0, sizeof(cert_info->cert_ca));
        }
    }
    else
    {
        return WIFI_ERR_MODULE_COM;
    }
#endif

    /* Get Client/Server Certificate */
    if (AT_OK == at_exec("AT+TRSSLCERTLIST=1\r"))
    {
        if ((DATA_NOT_FOUND == at_read("+TRSSLCERTLIST:%*d,%[^,],%*d,%s",
                                   cert_info->cert_name, certificate_key_tmp)))
        {
            memset(cert_info->cert_name, 0, sizeof(cert_info->cert_name));
        }
    }
    else
    {
        return WIFI_ERR_MODULE_COM;
    }

    return api_ret;
}
/**********************************************************************************************************************
 * End of function R_WIFI_DA16XXX_GetServerCertificate
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: R_WIFI_DA16XXX_WriteCertificate
 * Description  : Write certificate on WiFi module.
 * Arguments    : name
 *                type_key
 *                p_data
 *                len
 * Return Value : WIFI_SUCCESS
 *                WIFI_ERR_PARAMETER
 *                WIFI_ERR_NOT_OPEN
 *                WIFI_ERR_MODULE_COM
 *                WIFI_ERR_TAKE_MUTEX
 *********************************************************************************************************************/
wifi_err_t R_WIFI_DA16XXX_WriteCertificate(const uint8_t WIFI_FAR * name,
                                           wifi_tls_key_type_t type_key,
                                           const uint8_t WIFI_FAR * p_data,
                                           uint16_t len)
{
    wifi_err_t api_ret = WIFI_SUCCESS;
    uint8_t send_data[TEMP_BUF_MAX] = {0};

    /* Disconnected WiFi module? */
    if (0 != R_WIFI_DA16XXX_IsOpened())
    {
        WIFI_LOG_ERROR(("R_WIFI_DA16XXX_GetServerCertificate: R_WIFI_DA16XXX_Open is not called!"));
        return WIFI_ERR_NOT_OPEN;
    }

    /* Check parameters */
    if (type_key >= 3)
    {
        return WIFI_ERR_PARAMETER;
    }

    /* take mutex */
    if (OS_WRAP_SUCCESS != os_wrap_mutex_take(MUTEX_TX | MUTEX_RX))
    {
        return WIFI_ERR_TAKE_MUTEX;
    }

    switch (type_key) {
        case WIFI_TLS_TYPE_CA_CERT:
            snprintf((char WIFI_FAR *) send_data, DA16XXX_AT_CMD_BUF_MAX, "AT+TRSSLCERTSTORE=0,0,1,%s,", name);
            at_send((char WIFI_FAR *) send_data);

            at_send_raw((uint8_t WIFI_FAR *) p_data, len);

            /* Send Indication of the end of content  */
            snprintf((char WIFI_FAR *) send_data, DA16XXX_AT_CMD_BUF_MAX, "%s", DA16XXX_CERT_END);
            at_send((char WIFI_FAR *) send_data);

            if (AT_OK != at_recv())
            {
                os_wrap_mutex_give(MUTEX_TX | MUTEX_RX);
                return WIFI_ERR_MODULE_COM;
            }
            break;

        case WIFI_TLS_TYPE_CLIENT_CERT:
            snprintf((char WIFI_FAR *) send_data, DA16XXX_AT_CMD_BUF_MAX, "AT+TRSSLCERTSTORE=1,1,1,%s,", name);
            at_send((char WIFI_FAR *) send_data);

            at_send_raw((uint8_t WIFI_FAR *) p_data, len);

            /* Send Indication of the end of content  */
            snprintf((char WIFI_FAR *) send_data, DA16XXX_AT_CMD_BUF_MAX, "%s", DA16XXX_CERT_END);
            at_send((char WIFI_FAR *) send_data);

            if (AT_OK != at_recv())
            {
                os_wrap_mutex_give(MUTEX_TX | MUTEX_RX);
                return WIFI_ERR_MODULE_COM;
            }
            break;

        case WIFI_TLS_TYPE_CLIENT_PRIVATE_KEY:
            snprintf((char WIFI_FAR *) send_data, DA16XXX_AT_CMD_BUF_MAX, "AT+TRSSLCERTSTORE=1,2,1,%s,", name);
            at_send((char WIFI_FAR *) send_data);

            at_send_raw((uint8_t WIFI_FAR *) p_data, len);

            /* Send Indication of the end of content  */
            snprintf((char WIFI_FAR *) send_data, DA16XXX_AT_CMD_BUF_MAX, "%s", DA16XXX_CERT_END);
            at_send((char WIFI_FAR *) send_data);

            if (AT_OK != at_recv())
            {
                os_wrap_mutex_give(MUTEX_TX | MUTEX_RX);
                return WIFI_ERR_MODULE_COM;
            }
            break;
    }

    /* give mutex */
    os_wrap_mutex_give(MUTEX_TX | MUTEX_RX);

    return api_ret;
}
/**********************************************************************************************************************
 * End of function R_WIFI_DA16XXX_WriteCertificate
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: R_WIFI_DA16XXX_DeleteCertificate
 * Description  : Delete certificate on WiFi module.
 * Arguments    : type_key
 *                cert_info
 * Return Value : WIFI_SUCCESS
 *                WIFI_ERR_PARAMETER
 *                WIFI_ERR_NOT_OPEN
 *                WIFI_ERR_MODULE_COM
 *********************************************************************************************************************/
wifi_err_t R_WIFI_DA16XXX_DeleteCertificate(wifi_tls_key_type_t type_key,
                                            wifi_tls_cert_info_t * cert_info)
{
    wifi_err_t api_ret = WIFI_SUCCESS;

    /* Disconnected WiFi module? */
    if (0 != R_WIFI_DA16XXX_IsOpened())
    {
        WIFI_LOG_ERROR(("R_WIFI_DA16XXX_GetServerCertificate: R_WIFI_DA16XXX_Open is not called!"));
        return WIFI_ERR_NOT_OPEN;
    }

    /* Check parameters */
    if (type_key >= 3)
    {
        return WIFI_ERR_PARAMETER;
    }

    if (cert_info == NULL)
    {
        return WIFI_ERR_PARAMETER;
    }

    switch (type_key) {
        case WIFI_TLS_TYPE_CLIENT_CERT:
        case WIFI_TLS_TYPE_CLIENT_PRIVATE_KEY:
            if (AT_OK != at_exec("AT+TRSSLCERTDELETE=1,%s\r", cert_info->cert_name))
            {
                api_ret = WIFI_ERR_MODULE_COM;
            }
            break;

        case WIFI_TLS_TYPE_CA_CERT:
            if (AT_OK != at_exec("AT+TRSSLCERTDELETE=0,%s\r", cert_info->cert_ca))
            {
                api_ret = WIFI_ERR_MODULE_COM;
            }
            break;
    }

    return api_ret;
}
/**********************************************************************************************************************
 * End of function R_WIFI_DA16XXX_DeleteCertificate
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: da16xxx_handle_incoming_secure_socket_data
 * Description  : Handles incoming TLS socket data.
 * Arguments    : type
 *                state
 *                data
 * Return Value : none
 *********************************************************************************************************************/
void da16xxx_handle_incoming_secure_socket_data(wifi_resp_type_t *type, wifi_recv_state_t *state, uint8_t data)
{
    static uint8_t cid = 0;

    switch (*state)
    {
        case WIFI_RECV_PARAM_CID:
        {
            if (',' == data)
            {
                if (WIFI_RESP_TRSSLXTC == *type)
                {
                    g_cur_sock_idx = g_sock_list[cid];
                    g_sock_tbl[g_cur_sock_idx].put_err_cnt = 0;
                    g_sock_tbl[g_cur_sock_idx].status = WIFI_SOCKET_STATUS_CLOSED;
                    *type = WIFI_RESP_NONE;
                    *state = WIFI_RECV_PREFIX;
                }
                else if (WIFI_RESP_TRSSLDTC == *type)
                {
                    g_cur_sock_idx = g_sock_list[cid];
                    *state = WIFI_RECV_PARAM_IP;
                }
                cid = 0;
            }
            else if (data >= '0' && data <= '9')
            {
                cid = cid * 10 + (data - '0');
            }
            else
            {
                cid = 0;
                *type = WIFI_RESP_NONE;
                *state = WIFI_RECV_PREFIX;
            }
            break;
        }
        case WIFI_RECV_PARAM_IP:
        {
            if (',' == data)
            {
                *state = WIFI_RECV_PARAM_PORT;
            }
            break;
        }
        case WIFI_RECV_PARAM_PORT:
        {
            if (',' == data)
            {
                *state = WIFI_RECV_PARAM_LEN;
                g_rx_idx = 0;
            }
            break;
        }
        case WIFI_RECV_PARAM_LEN:
        {
            if (',' == data)
            {
                *state = WIFI_RECV_DATA;
            }
            else if (data >= '0' && data <= '9')
            {
                g_sock_tbl[g_cur_sock_idx].recv_len = g_sock_tbl[g_cur_sock_idx].recv_len * 10 + (data - '0');
            }
            break;
        }

        case WIFI_RECV_DATA:
        {
            if (0 < g_sock_tbl[g_cur_sock_idx].recv_len--)
            {
                if (BYTEQ_SUCCESS != R_BYTEQ_Put(g_sock_tbl[g_cur_sock_idx].byteq_hdl, data))
                {
                    g_sock_tbl[g_cur_sock_idx].put_err_cnt++;
                    post_err_event(WIFI_EVENT_SOCKET_RXQ_OVF_ERR, g_cur_sock_idx);
                }
            }

            if (0 >= g_sock_tbl[g_cur_sock_idx].recv_len)
            {
                g_rx_idx = 0;
                g_cur_sock_idx = UINT8_MAX;
                *type = WIFI_RESP_NONE;
                *state = WIFI_RECV_PREFIX;
            }
            break;
        }

        default:
        {
            break;
        }
    }
}
/**********************************************************************************************************************
 * End of function da16xxx_handle_incoming_secure_socket_data
 *********************************************************************************************************************/
#endif /* WIFI_CFG_TLS_SUPPORT */
