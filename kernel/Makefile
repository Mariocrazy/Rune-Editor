# Cross-compiler settings
CROSS_COMPILE ?= arm-none-eabi-
CC = $(CROSS_COMPILE)gcc
AS = $(CROSS_COMPILE)as
LD = $(CROSS_COMPILE)ld
OBJCOPY = $(CROSS_COMPILE)objcopy
OBJDUMP = $(CROSS_COMPILE)objdump

# Compiler flags
CFLAGS = -mcpu=arm1176jzf-s -fpic -ffreestanding -std=gnu99 -O2 -Wall -Wextra
ASFLAGS = -mcpu=arm1176jzf-s
LDFLAGS = -T src/kernel.ld -nostdlib -nostartfiles

# Directories
BUILD_DIR = build
SRC_DIR = src
INCLUDE_DIR = include

# Source files
C_SOURCES = $(wildcard $(SRC_DIR)/*.c)
ASM_SOURCES = $(wildcard $(SRC_DIR)/*.S)
HEADERS = $(wildcard $(INCLUDE_DIR)/*.h)

# Object files
OBJ_DIR = $(BUILD_DIR)/obj
C_OBJECTS = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(C_SOURCES))
ASM_OBJECTS = $(patsubst $(SRC_DIR)/%.S,$(OBJ_DIR)/%.o,$(ASM_SOURCES))
OBJECTS = $(ASM_OBJECTS) $(C_OBJECTS)

# Output files
KERNEL = $(BUILD_DIR)/kernel.img
KERNEL_ELF = $(BUILD_DIR)/kernel.elf

# Default target
all: directories $(KERNEL)

# Create build directories
directories:
	@mkdir -p $(BUILD_DIR)
	@mkdir -p $(OBJ_DIR)

# Build the kernel image
$(KERNEL): $(KERNEL_ELF)
	@echo "Creating kernel image..."
	$(OBJCOPY) $(KERNEL_ELF) -O binary $@

# Link the kernel
$(KERNEL_ELF): $(OBJECTS)
	@echo "Linking kernel..."
	$(LD) $(LDFLAGS) $(OBJECTS) -o $@

# Compile C source files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(HEADERS)
	@echo "Compiling $<..."
	$(CC) $(CFLAGS) -I$(INCLUDE_DIR) -c $< -o $@

# Compile assembly source files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.S
	@echo "Assembling $<..."
	$(AS) $(ASFLAGS) $< -o $@

# Clean build files
clean:
	@rm -rf $(BUILD_DIR)

# Create SD card image
sdcard: $(KERNEL)
	@echo "Creating SD card image..."
	@dd if=/dev/zero of=$(BUILD_DIR)/sdcard.img bs=1M count=32
	@mkfs.vfat $(BUILD_DIR)/sdcard.img
	@mkdir -p $(BUILD_DIR)/boot
	@mount $(BUILD_DIR)/sdcard.img $(BUILD_DIR)/boot
	@cp $(KERNEL) $(BUILD_DIR)/boot/
	@cp bootcode.bin $(BUILD_DIR)/boot/
	@cp start.elf $(BUILD_DIR)/boot/
	@cp config.txt $(BUILD_DIR)/boot/
	@umount $(BUILD_DIR)/boot
	@rmdir $(BUILD_DIR)/boot

.PHONY: all clean sdcard directories
