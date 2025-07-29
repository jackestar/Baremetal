#ifndef __AVR_ATmega328P__
#define __AVR_ATmega328P__
#endif

#ifndef F_CPU
#define F_CPU 16000000ULL // 16Mhz
#endif

#include <avr/io.h>
#include <util/delay.h>

// Sensor properties
#define C_SOUND 343.0
#define DISTANCE_MIN 0.02
#define DISTANCE_MAX 4
#define DISTANCE_PRS 3e-3

// Echo Times
#define ECHO_MAX (DISTANCE_MAX / C_SOUND)
#define ECHO_MIN (DISTANCE_MIN / C_SOUND)
// For a microsecond precision
#define ECHO_DEL (DISTANCE_PRS / C_SOUND) // seconds per unit

// Instruction Correction
#define INST_LOOP 12
#define INST_TIME (INST_LOOP / (float)F_CPU)

// Echo Instruction correction
#define ECHO_DEL_US (1e6) * (ECHO_DEL - INST_TIME)
#define UNITS_DIST C_SOUND * (int(ECHO_DEL_US)*1e-6 + INST_TIME)

static_assert(ECHO_DEL_US > 0, "Too many Instructions in the loop");

#define UNITS_MAX ((ECHO_MAX - ECHO_MIN) / ECHO_DEL)
// #define UNITS_MAX 255

int main()
{
    // Config Ports
    DDRB |= (1 << DDB0);  // Trig
    DDRB &= ~(1 << DDB1); // Echo

    // Send trigger
    PORTB |= (1 << PORTB0);
    _delay_us(10);
    PORTB &= ~(1 << PORTB0);

    uint16_t units = 0;

    // wait until echo is true
    while (!(PINB & (1 << PINB1)))
        asm("nop");
    while ((PINB & (1 << PINB1)))
    {
        _delay_us(ECHO_DEL_US);
        units++;
        if (units >= UNITS_MAX)
            break;
    }

    // Calculate distance (meters)
    double distance = UNITS_DIST * units;
    _delay_us(distance);

    // Add a small delay before next measurement
    _delay_ms(100);
}