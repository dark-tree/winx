#!/bin/bash
set -x
gcc -g -O0 example/main.c -I. -ldl -lGL -lX11 -o main && ./main
