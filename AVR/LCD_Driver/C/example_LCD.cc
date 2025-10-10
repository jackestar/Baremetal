/**
 * @file example_LCD.cc
 * @brief Example usage of the LCDDriver class for a classical 7-segment display on AVR.
 *
 * Connect the LCD segment and common pins to the appropriate AVR ports and bits.
 * This example displays a counter in decimal and hexadecimal, updating every second.
 */

#ifndef __AVR_ATmega328P__
#define __AVR_ATmega328P__
#endif

#ifndef F_CPU
#define F_CPU 16000000ULL // 16Mhz
#endif

#include <avr/io.h>
#include <util/delay.h>
#include "lcd_driver.cc"

int main() {
    // Example pin assignments (adjust as needed for your hardware)
    uint8_t pin_1 = PD0;
    uint8_t pin_2 = PD1;
    uint8_t pin_3 = PD2;
    uint8_t pin_4 = PD3;
    uint8_t pin_5 = PD4;
    uint8_t com_6 = PD5;
    uint8_t com_7 = PD6;
    uint8_t com_8 = PD7;
    uint8_t com_9 = PB0;

    LCDDriver lcd(&DDRD, &DDRB, &PORTD, &PORTB,
        pin_1, pin_2, pin_3, pin_4, pin_5,
        com_6, com_7, com_8, com_9);

    uint8_t counter = 0;
    while (1) {
        lcd.segment(ARROW1);
        lcd.LCDOnTime(300);
        lcd.segment(ARROW2);
        lcd.LCDOnTime(300);
        lcd.segment(ARROW3);
        lcd.LCDOnTime(300);
        lcd.segment(ARROW4);
        lcd.LCDOnTime(300);
        lcd.segment(TIMMER);
        lcd.LCDOnTime(300);
        lcd.segment(BATTRY);
        lcd.LCDOnTime(300);
        // lcd.setNumberHex(counter);
        // lcd.LCDOnTime(300);

        lcd.segment(ARROW1, false);
        lcd.LCDOnTime(300);
lcd.segment(ARROW2, false);
        lcd.LCDOnTime(300);
lcd.segment(ARROW3, false);
        lcd.LCDOnTime(300);
lcd.segment(ARROW4, false);
        lcd.LCDOnTime(300);
lcd.segment(TIMMER, false);
        lcd.LCDOnTime(300);
lcd.segment(BATTRY, false);
        lcd.LCDOnTime(300);

        counter = (counter+1)& 0xFF;
    }
    return 0;
}
