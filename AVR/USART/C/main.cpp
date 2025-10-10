#ifndef __AVR_ATmega328P__
#define __AVR_ATmega328P__
#endif

#ifndef F_CPU
#define F_CPU 16000000ULL // 16Mhz
#endif

#include <avr/io.h>
#include <util/delay.h>
#define BAUD_RATE 9600ULL
#define UBRR0_VALUE (F_CPU / ((16 * BAUD_RATE)) - 1)

void send_chr(char c);
void send_str(char* str);
void send_int(int num);
void send_flt_LUT16(float num, uint8_t mant);
void setupUSART();

int main() {

  // Setup
  setupUSART();

  while (1) {
    _delay_ms(1000);
    send_int(12345);
    send_chr('\n');
    send_flt_LUT16(3.14159265358, 4);
    send_chr('\n');
    send_str("Distancia: ");
    send_chr('\n');
    }
  }

void send_chr(char c) {
  while (!(UCSR0A & (1 << UDRE0)))
    ; // Data Register Empty

  UDR0 = c;
  }

void send_str(char* str) {
  uint8_t pos = 0;
  while (str[pos] != '\0') {
    send_chr(str[pos++]);
    }
  }

void send_int(int num) {
  if (num < 0) {
    send_chr('-');
    num = -num;
    }
  int count = 10;
  char str[11] = "0000000000";

  while (num > 0) {
    str[count--] = '0' + num % 10;
    // send_chr(str[count]);
    num /= 10;
    }

  // Print left
  for (count++; count < 11; count++)
    send_chr(str[count]);
  }

// Up to 4 decimal points
void send_flt_LUT16(float num, uint8_t mant) {
  const int ent = int(num);
  send_int(ent);
  if (mant > 0) {
    send_chr('.');

    // if constrant time is needed Lookup Table (LUT) Method is the way
    // to save space Iterative Loop is the way

    // ~10 bytes in flash memory
    static const uint16_t powers[] = {
        1,      // 10^0
        10,     // 10^1
        100,    // 10^2
        1000,   // 10^3
        10000,  // 10^4
      };

    send_int(int((num - ent) * powers[mant]));
    }
  }
void setupUSART() {
  UCSR0B |= (1 << TXEN0);

  // 8N1

  // Modo UART (Default)
  UCSR0C &= ~(1 << UMSEL00) & ~(1 << UMSEL01);
  // Without parity
  UCSR0C &= ~(1 << UPM00) & ~(1 << UPM01);
  // 1 Stop bit
  UCSR0C &= ~(1 << USBS0);

  // 8 bit character
  UCSR0C |= (1 << UCSZ00) | (1 << UCSZ01);
  UCSR0B &= ~(1 << UCSZ02);

  UBRR0 = UBRR0_VALUE;
  }