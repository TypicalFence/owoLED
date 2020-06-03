#include <owoLED.h>
#include <util/delay.h>

int main (void) {

    owoled_init();
    while(1) {
        owoled_toggle();
        _delay_ms(500);
    }
    
    return 0;
}
