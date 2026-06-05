/*
* Copyright (c) 2025 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/
/***********************************************************************************************************************
*  File Name    : r_wifi_da16xxx_ota.c
*  Description  : Wi-Fi DA16XXX ota functions
***********************************************************************************************************************/

/**********************************************************************************************************************
 Includes   "Project Includes"
 *********************************************************************************************************************/
#include "r_wifi_da16xxx_private.h"

/**********************************************************************************************************************
 Macro definitions
 *********************************************************************************************************************/
#if WIFI_CFG_OTA_SUPPORT == 1
#if WIFI_CFG_OTA_TLS_AUTH != 0
#include WIFI_CFG_OTA_CERTS_HEADER

#define OTA_CERT_INCLUDE
#endif /* WIFI_CFG_OTA_TLS_AUTH */

#define RESP_HEX_MAX   (9)

/**********************************************************************************************************************
 Local Typedef definitions
 *********************************************************************************************************************/
/* OTA table information */
typedef struct st_da16xxx_ota
{
    /* String representing a trusted server root certificate. */
    const char * p_root_ca;

    /* Flag to track OTA download state. */
    wifi_ota_state_t ota_state;

    /* Flag to track OTA service status */
    da16xxx_ota_status_t open;

    /* HTTP TLS Authentication level.*/
    wifi_http_tls_auth_t tls_level;

    /* Size associated with root CA Certificate. */
    uint16_t root_ca_size;

    /* Flag to track OTA message status. */
    bool     ota_msg_ended_status;
    bool     ota_receive_status;
    uint32_t ota_fw_rcv_cnt;
    int32_t ota_fw_len;

    /* Status receive buffer */
    uint8_t ota_fw_resp_status[RESP_HEX_MAX];

    /* Command receive buffer. */
    uint8_t ota_fw_buf[WIFI_CFG_OTA_BLK_SIZE];
} st_da16xxx_ota_t;

/**********************************************************************************************************************
 Exported global variables
 *********************************************************************************************************************/
st_da16xxx_ota_t g_ota_tbl;

/**********************************************************************************************************************
 Private (static) variables and functions
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: R_WIFI_DA16XXX_OtaOpen
 * Description  : Initialize the DA16XXX on-chip OTA service.
 * Arguments    : none
 * Return Value : WIFI_SUCCESS
 *                WIFI_ERR_OTA_FAIL
 *                WIFI_ERR_NOT_OPEN
 *                WIFI_ERR_PARAMETER
 *                WIFI_ERR_NOT_CONNECT
 *********************************************************************************************************************/
