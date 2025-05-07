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

#include <stdint.h>
#include "errors.h"         /* swd_error_t */
#include "swd_dp_ap.h"      /* TAR / DRW helpers */

/* -----------------------------------------------------------------------------
 *  High level operations
 * --------------------------------------------------------------------------*/

swd_error_t  SWD_Unlock_Flash   (void);
swd_error_t  SWD_Erase_Page     (uint32_t page_address);
swd_error_t  SWD_Write_Word     (uint32_t addr, uint32_t data);
swd_error_t  SWD_Write_Firmware (void);
swd_error_t  SWD_Verify_Firmware(void);
swd_error_t  SWD_Lock_Flash     (void);

#endif /* SWD_FLASH_H */



