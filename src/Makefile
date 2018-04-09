CFLAGS = -Wall -pedantic -g -std=gnu11 -O3 -I$(HOME)/include -march=native -mno-tbm
LFLAGS = -L$(HOME)/lib -ltheatre -Wl,-R$(HOME)/lib
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

game: game.o overmap.so
	$(CC) game.o $(LFLAGS) -ldl -o game
	cp game ..

overmap.so: overmap_SC.o map.o unit.o button.o team.o backstage.o hover.o
	$(CC) -shared `ls *.o | grep -v game.o | tr '\n' ' '` $(LFLAGS) -o overmap.so
	mkdir -p ~/.config/tilegame/scenes
	cp overmap.so ~/.config/tilegame/scenes
	cp ~/.config/tilegame/scenes/overmap.so ~/.config/tilegame/scenes/sc1.so


%.o : %.c %.h
		$(CC) $(CFLAGS) -fPIC -c $<

clean:
	rm -rf ~/.config/tilegame
	rm -f ../game ./game *.o *.so meme
