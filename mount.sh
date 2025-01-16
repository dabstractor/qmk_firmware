#!/bin/bash
DEVICE=${1:-sdb1}


attempts=0
# while device does not exist, wait:
while [ ! -b /dev/$DEVICE ] && [ $attempts -lt 10 ]; do
    sleep 1
    attempts=$((attempts + 1))
done

sleep 2

attempts=0

# while attempt counter is less than 10
while [ $attempts -lt 40 ] && ! udisksctl mount -b /dev/$DEVICE > /dev/null; do
    sleep 1
    attempts=$((attempts + 1))
done

echo "Mounted $DEVICE"
