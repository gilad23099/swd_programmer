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

#include "stm32f4xx_hal.h"
#include <stdint.h>

/* -----------------------------------------------------------------------------
 *  Pin assignment – adjust to your board wiring
 * --------------------------------------------------------------------------*/
#define PORT_SWD   GPIOB          /* GPIO port used for SWDIO + SWCLK       */
#define PIN_SWDIO  GPIO_PIN_7     /* SWDIO pin number (e.g. PB7)            */
#define PIN_SWCLK  GPIO_PIN_6     /* SWCLK pin number (e.g. PB6)            */
/* #define PIN_nRESET GPIO_PIN_4 */  /* Optional target nRESET line           */

/* -----------------------------------------------------------------------------
 *  Public API
 * --------------------------------------------------------------------------*/
void     SWD_GPIO_Init        (void);
void     SWD_Set_IO_Mode_Output(void);
void     SWD_Set_IO_Mode_Input (void);
void     SWCLK_Cycle          (void);
void     SWD_LineReset        (void);
void     delay_us             (uint32_t us);

#endif /* SWD_GPIO_H */



