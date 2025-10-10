#include "Arduino.h"
#include "HardwareSerial.h"
/**
 * @file example_sonic_distance_icp.cpp
 * @brief Example usage of SonicDistanceICP (Timer1 Input Capture) for HC-SR04 on AVR ATmega328P.
 *
 * Connect:
 *   - Trig to PB0 (PORTB0)
 *   - Echo to PD6 (ICP1)
 */

#ifndef __AVR_ATmega328P__
#define __AVR_ATmega328P__
#endif

#ifndef F_CPU
#define F_CPU 16000000UL // 16MHz
#endif

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "sonic_distance_icp.h"
// #include "sonic_distance_icp.cc"
// #include "../../USART/C/USART.cc"

SonicDistanceICP sensor(&PORTB, PORTB0, 8, 0.343f); // Trig: PB0, prescaler 8, speed of sound 0.343 mm/us
// USART serial(9600);

ISR(TIMER1_CAPT_vect) {
    if (g_sonic_icp_instance) g_sonic_icp_instance->handleCapture();
}

void setup() {
    // Configure pins
    DDRB |= (1 << DDB0);  // Trig (PB0) as output
    DDRD &= ~(1 << DDD6); // Echo (PD6/ICP1) as input

    sei(); // Enable global interrupts
    Serial.begin(9600);
    // Serial.begin(9600);
}
void loop() {
    while (1) {
        sensor.trigger();
        while (!sensor.ready);
        float dist = sensor.getDistanceMM();
        Serial.println(dist);
        delay(500);
        // _delay_ms(500);
        // serial.sendStr("Distance: ");
        // serial.sendFloat(dist, 4);
        // serial.sendStr(" mm\n");
        // _delay_ms(500);
    }
    return 0;
}
