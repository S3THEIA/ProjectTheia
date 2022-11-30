# Makefile

CC = gcc
CFLAGS = -Wall -Wextra -O3 `pkg-config --cflags sdl2 SDL2_image` -D__NO_INLINE__
LDLIBS = `pkg-config --libs sdl2 SDL2_image`

all: grid_detection

SRC = grid_detection.c
OBJ = ${SRC:.c=.o}
EXE = ${SRC:.c=}

grid_detection: grid_detection.o

.PHONY: clean

clean:
	${RM} ${OBJ}
	${RM} ${EXE}

# END
