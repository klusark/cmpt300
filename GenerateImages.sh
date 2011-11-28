#!/bin/bash
FILES=*.gpt
for f in $FILES
do
    gnuplot $f
    wait $!
    z=.eps
    y=${f%.*}$z
    epstopdf $y
done
