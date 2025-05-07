/*
 * swd_transfer.h
 *
 *  Created on: May 7, 2025
 *      Author: gw230
 */

#ifndef SWD_TRANSFER_H
#define SWD_TRANSFER_H

#include <stdint.h>
#include <stdbool.h>
#include "swd_gpio.h"
#include "errors.h"

/* ACK encodings per ADIv5 */
#define ACK_OK     0b001
#define ACK_WAIT   0b010
#define ACK_FAULT  0b100

/* Low level generic transactions */
swd_error_t SWD_WriteReg(bool ap, uint8_t reg, uint32_t data, bool ignore_ack);
swd_error_t SWD_ReadReg (bool ap, uint8_t reg, uint32_t *data, bool ignore_ack);

/* Convenience wrappers (ignore_ack = false) */
static inline swd_error_t SWD_Write_DP(uint8_t reg, uint32_t data)
{ return SWD_WriteReg(false, reg, data, false); }

static inline swd_error_t SWD_Read_DP(uint8_t reg, uint32_t *data)
{ return SWD_ReadReg(false, reg, data, false); }

static inline swd_error_t SWD_Write_AP(uint8_t reg, uint32_t data)
{ return SWD_WriteReg(true,  reg, data, false); }

static inline swd_error_t SWD_Read_AP(uint8_t reg, uint32_t *data)
{ return SWD_ReadReg(true,  reg, data, false); }

#endif /* SWD_TRANSFER_H */













//======================================================last version =============================================================//
//#ifndef SWD_TRANSFER_H
//#define SWD_TRANSFER_H
//
//
//#include <stdint.h>          // uint8_t
//#include <stdbool.h>         // bool
//#include "swd_defs.h"        // START, READ/WRITE, REG0..3, PARK
//#include "swd_bitbang.h"     // SWD_Write_Bit(), SWD_Read_Bit()
//
///**
// * @brief Send an SWD request and retry up to 3 times until successful.
// * @param request The 8-bit SWD header to send.
// * @return true if ACK == 0b001 (OK), false otherwise.
// */
//bool SWD_Send_Request_WithRetry(uint8_t request);
//
///**
// * @brief Construct an SWD request header.
// * @param apndp Use AP or DP (AP / DP macros).
// * @param rnw   Read or Write macro.
// * @param reg   One of the Registers.
// * @return Encoded 8-bit SWD request header.
// */
//uint8_t SWD_Build_Header(uint8_t apndp, uint8_t rnw, uint8_t reg);
//
//#endif // SWD_TRANSFER_H
