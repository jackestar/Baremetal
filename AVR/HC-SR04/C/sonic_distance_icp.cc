/**
 * @file sonic_distance_icp.cc
 * @brief Implementation of SonicDistanceICP using Timer1 Input Capture on AVR.
 */

#include "sonic_distance_icp.h"

SonicDistanceICP* g_sonic_icp_instance = nullptr;

SonicDistanceICP::SonicDistanceICP(volatile uint8_t* trig_port, uint8_t trigBit, uint8_t prescaler, float sound_speed)
  : trigPort(trig_port), trigBit(trigBit), prescaler(prescaler), soundSpeed(sound_speed), ready(false), start(0), end(0), waitingRising(true) {
  g_sonic_icp_instance = this;
  // Configure trigger pin as output
  // DDRB |= (1 << trigBit);
  // Configure ICP1 (PD6) as input
  // DDRD &= ~(1 << PD6);
  // setupTimer1();
  TCCR1A = 0;
  setPrescaler(prescaler);
  TCCR1B |= (1 << ICES1); // Input capture on rising edge
  TIMSK1 |= (1 << ICIE1); // Enable input capture interrupt
}

// void SonicDistanceICP::setupTimer1() {

// }

void SonicDistanceICP::setPrescaler(uint16_t prescaler) {
  TCCR1B &= ~((1 << CS12) | (1 << CS11) | (1 << CS10));
  switch (prescaler) {
    // 1
  case 1:    TCCR1B |= (1 << CS10); break;
    // 8
  case 8:    TCCR1B |= (1 << CS11); break;
    // 64
  case 64:   TCCR1B |= (1 << CS11) | (1 << CS10); break;
    // 256
  case 256:  TCCR1B |= (1 << CS12); break;
    // 1024
  case 1024: TCCR1B |= (1 << CS12) | (1 << CS10); break;
  default:   TCCR1B |= (1 << CS11); break; // Default to 8
  }
}

void SonicDistanceICP::trigger() {
  *trigPort |= (1 << trigBit);
  _delay_us(10);
  *trigPort &= ~(1 << trigBit);
  ready = false;
  waitingRising = true;
  TCCR1B |= (1 << ICES1); // Set to capture rising edge
  TCNT1 = 0;
}

// bool SonicDistanceICP::isReady() const {
//     return ready;
// }

float SonicDistanceICP::getDistanceMM() const {
  uint16_t ticks = (end > start) ? (end - start) : (65536 + end - start);
  float tickTime_us = (float)prescaler * 1e6f / (float)F_CPU; // time per tick in us
  float time_us = ticks * tickTime_us;
  return (time_us * soundSpeed) / 2.0f; // Divide by 2 for round trip
}

void SonicDistanceICP::handleCapture() {
  if (waitingRising) {
    start = ICR1;
    TCCR1B &= ~(1 << ICES1); // Switch to falling edge
    waitingRising = false;
  } else {
    end = ICR1;
    ready = true;
    TCCR1B |= (1 << ICES1); // Switch back to rising edge
    waitingRising = true;
  }
}

// ISR linkage (put this in your main .cpp):
// ISR(TIMER1_CAPT_vect) { if (g_sonic_icp_instance) g_sonic_icp_instance->handleCapture(); }
