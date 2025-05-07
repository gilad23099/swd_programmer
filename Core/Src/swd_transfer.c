/*
 * swd_transfer.c
 *
 *  Created on: May 7, 2025
 *      Author: gw230
 */

#include "swd_transfer.h"
#include "errors.h"

/* -----------------------------------------------------------------------------
 *  Local bit helpers (no clash with HAL READ_BIT macro)
 * --------------------------------------------------------------------------*/
#define START_BIT   1
#define STOP_BIT    0
#define PARK_BIT    1

#define SWD_WRITE_BIT(b)  do { \
        HAL_GPIO_WritePin(PORT_SWD, PIN_SWDIO, (b) ? GPIO_PIN_SET : GPIO_PIN_RESET); \
        SWCLK_Cycle(); \
    } while (0)

#define SWD_READ_BIT(bit_var) do { \
        SWCLK_Cycle(); \
        bit_var = HAL_GPIO_ReadPin(PORT_SWD, PIN_SWDIO); \
    } while (0)

static inline void idle_cycles(uint8_t n)
{
    SWD_Set_IO_Mode_Output();
    while (n--) SWD_WRITE_BIT(0);
}

/* -----------------------------------------------------------------------------
 *  Generic write (AP/DP) – returns swd_error_t
 * --------------------------------------------------------------------------*/
swd_error_t SWD_WriteReg(bool ap, uint8_t reg, uint32_t data, bool ignore_ack)
{
    uint8_t A2 =  reg       & 1;
    uint8_t A3 = (reg >> 1) & 1;
    uint8_t head_parity = (ap + /*RnW=*/0 + A2 + A3) & 1;

    /* Header (host ➜ target) */
    SWD_Set_IO_Mode_Output();
    SWD_WRITE_BIT(START_BIT);
    SWD_WRITE_BIT(ap);
    SWD_WRITE_BIT(0);            /* RnW = 0 */
    SWD_WRITE_BIT(A2);
    SWD_WRITE_BIT(A3);
    SWD_WRITE_BIT(head_parity);
    SWD_WRITE_BIT(STOP_BIT);
    SWD_WRITE_BIT(PARK_BIT);

    /* Turn‑around (host→target) */
    SWD_Set_IO_Mode_Input();
    SWCLK_Cycle();

    /* Read ACK */
    uint32_t ack = 0, bit;
    for (int i = 0; i < 3; ++i) { SWD_READ_BIT(bit); ack |= bit << i; }

    if (ack != ACK_OK && !ignore_ack) {
        return (ack == ACK_WAIT)  ? SWD_ERROR_WAIT   :
               (ack == ACK_FAULT) ? SWD_ERROR_FAULT  : SWD_ERROR_PROTOCOL;
    }

    /* Turn‑around (target→host) */
    SWCLK_Cycle();
    SWD_Set_IO_Mode_Output();

    /* Send data + parity */
    uint8_t parity = 0;
    for (int i = 0; i < 32; ++i) {
        bit = (data >> i) & 1; SWD_WRITE_BIT(bit); parity ^= bit; }
    SWD_WRITE_BIT(parity);

    idle_cycles(8);
    return SWD_ERROR_OK;
}

/* -----------------------------------------------------------------------------
 *  Generic read (AP/DP)
 * --------------------------------------------------------------------------*/
swd_error_t SWD_ReadReg(bool ap, uint8_t reg, uint32_t *data, bool ignore_ack)
{
    uint8_t A2 =  reg       & 1;
    uint8_t A3 = (reg >> 1) & 1;
    uint8_t head_parity = (ap + /*RnW=*/1 + A2 + A3) & 1;

    SWD_Set_IO_Mode_Output();
    SWD_WRITE_BIT(START_BIT);
    SWD_WRITE_BIT(ap);
    SWD_WRITE_BIT(1);            /* RnW = 1 */
    SWD_WRITE_BIT(A2);
    SWD_WRITE_BIT(A3);
    SWD_WRITE_BIT(head_parity);
    SWD_WRITE_BIT(STOP_BIT);
    SWD_WRITE_BIT(PARK_BIT);

    /* Turn‑around */
    SWD_Set_IO_Mode_Input();
    SWCLK_Cycle();

    uint32_t ack = 0, bit;
    for (int i = 0; i < 3; ++i) { SWD_READ_BIT(bit); ack |= bit << i; }

    if (ack != ACK_OK && !ignore_ack) {
        return (ack == ACK_WAIT)  ? SWD_ERROR_WAIT   :
               (ack == ACK_FAULT) ? SWD_ERROR_FAULT  : SWD_ERROR_PROTOCOL;
    }

    /* Read 32‑bit data */
    uint32_t val = 0; uint8_t parity = 0;
    for (int i = 0; i < 32; ++i) { SWD_READ_BIT(bit); val |= bit << i; parity ^= bit; }

    /* Parity bit */
    uint8_t parity_bit; SWD_READ_BIT(parity_bit);
    if (parity != parity_bit) { idle_cycles(8); return SWD_ERROR_PARITY; }

    *data = val;
    idle_cycles(8);
    return SWD_ERROR_OK;
}






//======================================================last version =============================================================//

//#include "swd_transfer.h"
//#include "errors.h"

/*
 * SWD_Send_Request_WithRetry
 * Sends an SWD request byte and retries up to 3 times if ACK is not OK.
 * Parameters:
 *   request – the 8-bit SWD request header
 * Returns:
 *   true if ACK == 0b001 (OK), false otherwise
 */
//bool SWD_Send_Request_WithRetry(uint8_t request) {
//    uint8_t ack = 0xFF;
//    for (int retry = 0; retry < 3; retry++) {
//        SWD_Set_IO_Mode_Output();
//        SWD_Write_Byte(request);
//        SWD_Set_IO_Mode_Input();
//        ack = SWD_Read_Byte();
//        if (ack == 0b001) return true;
//    }
//    printf("SWD request 0x%02X failed: ACK = 0x%02X after 3 attempts\n", request, ack);
//    return false;
//}
//
//
///*
// * SWD_Build_Header
// * Builds an 8-bit SWD request header for DP/AP read/write transactions.
// * Combines START, AP/DP, RnW, register address bits, parity, and PARK.
// */
//uint8_t SWD_Build_Header(uint8_t apndp, uint8_t rnw, uint8_t reg) {
//    uint8_t header = 0;
//    // Start bit
//    header |= START;
//    // APnDP and RnW bits
//    header |= apndp;
//    header |= rnw;
//    header |= reg;
//    // Parity over bits APnDP, RnW, A2, A3
//    header |= __builtin_parity(header>>1)<<5;
//    // Stop bit always 0, Park bit set
//    header |= PARK;
//    return header;
//}

