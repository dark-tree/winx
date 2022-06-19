#!/bin/bash

set -x

gcc main.c -lGL -lX11 -o main && ./main
