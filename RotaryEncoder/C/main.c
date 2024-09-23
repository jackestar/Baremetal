#define __AVR_ATmega328P__

// CPU freq (max 16000000UL)
#define F_CPU 16000000ULL // 16Mhz

#include <avr/io.h>

#include <avr/interrupt.h>

volatile uint8_t button_enable = 1;

// volatile uint8_t led_state = 0;

// ISR(TIMER0_COMPA_vect);

int main(void)
{
    // set 5 6 7 as encoder input
    // 5 -> 1; 6 -> 2; 7 -> a
    DDRD &= ~(1 << DDD7);

    //  set 7 with push-up resistor
    // PORTD |= (1 << PORTD7);

    // 13 as output to show direction
    // 12 as output toggle button
    // 11 as output to show errors
    DDRB |= (1 << DDB4);


    // set CTC mode to use OCR0A as TOP
    TCCR0A = (1 << WGM01);

    // Compare value (CTC TOP)
    OCR0A = 250;

    // Stop clock and stop interrups
    TCCR0B = 0;
    TIMSK0 = 0;

    sei();

    button_enable = 1;
    while (1)
    {
        // uint8_t step_new = 2 * (PIND & (1 << PD5)) + (PIND & (1 << PD6));
        uint8_t step_new = ((PIND & (1 << PIND5)) ? 2 : 0) | ((PIND & (1 << PIND6)) ? 1 : 0);

        // Debounce without delay
        if ((PIND & (1 << PIND7)))
        {
            if (button_enable) {
                button_enable = 0;
                PORTB = PORTB ^ (1 << PORTB4);

                //  set prescaler fro timer0
                TCCR0B = (1 << CS02) | (1 << CS00);
                // set B compare interupt
                TIMSK0 = (1 << OCIE0A);
            }
        }
    }

    return 0;
}

ISR(TIMER0_COMPA_vect) { 
    button_enable = 1;
    // Stop clock and stop interrups
    TCCR0B = 0;
    TIMSK0 = 0;
}