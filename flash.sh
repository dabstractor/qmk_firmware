#!/bin/bash


qmk compile -kb handwired/dactyl_manuform/5x7_1 ./keymap.json &&\
sudo picotool load .build/handwired_dactyl_manuform_5x7_1_default.uf2



