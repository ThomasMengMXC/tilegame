DIRECTORY = $(HOME)
CFLAGS = -Wall -pedantic -g -std=c11 -O3 -I$(DIRECTORY)/include
LFLAGS = -L$(DIRECTORY)/lib -Wl,-R$(DIRECTORY)/lib
LIBS = -ltheatre -llua
CC = gcc
SOURCES = $(wildcard *.c)
OBJECTS = $(SOURCES:.c=.o)

all: game

play: all
	valgrind --leak-check=full ../game 2> meme

memtest: all
	valgrind --tool=massif ../game
	ms_print massif* | less
	rm massif*

game: game.o $(OBJECTS)
	$(CC) *.o $(LFLAGS) -o game $(LIBS)
	cp game ..

%.o : %.cc %.h
		$(CC) $(CFLAGS) -fPIC -c $<

clean:
	rm -rf ~/.config/tilegame
	rm -f ../game ./game *.o *.so meme
