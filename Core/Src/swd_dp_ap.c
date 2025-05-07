/*
 * swd_dp_ap.c
 *
 *  Created on: May 7, 2025
 *      Author: gw230
 */
#include "swd_dp_ap.h"
/* ---------------------------------------------------------------------------
 *  Helper macros – AP register indices (A[3:2])
 *  0 = CSW / IDR , 1 = TAR , 2 = reserved , 3 = DRW
 *---------------------------------------------------------------------------*/
#define AP_TAR   1    /* A3:A2 = 01b */
#define AP_DRW   3    /* A3:A2 = 11b */


/* ================================================================== */
/*   Retry wrappers – repeat until ACK-OK, or first non-WAIT error     */
/* ================================================================== */
swd_error_t SWD_Write_DP_Retry(uint8_t reg, uint32_t data, int tries)
{
    while (tries--) {
        swd_error_t e = SWD_Write_DP(reg, data);
        if (e == SWD_ERROR_OK)   return e;
        if (e != SWD_ERROR_WAIT) return e;      /* FAULT / PROTOCOL / PARITY */
    }
    return SWD_ERROR_WAIT;
}

swd_error_t SWD_Read_DP_Retry(uint8_t reg, uint32_t *data, int tries)
{
    while (tries--) {
        swd_error_t e = SWD_Read_DP(reg, data);
        if (e == SWD_ERROR_OK)   return e;
        if (e != SWD_ERROR_WAIT) return e;
    }
    return SWD_ERROR_WAIT;
}

swd_error_t SWD_Write_AP_Retry(uint8_t reg, uint32_t data, int tries)
{
    while (tries--) {
        swd_error_t e = SWD_Write_AP(reg, data);
        if (e == SWD_ERROR_OK)   return e;
        if (e != SWD_ERROR_WAIT) return e;
    }
    return SWD_ERROR_WAIT;
}

swd_error_t SWD_Read_AP_Retry(uint8_t reg, uint32_t *data, int tries)
{
    while (tries--) {
        swd_error_t e = SWD_Read_AP(reg, data);
        if (e == SWD_ERROR_OK)   return e;
        if (e != SWD_ERROR_WAIT) return e;
    }
    return SWD_ERROR_WAIT;
}



/* ---------------------------------------------------------------------------
 *  Poll DP_CTRL_STAT for power-up acknowledge
 *---------------------------------------------------------------------------*/
bool SWD_PollPowerUp(uint32_t timeout_ms)
{
    uint32_t stat;

    while (timeout_ms--)
    {
        if (SWD_Read_DP(DP_CTRL_STAT, &stat) != SWD_ERROR_OK)
            continue;                       /* retry on WAIT / FAULT */

        /* CDBGPWRUPACK (bit 29) && CSYSPWRUPACK (bit 31) */
        if ( (stat & (1u << 29)) && (stat & (1u << 31)) )
            return true;

        delay_us(1000);                     /* ~1 ms back-off   */
    }
    return false;                            /* timeout          */
}

/* ---------------------------------------------------------------------------
 *  Write Target Address Register (AP-TAR)
 *---------------------------------------------------------------------------*/
swd_error_t SWD_Write_TAR(uint32_t addr)
{
    return SWD_Write_AP(AP_TAR, addr);       /* returns SWD_ERROR_* */
}

/* ---------------------------------------------------------------------------
 *  Write Data Read/Write register (AP-DRW)
 *---------------------------------------------------------------------------*/
swd_error_t SWD_Write_DRW(uint32_t data)
{
    return SWD_Write_AP(AP_DRW, data);
}

/* ---------------------------------------------------------------------------
 *  Read AP-DRW with ARM two-phase pipeline
 *     first read is dummy, second read gets real data
 *---------------------------------------------------------------------------*/
swd_error_t SWD_Read_DRW(uint32_t *data_out)
{
    uint32_t dummy;

    /* dummy read (value is discarded) */
    if (SWD_Read_AP(AP_DRW, &dummy) != SWD_ERROR_OK)
        return SWD_ERROR_FAULT;             /* or whatever you prefer */

    /* real read */
    return SWD_Read_AP(AP_DRW, data_out);
}


/* -----------------  Clear sticky error flags in DP --------------------
 *  Must be called after any transaction that returns ACK_FAULT,
 *  OR once straight after power-up (recommended).
 * -------------------------------------------------------------------*/
swd_error_t SWD_ClearSticky(void)
{
    /* DP_ABORT is register index 0 (A3:A2 = 00b)                        *
     * We write a 1 to every “*_CLR” bit we want to clear.                */
    uint32_t value = DP_ABORT_STKERRCLR |
                     DP_ABORT_WDERRCLR  |
                     DP_ABORT_ORUNERRCLR;

    return SWD_Write_DP(/*reg=*/0, value);
}






//------------------------------------------------try-------------------------------------//












