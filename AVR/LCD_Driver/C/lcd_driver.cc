/**
 * @file lcd_driver.cc
 * @brief Implementation of the LCDDriver class for LCD driver.
 */


#include "lcd_driver.h"

LCDDriver::LCDDriver(volatile uint8_t* DDR_1, volatile uint8_t* DDR_2,
    volatile uint8_t* PORT_1, volatile uint8_t* PORT_2,
    uint8_t pin_1,
    uint8_t pin_2,
    uint8_t pin_3,
    uint8_t pin_4,
    uint8_t pin_5,
    uint8_t com_6,
    uint8_t com_7,
    uint8_t com_8,
    uint8_t com_9)
    : ddr1(DDR_1), ddr2(DDR_2),
    port1(PORT_1), port2(PORT_2),
    pin1(pin_1), pin2(pin_2), pin3(pin_3), pin4(pin_4), pin5(pin_5),
    com6(com_6), com7(com_7), com8(com_8), com9(com_9) {
    // Default High-Z

    *ddr1 = 0x00;
    *ddr2 &= ~(1 << com9);
    pinMask = (1 << pin1) | (1 << pin2) | (1 << pin3) | (1 << pin4) | (1 << pin5);
}
/**
 * @brief Set digit 1 (left) to display a value (0-9, A-F).
 * @param number Value to display (0-15)
 */
void LCDDriver::setDigit2(uint8_t number) {
    // Classical 7-segment encoding for digits 0-9
    static const uint8_t segment_map[16] = {
        A | B | C | D | E | F,        // 0
        B | C,                        // 1
        A | B | D | E | G,            // 2
        A | B | C | D | G,            // 3
        B | C | F | G,                // 4
        A | C | D | F | G,            // 5
        A | C | D | E | F | G,        // 6
        A | B | C,                    // 7
        A | B | C | D | E | F | G,    // 8
        A | B | C | D | F | G,        // 9
        A | B | C | E | F | G,        // A (hex)
        C | D | E | F | G,            // b (hex)
        A | D | E | F,                // C (hex)
        B | C | D | E | G,            // d (hex)
        A | D | E | F | G,            // E (hex)
        A | E | F | G                 // F (hex)
    };
    segS1 = segment_map[number & 0x0F];
}
/**
 * @brief Set digit 2 (right) to display a value (0-9, A-F).
 * @param number Value to display (0-15)
 */
void LCDDriver::setDigit1(uint8_t number) {
    // Classical 7-segment encoding for digits 0-9
    static const uint8_t segment_map[16] = {
        A | B | C | D | E | F,        // 0
        B | C,                        // 1
        A | B | D | E | G,            // 2
        A | B | C | D | G,            // 3
        B | C | F | G,                // 4
        A | C | D | F | G,            // 5
        A | C | D | E | F | G,        // 6
        A | B | C,                    // 7
        A | B | C | D | E | F | G,    // 8
        A | B | C | D | F | G,        // 9
        A | B | C | E | F | G,        // A (hex)
        C | D | E | F | G,            // b (hex)
        A | D | E | F,                // C (hex)
        B | C | D | E | G,            // d (hex)
        A | D | E | F | G,            // E (hex)
        A | E | F | G                 // F (hex)
    };
    segS2 = segment_map[number & 0x0F];
}
/**
 * @brief Display a two-digit decimal number (00-99).
 * @param number Value to display (0-99)
 */
void LCDDriver::setNumber(uint8_t number) {
    // Display two digits: high nibble = digit1, low nibble = digit2
    setDigit1((number / 10) % 10);
    setDigit2(number % 10);
}
/**
 * @brief Display a hexadecimal digit (0-F) on digit 1.
 * @param number Value to display (0-15)
 */
void LCDDriver::setDigitHex1(uint8_t number) {
    // Display hex digit (0-F) on digit 1
    setDigit1(number & 0x0F);
}
/**
 * @brief Display a hexadecimal digit (0-F) on digit 2.
 * @param number Value to display (0-15)
 */
void LCDDriver::setDigitHex2(uint8_t number) {
    // Display hex digit (0-F) on digit 2
    setDigit2(number & 0x0F);
}
/**
 * @brief Display a two-digit hexadecimal number (00-FF).
 * @param number Value to display (0-255)
 */
void LCDDriver::setNumberHex(uint8_t number) {
    // Display two hex digits: high nibble = digit1, low nibble = digit2
    setDigitHex1((number >> 4) & 0x0F);
    setDigitHex2(number & 0x0F);
}
/**
 * @brief Clear a digit (set all segments off).
 * @param pos 0 = right digit, 1 = left digit
 */
