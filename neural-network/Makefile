make:
	gcc -Wall -Wextra -Werror -O3 -o networkXor networkXor.c -lm

SRC = networkXor.c
OBJ = ${SRC:.c=.o}
EXE = ${SRC:.c=}

.PHONY:

clean:
	${RM} ${OBJ}
	${RM} ${EXE}
