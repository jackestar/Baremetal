#include "stepper_driver.h"

StepperDriver::StepperDriver(gpio_num_t gpioA, gpio_num_t gpioB, gpio_num_t gpioC, gpio_num_t gpioD)
  : direction_(kClockwise), step_speed_(1), priority_(1), status_(0) {

  // Store GPIO pins and initialize drive state. The constructor configures
  // the GPIOs as outputs so the driver is ready to emit patterns after
  // Start() is called.
  gpioA_ = gpioA;
  gpioB_ = gpioB;
  gpioC_ = gpioC;
  gpioD_ = gpioD;

  // A negative step_count_ denotes "run forever". Use Step(n) to set a
  // finite positive count.
  step_count_ = -1;

  gpio_set_direction(gpioA_, GPIO_MODE_OUTPUT);
  gpio_set_direction(gpioB_, GPIO_MODE_OUTPUT);
  gpio_set_direction(gpioC_, GPIO_MODE_OUTPUT);
  gpio_set_direction(gpioD_, GPIO_MODE_OUTPUT);

  // Default to the first pattern. Start() will align this with the
  // configured direction.
  pattern_ = 0;
}

void StepperDriver::Start() {
  // Mark the driver as running and ensure the pattern starts consistent with
  // the configured direction.
  status_ = 1;
  pattern_ = direction_ == kClockwise ? 0 : 3;

  // Configure the hardware timer. This timer uses a high-resolution clock
  // (RESOLUTION = 1 MHz) so the alarm period needs to be computed in ticks.
  gptimer_config_t timer_config = {
      .clk_src = GPTIMER_CLK_SRC_DEFAULT,  // default clock source
      .direction = GPTIMER_COUNT_UP,       // counting direction up
      .resolution_hz = RESOLUTION,         // 1 MHz resolution
      .intr_priority = priority_,          // interrupt priority
      .flags = {
          .intr_shared = 0,
          .allow_pd = 0,
      },
  };

  // Create the timer only once. Repeated calls to Start() should reuse the
  // existing timer handle to avoid leaking resources. If creation fails the
  // driver is left in a stopped state (status_ == 0) so callers can detect
  // and handle the error.
  if (gptimer_ == NULL) {
    esp_err_t err = gptimer_new_timer(&timer_config, &gptimer_);
    if (err != ESP_OK) {
      // Failed to create a timer; mark as stopped and return.
      status_ = 0;
      return;
    }
  }

  // Compute alarm_count in timer ticks. The original code divided by 4; this
  // code keeps the same semantics (application-level interpretation of
  // `step_speed_` should be clarified to callers - see header comments).
  // Compute alarm_count in timer ticks. Use 64-bit arithmetic for the
  // intermediate multiplication to avoid overflow when `step_speed_` is
  // large. The original code multiplied in 32-bit which could overflow.
  uint64_t ticks = RESOLUTION / ((uint64_t)step_speed_ * 4);
  if (ticks == 0) {
    ticks = 1;
  }

  gptimer_alarm_config_t alarm_config = {
      .alarm_count = ticks,   // set the actual alarm period
      .reload_count = 0,      // when the alarm event occurs, reload to 0
      .flags = {
          .auto_reload_on_alarm = true,  // enable auto-reload
      },
  };

  // Apply alarm configuration and register the callback. The callback runs
  // in timer ISR context so keep it minimal.
  gptimer_set_alarm_action(gptimer_, &alarm_config);

  gptimer_event_callbacks_t cbs = {
      .on_alarm = StepperTask,  // called on each alarm
  };
  gptimer_register_event_callbacks(gptimer_, &cbs, this);

  // Enable and start the timer. If the timer is already running these calls
  // are idempotent for the intended use (driver will resume emitting
  // patterns).
  gptimer_enable(gptimer_);
  gptimer_start(gptimer_);
}

bool StepperDriver::StepperTask(gptimer_handle_t /*timer*/, const gptimer_alarm_event_data_t* /*edata*/, void* user_ctx) {
  // This callback is invoked in timer ISR context. Keep work minimal and
  // deterministic. Do not block.
  StepperDriver* self = static_cast<StepperDriver*>(user_ctx);

  // Output the 4-phase drive pattern. The pattern_ index must be in [0,3]. If
  // it's not, normalize it to 0.
  switch (self->pattern_) {
  case 0:
    gpio_set_level(self->gpioA_, 1);
    gpio_set_level(self->gpioB_, 0);
    gpio_set_level(self->gpioC_, 1);
    gpio_set_level(self->gpioD_, 0);
    break;
  case 1:
    gpio_set_level(self->gpioA_, 0);
    gpio_set_level(self->gpioB_, 1);
    gpio_set_level(self->gpioC_, 1);
    gpio_set_level(self->gpioD_, 0);
    break;
  case 2:
    gpio_set_level(self->gpioA_, 0);
    gpio_set_level(self->gpioB_, 1);
    gpio_set_level(self->gpioC_, 0);
    gpio_set_level(self->gpioD_, 1);
    break;
  case 3:
    gpio_set_level(self->gpioA_, 1);
    gpio_set_level(self->gpioB_, 0);
    gpio_set_level(self->gpioC_, 0);
    gpio_set_level(self->gpioD_, 1);
    break;
  default:
    // Ensure a valid pattern index.
    self->pattern_ = 0;
    break;
  }

  // Advance the pattern index according to direction. Use modulo-4
  // arithmetic so the index always stays in [0,3]. This advances one pattern
  // per callback invocation.
  if (self->direction_ == kCounterclockwise) {
    self->pattern_ = (self->pattern_ - 1) & 0x3;
  } else {  // kClockwise
    self->pattern_ = (self->pattern_ + 1) & 0x3;
  }

  // If a finite step count was requested (positive), decrement it on every
  // pattern advance. When it reaches zero stop the driver.
  if (self->step_count_ > 0) {
    self->step_count_--;
    if (self->step_count_ == 0) {
      self->Stop();
    }
  }

  // Return value indicates whether a higher-priority task was woken. There is
  // no task wake here so return false.
  return false;
}

void StepperDriver::Step(uint32_t steps) {
  // Set the requested number of patterns to output.
  // - A value of 0: stop immediately.
  // - A positive value: execute that many pattern steps.
  // - A negative value in `step_count_` denotes "run forever".
  // This method starts the timer if a positive (or infinite) run is
  // requested; Step(0) will stop the driver immediately.
  if (steps == 0) {
    step_count_ = 0;
    Stop();
    return;
  }

  step_count_ = static_cast<int32_t>(steps);
  Start();
}

void StepperDriver::Stop() {
  // Mark stopped, set pattern to a known state aligned with direction and
  // stop the timer. We intentionally do not delete the timer handle here so
  // `Start()` can reuse it without reallocation.
  status_ = 0;
  pattern_ = direction_ == kClockwise ? 0 : 3;

  // Guard against the timer handle being NULL. The caller may call Stop()
  // even if Start() failed earlier.
  if (gptimer_ != NULL) {
    gptimer_stop(gptimer_);
    gptimer_disable(gptimer_);
  }
}