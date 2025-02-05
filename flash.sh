#!/bin/bash

SIDE=${1:-right}
PARAMS="flash"

if [ -n "$SIDE" ]; then
    PARAMS="flash -bl uf2-split-$SIDE"
fi

echo $PARAMS
./mount.sh > /dev/null 2>&1 &

mount_pid=$!

trap "kill $mount_pid; exit" SIGINT

qmk $PARAMS

killall mount.sh > /dev/null 2>&1

