#
# lgen - logic generator
# Generate a sequence of logic signals over n pins
# Copyright (C) 2023 - Ethan Marshall
#

SRC = lgen.c
HDR = config.h
OBJ = ${SRC:.c=.o}
EXE = lgen.elf

CC        = avr-gcc
FLASHER   = avrdude
BOARD     = attiny3217
AVRLIBS   = /usr/avr/lib/
BOARDLIBS = ~/dl/attiny/gcc/dev/${BOARD}
BOARDPORT = usb
BOARDPROT = pkobn_updi

all: conf ${EXE}

conf:
	@echo "BUILD CONFIGURATION"
	@echo "compiler: " ${CC}
	@echo "board name: " ${BOARD}
	@echo "library paths: " ${BOARDLIBS} ${AVRLIBS}
	@echo "flashing to: " ${BOARDPORT}
	@echo "flasher protocol: " ${BOARDPROT}
	@echo ""

${EXE}: ${OBJ}
	${CC} -o ${EXE} ${CFLAGS} ${LDFLAGS} -mmcu=${BOARD} -B ${BOARDLIBS} -B ${AVRLIBS} ${OBJ}

flash: conf ${EXE}
	${FLASHER} -p ${BOARD} -P ${BOARDPORT} -c ${BOARDPROT} -U flash:w:${EXE}:e

.c.o:
	${CC} -c ${CFLAGS} -mmcu=${BOARD} -B ${BOARDLIBS} -B ${AVRLIBS} $<

${OBJ}: ${HDR}

clean:
	rm -f *.o ${EXE}

.PHONY: all conf flash clean
