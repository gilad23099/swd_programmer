/*
 * swd_transfer.c
 *
 *  Created on: May 7, 2025
 *      Author: gw230
 */

#include "swd_transfer.h"

/*
 * SWD_Send_Request_WithRetry
 * Sends an SWD request byte and retries up to 3 times if ACK is not OK.
 * Parameters:
 *   request – the 8-bit SWD request header
 * Returns:
 *   true if ACK == 0b001 (OK), false otherwise
 */
bool SWD_Send_Request_WithRetry(uint8_t request) {
    uint8_t ack = 0xFF;
    for (int retry = 0; retry < 3; retry++) {
        SWD_Set_IO_Mode_Output();
        SWD_Write_Byte(request);
        SWD_Set_IO_Mode_Input();
        ack = SWD_Read_Byte();
        if (ack == 0b001) return true;
    }
    printf("SWD request 0x%02X failed: ACK = 0x%02X after 3 attempts\n", request, ack);
    return false;
}


/*
 * SWD_Build_Header
 * Builds an 8-bit SWD request header for DP/AP read/write transactions.
 * Combines START, AP/DP, RnW, register address bits, parity, and PARK.
 */
uint8_t SWD_Build_Header(uint8_t apndp, uint8_t rnw, uint8_t reg) {
    uint8_t header = 0;
    // Start bit
    header |= START;
    // APnDP and RnW bits
    header |= apndp;
    header |= rnw;
    header |= reg;
    // Parity over bits APnDP, RnW, A2, A3
    header |= __builtin_parity(header>>1)<<5;
    // Stop bit always 0, Park bit set
    header |= PARK;
    return header;
}

