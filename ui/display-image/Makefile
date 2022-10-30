# Makefile

CC = gcc
CFLAGS = -Wall -Wextra -O3 `pkg-config --cflags sdl2 SDL2_image` -D__NO_INLINE__
LDLIBS = `pkg-config --libs sdl2 SDL2_image`

all: display

SRC = display.c
OBJ = ${SRC:.c=.o}
EXE = ${SRC:.c=}

display: display.o

.PHONY: clean

clean:
	${RM} ${OBJ}
	${RM} ${EXE}

# END
