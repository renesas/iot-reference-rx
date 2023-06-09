/*
 * rm_littlefs_flash_config.h
 *
 *  Created on: May 11, 2023
 */

#ifndef FRTOS_CONFIG_RM_LITTLEFS_FLASH_CONFIG_H_
#define FRTOS_CONFIG_RM_LITTLEFS_FLASH_CONFIG_H_

#if defined (BSP_MCU_RX65N)
	#define LFS_FLASH_READ_SIZE				(1)
	#define LFS_FLASH_PROGRAM_SIZE			(4)
	#define LFS_FLASH_BLOCK_SIZE			(128)
	#define LFS_FLASH_BLOCK_COUNT			((BSP_DATA_FLASH_SIZE_BYTES - (rm_littlefs_flash_data_start - FLASH_DF_BLOCK_0)) / LFS_FLASH_BLOCK_SIZE)
	#define RM_LITTLEFS_FLASH_DATA_START	FLASH_DF_BLOCK_32

#else
	#error "rm_littlefs_flash_config.h does not support your MCU"
#endif

#endif /* FRTOS_CONFIG_RM_LITTLEFS_FLASH_CONFIG_H_ */
