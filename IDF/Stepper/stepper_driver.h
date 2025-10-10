#pragma once

#include <stdint.h>
#include "freertos/FreeRTOS.h"
#include "driver/gpio.h"
#include "driver/gptimer.h"

#define RESOLUTION 1000000ULL // 1MHz timer resolution

typedef enum {
  kClockwise,
  kCounterclockwise
} Direction;

// StepperDriver controls a 4-phase stepper motor using the ESP32 GPTIMER
// peripheral. It toggles four GPIOs in a 4-step sequence at a rate derived
// from `step_speed_` and the high-resolution timer defined by RESOLUTION.
//
// Design contract (short):
// - Inputs: direction (clockwise/counterclockwise), step_speed (unitless
//   application-level rate), optional finite step count.
// - Outputs: four GPIO pins driven with a 4-step sequence.
// - Error modes: timer creation failure leaves the driver stopped
//   (status_ == 0). Callers may inspect status_ to detect failures.
// - Success: timer running and patterns emitted at the configured rate.
//
// Threading / ISR notes:
// - The timer callback (`StepperTask`) executes in timer callback (ISR)
//   context. Work done there must be minimal and non-blocking.
// - `StepperTask` toggles GPIOs and updates small integer state only. It
//   does not perform heap allocations or blocking calls.
// - Public methods (Start/Stop/Step) may be called from task context. The
//   implementation attempts to avoid reallocating the timer handle on
//   repeated `Start()` calls.
//
// Style notes:
// - Inline accessors are kept in the header for performance and ABI
//   locality. They are intentionally trivial (no locking). Callers are
//   responsible for synchronization if they access the driver from multiple
//   threads concurrently.
class StepperDriver {
 public:
  // Construct a StepperDriver bound to four GPIO pins. Pins are configured
  // as outputs by the constructor.
  StepperDriver(gpio_num_t gpioA, gpio_num_t gpioB, gpio_num_t gpioC, gpio_num_t gpioD);

  inline Direction GetDirection() {
    return direction_;
  }

  inline void SetDirection(Direction direction) {
    if (direction != kClockwise && direction != kCounterclockwise) {
      direction_ = kClockwise;
    } else {
      direction_ = direction;
    }
  }

  inline uint32_t GetStepSpeed() {
    return step_speed_;
  }

  // Set the step speed. Values less than 1 are clamped to 1.
  inline void SetStepSpeed(uint32_t speed) {
    if (speed < 1) {
      speed = 1;
    }
    step_speed_ = speed;
  }

  inline int GetPriority() {
    return priority_;
  }

  inline void SetPriority(int priority) {
    priority_ = priority;
  }

  inline uint32_t GetStatus() {
    return status_;
  }

  // Start the timer and begin driving the motor. Safe to call when already
  // started; the implementation will reuse the existing timer handle.
  void Start();

  // Stop driving the motor. Leaves the timer handle intact so Start() can be
  // called again without reallocating resources.
  void Stop();

  // Request an exact number of pattern steps to be executed. Calling Step(0)
  // will result in no movement. Use Step() followed by Start() is not
  // required: Step() will call Start().
  void Step(uint32_t steps);

 private:
  // GPIO pins used to drive the coils.
  gpio_num_t gpioA_;
  gpio_num_t gpioB_;
  gpio_num_t gpioC_;
  gpio_num_t gpioD_;

  // Current drive pattern index in [0,3].
  int32_t pattern_;

  // Remaining steps to execute. A negative value indicates "run forever".
  int32_t step_count_;

  // GPTimer handle. Initialized to NULL and created on the first Start().
  gptimer_handle_t gptimer_ = NULL;

  // Configuration/state fields.
  Direction direction_;
  uint32_t step_speed_;
  int priority_;
  uint32_t status_;

  // Timer callback invoked from ISR context.
  static bool StepperTask(gptimer_handle_t timer, const gptimer_alarm_event_data_t* edata, void* user_ctx);
};