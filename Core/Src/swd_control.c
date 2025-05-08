/*
 * swd_control.c
 *
 *  Created on: May 7, 2025
 *      Author: gw230
 */
/* -----------------------------------------------------------------------------
 *  swd_control.c
 *  High level control sequence (init, halt, power‑up) – new SWD API
 * ---------------------------------------------------------------------------*/
#include "swd_control.h"
#include "swd_dp_ap.h"     /* DP/AP helpers */
#include "swd_gpio.h"      /* WRITE_BIT / SWD_LineReset */
#include "swd_transfer.h"
#include "swd_bitbang.h"
#include <stdio.h>
/* -----------------------------------------------------------------------------
 *  Send 16 bit SWJ sequence 0xE79E  (switch JTAG ➜ SWD)
 * --------------------------------------------------------------------------*/
static void SWD_Send_SwitchSequence(void)
{
    const uint16_t seq = 0xE79E;
    for (int i = 0; i < 16; ++i)
    	SWD_Write_Bit((seq >> i) & 1);
}

/* -----------------------------------------------------------------------------
 *  Read 32 bit IDCODE (DP register 0x00)
 * --------------------------------------------------------------------------*/
static swd_error_t SWD_Read_IDCODE(uint32_t *idcode)
{
    return SWD_Read_DP(0x0, idcode);   /* A3:A2 = 00b */
}

/* -----------------------------------------------------------------------------
 *  Halt target core via DHCSR (0xE000EDF0)
 * --------------------------------------------------------------------------*/
void SWD_Halt_Target(void)
{
    SWD_Write_TAR(0xE000EDF0);           /* DHCSR address */
    SWD_Write_DRW(0xA05F0003);           /* HALT + DBGKEY */
}


/* --------------------------------------------------------------- */
void SWD_Run_Target(void)
{
    /* DHCSR address = 0xE000EDF0 ; value = DBGKEY | C_DEBUGEN */
    SWD_Write_TAR(0xE000EDF0);
    SWD_Write_DRW(0xA05F0001);          /* clear C_HALT, keep DEBUGEN */
}

/* --------------------------------------------------------------- */
void SWD_Target_HW_Reset(void)
{
#ifdef PIN_nRESET
    HAL_GPIO_WritePin(PORT_SWD, PIN_nRESET, GPIO_PIN_RESET);
    HAL_Delay(20);                      /* 20 ms low */
    HAL_GPIO_WritePin(PORT_SWD, PIN_nRESET, GPIO_PIN_SET);
    HAL_Delay(20);                      /* allow target boot */
#endif
}

/* -----------------------------------------------------------------------------
 *  Power up debug interface (CDBGPWRUPREQ | CSYSPWRUPREQ)
 * --------------------------------------------------------------------------*/
static swd_error_t SWD_PowerUpDebugInterface(void)
{
    return SWD_Write_DP(DP_CTRL_STAT, (1u << 28) | (1u << 30));
}

/* -----------------------------------------------------------------------------
 *  Select default AHB AP  (APSEL = 0, APBANKSEL = 0)
 * --------------------------------------------------------------------------*/
static swd_error_t SWD_Select_AHB_AP(void)
{
    return SWD_Write_DP(DP_SELECT, 0x00000000);
}

/* -----------------------------------------------------------------------------
 *  Public init sequence – returns swd_error_t
 * --------------------------------------------------------------------------*/
swd_error_t SWD_Init(void)
{
    SWD_GPIO_Init();
    HAL_Delay(10);

    /* Line reset and switch JTAG ➜ SWD */
    SWD_LineReset();
    SWD_Send_SwitchSequence();
    SWD_LineReset();

    /* Read IDCODE */
    uint32_t id;
    if (SWD_Read_IDCODE(&id) != SWD_ERROR_OK) {
        printf("[SWD] Failed to read IDCODE\n");
        return SWD_ERROR_FAULT;
    }
    printf("[SWD] IDCODE = 0x%08lX\n", id);

    /* Request debug power up */
    if (SWD_PowerUpDebugInterface() != SWD_ERROR_OK) {
        printf("[SWD] CTRL/STAT write failed\n");
        return SWD_ERROR_FAULT;
    }

    /* Wait for power up ACK */
    if (!SWD_PollPowerUp(1000)) {
        printf("[SWD] Power‑up timeout\n");
        return SWD_ERROR_WAIT;
    }

    if (SWD_ClearSticky() != SWD_ERROR_OK) {
        printf("[SWD] Failed to clear sticky flags\\n");
        return SWD_ERROR_FAULT;
    }

    /* Default AP selection */
    if (SWD_Select_AHB_AP() != SWD_ERROR_OK) {
        printf("[SWD] SELECT write failed\n");
        return SWD_ERROR_FAULT;
    }

    HAL_Delay(10);
    return SWD_ERROR_OK;
}










