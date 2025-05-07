/*
 * swd_transfer.h
 *
 *  Created on: May 7, 2025
 *      Author: gw230
 */

#ifndef SWD_TRANSFER_H
#define SWD_TRANSFER_H


#include <stdint.h>          // uint8_t
#include <stdbool.h>         // bool
#include "swd_defs.h"        // START, READ/WRITE, REG0..3, PARK
#include "swd_bitbang.h"     // SWD_Write_Bit(), SWD_Read_Bit()

/**
 * @brief Send an SWD request and retry up to 3 times until successful.
 * @param request The 8-bit SWD header to send.
 * @return true if ACK == 0b001 (OK), false otherwise.
 */
bool SWD_Send_Request_WithRetry(uint8_t request);

/**
 * @brief Construct an SWD request header.
 * @param apndp Use AP or DP (AP / DP macros).
 * @param rnw   Read or Write macro.
 * @param reg   One of the Registers.
 * @return Encoded 8-bit SWD request header.
 */
uint8_t SWD_Build_Header(uint8_t apndp, uint8_t rnw, uint8_t reg);

#endif // SWD_TRANSFER_H
