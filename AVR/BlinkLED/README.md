## Make para Arduino CLI

```
Uso del Makefile:
  make                  - Compila el sketch Blink en el directorio build
  make upload           - Compila y Programa el Arduino UNO
  make help             - Muestra esta información

Opciones [opc]
  folder=               - Sketch (default Blink)
  PlataformID=          - ID de plataforma (default arduino:avr:uno)
  port=                 - Puerto (default /dev/ttyUSB0)
Este Makefile está orientado al Arduino UNO R3 con un ATmega328P.
Para utilizar otro microcontrolador AVR, actualiza las variables 'PlataformID'
```

## Make para C

```
Uso del Makefile:
  Makefile                   - Compila baremetal.c y genera los archivos .elf y .hex
  make upload [opc]          - Programa el ATmega328P con el archivo .hex generado
  make upload-force [opc]    - Fuerza la programación del ATmega328P, ignorando la verificación y la firma

Opciones [opc]
  filename=                  - Nombre del archivo a compilar
  port=                      - Puerto (default /dev/ttyUSB69)
  mcu=                       - microcontrolador (default m328p)
  programmer=                - programador (default arduino)
Este Makefile está orientado al Arduino UNO R3 con un ATmega328P.
Para utilizar otro microcontrolador AVR, actualiza las variables 'mcu', 'programmer' y declara los macros correspondientes.
```

## Rust

Compilar release

```
cargo build --profile=release
```

Programar

cargo run