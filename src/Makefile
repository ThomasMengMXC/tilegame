DIRECTORY = $(HOME)
CFLAGS = -Wall -pedantic -g -std=gnu++2a -O3 -I$(DIRECTORY)/include
LFLAGS = -L$(DIRECTORY)/lib -Wl,-R$(DIRECTORY)/lib
LIBS = -ltheatre `pkg-config --libs luajit`
CC = g++

SOURCES = $(wildcard *.cc)
OBJECTS = $(SOURCES:.cc=.o)

all: game

play: all
	valgrind --leak-check=full ./game 2> meme

memtest: all
	valgrind --tool=massif ./game
	ms_print massif* | less
	rm massif*

game: $(OBJECTS)
	$(CC) *.o $(LFLAGS) -o game $(LIBS)

%.o : %.cc *.h
	$(CC) $(CFLAGS) -fPIC -c $<

clean:
	rm -f ./game *.o *.so meme
