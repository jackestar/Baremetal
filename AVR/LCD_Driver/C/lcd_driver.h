
/**
 * @file lcd_driver.h
 * @brief Class definition for LCD driver on AVR.
 *
 * Provides a C++ interface to control a multiplexed custom LCD display using direct AVR register access.
 */

#ifndef LCD_DRIVER_H
#define LCD_DRIVER_H

#include <avr/io.h>
#include <util/delay.h>

#ifndef AC_FREQ
#define AC_FREQ 240.0 // Default frequency if not defined
#endif

#define DELAY_AC (500.0 / AC_FREQ)

/**                    LCD PINOUT (MULTIPLEX)
         @@                @@                @@                @@          
        @49@              @39@              @29@              @19@
      @@@@@@@@          @@@@@@@@          @@@@@@@@          @@@@@@@@     
                                                                           
             @                             =@@@59@@@@=      =@@@28@@@= 
            @@@              ===          @@        @@     @@        @@
       =@    @   @@        ==   ==        48        58     38        18
        @@@       @@       @     @        @@        @@     @@        @@
      @  =@@@      #@      @     @        @@        @@     @@        @@
      @    @@@      @      @     @         =@@@57@@@=       =@@@27@@@= 
      @#     @     #@      @  47 @         =@@@@@@@@=       =@@@@@@@@= 
       @           @       @ @@@ @        @@        @@     @@        @@
       #@@  46   @@#       @ @@@ @        @@        @@     @@        @@
         @@@@@@@@@         =======        56        37     26        17
                                          @@        @@     @@        @@
                                           =@@@36@@@=       =@@@16@@@=  

// Segments
PIN Var Bit
19  M1  0   Arrow
29  M1  1   Arrow
39  M1  2   Arrow
49  M1  3   Arrow
46  M1  4   Clock
47  M1  5   Batty
28  S1  0   SegA1
18  S1  1   SegB1
17  S1  2   SegC1
16  S1  3   SegD1
26  S1  4   SegE1
38  S1  5   SegF1
27  S1  6   SegG1
59  S2  0   SegA2
58  S2  1   SegB2
37  S2  2   SegC2
36  S2  3   SegD2
56  S2  4   SegE2
48  S2  5   SegF2
57  S2  6   SegG2
*/                                                                     


enum SEGMENTS {
ARROW1 = 0b00000001,
ARROW2 = 0b00000010,
ARROW3 = 0b00000100,
ARROW4 = 0b00001000,
TIMMER = 0b00100000,
BATTRY = 0b01000000
};

enum NUM_SEGMENTS {
A = 0b00000001,
B = 0b00000010,
C = 0b00000100,
D = 0b00001000,
E = 0b00010000,
F = 0b00100000,
G = 0b01000000,
};

/**
 * @class LCDDriver
 * @brief Bare-metal driver for multiplexed 7-segment LCDs on AVR microcontrollers.
 *
 * Provides direct register-level control for segment and custom symbol display.
 */
class LCDDriver {
  public:
    /**
     * @brief Constructor. Initializes the driver with port and pin assignments.
     * @param DDR_1 Pointer to Data Direction Register for segment pins
     * @param DDR_2 Pointer to Data Direction Register for common pins
     * @param PORT_1 Pointer to PORT register for segment pins
     * @param PORT_2 Pointer to PORT register for common pins
     * @param pin_1 Segment pin 1
     * @param pin_2 Segment pin 2
     * @param pin_3 Segment pin 3
     * @param pin_4 Segment pin 4
     * @param pin_5 Segment pin 5
     * @param com_6 Common pin 6
     * @param com_7 Common pin 7
     * @param com_8 Common pin 8
     * @param com_9 Common pin 9
     */
    LCDDriver(volatile uint8_t *DDR_1, volatile uint8_t *DDR_2,
              volatile uint8_t *PORT_1, volatile uint8_t *PORT_2,
              uint8_t pin_1 = 0,
              uint8_t pin_2 = 1,
              uint8_t pin_3 = 2,
              uint8_t pin_4 = 3,
              uint8_t pin_5 = 4,
              uint8_t com_6 = 5,
              uint8_t com_7 = 6,
              uint8_t com_8 = 7,
              uint8_t com_9 = 0
            );

    /**
     * @brief Set digit 1 (left) to display a value (0-9, A-F).
     * @param number Value to display (0-15)
     */
    void setDigit1(uint8_t number);

    /**
     * @brief Set digit 2 (right) to display a value (0-9, A-F).
     * @param number Value to display (0-15)
     */
    void setDigit2(uint8_t number);

    /**
     * @brief Display a two-digit decimal number (00-99).
     * @param number Value to display (0-99)
     */
    void setNumber(uint8_t number);

    /**
     * @brief Display a hexadecimal digit (0-F) on digit 1.
     * @param number Value to display (0-15)
     */
    void setDigitHex1(uint8_t number);

    /**
     * @brief Display a hexadecimal digit (0-F) on digit 2.
     * @param number Value to display (0-15)
     */
    void setDigitHex2(uint8_t number);

    /**
     * @brief Display a two-digit hexadecimal number (00-FF).
     * @param number Value to display (0-255)
     */
    void setNumberHex(uint8_t number);

    /**
     * @brief Clear a digit (set all segments off).
     * @param pos 0 = right digit, 1 = left digit
     */
    void clearDigit(uint8_t pos);

    /**
     * @brief Clear both digits (set all segments off).
     */
    void clearNumber();

    /**
     * @brief Set or clear a custom segment (arrows, timer, battery).
     * @param segment Segment to control
     * @param state true = on, false = off
     */
    void segment(SEGMENTS segment, bool state = true);

    /**
     * @brief Clear all segments (digits and custom segments).
     */
    void clear();

    /**
     * @brief Multiplex the LCD for a given time (ms).
     * @param ms Duration in milliseconds
     *
     * This function cycles through the COM pins and displays the current segment state.
     * Timing is calculated using integer math for accuracy.
     */
    void LCDOnTime(uint16_t ms);

    /**
     * @brief Custom segment register (arrows, timer, battery).
     */
    uint8_t segM1 = 0x00;
    /**
     * @brief Segment register for digit 1 (left).
     */
    uint8_t segS1 = 0x00;
    /**
     * @brief Segment register for digit 2 (right).
     */
    uint8_t segS2 = 0x00;

  private:
    /**
     * @brief Data Direction Registers for segment and common pins.
     */
    volatile uint8_t *ddr1, *ddr2;
    /**
     * @brief PORT registers for segment and common pins.
     */
    volatile uint8_t *port1, *port2;
    /**
     * @brief Segment pin assignments.
     */
    uint8_t pin1, pin2, pin3, pin4, pin5;
    /**
     * @brief Common pin assignments.
     */
    uint8_t com6, com7, com8, com9;
    /**
     * @brief Pin mask for segment pins.
     */
    uint8_t pinMask;
};

#endif // LCD_DRIVER_H
