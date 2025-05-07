/*
 * swd_flash.h
 *
 * Created on: May 7, 2025
 * Author: gw230
 *
 * Header for SWD flash memory programming functions.
 */

#ifndef SWD_FLASH_H
#define SWD_FLASH_H

#include <stdint.h>          // uint32_t, uint8_t
#include <stdbool.h>         // bool
#include "swd_defs.h"        // FLASH_* addresses
#include "swd_transfer.h"    // SWD_Write_Parity
#include "swd_dp_ap.h"       // SWD_Write_TAR, SWD_Write_DRW, SWD_Read_DRW


/**
 * @brief Enable write operations on the EFM32 flash MSC
 */
void SWD_Unlock_Flash(void);

/**
 * @brief Erase a flash page on the EFM32 via its MSC peripheral.
 * @param page_address Start address of the page to erase.
 */
void SWD_Erase_Page(uint32_t page_address);


/**
 * @brief Send parity bit (even parity).
 * @param data 32 bit.
 */
void SWD_Write_Parity(uint32_t data);

/**
 * @brief  Write a 32-bit word into EFM32 flash via its MSC peripheral.
 * @param addr Target memory address.
 * @param data 32-bit data to write.
 */
void SWD_Write_Word(uint32_t addr, uint32_t data);

/**
 * @brief Write the firmware_bin[] array to flash memory.
 */
void SWD_Write_Firmware(void);

/**
 * @brief Verify that the firmware was written correctly.
 * @return 1 if verification passed, 0 if mismatch occurred.
 */
uint8_t SWD_Verify_Firmware(void);

/**
 * @brief Lock the flash memory to prevent further modifications.
 */
void SWD_Lock_Flash(void);

#endif // SWD_FLASH_H



