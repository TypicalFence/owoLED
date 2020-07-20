CC=avr-gcc
MCU=atmega328p
CLOCK=16000000

CFLAGS := -std=gnu99 -Os -Wall -ffunction-sections -fdata-sections -mmcu=$(MCU) -DF_CPU=$(CLOCK)
CFLAGS += -I ./include

SOURCES := $(wildcard src/*.c src/*/*.c)
HEADERS := $(wildcard src/*.h src/*/*.h)
OBJECTS := $(subst .c,.o, $(subst src,build, $(SOURCES)))
ASM := $(subst .c,.s, $(subst src,build, $(SOURCES)))

.Phony: clean asm

default: libowoled.a

libowoled.a: $(OBJECTS)
	avr-ar rcs libowoled.a $^

$(OBJECTS): ./build/%.o: ./src/%.c
	mkdir -p $(@D)
	$(CC) -c $< -o $@ $(CFLAGS)

$(ASM): ./build/%.s: ./src/%.c
	mkdir -p $(@D)
	$(CC) -c $< -o $@ $(CFLAGS) -S

asm: $(ASM)

clean:
	rm -f libowoled.a
	rm -rf build
	$(MAKE) -C ./examples/basic clean
