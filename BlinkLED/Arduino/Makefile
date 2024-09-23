# Variables
folder = Blink
PlataformID = arduino:avr:uno
port = /dev/ttyUSB0

# Targets
default:
	arduino-cli compile --output-dir ./build --fqbn $(PlataformID) $(folder)

upload:
	arduino-cli compile --fqbn $(PlataformID) $(folder)
	arduino-cli upload -p $(port) --fqbn $(PlataformID) $(folder)

# Mostrar ayuda
help:
	@echo "Uso del Makefile:"
	@echo "  make                  - Compila el sketch $(folder) en el directorio build"
	@echo "  make upload           - Compila y Programa el Arduino UNO"
	@echo "  make help             - Muestra esta información"
	@echo ""
	@echo "Opciones [opc]"
	@echo "  folder=               - Sketch (default $(folder))"
	@echo "  PlataformID=          - ID de plataforma (default $(PlataformID))"
	@echo "  port=                 - Puerto (default $(port))"
	@echo "Este Makefile está orientado al Arduino UNO R3 con un ATmega328P."
	@echo "Para utilizar otro microcontrolador AVR, actualiza las variables 'PlataformID'"
