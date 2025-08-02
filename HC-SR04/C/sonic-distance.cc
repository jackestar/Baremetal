/**
 * @file sonic-distance.cc
 * @brief Implementation of the SonicDistance class for ultrasonic measurement.
 */

#include "sonic-distance.h"

/**
 * @brief Constructs a SonicDistance object and configures the trigger/echo pins.
 * @param trig_port Pointer to the port register for the trigger pin.
 * @param echo_pin Pointer to the pin register for the echo pin.
 * @param trig_bit Bit number for the trigger pin.
 * @param echo_bit Bit number for the echo pin.
 */
SonicDistance::SonicDistance(volatile uint8_t *trig_port, volatile uint8_t *echo_pin, uint8_t trig_bit, uint8_t echo_bit)
    : trigPort(trig_port), echoPin(echo_pin), trigBit(trig_bit), echoBit(echo_bit) {
    soundSpeed = C_SOUND;
}

/**
 * @brief Sends a trigger pulse and measures the echo time to calculate distance.
 * @return Distance to object in millimeters.
 */
float SonicDistance::getDistance() {
    *trigPort |= (1 << trigBit);
    _delay_us(10);
    *trigPort &= ~(1 << trigBit);

    uint16_t units = 0;
    uint16_t timeout = UNIT_LIMIT;

    // Wait for echo to go high (with timeout)
    while (!(*echoPin & (1 << echoBit)) && timeout--)
        asm("nop");
    if (timeout == 0) return -1; // Indicate timeout

    // Measure echo high time
    units = 0;
    while ((*echoPin & (1 << echoBit)) && units < UNIT_LIMIT) {
        units++;
    }

    // Calculate distance (mm)
    return (units * CYCLE_LOOP + CYCLE_CONF) / 2.0;
}