# Baremetal

Collection of libraries and programs for microcontrollers in baremetal (Rust/C/C++/ASM)

## Makefile (C/C++)

### AVR

```
Makefile Usage:
  make              - Build debug version with warnings
  make production   - Build optimized version without debug info
  make upload       - Upload firmware
  make upload-force - Force upload
  make clean        - Remove build directory
  make size         - Show program size
```

## Libraries

Where possible, libraries will be written in C++ instead of C, and will follow "OpenTitan's C and C++ Coding Style Guide" based on the "Google C++ Style Guide".

### List

**AVR**

* [Incremental Encoder](./AVR/RotaryEncoder/)
* [HC-SR04](./AVR/HC-SR04/)
* [SLX24C04](./AVR/SLX24C04/)
* [USART](./AVR/USART/)

**IDF**

* [Stepper](./IDF/Stepper/)