#include <stdbool.h>
#include <util/delay.h>
#include <avr/io.h>

#define PIXEL_PORT  PORTB  // Port of the pin the pixels are connected to
#define PIXEL_DDR   DDRB   // Port of the pin the pixels are connected to
#define PIXEL_BIT   0      // Bit of the pin the pixels are connected to


void sendPixel(unsigned char r, unsigned char g , unsigned char b); 
void show();
