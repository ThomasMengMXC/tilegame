DIRECTORY = $(HOME)
CFLAGS = -Wall -pedantic -g -std=c++17 -O3 -I$(DIRECTORY)/include
LFLAGS = -L$(DIRECTORY)/lib -Wl,-R$(DIRECTORY)/lib
LIBS = -ldl -ltheatre
CC = g++
SOURCES = $(wildcard *.cc)
OBJECTS = $(SOURCES:.cc=.o)

all: game

play: all
	valgrind --leak-check=full ../game 2> meme
memtest: all
	valgrind --tool=massif ../game
	ms_print massif* | less
	rm massif*

game: game.o overmap.so
	$(CC) game.o $(LFLAGS) -o game $(LIBS)
	cp game ..

overmap.so: overmap_SC.o map.o unit.o button.o backstage.o hover.o
	$(CC) -shared `ls *.o | grep -v game.o | tr '\n' ' '` $(LFLAGS) -o overmap.so
	mkdir -p ~/.config/tilegame/scenes
	cp overmap.so ~/.config/tilegame/scenes
	cp ~/.config/tilegame/scenes/overmap.so ~/.config/tilegame/scenes/sc1.so


%.o : %.cc %.h
		$(CC) $(CFLAGS) -fPIC -c $<

clean:
	rm -rf ~/.config/tilegame
	rm -f ../game ./game *.o *.so meme
