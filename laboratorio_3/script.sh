#!/bin/bash

gcc -o main main.c -Wall -lpthread

echo "compiled"

for THREAD in 0 2 4
do
    for DIM in 100000 10000000 1000000000
    do
        for I in $(seq 1 10)
        do
            ./main $DIM $THREAD >> "stats/stats_${THREAD}_${DIM}.csv"
            echo "executed ${I} times"
        done
    done
done