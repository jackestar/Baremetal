<h3 align="center">Rotary Encoder Library</h3>
<div align="center">
<img src="Rotary_Encoder.svg" width="96">

This library provides a C++ interface for reading a rotary encoder on AVR microcontrollers. It handles tracking the encoder's rotation, direction, and position in a simple and intuitive manner.

</div>

## Features

- **Easy Integration:** Provides a simple class interface to read a rotary encoder.
- **Direction Detection:** Determines if the encoder is rotating clockwise or counterclockwise.
- **Position Tracking:** Maintains the encoder's position and wraps around based on the number of steps per revolution.
- **Example Usage:** Includes an example (`example_rotary_encoder.cc`) demonstrating how to integrate and test the encoder functionality.

## How to Use

1. **Include the header:**
   ```cpp
   #include "rotary_encoder.h"
   ```

2. **Create an instance of the RotaryEncoder:**
   ```cpp
   // Example initialization (pins and steps per revolution may vary)
   RotaryEncoder encoder(&PIND, PIND5, &PIND, PIND6, 60);
   ```

3. **Call the member functions:**
   - Use `encoder.Read()` in your main loop to update the encoder's state.
   - Retrieve incremental changes with `encoder.IncrementalRead()`.
   - Reset the encoder state using `encoder.Reset()` if needed.
   - Check `encoder.position` and `encoder.direction` for the current encoder metrics.

## Example Code

In the example file `example_rotary_encoder.cc`, you can see how the library is used

## Compilation

A sample Makefile is included [Makefile](C/Makefile):

Build:

```
make filename=example_rotary_encoder.cpp
```

Upload

```
make filename=example_rotary_encoder.cpp upload
```

## Resources

- Blog Post (Español): [Incremental Encoder](https://jackestar.netlify.app/Blog/Baremetal/incremental_encoder.html)
- YouTube Demo: [Rotary Encoder Library - example_rotary_encoder.cc [Test]](https://youtu.be/ZgM7oFhrcaU)

---

Feel free to update the paths, links, and additional instructions based on your project specifics and environment.
