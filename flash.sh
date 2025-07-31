#!/bin/bash
SIDE=${1:-right}
COMMAND=""
POST_COMPILE_COMMAND=""

# Option 1: Use functions for clarity
is_arch() {
  type pacman &> /dev/null
}

is_macos() {
  [[ $(uname) == "Darwin" ]]
}

# Display which side is being flashed
echo
echo "Flashing the *$SIDE* side of the keyboard"
if [ "$SIDE" == "left" ]; then
    echo "<----------------------- The one over here"
elif [ "$SIDE" == "right" ]; then
    echo "The one over there ---------------------->"
else
    echo "You passed an invalid side: $SIDE"
    exit 1
fi
echo

if is_arch; then
    # qmk flash works fine on arch, it's mounting that's the problem
    has_automount=$(which udiskie)
    COMMAND="qmk flash -bl uf2-split-$SIDE"
    if [ -z "$has_automount" ]; then
        ./mount.sh > /dev/null 2>&1 &

        mount_pid=$!
        trap "kill $mount_pid; exit" SIGINT
        POST_COMPILE_COMMAND="killall mount.sh > /dev/null 2>&1"
    else
        POST_COMPILE_COMMAND=""
    fi
elif is_macos; then
    if ! type picotool &> /dev/null; then
        echo "Error: picotool is not installed. Please install picotool and try again."
        echo "Run 'brew install picotool' to install picotool."
        exit 1
    fi
    # qmk flash DOES NOT work on macos, but mounting does, so we use picotool for safe flashing
    OPERATION="picotool load"
    # $SIDE needs to be uppercased here
    COMMAND="EXTRAFLAGS=\"-DMASTER_`echo $SIDE | tr '[:lower:]' '[:upper:]'`\" qmk compile"

    # Define a function to wait for the RPI-RP2 drive to appear
    wait_for_drive() {
        local drive="/Volumes/RPI-RP2"
        local timeout=30  # Maximum wait time in seconds
        local start_time=$(date +%s)

        echo "Waiting for RPI-RP2 drive to appear..."

        while [ ! -d "$drive" ]; do
            local current_time=$(date +%s)
            local elapsed=$((current_time - start_time))

            if [ $elapsed -ge $timeout ]; then
                echo "Timeout waiting for RPI-RP2 drive. Make sure your keyboard is in bootloader mode."
                return 1
            fi

            echo -n "."
            sleep 0.5
        done

        echo -e "\nRPI-RP2 drive detected at $drive!"
        return 0
    }

    params() {
        qmk config user.$1 | cut -d'=' -f2 | sed -e 's/\//_/g'
    }

    # Set up trap for SIGINT to properly cleanup
    trap "echo 'Process interrupted. Cleaning up...'; eject_drive; exit" SIGINT

    FILE="`params keyboard`_`params keymap`.uf2"

    POST_COMPILE_COMMAND="wait_for_drive && picotool load ./$FILE && picotool reboot || echo 'Failed to flash due to drive not being detected.'"
    echo $POST_COMPILE_COMMAND
else
    echo "Unknown OS, just use qmk flash"
    exit 1
fi

echo -e "\nrunning: $COMMAND\n"

eval $COMMAND
eval $POST_COMPILE_COMMAND
