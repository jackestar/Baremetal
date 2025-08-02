/**
 * @file USART.h
 * @brief USART serial communication class for AVR ATmega328P.
 *
 * Provides basic USART initialization and send functions for characters, strings, integers, and floats.
 */

#ifndef USART_H
#define USART_H

#ifndef _AVR_IO_H_
#include <avr/io.h>
#endif

class USART {
 public:
  /**
   * @brief Constructor. Initializes USART with the given baud rate and F_CPU.
   * @param baud Baud rate (default 9600)
   */
  USART(uint32_t baud = 9600);

  /**
   * @brief Send a single character.
   * @param c Character to send
   */
  void sendChar(char c);

  /**
   * @brief Send a null-terminated string.
   * @param str Pointer to string
   */
  void sendStr(const char* str);

  /**
   * @brief Send an integer as ASCII.
   * @param num Integer to send
   */
  void sendInt(int num);

  /**
   * @brief Send a float with up to 4 decimal places.
   * @param num Float to send
   * @param mant Number of decimal places (0-4)
   */
  void sendFloat(float num, uint8_t mant = 4);

 private:
  void setup(uint32_t baud);
};

#endif // USART_H
