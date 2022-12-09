#! /bin/bash

for n in 10 30 50 100 250 500
do
    for p in 0.05 0.1 0.9
    do
        for i in {1..30}
        do
            ../minaa.exe ../testdata/simulated/Graph1-$n-$p-$i.csv ../testdata/simulated/Graph2-$n-$p-$i.csv
        done
    done
done

# Calculate average runtimes (must edit minaa to print only runtime)
# for n in 10 30 50 100 250 500
# do
#     avg=0
#     for p in 0.05 0.1 0.9
#     do
#         for i in {1..30}
#         do
#             out=$(../minaa.exe ../testdata/simulated/Graph1-$n-$p-$i.csv ../testdata/simulated/Graph2-$n-$p-$i.csv)
#             avg=$((avg + out))
#         done
#     done
#     echo $n ":" $avg
# done
