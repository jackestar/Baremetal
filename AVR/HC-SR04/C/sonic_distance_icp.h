/**
 * @file sonic_distance_icp.h
 * @brief Ultrasonic distance measurement using Timer1 Input Capture on AVR.
 *
 * Provides a C++ interface to trigger an HC-SR04 (or compatible) ultrasonic sensor and measure distance using Timer1 Input Capture (ICP1) with configurable prescaler and speed of sound.
 */

#ifndef SONIC_DISTANCE_ICP_H
#define SONIC_DISTANCE_ICP_H

#define C_SOUND 343000.0

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

class SonicDistanceICP {
public:
    /**
     * @brief Constructor for SonicDistanceICP.
     * @param trig_port Pointer to the port register for the trigger pin.
     * @param trigBit Bit number for the trigger pin.
     * @param prescaler Timer1 prescaler (1, 8, 64, 256, 1024)
     * @param sound_speed Speed of sound in mm/us (default: 0.343)
     */
    SonicDistanceICP(volatile uint8_t *trig_port, uint8_t trigBit, uint8_t prescaler = 8, float sound_speed = C_SOUND);

    /**
     * @brief Sends a trigger pulse to the sensor.
     */
    void trigger();

    /**
     * @brief Returns true if a measurement is ready.
     */
    // bool isReady() const;

    /**
     * @brief Returns the last measured distance in millimeters.
     */
    float getDistanceMM() const;

    /**
     * @brief Call from ISR(TIMER1_CAPT_vect) to handle capture event.
     */
    void handleCapture();
    volatile bool ready;
    float soundSpeed;

private:
    volatile uint8_t *trigPort;
    uint8_t trigBit;
    uint16_t prescaler;
    
    volatile uint16_t start;
    volatile uint16_t end;
    volatile bool waitingRising;
    // void setupTimer1();
    void setPrescaler(uint16_t prescaler);
};

extern SonicDistanceICP* g_sonic_icp_instance; // For ISR linkage

#endif // SONIC_DISTANCE_ICP_H
