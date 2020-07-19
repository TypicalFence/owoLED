#ifndef OWOLED_H
#define OWOLED_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <util/delay.h>
#include <avr/io.h>

/**
 * Represents the address where a LED strip is connected to.
 */
typedef struct {
  volatile uint8_t *port;
  volatile uint8_t *ddr;
  uint8_t pin;
} OwOLedAddress;

/**
 * Represents the state of a single LED or "pixel" on the LED strip.
 */
typedef struct {
    uint8_t red;
    uint8_t green;
    uint8_t blue;
} OwOLedPixel;

/**
 * Initialises your MCU for driving  a LED strip.
 * 
 * If you have multiple LED strips connected you can call it multiple times.
 * It will take care of everything.
 *
 * \returns a OwOLedAddress struct that is needed for all other functions.
 */
OwOLedAddress owoled_init(volatile uint8_t *port, volatile uint8_t *ddr, uint8_t pin);

/**
 * Sends a color to the LED strip.
 * 
 * You are supposed to call this function in sequence,
 * every subsequent call will set the state of the next LED, 
 * or in other words you'll basically travel down the strip.
 */
void owoled_send_colors(OwOLedAddress *address, uint8_t red, uint8_t green, uint8_t blue);

/**
 * Sends a Pixel struct to the LED strip.
 *
 * Same as owoled_send_colors, but using the pixel struct.
 * 
 * Only really exists to be cute.
 */
void owoled_send_pixel(OwOLedAddress *address, OwOLedPixel pixel);

/**
 * Waits a bit so that the LED strip displays what you sent it.
 *
 * You could also just set a delay or sleep yourself, its just for convenience.
 */
void owoled_show();

#ifdef __cplusplus
}
#endif

#endif
