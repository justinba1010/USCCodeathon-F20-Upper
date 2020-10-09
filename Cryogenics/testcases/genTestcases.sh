#!/bin/bash

RUN_PROG=../sol/python/main.py

echo "Generating..."

for i in $(seq 0 30); do

    echo "Input $i"
    if [ $i -lt 10 ]; then
        echo "Small"
        ./gen.py 10 10 > input/input$i.txt
    elif [ $1 -lt 20 ]; then
        echo "Medium"
        ./gen.py 1000 2147483647 > input/input$i.txt
    else
        echo "Large"
        ./gen.py 1000000 2147483647 > input/input$i.txt
    fi

    echo "Output $i"
    cat input/input$i.txt | python3 $RUN_PROG > output/output$i.txt
done