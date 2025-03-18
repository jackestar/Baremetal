/**
 * @file rotaryEncoder.cpp
 * @brief Implementation of the RotaryEncoder class.
 */

#include "rotary_encoder.h"

RotaryEncoder::RotaryEncoder(volatile uint8_t *pin_a_port, uint8_t pin_a_bit,
                             volatile uint8_t *pin_b_port, uint8_t pin_b_bit,
                             uint8_t step_per_revolution)
    : pinAPort(pin_a_port),
      pinABit(pin_a_bit),
      pinBPort(pin_b_port),
      pinBBit(pin_b_bit),
      stepPerRevolution(step_per_revolution),
      position(0),
      step_(0),
      direction(ENCODER_STOPPED) {
  // static_assert could be used here if needed (in C++11 and later)
}

inline void RotaryEncoder::PositionInc() {
  if (position < stepPerRevolution) {
    position++;
  } else {
    position = 0;
  }
}

inline void RotaryEncoder::PositionDec() {
  if (position > 0) {
    position--;
  } else {
    position = stepPerRevolution;
  }
}

void RotaryEncoder::Read() {
  uint8_t new_step = ((*pinAPort & (1 << pinABit)) ? 2 : 0) |
                     ((*pinBPort & (1 << pinBBit)) ? 1 : 0);

  switch (step_) {
    case 0:
      if (new_step == 1) {
        direction = ENCODER_CLOCKWISE;
        PositionInc();
      } else if (new_step == 2) {
        direction = ENCODER_COUNTERCLOCKWISE;
        PositionDec();
      } else {
        direction = ENCODER_STOPPED;
      }
      break;
    case 1:
      if (new_step == 3) {
        direction = ENCODER_CLOCKWISE;
        PositionInc();
      } else if (new_step == 0) {
        direction = ENCODER_COUNTERCLOCKWISE;
        PositionDec();
      } else {
        direction = ENCODER_STOPPED;
      }
      break;
    case 2:
      if (new_step == 0) {
        direction = ENCODER_CLOCKWISE;
        PositionInc();
      } else if (new_step == 3) {
        direction = ENCODER_COUNTERCLOCKWISE;
        PositionDec();
      } else {
        direction = ENCODER_STOPPED;
      }
      break;
    case 3:
      if (new_step == 2) {
        direction = ENCODER_CLOCKWISE;
        PositionInc();
      } else if (new_step == 1) {
        direction = ENCODER_COUNTERCLOCKWISE;
        PositionDec();
      } else {
        direction = ENCODER_STOPPED;
      }
      break;
    default:
      direction = ENCODER_STOPPED;
      break;
  }

  step_ = new_step;
}

int8_t RotaryEncoder::IncrementalRead() {
  if (direction == ENCODER_CLOCKWISE) {
    return 1;
  } else if (direction == ENCODER_COUNTERCLOCKWISE) {
    return -1;
  }
  return 0;
}

void RotaryEncoder::Reset() {
  position = 0;
  direction = ENCODER_STOPPED;
}