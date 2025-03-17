#ifndef __AVR_ATmega328P__
    #define __AVR_ATmega328P__
#endif

#ifndef F_CPU
    #define F_CPU 16000000ULL // 16Mhz
#endif

#include <avr/io.h>
#include <avr/interrupt.h>

#include "rotaryEncoder.cpp"

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
    DDRB |= (1 << DDB3); // DIM (Pin 11)

    // no-inverter mode
    // Fix frequency PWM
    TCCR2A = 0 | (1 << COM2A1) | (1 << WGM21) | (1 << WGM20);

    uint8_t prev_position = 0;

    RotaryEncoder encoder(&PIND, PIND5, &PIND, PIND6, 60);

    while (1)
    {
        encoder.read();
        if (encoder.direction == ENCODER_CLOCKWISE) {
            PORTB |= (1 << PORTB5);
        } else if (encoder.direction == ENCODER_COUNTERCLOCKWISE) {
            PORTB &= ~(1 << PORTB5);
        }

        if (PIND & (1 << PIND7))
            encoder.reset();

        if (encoder.position == encoder.stepPerRevolution && prev_position != encoder.position)
            PORTB ^= (1 << PORTB4);

        prev_position = encoder.position;

        // No prescaler
        TCCR2B = 0 | (1 << CS20);

        OCR2A = encoder.position * 255 / encoder.stepPerRevolution;

    }

    return 0;
}