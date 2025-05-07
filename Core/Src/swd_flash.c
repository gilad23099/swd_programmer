/*
 * swd_flash.c
 *
 *  Created on: May 7, 2025
 *      Author: gw230
 */

#include "stm32f4xx_hal.h"
#include "swd_flash.h"
#include <stdio.h>


// ========== Sample firmware array to burn into target ==========
const uint32_t firmware_bin[] = {
    0x20001000, 0x08000111, 0xF000F8EF, 0x00000000  // Example data
};

const uint32_t firmware_size = sizeof(firmware_bin) / sizeof(firmware_bin[0]);

//===============================================================================//

/**
 * @brief  Enable write operations on the EFM32 flash MSC.
 *         Sets the WREN bit in the MSC_WRITECTRL register.
 * @note   This must be called before any Erase or Write commands.
 */
void SWD_Unlock_Flash(void) {

    SWD_Write_TAR(MSC_WRITECTRL);         // 1) Select the MSC_WRITECTRL register in the target's MSC block

    SWD_Write_DRW(MSC_WRITECTRL_WREN);    // 2) Write the WREN bit to enable flash programming

    // No need to poll: write-enable takes effect immediately
}

/**
 * @brief  Erase a flash page on the EFM32 via its MSC peripheral.
 * @param  page_address  Start address of the flash page to erase.
 * @note   Uses MSC_ADDRB, MSC_WRITECMD_ERASEPAGE and polls MSC_STATUS_BUSY.
 */
void SWD_Erase_Page(uint32_t page_address) {
    uint32_t status;

    // 1) Write page address to MSC_ADDRB
    SWD_Write_TAR(MSC_ADDRB);
    SWD_Write_DRW(page_address);

    // 2) Issue ERASEPAGE command
    SWD_Write_TAR(MSC_WRITECMD);
    SWD_Write_DRW(MSC_WRITECMD_ERASEPAGE);

    // 3) Poll MSC_STATUS until BUSY flag clears
    do {
        SWD_Write_TAR(MSC_STATUS);
        SWD_Read_DRW(&status);
    } while (status & MSC_STATUS_BUSY);
}


// Send parity bit (even parity)
void SWD_Write_Parity(uint32_t data) {
    uint8_t parity = __builtin_parity(data);
    SWD_Write_Bit(parity);
}


/**
 * @brief  Write a 32-bit word into EFM32 flash via its MSC peripheral.
 * @param  addr  Absolute flash address where the word will be written.
 * @param  data  32-bit word to program.
 * @note   Assumes MSC write enable has already been set (via SWD_Unlock_Flash).
 */
void SWD_Write_Word(uint32_t addr, uint32_t data) {
    uint32_t status;

    // 1) Set target address (MSC_ADDRB)
    SWD_Write_TAR(MSC_ADDRB);
    SWD_Write_DRW(addr);

    // 2) Load the data word into MSC_WDATA
    SWD_Write_TAR(MSC_WDATA);
    SWD_Write_DRW(data);

    // 3) Issue WRITEWORD command
    SWD_Write_TAR(MSC_WRITECMD);
    SWD_Write_DRW(MSC_WRITECMD_WRITEWORD);

    // 4) Poll MSC_STATUS until BUSY flag clears
    do {
        SWD_Write_TAR(MSC_STATUS);
        SWD_Read_DRW(&status);
    } while (status & MSC_STATUS_BUSY);
}


/**
 * @brief  Program the firmware_bin array into the target’s flash memory.
 * @note   Assumes SWD_Unlock_Flash() has been called beforehand.
 */
void SWD_Write_Firmware(void) {
    for (uint32_t i = 0; i < firmware_size; ++i) {
        // Calculate the absolute flash address for this word
        uint32_t addr = FLASH_BASE + (i * sizeof(uint32_t));
        uint32_t data = firmware_bin[i];

        // Write one 32-bit word
        SWD_Write_Word(addr, data);
    }
}


uint8_t SWD_Verify_Firmware(void) {
    for (uint32_t i = 0; i < firmware_size; i++) {
        uint32_t addr = 0x08000000 + (i * 4);
        uint32_t value = 0;

        SWD_Write_TAR(addr);
        if (!SWD_Read_DRW(&value)) {
            printf("SWD read error at 0x%08lX\n", addr);
            return 0;
        }

        if (value != firmware_bin[i]) {
            printf("Mismatch at 0x%08lX: expected 0x%08lX, got 0x%08lX\n",addr, firmware_bin[i], value);
            return 0;
        }
    }

    printf("[SWD] Firmware verification passed.\n");
    return 1;
}


/**
 * @brief  Lock the EFM32 flash MSC to prevent further write or erase operations.
 * @note   Should be called after all programming and verification are complete.
 */
void SWD_Lock_Flash(void) {
    // 1) Select the MSC_LOCK register in the target’s MSC block
    SWD_Write_TAR(MSC_LOCK);
    // 2) Write the lock key to disable further flash programming
    SWD_Write_DRW(MSC_LOCK_LOCKKEY);
}








