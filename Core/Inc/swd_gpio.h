/*
 * swd_gpio.h
 *
 * Created on: May 7, 2025
 * Author: gw230
 *
 * GPIO and timing primitives for SWD bit-banging.
 */

#ifndef SWD_GPIO_H
#define SWD_GPIO_H

#include "swd_defs.h"
#include "stm32f4xx_hal.h"


/**
 * @brief  Generate a short delay (approx. one SWCLK half-period).
 */
void delay_short(void);

/**
 * @brief  Configure SWDIO pin as push-pull output.
 */
void SWD_Set_IO_Mode_Output(void);

/**
 * @brief  Configure SWDIO pin as input (high-impedance).
 */
void SWD_Set_IO_Mode_Input(void);

/**
 * @brief  Initialize SWCLK and SWDIO GPIOs for SWD operation.
 *         Enables clock and sets SWCLK to output, SWDIO to output initially.
 */
void SWD_GPIO_Init(void);

/**
 * @brief  Toggle SWCLK line once (high then low).
 */
void SWCLK_Cycle(void);

/**
 * @brief  Perform a line reset: 60 SWCLK cycles with SWDIO held high.
 */
void SWD_LineReset(void);

#endif /* SWD_GPIO_H */



