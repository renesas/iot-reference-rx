/*******************************************************************************
* Copyright (C) 2023 Renesas Electronics Corporation. All rights reserved.
*
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only
* intended for use with Renesas products. No other uses are authorized. This
* software is owned by Renesas Electronics Corporation and is protected under
* all applicable laws, including copyright laws.
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
* THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT
* LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE
* AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.
* TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS
* ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE
* FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR
* ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE
* BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software
* and to discontinue the availability of this software. By using this software,
* you agree to the additional terms and conditions found by accessing the
* following link:
* http://www.renesas.com/disclaimer
*******************************************************************************/

/**
  ******************************************************************************
  * @file    entropy_hardware_poll.c
  *
  ******************************************************************************
  */

#include <string.h>
#include "platform.h"   // __LIT for all compilers
#include "r_s12ad_rx_if.h"

#if BSP_CFG_MCU_PART_ENCRYPTION_INCLUDED == true
#include "r_tsip_rx_if.h"
#include "r_tsip_rx_config.h"

tsip_tls_ca_certification_public_key_index_t s_inst1 =
{
    0
};
/* Update keyring key index data */
tsip_update_key_ring_t s_inst2 =
{
    0
};

#endif

void get_random_number(uint8_t *data, uint32_t len);

/******************************************************************************
Functions : hardware entropy collector(repeatedly called until enough gathered)
******************************************************************************/
int mbedtls_hardware_poll( void *data,
                           unsigned char *output, size_t len, size_t *olen )
{
    INTERNAL_NOT_USED(data);
	INTERNAL_NOT_USED(len);
	#if BSP_CFG_MCU_PART_ENCRYPTION_INCLUDED == true
		e_tsip_err_t error_code = TSIP_SUCCESS;

		static uint32_t gs_random_number_buffer[4] =
		{
			0
		};

		size_t num_bytes = ( len < sizeof( uint32_t ) ) ? len : sizeof( uint32_t );

		error_code = R_TSIP_Open(&s_inst1, &s_inst2);
		R_TSIP_GenerateRandomNumber(( uint32_t * ) &gs_random_number_buffer);
		*olen = 0;

		memcpy( output, &gs_random_number_buffer, num_bytes );
		*olen = num_bytes;

		error_code = R_TSIP_Close();

		return error_code;
	#else
		uint32_t random_number = 0;
		size_t num_bytes = ( len < sizeof( uint32_t ) ) ? len : sizeof( uint32_t );

		get_random_number( ( uint8_t * ) &random_number, sizeof( uint32_t ) );
		*olen = 0;

		memcpy( output, &random_number, num_bytes );
		*olen = num_bytes;

		return 0;


	#endif
}

