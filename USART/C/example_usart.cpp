/**
 * @file example_usart.cpp
 * @brief Example usage of the USART class for serial communication on AVR ATmega328P.
 *
 * This example demonstrates sending integers, floats, and strings over USART.
 */

#ifndef __AVR_ATmega328P__
#define __AVR_ATmega328P__
#endif

#ifndef F_CPU
#define F_CPU 16000000UL // 16MHz
#endif

#include <avr/io.h>
#include <util/delay.h>
#include "USART.cc"

int main() {
    USART serial(9600); // Initialize USART at 9600 baud

    while (1) {
        _delay_ms(1000);
        serial.sendInt(12345);
        serial.sendChar('\n');
        serial.sendFloat(3.1416, 4);
        serial.sendChar('\n');
        serial.sendStr("Cadena de texto ");
        serial.sendChar('\n');
    }
    return 0;
}
