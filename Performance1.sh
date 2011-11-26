#!/bin/bash
REQUEST_THREADS=2
WORK_THREADS=25
MAX_THREADS=100
while [ $REQUEST_THREADS -lt $MAX_THREADS ]; do
    ./a.out $REQUEST_THREADS $WORK_THREADS < data.txt
    wait $!
    result1=$(awk '{sum=sum+$1} END {print sum/NR}' TimePerRequest.txt)
    result2=$(awk '{sum=sum+$2} END {print sum/NR}' TimePerRequest.txt)
    echo $REQUEST_THREADS $WORK_THREADS $result1 $result2 >> RelativeThreadData.txt
    let REQUEST_THREADS=REQUEST_THREADS+1
done

