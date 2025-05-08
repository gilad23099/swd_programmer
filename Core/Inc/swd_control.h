/*
 * swd_control.h
 *
 * Created on: May 7, 2025
 * Author: gw230
 *
 * Header for SWD control and initialization functions.
 */

#ifndef SWD_CONTROL_H
#define SWD_CONTROL_H

#include "errors.h"         /* swd_error_t */
#include "stm32f4xx_hal.h"  /* HAL_Delay   */

/* -----------------------------------------------------------------------------
 *  Public API
 * --------------------------------------------------------------------------*/

/**
 * Initialise SWD link: line‑reset, switch sequence, power‑up, AHB‑AP select.
 * @return SWD_ERROR_OK on success, otherwise a specific swd_error_t code.
 */
swd_error_t SWD_Init(void);

/**
 * Halt the target CPU by writing to DHCSR (0xE000EDF0).
 */
void  SWD_Halt_Target(void);

/**
 * Release the CPU from halt (write 0xA05F0001 to DHCSR).
 */
void SWD_Run_Target(void);

/**
 * Pulse nRESET low → high (requires PIN_nRESET).
 */
void SWD_Target_HW_Reset(void);

#endif /* SWD_CONTROL_H */






