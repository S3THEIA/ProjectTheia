# Makefile

CC = gcc
CFLAGS = -Wall -Wextra -O3 `pkg-config --cflags sdl2 SDL2_image`
LDLIBS = `pkg-config --libs sdl2 SDL2_image`

all: rotate

SRC = rotate.c
OBJ = ${SRC:.c=.o}
EXE = ${SRC:.c=}

grayscale: rotate.o

.PHONY: clean

clean:
	${RM} ${OBJ}
	${RM} ${EXE}
	${RM} rotate-picture.jpeg

# END
