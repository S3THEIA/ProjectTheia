# Makefile

CC = gcc
CFLAGS = -Wall -Wextra -O3 `pkg-config --cflags sdl2 SDL2_image`
LDLIBS = `pkg-config --libs sdl2 SDL2_image`

all: grayscale

SRC = grayscale.c
OBJ = ${SRC:.c=.o}
EXE = ${SRC:.c=}

grayscale: grayscale.o

.PHONY: clean

clean:
	${RM} ${OBJ}
	${RM} ${EXE}
	${RM} gray-picture.jpeg

# END
