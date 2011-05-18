CC=gcc
CFLAGS=-Wall -ltelldus-core

OBJS = main.o

all: ${OBJS}
	${CC} -o callbacks ${CFLAGS} ${OBJS}

main.cpp:
	${CC} ${CFLAGS} -c main.cpp

clean:
	rm -f callbacks main.o
