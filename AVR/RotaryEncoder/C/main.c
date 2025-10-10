#ifndef __AVR_ATmega328P__
    #define __AVR_ATmega328P__
#endif

#ifndef F_CPU
    #define F_CPU 16000000ULL // 16Mhz
#endif

#include <avr/io.h>

#include <avr/interrupt.h>

volatile uint8_t button_enable = 1;
uint8_t button_state = 0;

ISR(TIMER0_COMPA_vect);

int main(void)
{
    DDRD &= ~(1 << DDD5); // A (Pin 5)
    DDRD &= ~(1 << DDD6); // B (Pin 6)
    DDRD &= ~(1 << DDD7); // Swh (Pin 7)

    // 13 as output to show direction
    // 12 as output toggle button
    // 11 as output to show errors
    DDRB |= (1 << DDB5); // Dir (Pin 13)
    DDRB |= (1 << DDB4); // Tgl (Pin 12)
    DDRB |= (1 << DDB3); // Err (Pin 11)

    // set CTC mode to use OCR0A as TOP
    TCCR0A = (1 << WGM01);

    // Compare value (CTC TOP)
    OCR0A = 250;

    // Stop clock
    TCCR0B = 0;

    // set A compare interupt
    TIMSK0 = (1 << OCIE0A);

    sei();

    button_enable = 1;

    uint8_t button_state = PIND & (1 << PIND7);

    uint8_t step = 0;

    while (1)
    {
        uint8_t step_new = ((PIND & (1 << PIND5)) ? 2 : 0) | ((PIND & (1 << PIND6)) ? 1 : 0);
        if (step_new != step)
        {
            switch (step)
            {
            case 0:
                if (step_new == 1)
                    PORTB = PORTB | (1 << PORTB5);
                else if (step_new == 2)
                    PORTB = PORTB & ~(1 << PORTB5);
                else
                    PORTB = PORTB ^ (1 << PORTB3);
                break;
            case 1:
                if (step_new == 3)
                    PORTB = PORTB | (1 << PORTB5);
                else if (step_new == 0)
                    PORTB = PORTB & ~(1 << PORTB5);
                else
                    PORTB = PORTB ^ (1 << PORTB3);
                break;
            case 2:
                if (step_new == 0)
                    PORTB = PORTB | (1 << PORTB5);
                else if (step_new == 3)
                    PORTB = PORTB & ~(1 << PORTB5);
                else
                    PORTB = PORTB ^ (1 << PORTB3);
                break;
            case 3:
                if (step_new == 2)
                    PORTB = PORTB | (1 << PORTB5);
                else if (step_new == 1)
                    PORTB = PORTB & ~(1 << PORTB5);
                else
                    PORTB = PORTB ^ (1 << PORTB3);
                break;
            default:
                break;
            }
            step = step_new;
        }

        // Debounce without delay
        if ((PIND & (1 << PIND7)) &&              // Button press
            button_enable &&                      // Change enable
            (PIND & (1 << PIND7) != button_state) // State change?
        )
        {
            button_enable = 0;

            PORTB = PORTB ^ (1 << PORTB4);

            //  Set prescaler for timer0
            TCCR0B = (1 << CS02) | (1 << CS00);
        }

        button_state = PIND & (1 << PIND7);
    }

    return 0;
}

ISR(TIMER0_COMPA_vect)
{
    button_enable = 1;
    button_state = PIND & (1 << PIND7);

    // Stop clock
    TCCR0B = 0;
}