#Makefile

CC = gcc
CFLAGS = -Wall -Wextra -O3 `pkg-config --cflags sdl2 SDL2_image` -D__NO_INLINE__
LDLIBS = `pkg-config --libs sdl2 SDL2_image` -lm 

all: mnist

SRC = mnist.c
OBJ = ${SRC:.c=.o}
EXE = ${SRC:.c=}

grayscale: mnist.o

.PHONY: clean

clean:
	${RM} ${OBJ}
	${RM} ${EXE}
	${RM} gray-image.jpeg

# END