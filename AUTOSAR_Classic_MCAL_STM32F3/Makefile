# Makefile for AUTOSAR Classic MCAL STM32F3 Project

# Cross-compiler toolchain prefix
TOOLCHAIN_PREFIX = arm-none-eabi-

# Compiler and Linker
CC = $(TOOLCHAIN_PREFIX)gcc
LD = $(TOOLCHAIN_PREFIX)gcc
AR = $(TOOLCHAIN_PREFIX)ar
AS = $(TOOLCHAIN_PREFIX)as
CP = $(TOOLCHAIN_PREFIX)objcopy
OD = $(TOOLCHAIN_PREFIX)objdump
SZ = $(TOOLCHAIN_PREFIX)size

# Target executable name (without extension)
TARGET_NAME = App_SimpleLedToggle
TARGET_ELF  = $(TARGET_NAME).elf
TARGET_BIN  = $(TARGET_NAME).bin
TARGET_HEX  = $(TARGET_NAME).hex

# Directories
# Assuming this Makefile is in AUTOSAR_Classic_MCAL_STM32F3
ROOT_DIR := .
BUILD_DIR := $(ROOT_DIR)/build

# Source file directories relative to ROOT_DIR
COMMON_SRC_DIR := $(ROOT_DIR)/Common/Src
CONFIG_SRC_DIR := $(ROOT_DIR)/Config  # PBcfg.c files are here
DIO_SRC_DIR    := $(ROOT_DIR)/Dio/Src
PORT_SRC_DIR   := $(ROOT_DIR)/Port/Src
DET_SRC_DIR    := $(ROOT_DIR)/Det/Src
GPT_SRC_DIR    := $(ROOT_DIR)/Gpt/Src
APP_SRC_DIR    := $(ROOT_DIR)/App_Examples/SimpleLedToggle

# Include directories relative to ROOT_DIR
COMMON_INC_DIR := $(ROOT_DIR)/Common/Include
CONFIG_INC_DIR := $(ROOT_DIR)/Config
DIO_INC_DIR    := $(ROOT_DIR)/Dio/Include
PORT_INC_DIR   := $(ROOT_DIR)/Port/Include
DET_INC_DIR    := $(ROOT_DIR)/Det/Include
GPT_INC_DIR    := $(ROOT_DIR)/Gpt/Include
/* PUBLIC_INC_DIR is removed as headers are now module-specific */
APP_INC_DIR    := $(ROOT_DIR)/App_Examples/SimpleLedToggle # If main.c had local headers

# VPATH: List of directories where source files can be found
VPATH = $(COMMON_SRC_DIR):$(CONFIG_SRC_DIR):$(DIO_SRC_DIR):$(PORT_SRC_DIR):$(DET_SRC_DIR):$(GPT_SRC_DIR):$(APP_SRC_DIR)

# C source files (just the basenames, VPATH will find them)
C_SOURCES_BASENAMES = \
    Det.c             \
    Dio_PBcfg.c       \
    Dio.c             \
    Port_PBcfg.c      \
    Port.c            \
    Gpt_PBcfg.c       \
    Gpt.c             \
    main.c

# Object files: All placed flat in BUILD_DIR
OBJS = $(addprefix $(BUILD_DIR)/,$(C_SOURCES_BASENAMES:.c=.o))

# Include paths for the compiler
INCLUDES = \
    -I$(COMMON_INC_DIR) \
    -I$(CONFIG_INC_DIR) \
    -I$(DIO_INC_DIR)    \
    -I$(PORT_INC_DIR)   \
    -I$(DET_INC_DIR)    \
    -I$(GPT_INC_DIR)    \
    -I$(APP_INC_DIR)

# Compiler flags
# STM32F303VCT6 is a common STM32F3 device with Cortex-M4 and FPU. Adjust if needed.
CPU_FLAGS = -mcpu=cortex-m4 -mthumb -mfpu=fpv4-sp-d16 -mfloat-abi=hard
COMMON_CFLAGS = $(CPU_FLAGS) -Wall -Wextra -g -O1 $(INCLUDES) -std=c99
CFLAGS = $(COMMON_CFLAGS)

# Linker script (STM32F303XC is for 256KB Flash devices like STM32F303VC)
# This file needs to be present in the APP_SRC_DIR or path adjusted.
LINKER_SCRIPT = $(APP_SRC_DIR)/stm32f303xc_flash.ld

# Linker flags
LDFLAGS = $(CPU_FLAGS) -T$(LINKER_SCRIPT) -Wl,-Map=$(BUILD_DIR)/$(TARGET_NAME).map,--cref -Wl,--gc-sections --specs=nano.specs --specs=nosys.specs

# Default rule: Build the ELF file and associated formats
all: $(BUILD_DIR)/$(TARGET_ELF) $(BUILD_DIR)/$(TARGET_BIN) $(BUILD_DIR)/$(TARGET_HEX)

# Rule to link object files into an ELF executable
$(BUILD_DIR)/$(TARGET_ELF): $(OBJS)
	@mkdir -p $(@D)
	$(LD) $(OBJS) $(LDFLAGS) -o $@
	@echo "Linking complete: $@"
	@echo "Size of $(TARGET_ELF):"
	$(SZ) $@

# Rule to create BIN file from ELF
$(BUILD_DIR)/$(TARGET_BIN): $(BUILD_DIR)/$(TARGET_ELF)
	$(CP) -O binary $< $@
	@echo "Created BIN file: $@"

# Rule to create HEX file from ELF
$(BUILD_DIR)/$(TARGET_HEX): $(BUILD_DIR)/$(TARGET_ELF)
	$(CP) -O ihex $< $@
	@echo "Created HEX file: $@"

# Rule to compile C source files found via VPATH into object files in BUILD_DIR
$(BUILD_DIR)/%.o: %.c Makefile
	@mkdir -p $(@D)
	$(CC) -c $(CFLAGS) $< -o $@
	@echo "Compiled $< into $@"

# Clean rule: Remove build artifacts
clean:
	rm -rf $(BUILD_DIR)
	@echo "Build directory cleaned."

# Phony targets
.PHONY: all clean
