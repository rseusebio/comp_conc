#!/bin/bash

gcc -o main main.c -Wall -lpthread

echo "compiled"

for THREAD in 0 2 4 6
do
    for DIM in 5 7 10 12
    do
        echo "Threads, Dimension, StartTime, ExecutionTime, EndTime, TotalTime" >> "stats/stats_${THREAD}_${DIM}.csv"
        for I in $(seq 1 5)
        do
            ./main  $DIM 5 $THREAD >> "stats/stats_${THREAD}_${DIM}.csv"
            echo "Dimension: $DIM and Thread: $THREAD executed ${I} times"
        done
    done
done