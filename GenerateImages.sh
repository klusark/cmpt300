#!/bin/bash
FILES=*.gpt
for f in $FILES
do
    gnuplot $f
    wait $!
done
