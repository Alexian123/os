#!/usr/bin/bash

set -e

if [ $# -lt 1 ]; then
    echo "Usage: ${1} [OUTPUT_FILE]"
    exit 1
fi

echo "Generating assembly ISR stubs... "

FILE=$1     
if [ -f $FILE ]; then
   echo "File $FILE already exists! Overwriting..."
else
   echo "File $FILE will be created."
fi

echo "; ! THIS FILE IS GENERATED AUTOMATICALLY !" > $FILE

ISRS_WITH_ERROR="8 10 11 12 13 14 17 30"
for i in {0..255}; do
    if echo "$ISRS_WITH_ERROR" | grep -q "\b${i}\b"; then
        echo "ISR_ERR ${i}" >> $FILE
    else
        echo "ISR_NOERR ${i}" >> $FILE
    fi
done

echo "Done."

exit 0