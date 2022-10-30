# Makefile

CC = gcc
CFLAGS = -Wall -Wextra -O3 `pkg-config --cflags sdl2 SDL2_image` -D__NO_INLINE__
LDLIBS = `pkg-config --libs sdl2 SDL2_image`

all: split-grid

SRC = split-grid.c
OBJ = ${SRC:.c=.o}
EXE = ${SRC:.c=}

split-grid: split-grid.o

.PHONY: clean

clean:
	${RM} ${OBJ}
	${RM} ${EXE}
	${RM} Sudoku\(*

# END