wifi_err_t R_WIFI_DA16XXX_OtaOpen(void)
{
    wifi_err_t api_ret = WIFI_SUCCESS;

    /* Do parameter checking */
    if (0 != R_WIFI_DA16XXX_IsOpened())
    {
        WIFI_LOG_ERROR(("R_WIFI_DA16XXX_OtaOpen: R_WIFI_DA16XXX_Open is not called!"));
        return WIFI_ERR_NOT_OPEN;
    }

    /* Connected access point? */
    if (0 != R_WIFI_DA16XXX_IsConnected())
    {
        WIFI_LOG_ERROR(("R_WIFI_DA16XXX_OtaOpen: Not connected to access point!"));
        return WIFI_ERR_NOT_CONNECT;
    }

    if (DA16XXX_OTA_CLOSE != g_ota_tbl.open)
    {
        WIFI_LOG_ERROR(("R_WIFI_DA16XXX_OtaOpen is called before!"));
        return WIFI_ERR_OTA_ALREADY_OPEN;
    }

    memset(&g_ota_tbl, 0, sizeof(g_ota_tbl));

    /* Set TLS Authentication Level */
    g_ota_tbl.tls_level = (wifi_http_tls_auth_t) WIFI_CFG_OTA_TLS_AUTH;
    if (AT_OK != at_exec("AT+NWOTATLSAUTH=%hu\r", g_ota_tbl.tls_level))
    {
        WIFI_LOG_ERROR(("R_WIFI_DA16XXX_OtaOpen: Cannot set TLS Authentication Level!"));
        return WIFI_ERR_MODULE_COM;
    }

#ifdef OTA_CERT_INCLUDE
    /* Do cert size checking */
    g_ota_tbl.p_root_ca = (const char *)WIFI_CFG_OTA_ROOT_CA;
    g_ota_tbl.root_ca_size = strlen(g_ota_tbl.p_root_ca);
    if ((g_ota_tbl.root_ca_size > DA16XXX_MAX_CERT_SIZE))
    {
        WIFI_LOG_ERROR(("R_WIFI_DA16XXX_OtaOpen: The key provided is larger than the maximum size (%d)!",
                        DA16XXX_MAX_CERT_SIZE));
        return WIFI_ERR_PARAMETER;
    }
#else
    g_ota_tbl.p_root_ca    = NULL;
    g_ota_tbl.root_ca_size = 0;
#endif

    if (NULL != g_ota_tbl.p_root_ca)
    {
        /* Clear cert */
        if (AT_OK != at_exec("AT+NWDCRT\r"))
        {
            api_ret = WIFI_ERR_MODULE_COM;
        }
        else
        {
            /* Put the DA16XXX module into certificate/key input mode */
            at_send("%sC3,", (char WIFI_FAR *)DA16XXX_CERT_START);

            /* Send certificate/key ascii text */
            at_send_raw((uint8_t WIFI_FAR *)g_ota_tbl.p_root_ca, g_ota_tbl.root_ca_size);

            /* Send Indication of the end of content  */
            at_send("%s", (char WIFI_FAR *)DA16XXX_CERT_END);

            /* Block to receive OK  */
            at_recv();
        }
    }

    if (WIFI_SUCCESS == api_ret)
    {
        g_ota_tbl.open = DA16XXX_OTA_OPEN;
    }

    return api_ret;
}
/**********************************************************************************************************************
 * End of function R_WIFI_DA16XXX_OtaOpen
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: R_WIFI_DA16XXX_OtaClose
 * Description  : Close the DA16XXX OTA service.
 * Arguments    : none
 * Return Value : WIFI_SUCCESS
 *                WIFI_ERR_OTA_FAIL
 *                WIFI_ERR_OTA_NOT_OPEN
 *                WIFI_ERR_MODULE_COM
 *********************************************************************************************************************/
wifi_err_t R_WIFI_DA16XXX_OtaClose(void)
{
    wifi_err_t api_ret = WIFI_SUCCESS;

    /* Check if OTA service is not open */
    if (DA16XXX_OTA_OPEN != g_ota_tbl.open)
    {
        return WIFI_ERR_OTA_NOT_OPEN;
    }

    /* Clear cert */
    if (AT_OK != at_exec("AT+NWDCRT\r"))
    {
        api_ret = WIFI_ERR_MODULE_COM;
    }

    if (WIFI_SUCCESS == api_ret)
    {
        if (WIFI_OTA_DOWNLOAD_FINISH == g_ota_tbl.ota_state)
        {
            /* Erase firmware on DA side */
            if (AT_OK != at_exec("AT+NWOTAERASEFW\r"))
            {
                /* Fail */
                api_ret = WIFI_ERR_OTA_FAIL;
            }
        }
    }

    if (WIFI_SUCCESS == api_ret)
    {
        g_ota_tbl.open = DA16XXX_OTA_CLOSE;
    }

    return api_ret;
}
/**********************************************************************************************************************
 * End of function R_WIFI_DA16XXX_OtaClose
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: R_WIFI_DA16XXX_OtaStart
 * Description  : Start downloading firmware from an OTA server
 * Arguments    : fw_url
 * Return Value : WIFI_SUCCESS
 *                WIFI_ERR_OTA_FAIL
 *                WIFI_ERR_NOT_OPEN
 *                WIFI_ERR_PARAMETER
 *                WIFI_ERR_NOT_CONNECT
 *                WIFI_ERR_OTA_NOT_IDLE
 *********************************************************************************************************************/
