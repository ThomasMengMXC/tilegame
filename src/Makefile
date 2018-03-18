CFLAGS = -Wall -pedantic -g -std=gnu11 -O3
LFLAGS = -Wall -ltheatre -ltinfo -lncursesw
CC = gcc
TARGET = game

all: $(TARGET)

play: all
	valgrind --leak-check=full ../game 2> meme


$(TARGET): $(TARGET).o overmap_SC.o map.o unit.o cursor.o button.o
	$(CC) $(LFLAGS) -o $(TARGET) $(TARGET).o overmap_SC.o map.o unit.o cursor.o button.o
	cp $(TARGET) ..

$(TARGET).o: $(TARGET).c $(TARGET).h unit.h
	$(CC) $(CFLAGS) -c $(TARGET).c

title_SC.o: title_SC.c title_SC.h game.h scene.h
	$(CC) $(CFLAGS) -c title_SC.c

submap_SC.o: submap_SC.c submap_SC.h game.h map.h unit.h
	$(CC) $(CFLAGS) -c submap_SC.c

overmap_SC.o: overmap_SC.c overmap_SC.h game.h map.h cursor.h
	$(CC) $(CFLAGS) -c overmap_SC.c

map.o: map.c map.h cursor.h tile.h unit.h button.h
	$(CC) $(CFLAGS) -c map.c

unit.o: unit.c unit.h
	$(CC) $(CFLAGS) -c unit.c

cursor.o: cursor.c cursor.h tile.h
	$(CC) $(CFLAGS) -c cursor.c

button.o: button.c button.h overmap_SC.h cursor.h
	$(CC) $(CFLAGS) -c button.c

clean:
	rm ../$(TARGET) ./$(TARGET) *.o
