#!/bin/bash

# Get the initial list of devices
initial_devices=$(ls /dev/sd*[0-9] /dev/ttyACM*[0-9] 2>/dev/null)

echo "Waiting for new devices..."

while true; do
    # Get the current list of devices
    current_devices=$(ls /dev/sd*[0-9] /dev/ttyACM*[0-9] 2>/dev/null)

    # Compare the current list with the initial list
    new_devices=$(comm -13 <(echo "$initial_devices" | sort) <(echo "$current_devices" | sort))

    if [ -n "$new_devices" ]; then
        DEVICE=$(echo "$new_devices" | head -n 1 | sed 's/\/dev\///')
        break
    fi

    # Wait before checking again
    sleep 0.1
done


sleep 2


# if it fails the first time, continue trying
while ! udisksctl mount -b /dev/$DEVICE > /dev/null; do
    sleep 0.1
done

echo "Mounted $DEVICE"
