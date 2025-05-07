/*
 * swd_dp_ap.h
 *
 * Created on: May 7, 2025
 * Author: gw230
 *
 * Header for SWD Debug Port (DP) and Access Port (AP) functions.
 */

#ifndef SWD_DP_AP_H
#define SWD_DP_AP_H

#include <stdint.h>
#include <stdbool.h>
#include "swd_transfer.h"     /* provides SWD_Write_DP/AP, SWD_Read_DP/AP */


/* -----------------------------------------------------------------------------
 *  DP register indices (A[3:2])
 * --------------------------------------------------------------------------*/
#define DP_ABORT        0x0   /* ABORT register  */
#define DP_CTRL_STAT    0x1   /* CTRL/STAT       */
#define DP_SELECT       0x2   /* SELECT          */
#define DP_RDBUFF       0x3   /* Read buffer     */


/* -----------------------------------------------------------------------------
 *  DP_ABORT bit masks for clearing sticky errors
 * --------------------------------------------------------------------------*/
#define DP_ABORT_STKERRCLR   (1u << 5)   /* Sticky error      */
#define DP_ABORT_WDERRCLR    (1u << 4)   /* Watchdog error    */
#define DP_ABORT_ORUNERRCLR  (1u << 3)   /* Over-run error    */


/* -----------------------------------------------------------------------------
 * AP register indices used by retry helpers
 * --------------------------------------------------------------------------*/
#define AP_TAR   1   /* A3:A2 = 01b */
#define AP_DRW   3   /* A3:A2 = 11b */
#define AP_IDR   0xFC  /* Full address (read-only), useful for chip-detect */


/* -----------------------------------------------------------------------------
 *  Public helper API (return swd_error_t unless noted)
 * --------------------------------------------------------------------------*/


/* Poll CTRL/STAT until CDBGPWRUPACK and CSYSPWRUPACK are both set */
bool         SWD_PollPowerUp   (uint32_t timeout_ms);

/* AP – Target Address Register, Data Read/Write register */
swd_error_t  SWD_Write_TAR     (uint32_t address);
swd_error_t  SWD_Write_DRW     (uint32_t data);
swd_error_t  SWD_Read_DRW      (uint32_t *data_out);


/* Public helper  */
swd_error_t SWD_ClearSticky(void);

/* ------------------------------------------------------------------ */
/*  Generic “retry-on-WAIT” helpers                                   */
/* ------------------------------------------------------------------ */
#define SWD_WAIT_RETRIES_DEFAULT  50   /* 50 × 8-clocks ≈ 0.4 ms @1 MHz */

swd_error_t SWD_Write_DP_Retry(uint8_t reg, uint32_t data,int tries /* use  SWD_WAIT_RETRIES_DEFAULT */);

swd_error_t SWD_Read_DP_Retry (uint8_t  reg, uint32_t *data,int tries);

swd_error_t SWD_Write_AP_Retry(uint8_t reg, uint32_t data,int tries);

swd_error_t SWD_Read_AP_Retry (uint8_t  reg, uint32_t *data,int tries);

#endif /* SWD_DP_AP_H */






