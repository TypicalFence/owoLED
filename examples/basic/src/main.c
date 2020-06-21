//#define _SFR_ASM_COMPAT 1
#include <owoLED.h>
#include <util/delay.h>
#include <avr/io.h> 
//#include <avr/sfr_defs.h> 

#define PIXELS 60  // Number of pixels in the string

//volatile uint8_t *port = (uint8_t *) _SFR_IO8(0x05);

int main (void) {
    OwOLedAddress addr = owoled_init(&PORTB, &DDRB, 1);

    while (1) {

        for (int i = 0; i<PIXELS/2; i++) {
            owoled_send_colors(&addr, 0x00, 0x00, 0x10);
        }

        for (int i = 0; i<PIXELS/2; i++) {
            owoled_send_colors(&addr, 0x10, 0x00, 0x00);
        }

        owoled_show();
        _delay_ms(10);
    }
    
    return 0;
}

