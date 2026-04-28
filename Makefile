include config/config.mk

.PHONY: all run debug always run_iso clean iso raw kernel libc

all: iso

include config/toolchain.mk

run: raw
	qemu-system-i386 -debugcon stdio -kernel $(BUILD_DIR)/kernel.bin

run_iso: iso
	qemu-system-i386 -cdrom $(BUILD_DIR)/os.iso

debug: iso
	bochs -f $(CONFIG_DIR)/bochs.cfg

iso: raw
	mkdir -p $(ISO_DIR)/boot/grub
	cp $(BUILD_DIR)/kernel.bin $(ISO_DIR)/boot/os
	cp $(CONFIG_DIR)/grub.cfg $(ISO_DIR)/boot/grub/grub.cfg
	grub-mkrescue -o $(BUILD_DIR)/os.iso $(ISO_DIR)
	@echo "--> Created  os.iso"

raw: kernel

kernel: $(BUILD_DIR)/kernel.bin
	@$(SCRIPTS_DIR)/check_multiboot.sh $<

$(BUILD_DIR)/kernel.bin: always
	$(MAKE) -C $(KERNEL_SRC_DIR)

always: clean

clean:
	$(MAKE) -C $(KERNEL_SRC_DIR) clean
	rm -rf $(ISO_DIR)
	rm -f $(BUILD_DIR)/os.iso