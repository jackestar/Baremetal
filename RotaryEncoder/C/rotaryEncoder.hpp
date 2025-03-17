// Rotary Relative Encoder

#ifndef _ROTARY_ENCODER_

    #define _ROTARY_ENCODER_

    #ifndef _AVR_IO_H_

        #include <avr/io.h>

    #endif

typedef enum {
    ENCODER_STOPPED,
    ENCODER_CLOCKWISE,
    ENCODER_COUNTERCLOCKWISE
} encoderDirection;

class RotaryEncoder
{
    public:
        encoderDirection direction;
        volatile uint8_t *pinAPort;
        uint8_t pinABit;
        volatile uint8_t *pinBPort;
        uint8_t pinBBit;
        uint8_t stepPerRevolution;
        uint8_t position; // 8bit resolution
    
        RotaryEncoder(volatile uint8_t *pinAPort, uint8_t pinABit, volatile uint8_t *pinBPort, uint8_t pinBBit, uint8_t stepPerRevolution);
        void read();
        int8_t incrementalRead();
        void reset();
    private:
        uint8_t step;
        
        inline void positionInc();
        inline void positionDec();
};

#endif