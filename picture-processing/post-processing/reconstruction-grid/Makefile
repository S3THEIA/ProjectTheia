# Makefile

CC = gcc
CFLAGS = -Wall -Wextra -O3 `pkg-config --cflags sdl2 SDL2_image` -D__NO_INLINE__
LDLIBS = `pkg-config --libs sdl2 SDL2_image`

all: reconstruction_grid

SRC = reconstruction_grid.c
OBJ = ${SRC:.c=.o}
EXE = ${SRC:.c=}

reconstruction_grid: reconstruction_grid.o

.PHONY: clean

clean:
	${RM} ${OBJ}
	${RM} ${EXE}
#	${RM} *.result
	${RM} *.d
	${RM} testcase*


# END
