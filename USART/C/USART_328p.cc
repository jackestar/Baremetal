#ifndef USART_H
#include "USART.h"
#endif

void USART::setup(uint32_t baud) {
  uint16_t ubrr = (F_CPU / (16UL * baud)) - 1;
  UBRR0 = ubrr;
  UCSR0B |= (1 << TXEN0);
  UCSR0C &= ~(1 << UMSEL00) & ~(1 << UMSEL01); // Async UART
  UCSR0C &= ~(1 << UPM00) & ~(1 << UPM01);    // No parity
  UCSR0C &= ~(1 << USBS0);                    // 1 stop bit
  UCSR0C |= (1 << UCSZ00) | (1 << UCSZ01);    // 8 data bits
  UCSR0B &= ~(1 << UCSZ02);
}

void USART::sendChar(char c) {
  while (!(UCSR0A & (1 << UDRE0)));
  UDR0 = c;
}