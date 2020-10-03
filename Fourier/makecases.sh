#!/bin/sh

cd "$(dirname "$0")"

NUM=2
STOPNUM=50

while true ; do
	if [ $NUM -gt $STOPNUM ] ; then
		exit 0
	fi

	echo "creating case $NUM/$STOPNUM"

	python3 ./makeinput.py > "input/input$NUM.txt"

	python3 ./solutions/main.py < "input/input$NUM.txt" > "output/output$NUM.txt"

	NUM=$(expr $NUM + 1)

done
