

#ifndef SWD_ERROR_H_
#define SWD_ERROR_H_

/* errors.h – unified SWD error codes -----------------------------------*/
#include <stdint.h>

/* Return values for low level SWD transactions */
typedef enum {
    SWD_ERROR_OK       = 0,   /* ACK = 0b001 */
    SWD_ERROR_WAIT     = 1,   /* ACK = 0b010 (target busy) */
    SWD_ERROR_FAULT    = 2,   /* ACK = 0b100 (sticky fault) */
    SWD_ERROR_PROTOCOL = 3,   /* Line not driven or framing error */
    SWD_ERROR_PARITY   = 4    /* Wrong parity bit on read */
} swd_error_t;

/* Convenience macro for run time logging (can redirect later) */
#ifndef SWD_LOG
#  include <stdio.h>
#  define SWD_LOG(...)   printf(__VA_ARGS__)
#endif


#endif
