#!/usr/bin/env bash
set -euo pipefail

/ucrt64/bin/gcc -std=c11 -Wall -Wextra -pedantic \
  main.c game.c snake.c food.c utils.c \
  -o snake.exe \
  -lraylib -lopengl32 -lgdi32 -lwinmm

echo "build ok"
ls -l snake.exe