wifi_err_t R_WIFI_DA16XXX_OtaStart(const uint8_t * fw_url)
{
    wifi_err_t api_ret = WIFI_SUCCESS;

    /* Do parameter checking */
    if (0 != R_WIFI_DA16XXX_IsOpened())
    {
        WIFI_LOG_ERROR(("R_WIFI_DA16XXX_OtaStart: R_WIFI_DA16XXX_Open is not called!"));
        return WIFI_ERR_NOT_OPEN;
    }

    /* Connected access point? */
    if (0 != R_WIFI_DA16XXX_IsConnected())
    {
        WIFI_LOG_ERROR(("R_WIFI_DA16XXX_OtaStart: Not connected to access point!"));
        return WIFI_ERR_NOT_CONNECT;
    }

    /* Check if OTA service is not open */
    if (DA16XXX_OTA_OPEN != g_ota_tbl.open)
    {
        return WIFI_ERR_OTA_NOT_OPEN;
    }

    /* Check if firmware is not downloaded */
    if (WIFI_OTA_IDLE != g_ota_tbl.ota_state)
    {
        return WIFI_ERR_OTA_NOT_IDLE;
    }

    /* Check parameters */
    if (NULL == fw_url)
    {
        return WIFI_ERR_PARAMETER;
    }

    memset(&g_ota_tbl.ota_fw_resp_status, 0, RESP_HEX_MAX);
    g_ota_tbl.ota_receive_status = false;

    /* Set timeout for OTA command */
    at_set_timeout(1000);

    if (AT_OK == at_exec("AT+NWOTADWSTART=%s,%s\r", WIFI_OTA_TYPE_FW, fw_url))
    {
        /* Ensure that if an interrupt occurs, the state is set to FAIL
         * before executing the statement below.
         * This way, we don't need to manually update the state to WIFI_OTA_DOWNLOAD_INPROGRESS.
         */
        if (WIFI_OTA_FAIL != g_ota_tbl.ota_state)
        {
            g_ota_tbl.ota_state = WIFI_OTA_DOWNLOAD_INPROGRESS;
        }
    }
    else
    {
        api_ret = WIFI_ERR_OTA_FAIL;
    }

    at_set_timeout(ATCMD_RESP_TIMEOUT);

    return api_ret;
}
/**********************************************************************************************************************
 * End of function R_WIFI_DA16XXX_OtaStart
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: R_WIFI_DA16XXX_OtaEraseFirmware
 * Description  : Erase the MCU firmware stored in a serial flash of the DA16200/DA16600.
 * Arguments    : none
 * Return Value : WIFI_SUCCESS
 *                WIFI_ERR_NOT_OPEN
 *                WIFI_ERR_OTA_FAIL
 *********************************************************************************************************************/
wifi_err_t R_WIFI_DA16XXX_OtaEraseFirmware(void)
{
    wifi_err_t api_ret = WIFI_SUCCESS;

    /* Do parameter checking */
    if (0 != R_WIFI_DA16XXX_IsOpened())
    {
        WIFI_LOG_ERROR(("R_WIFI_DA16XXX_OtaEraseFirmware: R_WIFI_DA16XXX_Open is not called!"));
        return WIFI_ERR_NOT_OPEN;
    }

    /* Check if OTA service is not open */
    if (DA16XXX_OTA_OPEN != g_ota_tbl.open)
    {
        return WIFI_ERR_OTA_NOT_OPEN;
    }

    /* Check if firmware is downloading */
    if (WIFI_OTA_DOWNLOAD_INPROGRESS == g_ota_tbl.ota_state)
    {
        WIFI_LOG_ERROR(("R_WIFI_DA16XXX_OtaEraseFirmware: Firmware is downloading!"));
        return WIFI_ERR_OTA_FAIL;
    }

    if (AT_OK == at_exec("AT+NWOTAERASEFW\r"))
    {
        /* Set OTA state back to ready, so it can start download again */
        g_ota_tbl.ota_state = WIFI_OTA_IDLE;
    }

    return api_ret;
}
/**********************************************************************************************************************
 * End of function R_WIFI_DA16XXX_OtaEraseFirmware
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: R_WIFI_DA16XXX_OtaGetProgress
 * Description  : Get progress status of firmware download.
 * Arguments    : progress
 * Return Value : WIFI_SUCCESS
 *                WIFI_ERR_NOT_OPEN
 *                WIFI_ERR_OTA_FAIL
 *********************************************************************************************************************/
