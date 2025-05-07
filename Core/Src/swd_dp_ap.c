/*
 * swd_dp_ap.c
 *
 *  Created on: May 7, 2025
 *      Author: gw230
 */

#include <stdio.h>
#include "swd_dp_ap.h"

//---------------------------DP-------------------------------//


// Write to a Debug Port register (DP)
void SWD_Write_DP(uint8_t reg_address_name, uint32_t data) {
	uint8_t header=0;
	header=SWD_Build_Header(DP,WRITE,reg_address_name);
    if (!SWD_Send_Request_WithRetry(header)) return;

    SWD_Set_IO_Mode_Output();
    for (int i = 0; i < 32; i++) SWD_Write_Bit((data >> i) & 1);
    SWD_Write_Parity(data);
}

/**
 * SWD_Read_DP
 * Reads a 32-bit Debug Port register via SWD using the generic header builder.
 */
bool SWD_Read_DP(uint8_t reg_address_name, uint32_t *data_out) {
    // Build SWD header for DP read
    uint8_t header = SWD_Build_Header(DP, READ, reg_address_name);
    if (!SWD_Send_Request_WithRetry(header)) return false;

    // Read 32 data bits LSB-first
    uint32_t val = 0;
    for (int i = 0; i < 32; ++i) {
        val |= (SWD_Read_Bit() << i);
    }
    // Read and verify parity
    if (SWD_Read_Bit() != (__builtin_parity(val) & 1)) return false;
    *data_out = val;
    return true;

}

/**
 * SWD_PollPowerUp
 * Polls CTRL/STAT (DP 0x04->DP_CTRL_STAT) until power-up ACK bits set or timeout.
 */
bool SWD_PollPowerUp(int timeout) {
    uint32_t stat;
    while (timeout-- > 0) {
        if (!SWD_Read_DP(DP_CTRL_STAT, &stat)) continue;

        // Check CDBGPWRUPACK (bit 29) and CSYSPWRUPACK (bit 31)
        if ((stat & (1u << 29)) && (stat & (1u << 31))) {
            return true;
        }
    }
    return false;
}



//---------------------------AP-------------------------------//


/*
 * SWD_Write_TAR
 * Writes an address to the Target Address Register (TAR) in the AP.
 * Parameters:
 *   addr – 32-bit memory address to be targeted.
 */
void SWD_Write_TAR(uint32_t addr) {
    uint8_t request = 0b10010011;  // [Start=1][AP/DP=1][RnW=0][A2=0][A3=1][Parity=0][Stop=0][Park=1]
    if (!SWD_Send_Request_WithRetry(request)) return;
    SWD_Set_IO_Mode_Output();
    for (int i = 0; i < 32; i++) SWD_Write_Bit((addr >> i) & 1);
    SWD_Write_Parity(addr);
}

/*
 * SWD_Write_DRW
 * Writes data to the Data Read/Write register (DRW) in the AP.
 * Requires TAR to be set beforehand.
 * Parameters:
 *   data – 32-bit value to write.
 */
void SWD_Write_DRW(uint32_t data) {
	uint8_t request = 0b10111011;  // [Start=1][AP/DP=1][RnW=0][A2=1][A3=1][Parity=1][Stop=0][Park=1]
    if (!SWD_Send_Request_WithRetry(request)) return;
    SWD_Set_IO_Mode_Output();
    for (int i = 0; i < 32; i++) SWD_Write_Bit((data >> i) & 1);
    SWD_Write_Parity(data);
}



uint8_t SWD_Read_DRW(uint32_t *data_out) {
	// ignore first call
    if (!SWD_Send_Request_WithRetry(0b10111011)) return 0;
    for (int i = 0; i < 32; i++) SWD_Read_Bit();
    SWD_Read_Bit(); // discard parity

    // second call - real value
    if (!SWD_Send_Request_WithRetry(0b10111011)) return 0;
    uint32_t data = 0;
    for (int i = 0; i < 32; i++) data |= (SWD_Read_Bit() << i);

    uint8_t parity_bit = SWD_Read_Bit();
    if (__builtin_parity(data) != parity_bit) {
            printf("Parity error on DRW read: 0x%08lX\n", data);
            return 0;
    }
    *data_out = data;
    return 1;
}

