#!/bin/bash
set -x
gcc -g -O0 winx.c example/main.c example/glad/glad.c -I. -ldl -lGL -lX11 -o main && ./main
