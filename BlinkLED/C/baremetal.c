// Device
#define __AVR_ATmega328P__

// CPU freq (max 16000000UL)
#define F_CPU 16000000ULL // 16Mhz

#include <avr/io.h>
// #include <util/delay.h>

inline void delay(const unsigned long long);

int main(void) {
  	DDRB = DDRB | (1 << DDB5);
    // CLKPR = (uint8_t)0b0000;
    while (1)
    {
        // Operacion XOR
        PORTB = PORTB ^ (1 << PORTB5);
        delay(1000);
    }
    
}

// delay para valores constantes
// intervalos maximos por frecuencia
// 16Mhz    1879048ms       1879s       31m     
// 8Mhz     3758096ms       3758s       62m     1h
// 4Mhz     7516192ms       7516s       120m    2h
// 2Mhz     15032385ms      15032s      250m    4h
// 1Mhz     30064771ms      30064s      501m    8h
// 500khz   60129542ms      60129s      1002m   16h
// 250khz   120259084ms     120259s     2000m   33h
// 125khz   240518268ms     240518s     4008m   66h
// 62.5khz  481036337ms     481036s     8017m   133h

inline void delay(const unsigned long long ms) {
    // temp es calculado en tiempo de compilacion
    uint32_t ciclos = ((F_CPU / 1000ULL) * (ms)) / 7ULL; // Overflow
    while (ciclos)
    {
        ciclos--;
        asm("nop");
    }
}