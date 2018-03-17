CFLAGS = -Wall -pedantic -g -std=gnu11 -O3
LFLAGS = -Wall -ltheatre -ltinfo -lncursesw # -lSDL2_mixer -lSDL2 \
CC = gcc

libtheatre.so: stage.o scene.o screen.o layer.o sprite.o
	$(CC) -shared -o libtheatre.so *.o -lm
	sudo cp libtheatre.so /usr/local/lib
	sudo cp *.h /usr/local/include/theatre

stage.o: stage.c stage.h
	$(CC) $(CFLAGS) -fPIC -c stage.c

scene.o: scene.c scene.h
	$(CC) $(CFLAGS) -fPIC -c scene.c

screen.o: screen.c screen.h sprite.h
	$(CC) $(CFLAGS) -fPIC -c screen.c

layer.o: layer.c layer.h sprite.h
	$(CC) $(CFLAGS) -fPIC -c layer.c

sprite.o: sprite.c sprite.h
	$(CC) $(CFLAGS) -fPIC -c sprite.c

clean:
	rm *.o
	rm libtheatre.so
	sudo rm /usr/local/lib/libtheatre.so
	sudo rm /usr/local/include/theatre/*
