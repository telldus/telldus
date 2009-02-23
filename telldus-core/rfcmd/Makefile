
SRCS=	rfcmd.c ftdi.c
CFLAGS=-O2	-Wall -I/usr/local/include
LFLAGS=	-L/usr/local/lib -R/usr/local/lib
#linux:LFLAGS=-Wl,-rpath,/usr/local/lib
LIBS=	-lftdi -lusb
PROG= 	rfcmd
FT=	find_telldus
OBJS= 	$(SRCS:.c=.o)
CC= 	gcc

all: $(PROG) $(FT)

$(PROG): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(PROG) $(LFLAGS) $(LIBS)

rfcmd.o: rfcmd.c

ftdi.o:	ftdi.c

$(FT): $(FT).c
	$(CC) $(CFLAGS) -o $@ $(FT).c $(LFLAGS) $(LIBS)
