# Makefile AVR Version 2.1

# Variables
filename = main
port = /dev/ttyUSB0
mcu = atmega328p
f_cpu = 16000000L
programmer = arduino

# Compiler and tools
AVR_GCC = avr-gcc
AVR_GXX = avr-g++
AVR_OBJCOPY = avr-objcopy
AVRDUDE = avrdude
AVR_SIZE = avr-size

# Build directories
build_dir = build
debug_obj_dir = $(build_dir)/debug/obj
production_obj_dir = $(build_dir)/production/obj
obj_dir = $(debug_obj_dir)

# Compiler flags (default: debug build with warnings)
COMMON_FLAGS = -c -g -Os -ffunction-sections -fdata-sections -MMD -mmcu=$(mcu) -DF_CPU=$(f_cpu)
CFLAGS = $(COMMON_FLAGS)
CXXFLAGS = $(COMMON_FLAGS) -fno-exceptions -fno-threadsafe-statics -fno-rtti
LDFLAGS = -mmcu=$(mcu) -Wl,--gc-sections

# Source files (handles both .c and .cpp via filename=)
src_files = $(filename)

# Objects
obj_files = $(addprefix $(obj_dir)/, $(addsuffix .o, $(basename $(notdir $(src_files)))))

# Link objects
link_objects = $(obj_files)

# Includes
includes = -I.

# Targets
default: $(build_dir)/$(filename).hex

# Compile C files
$(obj_dir)/%.o: %.c | $(obj_dir)
	$(AVR_GCC) $(CFLAGS) $(includes) $< -o $@

# Compile C++ files
$(obj_dir)/%.o: %.cpp | $(obj_dir)
	$(AVR_GXX) $(CXXFLAGS) $(includes) $< -o $@

# Create build directories
$(debug_obj_dir) $(production_obj_dir) $(build_dir):
	mkdir -p $@

# Link to .elf
$(build_dir)/$(filename).elf: $(link_objects) | $(build_dir)
	$(AVR_GXX) $(LDFLAGS) -o "$@" $^ -lm

# Create .hex from .elf
$(build_dir)/$(filename).hex: $(build_dir)/$(filename).elf
	$(AVR_OBJCOPY) -j .text -j .data -O ihex "$<" "$@"

# Production target (no debug, suppressed warnings)
production: obj_dir = $(production_obj_dir)
production: COMMON_FLAGS = -c -Os -w -ffunction-sections -fdata-sections -MMD -mmcu=$(mcu) -DF_CPU=$(f_cpu)
production: CFLAGS = $(COMMON_FLAGS)
production: CXXFLAGS = $(COMMON_FLAGS) -fno-exceptions -fno-threadsafe-statics -fno-rtti
production: $(build_dir)/$(filename).hex

# Upload targets
upload: $(build_dir)/$(filename).hex
	$(AVRDUDE) -v -p $(mcu) -c $(programmer) -P $(port) -b 115200 -D -U flash:w:$<:i

upload-force: $(build_dir)/$(filename).hex
	$(AVRDUDE) -v -p $(mcu) -c $(programmer) -P $(port) -b 115200 -D -F -U flash:w:$<:i

size: $(build_dir)/$(filename).elf
	$(AVR_SIZE) -A "$<"

clean:
	rm -rf $(build_dir)

help:
	@echo "Makefile Usage:"
	@echo "  make              - Build debug version with warnings"
	@echo "  make production   - Build optimized version without debug info"
	@echo "  make upload       - Upload firmware"
	@echo "  make upload-force - Force upload"
	@echo "  make clean        - Remove build directory"
	@echo "  make size         - Show program size"
	@echo ""
	@echo "Variables:"
	@echo "  filename	    - Filename without extention [default main]"
	@echo "  port		    - Select the port to be use [default /dev/ttyUSB0]"
	@echo "  mcu		    - Select the microcontroller [default atmega328p]"
	@echo "  f_cpu		    - Select the frequency [default 16000000L]"
	@echo "  programmer	    - Select the programmer [default arduino]"

.SUFFIXES:  # Disable implicit rules
.PHONY: default production upload upload-force size clean help