#include "rotaryEncoder.hpp"

RotaryEncoder::RotaryEncoder(volatile uint8_t *pinAPort, uint8_t pinABit, volatile uint8_t *pinBPort, uint8_t pinBBit, uint8_t stepPerRevolution)
    : pinAPort(pinAPort), pinABit(pinABit), pinBPort(pinBPort), pinBBit(pinBBit), stepPerRevolution(stepPerRevolution), step(0), position(0), direction(ENCODER_STOPPED) {
    // if (pinAPort == pinBPort && pinABit == pinBBit)
    //     static_assert(true, "A signal and B signal cannot be the same");
}

inline void RotaryEncoder::positionInc() {
    if (position < stepPerRevolution) {
        position++;
    } else {
        position = 0;
    }
}
inline void RotaryEncoder::positionDec() {
    if (position > 0) {
        position--;
    } else {
        position = stepPerRevolution;
    }

}

void RotaryEncoder::read() {
    uint8_t new_step = ((*pinAPort & (1 << pinABit)) ? 2 : 0) | ((*pinBPort & (1 << pinBBit)) ? 1 : 0);

    switch (step)
    {
        case 0:
            if (new_step == 1) {
                direction = ENCODER_CLOCKWISE;
                positionInc();
            }
            else if (new_step == 2) {
                direction = ENCODER_COUNTERCLOCKWISE;
                positionDec();
            }
            else
                direction = ENCODER_STOPPED;
            break;
        case 1:
            if (new_step == 3) {
                direction = ENCODER_CLOCKWISE;
                positionInc();
            }
            else if (new_step == 0) {
                direction = ENCODER_COUNTERCLOCKWISE;
                positionDec();
            }
            else
                direction = ENCODER_STOPPED;
            break;
        case 2:
            if (new_step == 0) {
                direction = ENCODER_CLOCKWISE;
                positionInc();
            }
            else if (new_step == 3) {
                direction = ENCODER_COUNTERCLOCKWISE;
                positionDec();
            }
            else
                direction = ENCODER_STOPPED;
            break;
        case 3:
            if (new_step == 2) {
                direction = ENCODER_CLOCKWISE;
                positionInc();
            }
            else if (new_step == 1) {
                direction = ENCODER_COUNTERCLOCKWISE;
                positionDec();
            }
            else
                direction = ENCODER_STOPPED;
            break;
        default:
            direction = ENCODER_STOPPED;
            break;
    }

    step = new_step;
}

int8_t RotaryEncoder::incrementalRead() {
    if (direction == ENCODER_CLOCKWISE) {
        return 1;
    } else if (direction == ENCODER_COUNTERCLOCKWISE) {
        return -1;
    }
    return 0;
}

void RotaryEncoder::reset() {
    position = 0;
    direction = ENCODER_STOPPED;
}