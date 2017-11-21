CFLAGS = -Wall -pedantic -O3 -g
LFLAGS = -Wall -ltinfo -lncursesw
CC = gcc
TARGET = game

all: $(TARGET)

$(TARGET): $(TARGET).o scene.o title_SC.o submap_SC.o overmap_SC.o map.o
	$(CC) $(LFLAGS) -o $(TARGET) $(TARGET).o scene.o title_SC.o submap_SC.o overmap_SC.o map.o

$(TARGET).o: $(TARGET).c $(TARGET).h scene.h unit.h
	$(CC) $(CFLAGS) -c $(TARGET).c

scene.o: scene.c scene.h title_SC.h overmap_SC.h submap_SC.h
	$(CC) $(CFLAGS) -c scene.c

title_SC.o: title_SC.c title_SC.h game.h scene.h
	$(CC) $(CFLAGS) -c title_SC.c

submap_SC.o: submap_SC.c submap_SC.h game.h map.h scene.h unit.h
	$(CC) $(CFLAGS) -c submap_SC.c

overmap_SC.o: overmap_SC.c overmap_SC.h game.h map.h scene.h unit.h
	$(CC) $(CFLAGS) -c overmap_SC.c

map.o: map.c map.h game.h
	$(CC) $(CFLAGS) -c map.c

clean:
	$(RM) $(TARGET) *o *~
