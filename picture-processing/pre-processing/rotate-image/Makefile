# Makefile

CC = gcc
CFLAGS = -Wall -Wextra -O3 `pkg-config --cflags sdl2 SDL2_image` -lm \
	-D__NO_INLINE__
LDLIBS = `pkg-config --libs sdl2 SDL2_image` -lm

all: rotate

SRC = rotate.c
OBJ = ${SRC:.c=.o}
DEP = ${SRC:.c=.d}
EXE = ${SRC:.c=}

rotate: rotate.o

.PHONY: clean

clean:
	${RM} ${OBJ}
	${RM} ${EXE}
	${RM} ${DEP}
	${RM} rotate-image.jpeg

# END
