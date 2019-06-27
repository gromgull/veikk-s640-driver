LIBS=$(shell pkg-config --libs gtk+-3.0)
CFLAGS=$(shell pkg-config --cflags gtk+-3.0)

all: config.o panels.o
	gcc config.o panels.o $(CFLAGS) -o config $(LIBS)

config.o: config.c config.h
	gcc config.c $(CFLAGS) -c -I. $(LIBS) 

panels.o: panels.c config.h
	gcc panels.c $(CFLAGS) -c -I. $(LIBS)

clean:
	rm -f config *.o
