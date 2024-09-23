# Variables
filename = main
port = /dev/ttyUSB0
mcu = m328p
programmer = arduino

# Targets
default: build/$(filename).hex

build/$(filename).elf: $(filename).c
	@if [ ! -d "build" ]; then mkdir build; fi
	avr-gcc -o $@ $<

build/$(filename).hex: build/$(filename).elf
	avr-objcopy -j .text -j .data -O ihex $< $@

upload: build/$(filename).hex
	avrdude -p $(mcu) -c $(programmer) -P $(port) -U flash:w:$<

# Forzar la programación ignorando la firma y la verificación
upload-force: build/$(filename).hex
	avrdude -F -V -p $(mcu) -c $(programmer) -P $(port) -U flash:w:$<

# Mostrar ayuda
help:
	@echo "Uso del Makefile:"
	@echo "  Makefile                   - Compila $(filename).c y genera los archivos .elf y .hex"
	@echo "  make upload [opc]          - Programa el ATmega328P con el archivo .hex generado"
	@echo "  make upload-force [opc]    - Fuerza la programación del ATmega328P, ignorando la verificación y la firma"
	@echo ""
	@echo "Opciones [opc]"
	@echo "  filename=                  - Nombre del archivo a compilar"
	@echo "  port=                      - Puerto (default $(port))"
	@echo "  mcu=                       - microcontrolador (default $(mcu))"
	@echo "  programmer=                - programador (default $(programmer))"
	@echo "Este Makefile está orientado al Arduino UNO R3 con un ATmega328P."
	@echo "Para utilizar otro microcontrolador AVR, actualiza las variables 'mcu', 'programmer' y declara los macros correspondientes."
