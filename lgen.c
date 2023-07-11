/*
 * lgen - logic generator
 * Generate a sequence of logic signals over n pins
 * Copyright (C) 2023 - Ethan Marshall
 */

#include <stdint.h>
#include <avr/io.h>

#include "config.h"

/* panic constants */
#define PANIC_PORT PORTA
#define PANIC_BIT  (1 << 3)

/* button in constants */
#define BTN_PORT PORTB
#define BTN_BIT  (1 << 7)

/*
 * maximum value which can be outputted by the logic pins configured by the user.
 */
static const uint8_t outmax = ((1 << NPINS) - 1) << STPIN;

/*
 * Delay for approx. n clock cycles. Error rate is quite high, so timing will not be precise!
 */
static void delay(uint16_t n)
{
	for (uint16_t i = 0; i < n; i++)
		asm ("nop");
}

static void panic()
{
	/* now we can setup panic bit */
	PANIC_PORT.DIRSET = PANIC_BIT;

	for (;;) {
		if (PANIC_ENABLE) {
			PANIC_PORT.OUTSET = PANIC_BIT;
			delay(UINT16_MAX);
			PANIC_PORT.OUTCLR = PANIC_BIT;
			delay(UINT16_MAX);
		}
	}

	/* just in case */
	panic();
}

static void io_init()
{
	/* all ports touched by logic signals should be outputs
	 * else mark as inputs to avoid accidental writes */
	PORT.DIRSET = outmax;
	PORT.DIRCLR = ~outmax;

	/* mark button as input */
	BTN_PORT.DIRCLR = BTN_BIT;

	/*
	 * set user ports low at startup
	 */
	PORT.OUTCLR = UINT8_MAX;
}

static void block_pause()
{
	/* note: this needs to be a busy loop! */
	if (!(BTN_PORT.IN & BTN_BIT) || SINGLE_STEP) {
		/* block until depressed */
		while (!(BTN_PORT.IN & BTN_BIT))
			;

		/* block until pressed again */
		while (BTN_PORT.IN & BTN_BIT)
			;
	}
}

int main(void)
{
	uint8_t sstep = 0;
	uint8_t state;

	/* startup configuration validation */
	if (STPIN < 0 || STPIN > 7)
		panic();
	if (NPINS == 0 || (uint16_t)7 - (uint16_t)STPIN < NPINS - 1)
		panic();

	io_init();

	for (;;) {
		state = 0;
		do {
			state++;
			PORT.OUTSET = state << STPIN;
			PORT.OUTCLR = ~state << STPIN;
			delay(INTERVAL);

			block_pause();
		} while ((state << STPIN) < outmax);

		PORT.OUTCLR = outmax << STPIN - 1;
		delay(INTERVAL);
		block_pause();
	}
}
