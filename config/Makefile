LIBS=$(shell pkg-config --libs gtk+-3.0)
CFLAGS=$(shell pkg-config --cflags gtk+-3.0)

all: config.o
	gcc $(CFLAGS) -o config config.o $(LIBS)

config.o: config.c
	gcc -c $(CFLAGS) config.c $(LIBS) 

clean:
	rm config *.o
