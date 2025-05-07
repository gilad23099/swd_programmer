/*
 * swd_bitbang.c
 *
 *  Created on: May 7, 2025
 *      Author: gw230
 */

#include "swd_bitbang.h"
#include "swd_defs.h"
#include "swd_gpio.h"

/*
 * SWD_Write_Bit
 * Writes a single bit to the SWDIO line and toggles SWCLK.
 * Parameters:
 *   bit – the bit value (0 or 1) to send.
 */
void SWD_Write_Bit(uint8_t bit) {
    HAL_GPIO_WritePin(PORT_SWD, PIN_SWDIO, bit ? GPIO_PIN_SET : GPIO_PIN_RESET);
    SWCLK_Cycle();
}

/*
 * SWD_Read_Bit
 * Reads a single bit from the SWDIO line and toggles SWCLK.
 * Returns:
 *   The bit value read from the target.
 */
uint8_t SWD_Read_Bit(void) {
    HAL_GPIO_WritePin(PORT_SWD, PIN_SWCLK, GPIO_PIN_SET);
    delay_us(1);
    uint8_t bit = HAL_GPIO_ReadPin(PORT_SWD, PIN_SWDIO);
    HAL_GPIO_WritePin(PORT_SWD, PIN_SWCLK, GPIO_PIN_RESET);
    delay_us(1);
    return bit;
}

/*
 * SWD_Write_Byte
 * Sends a byte (8 bits) to the target via SWDIO.
 * Sent LSB-first.
 * Parameters:
 *   byte – the byte value to transmit.
 */
void SWD_Write_Byte(uint8_t byte) {
    for (int i = 0; i < 8; i++) {
        SWD_Write_Bit((byte >> i) & 1);
    }
}


/*
 * SWD_Read_Byte
 * Reads a byte (8 bits) from the target via SWDIO.
 * Reads LSB-first and returns the received value.
 */
uint8_t SWD_Read_Byte(void) {
    uint8_t value = 0;
    for (int i = 0; i < 8; i++) {
        value |= (SWD_Read_Bit() << i);
    }
    return value;
}

