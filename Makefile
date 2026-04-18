ASM=nasm
AMSFLAGS=-f bin

.PHONY: all run clean

all: boot.bin

run: all
	qemu-system-i386 -hda boot.bin

boot.bin: boot.asm
	$(ASM) $< $(AMSFLAGS) -o $@

clean:
	rm -f boot.bin