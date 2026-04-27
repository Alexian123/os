# os

## Dependencies (Ubuntu/Debian packages) - WIP
- wget
- tar
- build-essential
- bison
- flex
- libgmp3-dev
- libmpc-dev
- libmpfr-dev
- texinfo
- libisl-dev
- nasm
- qemu-system-x86
- grub2
- xorriso
- bochs
- bochs-x
- bochsbios
- bochs-term

## Build & Run Instructions

- Build toolchain (GCC cross compiler & binutils):
```sh
make toolchain
```

- Build project
```sh
make    # defaults to 'make all'
```

- Build and run in QEMU
```sh
make run
```

- Clean project:
```sh
make clean
```