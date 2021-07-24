#!/bin/bash

gcc -o atividade_1 atividade_1.c -Wall -lpthread

echo "compiled"

for THREAD in 1 2 4
do
    for DIM in 500 1000 2000
    do
        for I in $(seq 1 10)
        do
            ./atividade_1  $DIM $THREAD >> "stats/stats_${THREAD}_${DIM}.csv"
            echo "execute ${I} times"
        done
    done
done