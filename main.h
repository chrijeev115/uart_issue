/**
 * Copyright (c) 2018, Tiny Mesh AS - https://tiny-mesh.com
 * All rights reserved.
 */

#ifndef MAIN_H_
#define MAIN_H_

#include "event-queue.h"
#include "debug.h"

#define PROCESS(name, _ev) void name(event _ev)
#define PROCESS_BEGIN() PRINT("PROCESS - start process\n")

#define PROCESS_YIELD(_cond) { \
  if (1 == (_cond))                \
    return;                    \
}

#define PROCESS_WAIT_FOR_EVENT(t) {                      \
  if (0 == queue_peek().type || t != queue_peek().type)  \
    return;                                              \
}


#define PROCESS_END() PRINT("PROCESS - end process\n")

#define NUMARGS(...)  (sizeof((int[]){__VA_ARGS__})/sizeof(int))
#define PROCESS_REGISTER(__name__, ...) void (*__name__[ NUMARGS(__VA_ARGS__) ])() = ##__VA_ARGS__
#define NUMTASKS(t) (sizeof(t) / sizeof(t[0]))


/**
 * Called when byte is received on the UART
 *
 * Return 1 if data was successfully accepted, zero otherwise
 */
extern int recv_byte(unsigned char c);

/**
 * Prepare to receive a packet
 */
extern void reset_input(void);

/**
 * Write a single byte to UART, returns 1 if write was successful and 0
 * if write failed.
  */
extern int write_byte(char c);

/**
 * Enable raw mode for termios (read: linux) terminals
 */
extern void set_term_raw(void);

#endif /* MAIN_H_ */
