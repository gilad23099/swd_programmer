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

#include <stdint.h>          // uint8_t,uint32_t
#include <stdbool.h>         // bool
#include "swd_defs.h"        // DP/​AP macros
#include "swd_transfer.h"    // SWD_Build_Header, SWD_Send_Request_WithRetry
#include "swd_bitbang.h"     // SWD_Write_Bit, SWD_Read_Bit

//--------------------------- DP Functions -------------------------------//

/**
 * @brief Write a 32-bit value to a Debug Port register.
 * @param addr Address of the DP register.
 * @param data Data to write.
 */
void SWD_Write_DP(uint8_t addr, uint32_t data);

/**
 * @brief Read a 32-bit value from a Debug Port register.
 * @param reg Register (REG0–REG3).
 * @param data_out Pointer to store the read value.
 * @return true on success, false on error.
 */
bool SWD_Read_DP(uint8_t reg, uint32_t *data_out);

/**
 * @brief Poll CTRL/STAT register until power-up ACK bits are set.
 * @param timeout Number of polling attempts before giving up.
 * @return true if power-up bits are acknowledged, false otherwise.
 */
bool SWD_PollPowerUp(int timeout);

//--------------------------- AP Functions -------------------------------//

/**
 * @brief Write to the AP Target Address Register (TAR).
 * @param addr 32-bit target memory address.
 */
void SWD_Write_TAR(uint32_t addr);

/**
 * @brief Write to the AP Data Read/Write Register (DRW).
 * @param data 32-bit data to write.
 */
void SWD_Write_DRW(uint32_t data);

/**
 * @brief Read from the AP Data Read/Write Register (DRW).
 * @param data_out Pointer to store the read data.
 * @return 1 if success, 0 on error.
 */
uint8_t SWD_Read_DRW(uint32_t *data_out);

#endif // SWD_DP_AP_H
