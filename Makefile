CC=avr-gcc
MMCU=atmega328p
CLOCK=16000000

CFLAGS := -std=gnu99 -Os -Wall -ffunction-sections -fdata-sections -mmcu=$(MMCU) -DF_CPU=$(CLOCK)
CFLAGS += -I ./include

SOURCES := $(wildcard src/*.c src/*/*.c)
HEADERS := $(wildcard src/*.h src/*/*.h)
OBJECTS := $(subst .c,.o, $(subst src,build, $(SOURCES)))

.Phony: clean

default: libowoled.a

libowoled.a: $(OBJECTS)
	avr-ar rcs libowoled.a $^

$(OBJECTS): ./build/%.o: ./src/%.c
	mkdir -p $(@D)
	$(CC) -c $< -o $@ $(CFLAGS)

clean:
	rm -f libowoled.a
	rm -rf build
	$(MAKE) -C ./examples/basic clean
