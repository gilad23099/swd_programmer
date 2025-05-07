/*
 * swd_bitbang.h
 *
 * Bit-level primitives for SWD protocol (write/read single bits and bytes).
 *
 * Created on: May 7, 2025
 * Author: gw230
 */

#ifndef SWD_BITBANG_H
#define SWD_BITBANG_H

#include <stdint.h>

/**
 * @brief Write a single bit to SWDIO and toggle SWCLK.
 * @param bit The bit to write (0 or 1).
 */
void SWD_Write_Bit(uint8_t bit);

/**
 * @brief Read a single bit from SWDIO and toggle SWCLK.
 * @return The bit value read.
 */
uint8_t SWD_Read_Bit(void);

/**
 * @brief Write a byte (LSB-first) to SWDIO.
 * @param byte Byte to write.
 */
void SWD_Write_Byte(uint8_t byte);

/**
 * @brief Read a byte (LSB-first) from SWDIO.
 * @return Byte read from target.
 */
uint8_t SWD_Read_Byte(void);

#endif /* SWD_BITBANG_H */





