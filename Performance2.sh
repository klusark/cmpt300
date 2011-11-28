#!/bin/bash
REQUEST_THREADS=100
WORK_THREADS=1
MAX_WORK_THREADS=200
while [ $WORK_THREADS -lt $MAX_WORK_THREADS ]; do
    ./a.out $REQUEST_THREADS $WORK_THREADS < data.txt
    wait $!
    result1=$(awk '{sum=sum+$1} END {print sum/NR}' TimePerRequest.txt)
    result2=$(awk '{sum=sum+$2} END {print sum/NR}' TimePerRequest.txt)
    echo $REQUEST_THREADS $WORK_THREADS $result1 $result2 >> RelativeThreadData.txt
    let WORK_THREADS=WORK_THREADS+1
done

