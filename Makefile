CFLAGS = -Wall -pedantic -O3 -g
LFLAGS = -Wall -ltinfo -lncurses
CC = gcc
TARGET = game

all: $(TARGET)

$(TARGET): $(TARGET).o scene.o title_SC.o action_SC.o
	$(CC) $(LFLAGS) -o $(TARGET) $(TARGET).o scene.o title_SC.o action_SC.o

$(TARGET).o: $(TARGET).c $(TARGET).h
	$(CC) $(CFLAGS) -c $(TARGET).c

scene.o: scene.c scene.h title_SC.h
	$(CC) $(CFLAGS) -c scene.c

title_SC.o: title_SC.c title_SC.h
	$(CC) $(CFLAGS) -c title_SC.c

action_SC.o: action_SC.c action_SC.h
	$(CC) $(CFLAGS) -c action_SC.c

clean:
	$(RM) $(TARGET) *o *~
