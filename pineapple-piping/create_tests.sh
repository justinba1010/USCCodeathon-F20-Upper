#!/bin/bash

# generate test cases for hackerrank

cd $(dirname $0)

mkdir -p input
mkdir -p output

make all



# MUST SEED RANDOM FOR REPRODUCABILITY


S_SIZE=10
M_SIZE=100
L_SIZE=1000
H_SIZE=10000
S_DEG=5
M_DEG=50
L_DEG=100

#1 eg 1
#1 eg 2
#5 small,sd
#5 med, sd
#5 med, md
#5 lar, sd
#5 lar, md
#1 lar, ld
#1 hug, sd
#1 hug, ld



PAD='2'



for n in $(seq 3 24) ; do
	num=$(printf "%0*d" "$PAD" "$n")
	INPUT_FILE="./input/input$num.txt"
	OUTPUT_FILE="./output/output$num.txt"

	./gen > "$INPUT_FILE"
	cat "$INPUT_FILE" | ./sol > "$OUTPUT_FILE"
done


gen(size, degree source, target)
