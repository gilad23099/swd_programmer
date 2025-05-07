/*
 * swd_defs.h
 *
 *  Created on: May 7, 2025
 *      Author: gw230
 */

#ifndef SWD_DEFS_H
#define SWD_DEFS_H

// --- SWD request header bit definitions ---
#define START    (1u << 0)
#define AP       (1u << 1)
#define DP       (1u << 1)
#define READ     (1u << 2)
#define WRITE    (0u << 2)
#define STOP     (0u << 6)
#define PARK     (1u << 7)

// DP Registers
#define DP_IDCODE     (0u << 4)                      // 0x00 Read
#define DP_ABORT      (0u << 4)                      // 0x00 Write
#define DP_CTRL_STAT  (1u << 3)                      // 0x04 Read/Write
#define DP_RESEND     (1u << 4)                      // 0x08 Read
#define DP_SELECT     (1u << 4)                      // 0x08 Write
#define DP_RDBUFF     ((1u << 3) | (1u << 4))        // 0x0C Read

// AP registers
#define AP_CSW        (0u << 4)                      // 0x00 Read/Write
#define AP_TAR        (1u << 3)                      // 0x04 Read/Write
#define AP_DRW        ((1u << 3) | (1u << 4))        // 0x0C Read/Write
#define AP_IDR        0xFC                           // 0xFC Read-only


// ----EFM32 macros-----

#define MSC_BASE        0x400E0000UL  // Base address of the EFM32 MSC peripheral

// Register offsets for MSC
#define MSC_WRITECTRL   (MSC_BASE + 0x000)  // Write control register
#define MSC_ADDRB       (MSC_BASE + 0x004)  // Address buffer register
#define MSC_WRITECMD    (MSC_BASE + 0x008)  // Command register
#define MSC_STATUS      (MSC_BASE + 0x00C)  // Status register
#define MSC_WDATA       (MSC_BASE + 0x010)  // Write data register
#define MSC_LOCK        (MSC_BASE + 0x020)  // Lock register

// Bit definitions for MSC registers
#define MSC_WRITECTRL_WREN     (1u << 0)  // Write enable bit
#define MSC_WRITECMD_ERASEPAGE (1u << 1)  // Erase page command bit
#define MSC_WRITECMD_WRITEWORD (1u << 2)  // Write word command bit
#define MSC_STATUS_BUSY        (1u << 0)  // Busy flag bit
#define MSC_LOCK_LOCKKEY       0x1U       // Lock key value



// ===================== Pin Configuration =====================
#define PIN_SWDIO     GPIO_PIN_0
#define PIN_SWCLK     GPIO_PIN_1
#define PORT_SWD      GPIOB


#endif /* INC_SWD_DEFS_H_ */
