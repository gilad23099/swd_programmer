/*
 * swd_control.c
 *
 *  Created on: May 7, 2025
 *      Author: gw230
 */

#include "swd_control.h"

/*
 * SWD_Send_SwitchSequence
 * Sends a 16-bit SWJ switch sequence.
 * Used to switch the target from JTAG mode to SWD mode.
 */
void SWD_Send_SwitchSequence(void) {
    uint16_t swj = 0xE79E;
    for (int i = 0; i < 16; i++) {
        SWD_Write_Bit((swj >> i) & 1);
    }
}

/*
 * SWD_Read_IDCODE
 * Reads the 32-bit IDCODE from the Debug Port (DP register 0x00).
 * Verifies if SWD communication with target is alive.
 */
uint32_t SWD_Read_IDCODE(void) {
    uint8_t request = 0b10100101;  // [Start=1][AP/DP=0][RnW=1][A2=0][A3=0][Parity=1][Stop=0][Park=1]

    if (!SWD_Send_Request_WithRetry(request)) return 0xFFFFFFFF;

    uint32_t idcode = 0;
    for (int i = 0; i < 32; i++) {
        idcode |= (SWD_Read_Bit() << i);
    }
    SWD_Read_Bit(); // Read and discard parity bit
    return idcode;
}


/*
 * SWD_Halt_Target
 * Halts the target CPU by writing to the DHCSR register.
 * Required for safe flash access.
 */
void SWD_Halt_Target(void) {
    SWD_Write_TAR(0xE000EDF0);  // write the address of DHCSR to the TAR
    SWD_Write_DRW(0xA05F0003);  // write HALT + DEBUGKEY to DRW
}


// Power-up debug interface via CTRL/STAT
void SWD_PowerUpDebugInterface(void) {
    // CDBGPWRUPREQ (bit 28), CSYSPWRUPREQ (bit 30)
    SWD_Write_DP(DP_CTRL_STAT, (1 << 28) | (1 << 30));
}


// Select AHB-AP (APSEL=0, APBANKSEL=0)
void SWD_Select_AHB_AP(void) {
    SWD_Write_DP(DP_SELECT, 0x00000000); // SELECT register
}

// Unified init sequence including everything
void SWD_Init(void) {
    SWD_GPIO_Init();
    HAL_Delay(10);

    SWD_LineReset();
    SWD_Send_SwitchSequence();
    SWD_LineReset();

    uint32_t id = SWD_Read_IDCODE();
    printf("[SWD] IDCODE = 0x%08lX\n", id);

    SWD_PowerUpDebugInterface();

    // Poll the CTRL/STAT register until both power-up ACK bits are set
    if (!SWD_PollPowerUp(1000)) {
        printf("SWD_Init: Power-up timeout!\n");
        return;
    }

    SWD_Select_AHB_AP();

    HAL_Delay(10);
}










