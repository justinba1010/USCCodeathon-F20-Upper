#!/bin/bash

# generate test cases for hackerrank
set -ue

cd "$(dirname "$0")" || exit 1

mkdir -p input
mkdir -p output

make release

# must seed random for reproducability
export RANDOM=4 #RFC 1149.5
S_SIZE=10
M_SIZE=100
L_SIZE=1000
H_SIZE=10000
S_DEG=5
M_DEG=50
L_DEG=100

CURR_FILE=0
PAD=2
IFILE_PRE="./input/input"
OFILE_PRE="./output/output"
gen() {
	SIZE=$1
	DEG=$2

	S=$(("$RANDOM" % SIZE))
	T=$(("$RANDOM" % SIZE))
	while [ $S -eq $T ] ; do
		T=$(("$RANDOM" % SIZE))
	done

	NUM=$(printf "%0*d" "$PAD" "$CURR_FILE")
	CURR_FILE=$((CURR_FILE + 1))

	./gen "$SIZE" "$DEG" "$S" "$T" > "$IFILE_PRE""$NUM".txt
	./sol > "$OFILE_PRE""$NUM".txt < "$IFILE_PRE""$NUM".txt
}

gen 6 3
gen 8 3

gen "$S_SIZE" "$S_DEG"
gen "$S_SIZE" "$S_DEG"
gen "$S_SIZE" "$S_DEG"

gen "$M_SIZE" "$S_DEG"
gen "$M_SIZE" "$S_DEG"
gen "$M_SIZE" "$S_DEG"

gen "$M_SIZE" "$M_DEG"
gen "$M_SIZE" "$M_DEG"
gen "$M_SIZE" "$M_DEG"

gen "$L_SIZE" "$S_DEG"
gen "$L_SIZE" "$S_DEG"
gen "$L_SIZE" "$S_DEG"

gen "$L_SIZE" "$M_DEG"
gen "$L_SIZE" "$M_DEG"
gen "$L_SIZE" "$M_DEG"

gen "$L_SIZE" "$L_DEG"

gen "$H_SIZE" "$S_DEG"

gen "$H_SIZE" "$L_DEG"