/******************************************************************************
Functions : random number generator(XorShift method)

WARNING: For best security practice, it is recommended to utilize a
    random number generation solution that is truly randomized and conforms to
    the guidelines provided in the Device Qualification Program for FreeRTOS Guide
    (https://docs.aws.amazon.com/freertos/latest/qualificationguide/afq-checklist.html).
    The random number generator method presented in this file by the silicon vendor
    is not truly random in nature. The current solution takes entropy from the
    temperature sensor on the board and from the current system time.
    For production development, Renesas RX65x customers are recommended to use
    the TRNG included in the Trusted Secure IP Driver.
    Please see the following for more information on the Trusted Secure IP Driver:
    https://www.renesas.com/us/en/products/software-tools/software-os-middleware-driver/security-crypto/trusted-secure-ip-driver.html.
    Please contact the silicon vendor for details regarding the method implemented.

******************************************************************************/
void get_random_number(uint8_t *data, uint32_t len)
{
    static uint32_t y = 2463534242;
    uint32_t res;
    uint32_t lp;
    uint8_t *bPtr;
#if defined (BSP_MCU_RX65N) || (BSP_MCU_RX651) || (BSP_MCU_RX64M)
    adc_cfg_t ad_cfg;
    adc_ch_cfg_t ch_cfg;
    adc_sst_t sst;
    uint16_t temperature_data;

    /* initialize temperature sensor */
    memset(&ad_cfg, 0, sizeof(ad_cfg));
    ad_cfg.resolution = ADC_RESOLUTION_12_BIT;
    ad_cfg.trigger = ADC_TRIG_SOFTWARE;
    ad_cfg.priority = 0;
    ad_cfg.add_cnt   = ADC_ADD_OFF;
    ad_cfg.alignment = ADC_ALIGN_RIGHT;
    ad_cfg.clearing  = ADC_CLEAR_AFTER_READ_OFF;

    R_ADC_Open(1, ADC_MODE_SS_ONE_CH, &ad_cfg, NULL);

    memset(&sst, 0, sizeof(sst));
    sst.reg_id = ADC_SST_TEMPERATURE;
    sst.num_states = 240;
    R_ADC_Control(1, ADC_CMD_SET_SAMPLE_STATE_CNT, &sst);

    memset(&ch_cfg, 0, sizeof(ch_cfg));
    ch_cfg.chan_mask = ADC_MASK_TEMP;
    ch_cfg.diag_method = ADC_DIAG_OFF;
    ch_cfg.anex_enable = false;
    ch_cfg.sample_hold_mask = 0;
    R_ADC_Control(1, ADC_CMD_ENABLE_CHANS, &ch_cfg);

    vTaskDelay(10);

    R_ADC_Control(1, ADC_CMD_SCAN_NOW, NULL);
    while (R_ADC_Control(1, ADC_CMD_CHECK_SCAN_DONE, NULL) == ADC_ERR_SCAN_NOT_DONE);

    R_ADC_Read(1, ADC_REG_TEMP, &temperature_data);

    y += temperature_data;  /* randomness from internal temperature sensor */
#elif defined (BSP_MCU_RX63N) || (BSP_MCU_RX631) || (BSP_MCU_RX630)
    adc_cfg_t ad_cfg;
    adc_ch_cfg_t ch_cfg;
    uint16_t temperature_data;

    /* initialize temperature sensor */
    memset(&ad_cfg, 0, sizeof(ad_cfg));
    ad_cfg.trigger = ADC_TRIG_SOFTWARE;
    ad_cfg.priority = 0;
    ad_cfg.add_cnt   = ADC_ADD_OFF;
    ad_cfg.alignment = ADC_ALIGN_RIGHT;
    ad_cfg.clearing  = ADC_CLEAR_AFTER_READ_OFF;

    R_ADC_Open(1, ADC_MODE_SS_ONE_CH, &ad_cfg, NULL);

    memset(&ch_cfg, 0, sizeof(ch_cfg));
    R_ADC_Control(1, ADC_CMD_ENABLE_CHANS, &ch_cfg);

    vTaskDelay(10);

    R_ADC_Control(1, ADC_CMD_SCAN_NOW, NULL);
    while (R_ADC_Control(1, ADC_CMD_CHECK_SCAN_DONE, NULL) == ADC_ERR_SCAN_NOT_DONE);

    R_ADC_Read(1, ADC_REG_TEMP, &temperature_data);

    y += temperature_data;  /* randomness from internal temperature sensor, RX63N ver has not been confirmed. Maybe always zero is output. Please fix this anybody. */
#endif
    y += xTaskGetTickCount();   /* randomness from system timer */

    res = len / 4;
    for (lp = 0; lp < res; lp++)
    {
        y = y ^ (y << 13);
        y = y ^ (y >> 17);
        y = y ^ (y << 5);
        bPtr = (uint8_t*) & y;
#if __LIT
        *((uint32_t *)data) = (uint32_t)((*(bPtr + 3) << 24) | (*(bPtr + 2) << 16) | (*(bPtr + 1) << 8) | *(bPtr + 0));
#else
        *((uint32_t *)data) = y;
#endif
        data += 4;
    }
    y = y ^ (y << 13);
    y = y ^ (y >> 17);
    y = y ^ (y << 5);
    res = (uint32_t)len % 4;
    bPtr = (uint8_t*) & y;
    switch (res)
    {
        case 3:
#if __LIT
            *data++ = bPtr[3];
            *data++ = bPtr[2];
            *data++ = bPtr[1];
#else
            *data++ = bPtr[0];
            *data++ = bPtr[1];
            *data++ = bPtr[2];
#endif
            break;

        case 2:
#if __LIT
            *data++ = bPtr[3];
            *data++ = bPtr[2];
#else
            *data++ = bPtr[0];
            *data++ = bPtr[1];
#endif
            break;

        case 1:
#if __LIT
            *data++ = bPtr[3];
#else
            *data++ = bPtr[0];
#endif
            break;

        default:
            /* no op */
            break;
    }
}
