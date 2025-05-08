/*
 * swd_gpio.c
 *
 *  Created on: May 7, 2025
 *      Author: gw230
 */
#include "swd_gpio.h"
#include "stm32f4xx_hal.h"

/* -----------------------------------------------------------------------------
 *  DWT based micro second delay
 * --------------------------------------------------------------------------*/
static void enable_dwt_cycle_counter(void)
{
    /* Enable DWT CYCCNT for high resolution timing */
    if (!(CoreDebug->DEMCR & CoreDebug_DEMCR_TRCENA_Msk))
        CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;

    DWT->CYCCNT = 0;
    DWT->CTRL  |= DWT_CTRL_CYCCNTENA_Msk;
}

void delay_us(uint32_t us)
{
    const uint32_t hclk   = HAL_RCC_GetHCLKFreq();     /* Core frequency (Hz)   */
    const uint32_t cycles = (hclk / 1000000UL) * us;
    uint32_t start = DWT->CYCCNT;

    while ((DWT->CYCCNT - start) < cycles) __NOP();
}

/* -----------------------------------------------------------------------------
 *  GPIO configuration helpers
 * --------------------------------------------------------------------------*/
void SWD_Set_IO_Mode_Output(void)
{
    GPIO_InitTypeDef cfg = {0};
    cfg.Pin   = PIN_SWDIO;
    cfg.Mode  = GPIO_MODE_OUTPUT_PP;
    cfg.Pull  = GPIO_NOPULL;
    cfg.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    HAL_GPIO_Init(PORT_SWD, &cfg);
}

void SWD_Set_IO_Mode_Input(void)
{
    GPIO_InitTypeDef cfg = {0};
    cfg.Pin  = PIN_SWDIO;
    cfg.Mode = GPIO_MODE_INPUT;
    cfg.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(PORT_SWD, &cfg);
}

void SWD_GPIO_Init(void)
{
    /* Enable GPIO clock (change port if PORT_SWD != GPIOB) */
    __HAL_RCC_GPIOB_CLK_ENABLE();

    GPIO_InitTypeDef cfg = {0};

    /* SWCLK configured as push pull output */
    cfg.Pin   = PIN_SWCLK;
    cfg.Mode  = GPIO_MODE_OUTPUT_PP;
    cfg.Pull  = GPIO_NOPULL;
    cfg.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    HAL_GPIO_Init(PORT_SWD, &cfg);

    /* SWDIO default high (line idle) */
    SWD_Set_IO_Mode_Output();
    HAL_GPIO_WritePin(PORT_SWD, PIN_SWDIO, GPIO_PIN_SET);

#ifdef PIN_nRESET
    /* nRESET as open-drain output, start HIGH (released) */
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin   = PIN_nRESET;
    GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_OD;
    GPIO_InitStruct.Pull  = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(PORT_SWD, &GPIO_InitStruct);
    HAL_GPIO_WritePin(PORT_SWD, PIN_nRESET, GPIO_PIN_SET);
#endif

    enable_dwt_cycle_counter();
}

/* -----------------------------------------------------------------------------
 *  SWCLK pulse helper (high → low)
 * --------------------------------------------------------------------------*/
void SWCLK_Cycle(void)
{
    HAL_GPIO_WritePin(PORT_SWD, PIN_SWCLK, GPIO_PIN_SET);
    delay_us(1);                   /* ≈500 kHz clock (2 µs period) */
    HAL_GPIO_WritePin(PORT_SWD, PIN_SWCLK, GPIO_PIN_RESET);
    delay_us(1);
}

/* -----------------------------------------------------------------------------
 *  Line reset: 60 clock cycles, SWDIO kept high
 * --------------------------------------------------------------------------*/
void SWD_LineReset(void)
{
    SWD_Set_IO_Mode_Output();
    HAL_GPIO_WritePin(PORT_SWD, PIN_SWDIO, GPIO_PIN_SET);

    for (uint8_t i = 0; i < 60; ++i)
        SWCLK_Cycle();
}
















