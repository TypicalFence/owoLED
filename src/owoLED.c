#include <owoLED.h>

// Timing of the output signal in ns
#define T1H  900    // Width the hige part of a 1 bit
#define T1L  350    // Width the low part of a 1 bit

#define T0H  350    // Width the hige part of a 0 bit
#define T0L  900    // Width the low part of a 0 bit

#define RES 6000    // Width of the low gap between bits to cause a frame to latch

// Nanoseconds to CPU cycles
#define NS_TO_CYCLES(n) ( (n) / ( 1000000000L / F_CPU) ) // nanoseconds / ( nanoseconds per second / cycle per second ) 

static void sendBit(OwOLedAddress *address,  bool bitVal) {
    char mask = (1 << address->pin);
    if (bitVal) {				// 0 bit
		asm volatile (
            "ld r0, Z \n\t"
            "or r0, %[bit] \n\t"
            "st Z, r0 \n\t"
			".rept %[onCycles] \n\t"                                
            "nop \n\t"
			".endr \n\t"
            "st Z, %[null] \n\t"
            ".rept %[offCycles] \n\t"
            "nop \n\t"
			".endr \n\t"
			::
			[port]		"z" (address->port),
            [bit]		"l" (mask),
			[null]		"l" (0),
			[onCycles]	"I" (NS_TO_CYCLES(T1H) - 2),
            [offCycles] 	"I" (NS_TO_CYCLES(T1L) - 2)	
            :
            "r0", "r1"
		);
                                 
    } else {					// 1 bit
		asm volatile (
            "ld r0, Z \n\t"
            "or r0, %[bit] \n\t"
            "st Z, r0 \n\t"
			".rept %[onCycles] \n\t"                                
            "nop \n\t"
			".endr \n\t"
            "st Z, %[null] \n\t"
            ".rept %[offCycles] \n\t"
            "nop \n\t"
			".endr \n\t"
			::
			[port]		"z" (address->port),
            [bit]		"l" (mask),
			[null]		"l" (0),
			[onCycles]	"I" (NS_TO_CYCLES(T0H) - 2),
            [offCycles] 	"I" (NS_TO_CYCLES(T0L) - 2)	
            :
            "r0", "r1"
        );

    }
}  

static void sendByte(OwOLedAddress *address, unsigned char byte) {
    for(unsigned char bit = 0 ; bit < 8 ; bit++) {
      sendBit(address, byte & 0b10000000); 
      byte <<= 1;
    }           
} 

OwOLedAddress owoled_init(volatile uint8_t *port, volatile uint8_t *ddr, uint8_t pin) { 
    *ddr |= (1 << pin);

    OwOLedAddress addr;
    addr.port = port;
    addr.ddr = ddr;
    addr.pin = pin;
    return addr;
}

void owoled_send_colors(OwOLedAddress *address, uint8_t red, uint8_t green, uint8_t blue) {
  sendByte(address, green); 
  sendByte(address, red);
  sendByte(address, blue);
}

void owoled_send_pixel(OwOLedAddress *address, OwOLedPixel pixel) {
    owoled_send_colors(address, pixel.red, pixel.green, pixel.blue);
}

void owoled_show() {
	_delay_us( (RES / 1000UL) + 1);				
}
