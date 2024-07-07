#!/bin/bash

SIDE=${1:-left}
PARAMS="flash"

if [ -n "$SIDE" ]; then
    PARAMS="-v flash -bl uf2-split-$SIDE"
fi

echo $PARAMS
./mount.sh &

qmk $PARAMS

