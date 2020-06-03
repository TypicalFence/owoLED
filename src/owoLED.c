#include <owoLED.h>

void owoled_toggle() {
   PORTB ^= (1 << PB0);
}
