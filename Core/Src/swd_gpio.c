/*
 * swd_gpio.c
 *
 *  Created on: May 7, 2025
 *      Author: gw230
 */
#include "swd_gpio.h"

/*
 * delay_short
 * Generates a short delay using a no-operation loop.
 * Used to synchronize SWCLK clock edges in SWD communication.
 */
void delay_short(void) {
    for (volatile int i = 0; i < 50; i++) __NOP();
}

/*
 * SWD_Set_IO_Mode_Output
 * Configures the SWDIO pin as output.
 * Required when the host needs to send bits to the target (write phase).
 */
void SWD_Set_IO_Mode_Output(void) {
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = PIN_SWDIO;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    HAL_GPIO_Init(PORT_SWD, &GPIO_InitStruct);
}

/*
 * SWD_Set_IO_Mode_Input
 * Configures the SWDIO pin as input.
 * Required when the host expects to receive ACK or data from the target (read phase).
 */
void SWD_Set_IO_Mode_Input(void) {
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = PIN_SWDIO;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(PORT_SWD, &GPIO_InitStruct);
}

/*
 * SWD_GPIO_Init
 * Initializes the required GPIO pins (SWDIO and SWCLK) for SWD communication.
 * Enables the GPIO clock and sets up SWCLK as output.
 */
void SWD_GPIO_Init(void) {
    __HAL_RCC_GPIOB_CLK_ENABLE();

    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = PIN_SWCLK;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    HAL_GPIO_Init(PORT_SWD, &GPIO_InitStruct);

    SWD_Set_IO_Mode_Output();
}

/*
 * SWCLK_Cycle
 * Performs one cycle of the SWCLK line (high → low).
 * Used to clock data in/out on SWD interface.
 */
void SWCLK_Cycle(void) {
    HAL_GPIO_WritePin(PORT_SWD, PIN_SWCLK, GPIO_PIN_SET);
    delay_short();
    HAL_GPIO_WritePin(PORT_SWD, PIN_SWCLK, GPIO_PIN_RESET);
    delay_short();
}

/*
 * SWD_LineReset
 * Clocks 60 SWCLK cycles with SWDIO held high.
 * Resets SWD line to a known state before sending any SWD transactions.
 */
void SWD_LineReset(void) {
    SWD_Set_IO_Mode_Output();
    HAL_GPIO_WritePin(PORT_SWD, PIN_SWDIO, GPIO_PIN_SET); // Hold SWDIO high
    for (int i = 0; i < 60; i++) {
        SWCLK_Cycle();
    }
}

