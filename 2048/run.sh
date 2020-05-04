#!/bin/bash

if [[ $# -ne 1 ]]; then
    echo 'Usage:run filename'
    exit
fi

if [[ $1 != game.cpp ]]; then
    echo 'Usage:run game.cpp'
    exit
fi

g++ $1 include.cpp -lcurses -o 2048

./2048
