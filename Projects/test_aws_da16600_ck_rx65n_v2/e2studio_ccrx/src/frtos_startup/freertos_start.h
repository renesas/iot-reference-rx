/*
* Copyright (c) 2023-2025 Renesas Electronics Corporation and/or its affiliates
* SPDX-License-Identifier: BSD-3-Clause
*/
/*******************************************************************************
* File Name    : freertos_start.h
* Version      : 1.0
* Description  : FreeRTOS's user-defined functions header file.
******************************************************************************/
/*****************************************************************************
* History : DD.MM.YYYY Version  Description
*         : 30.06.2016 1.00     First Release
******************************************************************************/

#ifndef FREERTOS_START_H_
#define FREERTOS_START_H_

/******************************************************************************
Macro definitions
******************************************************************************/
#define RTOS_USB_SUPPORT 0

/******************************************************************************
Typedef definitions
******************************************************************************/

/******************************************************************************
Exported global variables
******************************************************************************/
/************* semaphore handler *****************/



/************* mutex handler *********************/


/************** queues handler *******************/


/************** event groups handler *************/


/************** mailbox handler ******************/


/************** memory pool handler **************/


/************** task handler *********************/


/******************************************************************************
Exported global functions (to be accessed by other files)
******************************************************************************/
extern void Processing_Before_Start_Kernel(void);
extern void Kernel_Object_init(void);

#endif /* FREERTOS_START_H_ */
