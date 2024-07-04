#!/bin/bash
DEVICE=${1:-sdb1}


# while device does not exist, wait:
while [ ! -b /dev/$DEVICE ]; do
    sleep 1
done

sleep 2

udisksctl mount -b /dev/$DEVICE
