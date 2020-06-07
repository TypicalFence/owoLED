#include <stdbool.h>
#include <util/delay.h>
#include <avr/io.h>

#define PIXEL_PORT  PORTB  // Port of the pin the pixels are connected to
#define PIXEL_DDR   DDRB   // Port of the pin the pixels are connected to
#define PIXEL_BIT   0      // Bit of the pin the pixels are connected to

typedef struct {
  int port, ddr, pin;
} OwOLedAddress;

typedef struct {
    uint8_t red;
    uint8_t green;
    uint8_t blue;
} OwOLedPixel;


OwOLedAddress owoled_init(int port, int ddr, int pin); 
void owoled_send_colors(OwOLedAddress *address, unsigned char red, unsigned char green, unsigned char blue); 
void owoled_send_pixel(OwOLedAddress *address, OwOLedPixel pixel); 
void owoled_show();
