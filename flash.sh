#!/bin/bash

SIDE=${1:-left}

qmk flash \
    -kb handwired/dactyl_manuform/5x7_1 \
    -bl PICO_PLATFORM=rp2040 \
    -e SIDE=$SIDE \
    -e CONVERT_TO=promicro_rp2040 \
    keymap.json \
    && \

    cp ./.build/handwired_dactyl_manuform_5x7_handwired_dactyl_manuform_5x7_layout_5x7_mine_promicro_rp2040.uf2 \
       ./dactyl_$SIDE.uf2 \

    sudo picotool load ./dactyl_$SIDE.uf2

