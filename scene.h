#ifndef SCENE_H
#define SCENE_H
#include <ncurses.h>
#include "game.h"

struct Game;

typedef struct Scene{
	void *data;

	void (*update)(void *data);
	void (*keyboard)(void *data, int ch);

	void (*sc_entry)(void *data);
	void (*sc_exit)(void *data);
} Scene;

void scene_setup(struct Game *game);

void scene_change(struct Game *game, int oldSc, int newSc);

void scene_exit(struct Game *game);

#endif
