CC=avr-gcc
MMCU=atmega328p
CLOCK=16000000
DEVICE=/dev/ttyACM0

CFLAGS := -std=gnu99 -Os -Wall -ffunction-sections -fdata-sections 
CFLAGS += -mmcu=$(MMCU) -DF_CPU=$(CLOCK) 
CFLAGS += -I ../../include 
LDFLAGS := -Os -mmcu=$(MMCU) -ffunction-sections -fdata-sections -Wl,--gc-sections 
LDFLAGS += -L ../../ -lowoled 

SOURCES := $(wildcard src/*.c src/*/*.c)
HEADERS := $(wildcard src/*.h src/*/*.h)
OBJECTS := $(subst .c,.o, $(subst src,build, $(SOURCES)))

.Phony: clean flash

default: basic.ihex

basic.ihex: basic.elf
	avr-objcopy -O ihex -R .eeprom basic.elf basic.ihex

basic.elf: $(OBJECTS) ../../libowoled.a
	$(CC) -o $@ $(OBJECTS) $(LDFLAGS)

$(OBJECTS): ./build/%.o: ./src/%.c
	mkdir -p $(@D)
	$(CC) -c $< -o $@ $(CFLAGS)

../../libowoled.a:
	$(MAKE) -C ../../ libowoled.a 

flash: basic.ihex
	avrdude -C /etc/avrdude.conf -p $(MMCU) -c arduino -b 115200 -P $(DEVICE) -D -U flash:w:basic.ihex

clean:
	rm -f basic.elf
	rm -f basic.ihex
	rm -rf build
	rm -f ../../libowoled.a
