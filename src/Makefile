CFLAGS = -Wall -pedantic -g -std=gnu11 -O3
LFLAGS = -Wall -ltheatre
CC = gcc
TARGET = game

all: $(TARGET)

play: all
	valgrind --leak-check=full ../game 2> meme


$(TARGET): $(TARGET).o overmap_SC.o map.o unit.o cursor.o button.o team.o backstage.o
	$(CC) *.o $(LFLAGS) -o $(TARGET)
	cp $(TARGET) ..

$(TARGET).o: $(TARGET).c
	$(CC) $(CFLAGS) -c $(TARGET).c

overmap_SC.o: overmap_SC.c
	$(CC) $(CFLAGS) -c overmap_SC.c

map.o: map.c map.h
	$(CC) $(CFLAGS) -c map.c

unit.o: unit.c
	$(CC) $(CFLAGS) -c unit.c

cursor.o: cursor.c
	$(CC) $(CFLAGS) -c cursor.c

button.o: button.c
	$(CC) $(CFLAGS) -c button.c

team.o: team.c
	$(CC) $(CFLAGS) -c team.c

backstage.o: backstage.c
	$(CC) $(CFLAGS) -c backstage.c

clean:
	rm -f ../$(TARGET) ./$(TARGET) *.o
