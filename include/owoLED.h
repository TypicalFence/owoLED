#include <stdbool.h>
#include <util/delay.h>
#include <avr/io.h>

typedef struct {
  volatile uint8_t *port;
  volatile uint8_t *ddr;
  uint8_t pin;
} OwOLedAddress;

typedef struct {
    uint8_t red;
    uint8_t green;
    uint8_t blue;
} OwOLedPixel;


OwOLedAddress owoled_init(volatile uint8_t *port, volatile uint8_t *ddr, uint8_t pin);
void owoled_send_colors(OwOLedAddress *address, uint8_t red, uint8_t green, uint8_t blue); 
void owoled_send_pixel(OwOLedAddress *address, OwOLedPixel pixel); 
void owoled_show();
