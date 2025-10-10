#ifndef __AVR_ATmega328P__
#define __AVR_ATmega328P__
#endif

#ifndef F_CPU
#define F_CPU 16000000ULL
#endif

#include <avr/io.h>
#include <util/delay.h>

#define AC_FREQ 120

#define CHANGE_TIME 4 // seconds
#define DELAY_AC (500.0 / AC_FREQ)

#define CHANGE_COUNT ((uint16_t)((CHANGE_TIME * 1000.0) / (DELAY_AC)))

int main(void) {
    DDRD = 0xFF;
    DDRB |= (1 << DDB0);

    uint8_t com_pin = 0;

    while (1) {
        if (com_pin == 8) {
          PORTB |= (1 << DDB0);
          PORTD = 0x00;
        } else {
          PORTB &= ~(1 << DDB0);
          PORTD = (1 << com_pin++);

        }
        for (uint16_t i = 0; i < CHANGE_COUNT; i++) {

            PORTD = ~PORTD;
            PORTB ^= (1 << PORTB0);

            _delay_ms(DELAY_AC);

        }
        if (com_pin > 8) {
            com_pin = 0;
        }
    }

    return 0;
}
