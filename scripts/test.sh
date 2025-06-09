#!/bin/bash

gcc src/main.c src/helpers.c src/input.c -o test-1


if [ $? -ne 0 ]; then
	echo "Test Build Failed, Exiting."
	exit 1
fi

./test-1 "$@"

rm test-1
