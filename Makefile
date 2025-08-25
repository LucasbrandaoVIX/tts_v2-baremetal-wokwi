# Makefile for TTS_V2 Project - EFM32GG
# Target: EFM32GG995F1024

# Toolchain
CC = arm-none-eabi-gcc
OBJCOPY = arm-none-eabi-objcopy
OBJDUMP = arm-none-eabi-objdump
SIZE = arm-none-eabi-size

# Target device
DEVICE = EFM32GG995F1024

# Project name
PROJECT = tts_v2

# Directories
SRC_DIR = src
INC_DIR = include
BUILD_DIR = build

# Source files
SOURCES = $(SRC_DIR)/main.c $(SRC_DIR)/tt_tasks-v2.c $(SRC_DIR)/led.c $(SRC_DIR)/display.c $(SRC_DIR)/adc.c

# Object files
OBJECTS = $(SOURCES:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)

# Include directories
INCLUDES = -I$(INC_DIR) -I./CMSIS/Include -I./Device/SiliconLabs/EFM32GG/Include

# Compiler flags
CFLAGS = -mcpu=cortex-m3 -mthumb -O2 -g -Wall -Wextra
CFLAGS += -D$(DEVICE) -DARM_MATH_CM3
CFLAGS += $(INCLUDES)
CFLAGS += -ffunction-sections -fdata-sections

# Linker flags
LDFLAGS = -mcpu=cortex-m3 -mthumb -T efm32gg.ld
LDFLAGS += -Wl,--gc-sections -Wl,--print-memory-usage

# Create build directory
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Default target
all: $(BUILD_DIR) $(BUILD_DIR)/$(PROJECT).elf $(BUILD_DIR)/$(PROJECT).hex $(BUILD_DIR)/$(PROJECT).bin

# Compile C files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Link
$(BUILD_DIR)/$(PROJECT).elf: $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@
	$(SIZE) $@

# Generate hex file
$(BUILD_DIR)/$(PROJECT).hex: $(BUILD_DIR)/$(PROJECT).elf
	$(OBJCOPY) -O ihex $< $@

# Generate binary file
$(BUILD_DIR)/$(PROJECT).bin: $(BUILD_DIR)/$(PROJECT).elf
	$(OBJCOPY) -O binary $< $@

# Generate disassembly
$(BUILD_DIR)/$(PROJECT).s: $(BUILD_DIR)/$(PROJECT).elf
	$(OBJDUMP) -d $< > $@

# Clean
clean:
	rm -rf $(BUILD_DIR)

# Phony targets
.PHONY: all clean
