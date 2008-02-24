CC=g++
CFLAGS=-Wall
FILES=main.cpp
LIBS=-L. -ltellusbd101

APP = tdtool

all: $(APP)

$(APP): main.cpp
	$(CC) -o $(APP) $(CFLAGS) $(LIBS) $(FILES)

clean:
	rm -f *.o ; rm $(APP)
