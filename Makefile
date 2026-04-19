ASM=nasm
ASMFLAGS=-f elf32

BUILD_DIR=build
ISO_DIR=$(BUILD_DIR)/isodir
SRC_DIR=src

MAIN_IMAGE=$(BUILD_DIR)/os.raw
MAIN_ISO=$(BUILD_DIR)/os.iso

SOURCES_ASM=$(wildcard $(SRC_DIR)/*.asm)
OBJECTS_ASM=$(patsubst %.asm, $(BUILD_DIR)/asm/%.o, $(SOURCES_ASM))

SOURCES_C=$(wildcard $(SRC_DIR)/*.c)
OBJECTS_C=$(patsubst %.c, $(BUILD_DIR)/c/%.o, $(SOURCES_C))

.PHONY: all run clean

all: $(MAIN_ISO)

run: all
	qemu-system-i386 -kernel $(MAIN_IMAGE)

run_cd: all
	qemu-system-i386 -cdrom $(MAIN_ISO)

$(MAIN_ISO): $(MAIN_IMAGE)
	mkdir -p $(ISO_DIR)/boot/grub
	cp $(MAIN_IMAGE) $(ISO_DIR)/boot/os
	cp grub.cfg $(ISO_DIR)/boot/grub/grub.cfg
	grub-mkrescue -o $@ $(ISO_DIR)

$(MAIN_IMAGE): $(OBJECTS_ASM) $(OBJECTS_C)
	./cross/bin/i686-elf-gcc -T linker.ld -o $@ -ffreestanding -O2 -nostdlib $^ -lgcc

$(BUILD_DIR)/asm/%.o: %.asm
	mkdir -p $(@D)
	$(ASM) $(ASMFLAGS) -o $@ $<
	echo "--> Done: " $<

$(BUILD_DIR)/c/%.o: %.c
	mkdir -p $(@D)
	./cross/bin/i686-elf-gcc -c $< -o $@ -std=gnu99 -ffreestanding -O2 -Wall -Wextra
	echo "--> Done: " $<

clean:
	rm -rf $(BUILD_DIR)
	rm -rf $(ISO_DIR)