CC=arduino-cli
BOARD=arduino:avr:nano
TARGET=sequencer/
SERIAL_PORT=/dev/ttyUSB0

build:
	${CC} compile --fqbn ${BOARD} ${TARGET}

upload: build
	${CC} upload -p ${SERIAL_PORT} --fqbn ${BOARD} ${TARGET}
