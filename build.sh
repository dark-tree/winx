#!/bin/bash

set -x

gcc -g -O0 main.c -lGL -lX11 -o main && ./main
