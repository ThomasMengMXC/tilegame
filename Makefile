CFLAGS = -Wall -pedantic -g -std=gnu11 -O3
LFLAGS = -Wall -ltinfo -lncursesw
CC = gcc
TARGET = game

all: $(TARGET)

play: all
	valgrind --leak-check=full ./game 2> meme


$(TARGET): $(TARGET).o scene.o title_SC.o submap_SC.o overmap_SC.o map.o unit.o cursor.o screen.o layer.o
	$(CC) $(LFLAGS) -o $(TARGET) $(TARGET).o scene.o title_SC.o submap_SC.o overmap_SC.o map.o unit.o cursor.o screen.o layer.o

$(TARGET).o: $(TARGET).c $(TARGET).h scene.h unit.h sprite.h
	$(CC) $(CFLAGS) -c $(TARGET).c

scene.o: scene.c scene.h title_SC.h overmap_SC.h submap_SC.h
	$(CC) $(CFLAGS) -c scene.c

title_SC.o: title_SC.c title_SC.h game.h scene.h
	$(CC) $(CFLAGS) -c title_SC.c

submap_SC.o: submap_SC.c submap_SC.h game.h map.h scene.h unit.h
	$(CC) $(CFLAGS) -c submap_SC.c

overmap_SC.o: overmap_SC.c overmap_SC.h game.h map.h scene.h cursor.h screen.h
	$(CC) $(CFLAGS) -c overmap_SC.c

map.o: map.c map.h cursor.h tile.h layer.h unit.h sprite.h
	$(CC) $(CFLAGS) -c map.c

unit.o: unit.c unit.h
	$(CC) $(CFLAGS) -c unit.c

cursor.o: cursor.c cursor.h tile.h
	$(CC) $(CFLAGS) -c cursor.c

screen.o: screen.c screen.h sprite.h
	$(CC) $(CFLAGS) -c screen.c

layer.o: layer.c layer.h sprite.h
	$(CC) $(CFLAGS) -c layer.c

clean:
	$(RM) $(TARGET) *.o *~
