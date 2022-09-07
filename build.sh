#!/bin/bash
set -x
gcc -g -Wall -Wextra -Wno-unused-parameter -O0 winx.c example/main.c example/glad/glad.c -I. -ldl -lGL -lX11 -lXcursor -o main && ./main