void LCDDriver::clearDigit(uint8_t pos) {
    // pos==0: clear segS2, pos!=0: clear segS1
    if (pos) {
        segS1 = 0x00;
    } else {
        segS2 = 0x00;
    }
}
/**
 * @brief Clear both digits (set all segments off).
 */
void LCDDriver::clearNumber() {
    segS1 = 0x00;
    segS2 = 0x00;
}
/**
 * @brief Set or clear a custom segment (arrows, timer, battery).
 * @param segment Segment to control
 * @param state true = on, false = off
 */
void LCDDriver::segment(SEGMENTS segment, bool state) {
    if (state) {
        segM1 |= segment;
    } else {
        segM1 &= ~segment;
    }
}
/**
 * @brief Clear all segments (digits and custom segments).
 */
void LCDDriver::clear() {
    segM1 = 0X00;
    segS1 = 0X00;
    segS2 = 0X00;
}
/**
 * @brief Multiplex the LCD for a given time (ms).
 * @param ms Duration in milliseconds
 *
 * This function cycles through the COM pins and displays the current segment state.
 * Timing is calculated using integer math for accuracy.
 */
void LCDDriver::LCDOnTime(uint16_t ms) {

    // Pins for each Common
    const uint8_t COM6 = 0x00 |
        ((segS1 & D) ? (1 << pin1) : 0x00) |
        ((segS1 & E) ? (1 << pin2) : 0x00) |
        ((segS2 & D) ? (1 << pin3) : 0x00) |
        ((segM1 & TIMMER) ? (1 << pin4) : 0x00) |
        ((segS2 & E) ? (1 << pin5) : 0x00);
    const uint8_t COM7 = 0x00 |
        ((segS1 & C) ? (1 << pin1) : 0x00) |
        ((segS1 & G) ? (1 << pin2) : 0x00) |
        ((segS2 & C) ? (1 << pin3) : 0x00) |
        ((segM1 & BATTRY) ? (1 << pin4) : 0x00) |
        ((segS2 & G) ? (1 << pin5) : 0x00);
    const uint8_t COM8 = 0x00 |
        ((segS1 & B) ? (1 << pin1) : 0x00) |
        ((segS1 & A) ? (1 << pin2) : 0x00) |
        ((segS1 & F) ? (1 << pin3) : 0x00) |
        ((segS2 & F) ? (1 << pin4) : 0x00) |
        ((segS2 & B) ? (1 << pin5) : 0x00);
    const uint8_t COM9 = 0x00 |
        ((segM1 & ARROW1) ? (1 << pin1) : 0x00) |
        ((segM1 & ARROW2) ? (1 << pin2) : 0x00) |
        ((segM1 & ARROW3) ? (1 << pin3) : 0x00) |
        ((segM1 & ARROW4) ? (1 << pin4) : 0x00) |
        ((segS2 & A) ? (1 << pin5) : 0x00);

    const uint16_t count = ms / (8.0 * DELAY_AC);

    *port1 = 0x00;
    *port2 &= ~(1 << com9);

    for (uint16_t i = 0; i < count; i++) {
        // COM7
        *ddr1 = (1 << com6) | COM6;
        *ddr2 &= ~(1 << com9);

        *port1 = COM6;// & ~(1 << com6);
        _delay_ms(DELAY_AC);

        *port1 = (1 << com6);
        _delay_ms(DELAY_AC);

        *port1 = 0x00;
        *ddr1 = 0x00;

        // COM7
        *ddr1 = (1 << com7) | COM7;
        // *ddr2 &= ~(1 << com9);

        *port1 = COM7;// & ~(1 << com7);
        _delay_ms(DELAY_AC);

        *port1 = (1 << com7);
        _delay_ms(DELAY_AC);

        *port1 = 0x00;
        *ddr1 = 0x00;

        // COM8
        *ddr1 = (1 << com8) | COM8;
        // *ddr2 &= ~(1 << com9);

        *port1 = COM8;// & ~(1 << com7);
        _delay_ms(DELAY_AC);

        *port1 = (1 << com8);
        _delay_ms(DELAY_AC);

        *port1 = 0x00;
        *ddr1 = 0x00;

        // COM9
        *ddr1 = COM9;
        *ddr2 |= (1 << com9);

        // *port1 = COM9 & pinMask;
        // *port2 &= ~(1 << com9);
        *port1 = COM9;
        *port2 &= ~(1 << com9);
        _delay_ms(DELAY_AC);

        *port1 = 0x00;
        *port2 |= (1 << com9);
        _delay_ms(DELAY_AC);

        *ddr1 = 0x00;

    }

}