# Variables
folder = Arduino
PlataformID = arduino:avr:uno
port = /dev/ttyUSB0

# Targets
default:
	arduino-cli compile --build-path ./build --fqbn $(PlataformID) $(folder)

upload:
	arduino-cli compile --fqbn $(PlataformID) $(folder)
	arduino-cli upload   -p $(port) --fqbn $(PlataformID) $(folder)

# Mostrar ayuda
help:
	@echo "Uso del Makefile:"
	@echo "  make                  - Compila el sketch $(folder) en el directorio build
	@echo "  make upload           - Compila y Programa el Arduino UNO"
	@echo ""
	@echo "Opciones [opc]"
	@echo "  folder=               - Sketck (default $(folder))"
	@echo "  PlataformID=          - ID de plataforma (default $(PlataformID))"
	@echo "  port=                 - Puerto (default $(port))"
	@echo "Este Makefile está orientado al Arduino UNO R3 con un ATmega328P."
	@echo "Para utilizar otro microcontrolador AVR, actualiza las variables 'PlataformID'"
