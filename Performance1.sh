#!/bin/bash
REQUEST_THREADS=2
WORK_THREADS=1
MAX_THREADS=200
while [ $REQUEST_THREADS -lt $MAX_THREADS ]; do
    ./a.out $REQUEST_THREADS $WORK_THREADS < data.txt
    wait $!
    result1=$(awk '{sum=sum+$1} END {print sum/NR}' TimePerRequest.txt)
    result2=$(awk '{sum=sum+$2} END {print sum/NR}' TimePerRequest.txt)
    result3=$(awk '{sum=sum+$3} END {print sum/NR}' TimePerRequest.txt)
    result4=$(awk '{sum=sum+$4} END {print sum/NR}' TimePerRequest.txt)
    echo $REQUEST_THREADS $WORK_THREADS $result1 $result2 $result3 $result4 >> RelativeThreadData.txt
    let REQUEST_THREADS=REQUEST_THREADS+1
done

