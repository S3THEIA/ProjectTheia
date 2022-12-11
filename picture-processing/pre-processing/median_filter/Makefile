# Makefile

CC = gcc
CFLAGS = -Wall -g -Wextra -O3 `pkg-config --cflags sdl2 SDL2_image`
LDLIBS = `pkg-config --libs sdl2 SDL2_image` -lm

all: medianFilter

SRC = medianFilter.c
OBJ = ${SRC:.c=.o}
EXE = ${SRC:.c=}

medianFilter: medianFilter.o

.PHONY: clean

clean:
	${RM} ${OBJ}
	${RM} ${EXE}
	${RM} filtred-picture.jpeg

# END
