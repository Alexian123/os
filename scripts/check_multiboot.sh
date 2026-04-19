#!/usr/bin/bash

if [ $# -lt 1 ]; then
    echo "Usage: ${1} [FILE]"
    exit 1
fi

echo -n "Checking if $1 is a valid multiboot file... "

grub-file --is-x86-multiboot "$1" 2> /dev/null

if [ $? -eq 0 ]; then
    echo "PASSED"
else
    echo "FAILED"
    exit 1
fi

exit 0