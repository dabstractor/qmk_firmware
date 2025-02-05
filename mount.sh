#!/bin/bash
DEVICE=${1:-sdb1}


# while device does not exist, wait:
while [ ! -b /dev/$DEVICE ]; do
    sleep 0.2
done

sleep 2


# if it fails the first time, continue trying
while ! udisksctl mount -b /dev/$DEVICE > /dev/null; do
    sleep 0.1
done

echo "Mounted $DEVICE"
