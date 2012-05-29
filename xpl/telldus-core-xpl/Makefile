#
# Makefile for xPLLib
#

#
# For LINUX, use the following
CCOPTS = -g -DLINUX -pedantic -Wall
LIBS = -g -lm -lxPL -ltelldus-core

LDOPTS	= -O
CC	=	cc $(CCOPTS)
LD	= 	cc $(LDOPTS)

CMD_LIST = xPL_TelldusCore

.c.o:
	$(CC) -c $<

.o:
	$(LD) -o $@ $< $(LIBS)

.c:
	$(CC) -c $<
	$(LD) -o $@ $< $(LIBS)


all:	${CMD_LIST}

clean:
	rm -f *.o *.a core ${CMD_LIST}

rebuild: clean all





