# Makefile

CC = gcc
CFLAGS = -Wall -Wextra -O3 `pkg-config --cflags sdl2 SDL2_image` -lm \
	-D__NO_INLINE_ -g
LDLIBS = `pkg-config --libs sdl2 SDL2_image` -lm

all: floodfill

SRC = floodfill.c
OBJ = ${SRC:.c=.o}
DEP = ${SRC:.c=.d}
EXE = ${SRC:.c=}

floodfill: floodfill.o

.PHONY: clean

clean:
	${RM} ${OBJ}
	${RM} ${EXE}
	${RM} ${DEP}
	${RM} rotate-image.jpeg

# END