wifi_err_t R_WIFI_DA16XXX_OtaGetProgress(uint8_t * progress)
{
    wifi_err_t ret_api = WIFI_SUCCESS;

    /* Do parameter checking */
    if (0 != R_WIFI_DA16XXX_IsOpened())
    {
        WIFI_LOG_ERROR(("R_WIFI_DA16XXX_OtaGetProgress: R_WIFI_DA16XXX_Open is not called!"));
        return WIFI_ERR_NOT_OPEN;
    }

    /* Check if OTA service is not open */
    if (DA16XXX_OTA_OPEN != g_ota_tbl.open)
    {
        return WIFI_ERR_OTA_NOT_OPEN;
    }

    if (AT_OK == at_exec("AT+NWOTADWPROG=%s\r", WIFI_OTA_TYPE_FW))
    {
        if (DATA_FOUND == at_read("+NWOTADWPROG:%hhu", progress))
        {
            WIFI_LOG_INFO(("OTA download progress: %hhu %%", *progress));
        }
    }
    else
    {
        ret_api = WIFI_ERR_OTA_FAIL;
    }

    return ret_api;
}
/**********************************************************************************************************************
 * End of function R_WIFI_DA16XXX_OtaGetProgress
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: R_WIFI_DA16XXX_OtaReadFirmwarebyBlock
 * Description  : Read the MCU firmware as much as the size from the fw_addr and transmit it
 * Arguments    : fw_addr
 *                size
 * Return Value : WIFI_SUCCESS
 *                WIFI_ERR_NOT_OPEN
 *                WIFI_ERR_OTA_FAIL
 *********************************************************************************************************************/
wifi_err_t R_WIFI_DA16XXX_OtaReadFirmwarebyBlock(uint32_t fw_addr, uint32_t size)
{
    wifi_err_t api_ret = WIFI_SUCCESS;

    /* Do parameter checking */
    if (0 != R_WIFI_DA16XXX_IsOpened())
    {
        WIFI_LOG_ERROR(("R_WIFI_DA16XXX_OtaReadFirmwarebyBlock: R_WIFI_DA16XXX_Open is not called!"));
        return WIFI_ERR_NOT_OPEN;
    }

    /* Check if OTA service is not open */
    if (DA16XXX_OTA_OPEN != g_ota_tbl.open)
    {
        return WIFI_ERR_OTA_NOT_OPEN;
    }

    /* Firmware should be ready on the DA side, so the OTA state should be WIFI_OTA_DOWNLOAD_FINISH */
    if (WIFI_OTA_DOWNLOAD_FINISH != g_ota_tbl.ota_state)
    {
        return WIFI_ERR_OTA_FAIL;
    }

    /* Reset fw buffer */
    memset(&g_ota_tbl.ota_fw_buf, 0, WIFI_CFG_OTA_BLK_SIZE);

    /* Get size */
    g_ota_tbl.ota_fw_len = size;

#if defined(__CCRX__) || defined(__ICCRX__) || defined(__RX__)
#if WIFI_CFG_DEBUG_LOG == LOG_DEBUG
    /* turn echo status off */
    at_exec("ATZ\r");
#endif
#endif

    /* Set state to receive image */
    uart_set_recv_state(WIFI_RECV_DATA);
    uart_set_recv_type(WIFI_RESP_NWOTAREADFW);

    /* Start OTA download command with firmware type and URL */
    if ( AT_OK != at_exec("AT+NWOTAREADFW=%x,%u\r", fw_addr, size) )
    {
        api_ret = WIFI_ERR_OTA_FAIL;
    }

#if defined(__CCRX__) || defined(__ICCRX__) || defined(__RX__)
#if WIFI_CFG_DEBUG_LOG == LOG_DEBUG
    /* turn echo status on */
    at_exec("ATE\r");
#endif
#endif

    return api_ret;
}
/**********************************************************************************************************************
 * End of function R_WIFI_DA16XXX_OtaReadFirmwarebyBlock
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: R_WIFI_DA16XXX_OtaGetAddress
 * Description  : Get the value set with NWOTASETADDR
 * Arguments    : fw_addr
 * Return Value : WIFI_SUCCESS
 *                WIFI_ERR_NOT_OPEN
 *                WIFI_ERR_OTA_FAIL
 *********************************************************************************************************************/
