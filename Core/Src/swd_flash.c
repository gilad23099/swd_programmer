/*
 * swd_flash.c
 *
 *  Created on: May 7, 2025
 *      Author: gw230
 */

/* swd_flash.c  ------------------------------------------------------------
 *  Flash-level helpers for EFM32 via SW-D (uses new transfer layer)
 * -----------------------------------------------------------------------*/
#include "swd_defs.h"
#include "stm32f4xx_hal.h"
#include "swd_flash.h"
#include "swd_dp_ap.h"      /* provides _Retry wrappers & macros */
#include <stdio.h>

/* Example firmware image -------------------------------------------------*/
const uint32_t firmware_bin[] = {
    0x20001000, 0x08000111, 0xF000F8EF, 0x00000000
};
const uint32_t firmware_size = sizeof(firmware_bin) / sizeof(firmware_bin[0]);

/* Unlock flash controller ------------------------------------------------*/
swd_error_t SWD_Unlock_Flash(void)
{
    if (SWD_Write_TAR(MSC_WRITECTRL) != SWD_ERROR_OK)
        return SWD_ERROR_FAULT;
    return SWD_Write_DRW(MSC_WRITECTRL_WREN);
}

/* Erase single flash page ------------------------------------------------*/
swd_error_t SWD_Erase_Page(uint32_t page_address)
{
    uint32_t status;

    SWD_Write_TAR(MSC_ADDRB);
    SWD_Write_DRW(page_address);

    SWD_Write_TAR(MSC_WRITECMD);
    SWD_Write_DRW(MSC_WRITECMD_ERASEPAGE);

    /* poll BUSY, retry on ACK‑WAIT */
    do {
        SWD_Write_TAR(MSC_STATUS);
        if (SWD_Read_AP_Retry(AP_DRW, &status,
                              SWD_WAIT_RETRIES_DEFAULT) != SWD_ERROR_OK)
            return SWD_ERROR_FAULT;
    } while (status & MSC_STATUS_BUSY);

    return SWD_ERROR_OK;
}

/* Write one 32 bit word --------------------------------------------------*/
swd_error_t SWD_Write_Word(uint32_t addr, uint32_t data)
{
    uint32_t status;

    SWD_Write_TAR(MSC_ADDRB);     SWD_Write_DRW(addr);
    SWD_Write_TAR(MSC_WDATA);     SWD_Write_DRW(data);
    SWD_Write_TAR(MSC_WRITECMD);  SWD_Write_DRW(MSC_WRITECMD_WRITEWORD);

    do {
        SWD_Write_TAR(MSC_STATUS);
        if (SWD_Read_AP_Retry(AP_DRW, &status,
                              SWD_WAIT_RETRIES_DEFAULT) != SWD_ERROR_OK)
            return SWD_ERROR_FAULT;
    } while (status & MSC_STATUS_BUSY);

    return SWD_ERROR_OK;
}

/* Bulk program firmware --------------------------------------------------*/
swd_error_t SWD_Write_Firmware(void)
{
    for (uint32_t i = 0; i < firmware_size; ++i) {
        uint32_t addr = FLASH_BASE + (i * 4);
        if (SWD_Write_Word(addr, firmware_bin[i]) != SWD_ERROR_OK)
            return SWD_ERROR_FAULT;
    }
    return SWD_ERROR_OK;
}

/* Verify programmed image ------------------------------------------------*/
swd_error_t SWD_Verify_Firmware(void)
{
    for (uint32_t i = 0; i < firmware_size; ++i) {
        uint32_t addr  = FLASH_BASE + (i * 4);
        uint32_t value = 0;

        SWD_Write_TAR(addr);
        if (SWD_Read_AP_Retry(AP_DRW, &value,
                              SWD_WAIT_RETRIES_DEFAULT) != SWD_ERROR_OK) {
            printf("Read error @ 0x%08lX\n", addr);
            return SWD_ERROR_FAULT;
        }

        if (value != firmware_bin[i]) {
            printf("Mismatch @ 0x%08lX: exp 0x%08lX, got 0x%08lX\n",
                   addr, firmware_bin[i], value);
            return SWD_ERROR_PARITY;
        }
    }
    printf("[SWD] Firmware verified OK\n");
    return SWD_ERROR_OK;
}

/* Relock flash controller ------------------------------------------------*/
swd_error_t SWD_Lock_Flash(void)
{
    if (SWD_Write_TAR(MSC_LOCK) != SWD_ERROR_OK)
        return SWD_ERROR_FAULT;
    return SWD_Write_DRW(MSC_LOCK_LOCKKEY);
}







