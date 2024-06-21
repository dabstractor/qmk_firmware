#!/bin/bash

SIDE=${1:-left}

if [ "$SIDE" = "left" ]; then
    qmk compile -kb handwired/dactyl_manuform/5x7_1 ./keymap.json &&\
    sudo picotool load .build/handwired_dactyl_manuform_5x7_1_default.uf2
else
    qmk compile -kb handwired/dactyl_manuform/5x7_1 ./keymap.json &&\
    sudo picotool load .build/handwired_dactyl_manuform_5x7_1_default.uf2
fi

# qmk compile -kb handwired/jankrp2040dactyl ~/Downloads/handwired_jankrp2040dactyl_testes.json
# sudo picotool load -f ./handwired_jankrp2040dactyl_handwired_jankrp2040dactyl_testes.uf2

# qmk compile -kb ziptyze/lets_split_v3 /home/dustin/Downloads/ziptyze_lets_split_v3_testes.json
# sudo picotool load -f ./ziptyze_lets_split_v3_ziptyze_lets_split_v3_testes.uf2


