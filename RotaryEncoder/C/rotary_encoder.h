/**
 * @file rotaryEncoder.hpp
 * @brief Declaration of the RotaryEncoder class.
 *
 * This module provides a C++ interface to read a rotary encoder.
 */

#ifndef ROTARY_ENCODER_H_
#define ROTARY_ENCODER_H_

#include <avr/io.h>

/**
 * @enum EncoderDirection
 * @brief Indicates the rotation direction.
 */
enum EncoderDirection {
    ENCODER_STOPPED,        /// No rotation
    ENCODER_CLOCKWISE,      /// Rotating clockwise
    ENCODER_COUNTERCLOCKWISE/// Rotating counterclockwise
};

/**
 * @class RotaryEncoder
 * @brief A class to manage a rotary encoder.
 *
 * The class reads the state of two digital pins to detect and track the rotary encoder's movement.
 */
class RotaryEncoder {
 public:
  /**
   * @brief Constructs a new RotaryEncoder object.
   * 
   * @param pin_a_port Pointer to the port for the A signal.
   * @param pin_a_bit Bit number for the A signal.
   * @param pin_b_port Pointer to the port for the B signal.
   * @param pin_b_bit Bit number for the B signal.
   * @param step_per_revolution Total steps per revolution.
   */
  RotaryEncoder(volatile uint8_t *pin_a_port, uint8_t pin_a_bit,
                volatile uint8_t *pin_b_port, uint8_t pin_b_bit,
                uint8_t step_per_revolution);

  /**
   * @brief Reads the current state of the encoder and updates its position.
   */
  void Read();

  /**
   * @brief Returns a value indicating the incremental change of the encoder.
   * 
   * @return int8_t 1 if clockwise, -1 if counterclockwise, 0 if stopped.
   */
  int8_t IncrementalRead();

  /**
   * @brief Resets the encoder's position and state.
   */
  void Reset();

  // Public members
  EncoderDirection direction;         ///< Current rotation direction.
  volatile uint8_t *pinAPort;           ///< Pointer to port for A signal.
  uint8_t pinABit;                    ///< Bit number for A signal.
  volatile uint8_t *pinBPort;           ///< Pointer to port for B signal.
  uint8_t pinBBit;                    ///< Bit number for B signal.
  uint8_t stepPerRevolution;          ///< Number of steps per full revolution.
  uint8_t position;                   ///< Current position (8-bit resolution).

 private:
  uint8_t step_;                      ///< Internal state tracking the last step.
  /**
   * @brief Increments the position.
   */
  inline void PositionInc();

  /**
   * @brief Decrements the position.
   */
  inline void PositionDec();
};

#endif // ROTARY_ENCODER_H_