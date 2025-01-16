#!/bin/bash

SIDE=${1:-right}
PARAMS="flash"

if [ -n "$SIDE" ]; then
    PARAMS="flash -bl uf2-split-$SIDE"
fi

echo $PARAMS
./mount.sh > /dev/null &

qmk $PARAMS

killall mount.sh

