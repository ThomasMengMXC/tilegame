CFLAGS = -Wall -pedantic -g -std=gnu11 -O3 -I$(HOME)/include -march=native -mno-tbm
LFLAGS = -L$(HOME)/lib -ltheatre -Wl,-R$(HOME)/lib
CC = gcc
TARGET = game

all: $(TARGET)

play: all
	valgrind --leak-check=full ../game 2> meme
memtest: all
	valgrind --tool=massif ../game
	ms_print massif* | less
	rm massif*

$(TARGET): $(TARGET).o overmap.so
	$(CC) game.o $(LFLAGS) -ldl -o $(TARGET)
	cp $(TARGET) ..

overmap.so: overmap_SC.o map.o unit.o cursor.o button.o team.o backstage.o
	$(CC) -shared `ls *.o | grep -v game.o | tr '\n' ' '` $(LFLAGS) -o overmap.so
	mkdir -p ~/.config/tilegame/scenes
	cp overmap.so ~/.config/tilegame/scenes
	cp ~/.config/tilegame/scenes/overmap.so ~/.config/tilegame/scenes/sc1.so


$(TARGET).o: $(TARGET).c
	$(CC) $(CFLAGS) -c $(TARGET).c


overmap_SC.o: overmap_SC.c
	$(CC) $(CFLAGS) -fPIC -c overmap_SC.c

map.o: map.c map.h
	$(CC) $(CFLAGS) -fPIC -c map.c

unit.o: unit.c
	$(CC) $(CFLAGS) -fPIC -c unit.c

cursor.o: cursor.c
	$(CC) $(CFLAGS) -fPIC -c cursor.c

button.o: button.c
	$(CC) $(CFLAGS) -fPIC -c button.c

team.o: team.c
	$(CC) $(CFLAGS) -fPIC -c team.c

backstage.o: backstage.c
	$(CC) $(CFLAGS) -fPIC -c backstage.c

clean:
	rm -rf ~/.config/tilegame
	rm -f ../$(TARGET) ./$(TARGET) *.o *.so meme
