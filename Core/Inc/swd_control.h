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


#include <stdint.h>          // uint32_t
#include <stdbool.h>         // bool
#include <stdio.h>           // printf (ל־SWD_Read_IDCODE debug)
#include "swd_defs.h"        // PIN_SWDIO, FLASH_BASE, DP/AP macros
#include "swd_gpio.h"        // SWD_GPIO_Init, SWD_LineReset, delay_short
#include "swd_bitbang.h"     // SWD_Write_Bit, SWD_Read_Bit
#include "swd_transfer.h"    // SWD_Send_Request_WithRetry, SWD_Build_Header
#include "swd_dp_ap.h"       // SWD_Write_DP, SWD_Read_DP

/**
 * @brief Sends a 16-bit SWJ switch sequence (JTAG to SWD).
 */
void SWD_Send_SwitchSequence(void);

/**
 * @brief Reads the 32-bit IDCODE from the DP register.
 * @return The IDCODE value, or 0xFFFFFFFF on failure.
 */
uint32_t SWD_Read_IDCODE(void);

/**
 * @brief Halts the target MCU by writing to the DHCSR register.
 */
void SWD_Halt_Target(void);

/**
 * @brief Powers up the debug interface via CTRL/STAT register.
 */
void SWD_PowerUpDebugInterface(void);

/**
 * @brief Selects the AHB-AP as active AP.
 */
void SWD_Select_AHB_AP(void);

/**
 * @brief Initializes the full SWD interface and prepares target.
 */
void SWD_Init(void);

#endif // SWD_CONTROL_H
