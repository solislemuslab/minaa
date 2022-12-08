#! /bin/bash

for n in 10 30 50
do
    for p in 0.05 0.1 0.9
    do
        for i in {1..30}
        do
            .././mna.exe ../testdata/simulated/Graph1-$n-$p-$i.csv ../testdata/simulated/Graph2-$n-$p-$i.csv
        done
    done
done
