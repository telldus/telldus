CC=gcc
CFLAGS=-Wall -ltelldus-core

OBJS = main.o

all: ${OBJS}
	${CC} -o sensor ${CFLAGS} ${OBJS}

main.c:
	${CC} ${CFLAGS} -c main.c

clean:
	rm -f sensor main.o
