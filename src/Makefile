DIRECTORY = $(HOME)/usr
CFLAGS = -Wall -pedantic -g -std=c++11 -O3 -I$(DIRECTORY)/include
LFLAGS = -L$(DIRECTORY)/lib -Wl,-R$(DIRECTORY)/lib
LIBS = -ltheatre -lluajit-5.1
CC = g++

SOURCES = $(wildcard *.cc)
OBJECTS = $(SOURCES:.cc=.o)

all: game

play: all
	valgrind --leak-check=full --track-origins=yes ./game 2> meme

analyse:
	clang-tidy -fix *.cc -- -I$(DIRECTORY)/include -std=c++11

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
