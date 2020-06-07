#include <owoLED.h>

// These are the timing constraints taken mostly from the WS2812 datasheets 
// These are chosen to be conservative and avoid problems rather than for maximum throughput 

#define T1H  900    // Width of a 1 bit in ns
#define T1L  600    // Width of a 1 bit in ns

#define T0H  400    // Width of a 0 bit in ns
#define T0L  900    // Width of a 0 bit in ns

// The reset gap can be 6000 ns, but depending on the LED strip it may have to be increased
// to values like 600000 ns. If it is too small, the pixels will show nothing most of the time.
#define RES 6000    // Width of the low gap between bits to cause a frame to latch

// Here are some convience defines for using nanoseconds specs to generate actual CPU delays

#define NS_PER_SEC (1000000000L) // Note that this has to be SIGNED since we want to be able to check for negative values of derivatives

#define CYCLES_PER_SEC (F_CPU)

#define NS_PER_CYCLE ( NS_PER_SEC / CYCLES_PER_SEC )

#define NS_TO_CYCLES(n) ( (n) / NS_PER_CYCLE )

static void sendBit(OwOLedAddress *address,  bool bitVal) {
    if (bitVal) {				// 0 bit
		asm volatile (
            // Set the output bit
			"sbi %[port], %[bit] \n\t"							
            ".rept %[onCycles] \n\t" // Execute NOPs to delay exactly the specified number of cycles
			"nop \n\t"
			".endr \n\t"
			"cbi %[port], %[bit] \n\t"                              // Clear the output bit
			".rept %[offCycles] \n\t"       // Execute NOPs to delay exactly the specified number of cycles
			"nop \n\t"
			".endr \n\t"
			::
			[port]		"g" (_SFR_IO_ADDR(address->port)),
			[bit]		"g" (address->pin),
			[onCycles]	"I" (NS_TO_CYCLES(T1H) - 2),		
            // 1-bit width less overhead  for the actual bit setting, i
            // note that this delay could be longer and everything would still work
			[offCycles] 	"I" (NS_TO_CYCLES(T1L) - 2)			
            // Minimum interbit delay. 
            // Note that we probably don't need this at all since the loop overhead will be enough, 
            // but here for correctness

		);
                                  
    } else {					// 1 bit
		asm volatile (
            // Set the output bit
			"sbi %[port], %[bit] \n\t"				
            // Now timing actually matters. 
            // The 0-bit must be long enough to be detected but not too long or it will be a 1-bit
			".rept %[onCycles] \n\t" 		
            // Execute NOPs to delay exactly the specified number of cycles
            "nop \n\t"                                              
            ".endr \n\t"
            // Clear the output bit
			"cbi %[port], %[bit] \n\t"
            // Execute NOPs to delay exactly the specified number of cycles
			".rept %[offCycles] \n\t"                               
			"nop \n\t"
			".endr \n\t"
			::
			[port]		"g" (_SFR_IO_ADDR(address->port)),
			[bit]		"g" (address->pin),
			[onCycles]	"I" (NS_TO_CYCLES(T0H) - 2),
			[offCycles]	"I" (NS_TO_CYCLES(T0L) - 2)

		);
      
    }
}  

static void sendByte(OwOLedAddress *address, unsigned char byte) {
    for(unsigned char bit = 0 ; bit < 8 ; bit++) {
      sendBit(address, byte & 0b10000000); 
      byte <<= 1;
    }           
} 

OwOLedAddress owoled_init(int port, int ddr, int pin) { 
    int volatile * const ddr_reg = (int *) ddr;
    *ddr_reg = 0b00000001;
    
    OwOLedAddress addr;
    addr.port = port;
    addr.ddr = ddr;
    addr.pin = pin;
    return addr;
}

void owoled_send_colors(OwOLedAddress *address, unsigned char red, unsigned char green, unsigned char blue) {
  // Neopixel wants colors in green then red then blue order
  sendByte(address, green); 
  sendByte(address, red);
  sendByte(address, blue);
}

void owoled_send_pixel(OwOLedAddress *address, OwOLedPixel pixel) {
    owoled_send_colors(address, pixel.red, pixel.green, pixel.blue);
}

// Just wait long enough without sending any bots to cause the pixels to latch and display the last sent frame
void owoled_show() {
    // Round up since the delay must be _at_least_ this long (too short might not work, too long not a problem)
	_delay_us( (RES / 1000UL) + 1);				
}
