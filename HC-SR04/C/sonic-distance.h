
/**
 * @file sonic-distance.h
 * @brief Class definition for ultrasonic distance measurement on AVR.
 *
 * Provides a C++ interface to trigger an HC-SR04 (or compatible) ultrasonic sensor and measure distance using direct AVR register access.
 */

#ifndef SONIC_DISTANCE_H
#define SONIC_DISTANCE_H

// Sensor properties
#define C_SOUND 343000.0 /**< Speed of sound in mm/s */

// For a microsecond precision
#define UNIT_LIMIT 0xffff /**< Maximum count for echo pulse (16 bits) */

#define C_LOOP 9
#define CYCLE_LOOP (C_LOOP * C_SOUND / (float)F_CPU)
#define C_CONF 4
#define CYCLE_CONF (C_CONF * C_SOUND / (float)F_CPU)

#ifndef _UTIL_DELAY_H_
#include <util/delay.h>
#endif

/**
 * @class SonicDistance
 * @brief Ultrasonic distance measurement class for AVR microcontrollers.
 *
 * This class provides methods to trigger an ultrasonic sensor and measure the distance to an object using echo timing.
 */
class SonicDistance {
public:
    /**
     * @brief Constructor for SonicDistance.
     * @param trig_port Pointer to the port register for the trigger pin.
     * @param echo_port Pointer to the pin register for the echo pin.
     * @param trigBit Bit number for the trigger pin.
     * @param echoBit Bit number for the echo pin.
     */
    SonicDistance(volatile uint8_t *trig_port, volatile uint8_t *echo_port, uint8_t trigBit, uint8_t echoBit);

    /**
     * @brief Measures and returns the distance in millimeters.
     * @return Distance to object in millimeters.
     */
    float getDistance();

    float soundSpeed;                /**< Speed of sound (mm/s) */
    volatile uint8_t *trigPort;      /**< Pointer to trigger port register */
    volatile uint8_t *echoPin;      /**< Pointer to echo pin register */
    uint8_t trigBit;                 /**< Trigger pin bit */
    uint8_t echoBit;                 /**< Echo pin bit */
};

#endif // SONIC_DISTANCE_H