wifi_err_t R_WIFI_DA16XXX_OtaGetAddress(uint32_t * fw_addr)
{
    wifi_err_t ret_api = WIFI_SUCCESS;

    /* Do parameter checking */
    if (0 != R_WIFI_DA16XXX_IsOpened())
    {
        WIFI_LOG_ERROR(("R_WIFI_DA16XXX_OtaGetAddress: R_WIFI_DA16XXX_Open is not called!"));
        return WIFI_ERR_NOT_OPEN;
    }

    /* Check if OTA service is not open */
    if (DA16XXX_OTA_OPEN != g_ota_tbl.open)
    {
        return WIFI_ERR_OTA_NOT_OPEN;
    }

    if (AT_OK == at_exec("AT+NWOTAGETADDR=%s\r", WIFI_OTA_TYPE_FW))
    {
        at_read("+NWOTAGETADDR:%x", fw_addr);
    }
    else
    {
        ret_api = WIFI_ERR_OTA_FAIL;
    }
    return ret_api;
}
/**********************************************************************************************************************
 * End of function R_WIFI_DA16XXX_OtaGetAddress
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: R_WIFI_DA16XXX_OtaFirmwareSize
 * Description  : Get a size in the header of the MCU firmware
 * Arguments    : fw_size
 * Return Value : WIFI_SUCCESS
 *                WIFI_ERR_NOT_OPEN
 *                WIFI_ERR_OTA_FAIL
 *********************************************************************************************************************/
wifi_err_t R_WIFI_DA16XXX_OtaFirmwareSize(uint32_t * fw_size)
{
    wifi_err_t ret_api = WIFI_SUCCESS;

    /* Do parameter checking */
    if (0 != R_WIFI_DA16XXX_IsOpened())
    {
        WIFI_LOG_ERROR(("R_WIFI_DA16XXX_OtaFirmwareSize: R_WIFI_DA16XXX_Open is not called!"));
        return WIFI_ERR_NOT_OPEN;
    }

    /* Check if OTA service is not open */
    if (DA16XXX_OTA_OPEN != g_ota_tbl.open)
    {
        return WIFI_ERR_OTA_NOT_OPEN;
    }

    if (AT_OK == at_exec("AT+NWOTAFWSIZE\r"))
    {
        at_read("+NWOTAFWSIZE:%u", fw_size);
    }
    else
    {
        ret_api = WIFI_ERR_OTA_FAIL;
    }
    return ret_api;
}
/**********************************************************************************************************************
 * End of function R_WIFI_DA16XXX_OtaFirmwareSize
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: R_WIFI_DA16XXX_OtaGetState
 * Description  : Get state of OTA transfer.
 * Arguments    : state
 * Return Value : WIFI_SUCCESS
 *                WIFI_ERR_NOT_OPEN
 *********************************************************************************************************************/
