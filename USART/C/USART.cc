#ifdef __AVR_ATmega328P__
#include "USART_328p.cc"
#endif

/**
 * @file USART.cc
 * @brief Implementation of USART serial communication class for AVR ATmega328P.
 */

#include "USART.h"

USART::USART(uint32_t baud) {
  setup(baud);
}

void USART::sendStr(const char* str) {
  while (*str) sendChar(*str++);
}

void USART::sendInt(int num) {
  if (num < 0) {
    sendChar('-');
    num = -num;
  }
  int count = 10;
  char str[11] = "0000000000";
  while (num > 0) {
    str[count--] = '0' + num % 10;
    num /= 10;
  }
  for (count++; count < 11; count++)
    sendChar(str[count]);
}

void USART::sendFloat(float num, uint8_t mant) {
  int ent = (int)num;
  sendInt(ent);
  if (mant > 0) {
    sendChar('.');
    static const uint16_t powers[] = { 1, 10, 100, 1000, 10000 };
    sendInt((int)((num - ent) * powers[mant]));
  }
}
