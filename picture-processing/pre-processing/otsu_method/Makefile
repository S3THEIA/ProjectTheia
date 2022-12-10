# Makefile

CC = gcc
CFLAGS = -Wall -g -Wextra -O3 `pkg-config --cflags sdl2 SDL2_image`
LDLIBS = `pkg-config --libs sdl2 SDL2_image`

all: OtsuMethod

SRC = OtsuMethod.c
OBJ = ${SRC:.c=.o}
EXE = ${SRC:.c=}

grayscale: OtsuMethod.o

.PHONY: clean

clean:
	${RM} ${OBJ}
	${RM} ${EXE}
	${RM} otsu-picture.jpeg

# END
