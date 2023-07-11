/*
 * lgen - logic generator
 * Generate a sequence of logic signals over n pins
 * Copyright (C) 2023 - Ethan Marshall
 */

/* config.h - compile time configuration for the system */

/*
 * Interval between each logic pin update.
 * This is not in any specific unit of time (and is not in clock cycles
 * either), but each "interval" lasts approx. 3 clock cycles.
 */
#define INTERVAL UINT16_MAX

/*
 * If nonzero, board single steps through each bit pattern. Interval is still
 * waited, but an additional wait is inserted blocking for button activity
 */
#define SINGLE_STEP 0

/*
 * I/O port to output logic signal on.
 * For AVR ATTiny8217 (test device), must be in the format PORT[A|B|C]. If your
 * AVR device has more ports, simply imagine more letters in the square
 * bracket.
 */
#define PORT PORTB

/*
 * Starting pin.
 * Start outputting the logic at the nth pin of the selected I/O port.
 * Must be an integer n, where 0 <= n <= 7.
 */
#define STPIN 1

/*
 * Number of pins to output alternating logic signals on to. Must not exceed 7 - STPIN.
 * Pins onto which logic signals will be written will be from STPIN to STPIN + NPINS - 1.
 * If zero, board halts at startup.
 */
#define NPINS 4

/*
 * If invalid configuration or a fatal error is detected at startup, panic by
 * flashing the SMT LED at PA3.
 * If disabled, board is still halted but no panic flashing occurs. This can be
 * used to clobber PA3.
 */
#define PANIC_ENABLE 1
