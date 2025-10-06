#ifndef __AVR_ATmega328P__
#define __AVR_ATmega328P__
#endif

#ifndef F_CPU
#define F_CPU 16000000ULL // 16Mhz
#endif

#define LOG

#include <avr/io.h>
#include <util/delay.h>
#include <util/twi.h>
#include <avr/interrupt.h>

#ifdef LOG
#include "../../USART/C/USART.cc"
#endif

#define SLC_FREQ (uint32_t)400e3
#define PRESCALER 1
#define TW_RATE  (F_CPU/SLC_FREQ - 16)/(2 * PRESCALER)

uint8_t i2c_data_EEA = 0x00;
uint8_t i2c_addr = 0x00;
uint8_t I2C_send_ACK_stop = 0;
uint16_t i2c_queue = 0;
bool i2c_IO; // 0 = write, 1 = read

#ifdef LOG
USART serial;
#endif

inline void I2C_start(void) {
    // i2c_state = 0; // reset state machine
    TWCR = _BV(TWINT) | _BV(TWSTA) | _BV(TWEN) | _BV(TWIE);
}

void I2C_stop(void) {
    TWCR = _BV(TWINT) | _BV(TWSTO) | _BV(TWEN);
    I2C_send_ACK_stop = 0;
}

inline void I2C_send_ACK(void) {
    TWCR = _BV(TWINT) | _BV(TWEN) | _BV(TWIE) | _BV(TWEA);
}

inline void I2C_send_NACK(void) {
    TWCR = _BV(TWINT) | _BV(TWEN) | _BV(TWIE);
}

void I2C_SLAW() {
    // i2c_IO = 0;
    TWDR = (i2c_addr << 1) | 0;
    I2C_send_ACK();
}
void I2C_SLAR() {
    // i2c_IO = 0;
    TWDR = (i2c_addr << 1) | 1;
    I2C_send_ACK();
}

void I2C_send_EEA() {
    TWDR = i2c_data_EEA;
    I2C_send_ACK();
};

void I2C_send_queue() {

}

void I2C_read(void) {
    // i2c_IO = 1;
    TWCR = _BV(TWINT) | _BV(TWEN) | _BV(TWIE) | _BV(TWEA);
}

void errorHandle(uint8_t code, const char* msg = "") {
#ifdef LOG
    serial.sendStr(msg);
    serial.sendInt(code);
    serial.sendStr("\n");
#endif
}

ISR(TWI_vect) {
    // errorHandle(TW_STATUS, "Interruption: \n");

    switch (TW_STATUS) { // TWSR & 0xF8
        // SLA+W = Master Sends Slave Address then Writes Data to Slave
        // SLA+R = Master Sends Slave Address then Reads Data from Slave
    case TW_REP_START: // 0x10 repeated START
        if (i2c_IO)
            I2C_SLAR();
        else
            I2C_SLAW();
        break;
        // SLA+R | break
        //
    case TW_START: // 0x08 START sent
        I2C_SLAW();
        break;

        // Transmition
    case TW_MT_SLA_ACK: // 0x18 -> SLA+W transmited; <- ACK received
        I2C_send_EEA();
        break;
    case TW_MT_DATA_ACK: // 0x28 -> Data transmited <- ACK received
        if (i2c_IO)
            I2C_start();
        else if (i2c_queue) {
            I2C_send_queue();
            i2c_queue--;
        }
        errorHandle(TW_STATUS, "SLA+W/DATA Received code: ");
        // else Data queue logic
        // write data
        break;

    case TW_MT_SLA_NACK: // 0x20 -> SLA+W transmited; <- NACK received
    case TW_MT_DATA_NACK: // 0x30 -> Data transmited <- NACK received

        // Reception
    case TW_MR_DATA_NACK:		// 0x58 <- data received, NACK returned
    case TW_MR_SLA_NACK:		// 0x48 <- SLA+R transmitted, NACK received
        if (i2c_queue) {
            errorHandle(TW_STATUS, "NACK error code: ");
            i2c_queue = 0;
        } else {
            errorHandle(TW_STATUS, "NACK code: ");
        }

        I2C_send_ACK_stop = 1;
        break;

    case TW_MR_DATA_ACK:		// 0x50 <- data received, ACK returned
        serial.sendInt(TWDR);
        serial.sendStr(" Code: ");
        serial.sendInt(TW_STATUS);
        serial.sendStr("\n");
    case TW_MR_SLA_ACK:		// 0x40 <- SLA+R transmitted, ACK received
        if (i2c_queue) {
            I2C_send_ACK();
            i2c_queue--;
        } else {
            I2C_send_NACK();
        }
        break;


    case TW_MT_ARB_LOST: // 0x38 Arbitration lost in SLA+W or data bytes
        errorHandle(TW_STATUS, "Arbitration Lost code: ");
        I2C_send_ACK_stop = 1;
    default:
        errorHandle(TW_STATUS, "UNK error code: ");
        I2C_send_ACK_stop = 1;
        // Error or unexpected state
        break;
    }
    if (I2C_send_ACK_stop == 1) I2C_stop();
}


// EEPROM READ SEQUENCE

// [S] [CSW]0 ACK [EEA] [S] [CSR]1 ACK [DATA] ...

int main(void) {
    // TWI Bit Rate Register
    TWBR = TW_RATE;

    // Interrups
    sei();

    // EEPROM
    const uint8_t EE_address = 0b00000000;
    // uint8_t EE_packagae_recibed = 0;
    const uint8_t EE_i2c_address = 0b1010000;
    serial.sendStr("SETUP\n");


    while (1) {
        // set address and direction
        serial.sendStr("LOOP\n");
        i2c_data_EEA = EE_address & 0x7f;
        i2c_addr = EE_i2c_address & ~(1 << 1) | ((EE_address) >> 7)<< 1;;
        i2c_IO = 1;
        i2c_queue = 512;

        I2C_start();

        _delay_ms(5000);
    }

    return 0;
}