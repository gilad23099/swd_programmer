/*
 * swd_defs.h
 *
 *  Created on: May 7, 2025
 *      Author: gw230
 */

#ifndef SWD_DEFS_H
#define SWD_DEFS_H

/* ---------------------------------------------------------------------
 *  EFM32 MSC peripheral base and bit definitions
 * -------------------------------------------------------------------*/
#define MSC_BASE        0x400E0000UL
/* Flash page size (bytes) – EFM32 TG/GG = 2048. can change to 4096 if needed */
#define MSC_PAGE_SIZE   2048UL

#define MSC_WRITECTRL   (MSC_BASE + 0x000)
#define MSC_ADDRB       (MSC_BASE + 0x004)
#define MSC_WRITECMD    (MSC_BASE + 0x008)
#define MSC_STATUS      (MSC_BASE + 0x00C)
#define MSC_WDATA       (MSC_BASE + 0x010)
#define MSC_LOCK        (MSC_BASE + 0x020)

/* Bit masks */
#define MSC_WRITECTRL_WREN       (1u << 0)
#define MSC_WRITECMD_ERASEPAGE   (1u << 1)
#define MSC_WRITECMD_WRITEWORD   (1u << 2)
#define MSC_STATUS_BUSY          (1u << 0)
#define MSC_LOCK_LOCKKEY         0x1u

/* FLASH base for EFM32 (for SWD_Write_Firmware) */
#define FLASH_BASE  0x08000000UL

#endif /* SWD_DEFS_H */