wifi_err_t R_WIFI_DA16XXX_OtaGetState (wifi_ota_state_t * state)
{
    /* Check if OTA service is not open */
    if (DA16XXX_OTA_OPEN != g_ota_tbl.open)
    {
        return WIFI_ERR_OTA_NOT_OPEN;
    }

    if (true == g_ota_tbl.ota_receive_status)
    {
        /* Check status */
        if (0 != strncmp((const char *)g_ota_tbl.ota_fw_resp_status, "0x00", 4))
        {
            /* Error */
            g_ota_tbl.ota_state = WIFI_OTA_FAIL;
        }
        else
        {
            g_ota_tbl.ota_state = WIFI_OTA_DOWNLOAD_FINISH;
        }
        g_ota_tbl.ota_receive_status = false;
    }

    *state = g_ota_tbl.ota_state;

    return WIFI_SUCCESS;
}
/**********************************************************************************************************************
 * End of function R_WIFI_DA16XXX_OtaGetState
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: R_WIFI_DA16XXX_OtaGetFirmware
 * Description  : Get data in ota_fw_buf by size
 * Arguments    : buffer
 *                offset
 *                size
 * Return Value : WIFI_SUCCESS
 *                WIFI_ERR_OTA_NOT_OPEN
 *********************************************************************************************************************/
wifi_err_t R_WIFI_DA16XXX_OtaGetFirmware(uint8_t * buffer, uint16_t offset, uint16_t size)
{
    /* Check if OTA service is not open */
    if (DA16XXX_OTA_OPEN != g_ota_tbl.open)
    {
        return WIFI_ERR_OTA_NOT_OPEN;
    }

    memcpy(buffer, (const void WIFI_FAR *)&g_ota_tbl.ota_fw_buf[offset], size);

    return WIFI_SUCCESS;
}
/**********************************************************************************************************************
 * End of function R_WIFI_DA16XXX_OtaGetFirmware
 *********************************************************************************************************************/

/*
 * Callback functions
 */
/**********************************************************************************************************************
 * Function Name: da16xxx_handle_incoming_ota_data
 * Description  : Handles incoming common data.
 * Arguments    : type
 *                state
 *                data
 * Return Value : none
 *********************************************************************************************************************/
void da16xxx_handle_incoming_ota_data(wifi_resp_type_t *type, wifi_recv_state_t *state, uint8_t data)
{
    switch (*state)
    {
        case WIFI_RECV_DATA:    /* MCU Fw */
        {
            if ( 0 < g_ota_tbl.ota_fw_len-- )
            {
                g_ota_tbl.ota_fw_buf[g_ota_tbl.ota_fw_rcv_cnt++] = data;
            }

            /* Ensure always recv cnt >= 0 */
            if (0 >= g_ota_tbl.ota_fw_len)
            {
                g_ota_tbl.ota_fw_rcv_cnt = 0;
                g_ota_tbl.ota_fw_len     = 0;
                *type                    = WIFI_RESP_NONE;
                *state                   = WIFI_RECV_PREFIX;
            }
            break;
        }
        case WIFI_RECV_PARAM_STATUS:     /* Start */
        {
            if ('\r' == data)   /* End? */
            {
                g_ota_tbl.ota_msg_ended_status = true;
            }
            else if (('\n' == data) && (true == g_ota_tbl.ota_msg_ended_status))
            {
                g_ota_tbl.ota_receive_status = true;

                /* Reset flag */
                g_ota_tbl.ota_msg_ended_status = false;
                g_ota_tbl.ota_fw_rcv_cnt       = 0;
                *type                          = WIFI_RESP_NONE;
                *state                         = WIFI_RECV_PREFIX;
            }
            else
            {
                g_ota_tbl.ota_fw_resp_status[g_ota_tbl.ota_fw_rcv_cnt++] = data;
            }
            break;
        }
        default:
        {
            *type  = WIFI_RESP_NONE;
            *state = WIFI_RECV_PREFIX;
            break;
        }
    }
}
/**********************************************************************************************************************
 * End of function da16xxx_handle_incoming_ota_data
 *********************************************************************************************************************/
#endif /* WIFI_CFG_OTA_SUPPORT == 1 */
