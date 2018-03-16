#ifndef SCENE_H
#define SCENE_H
#include <ncurses.h>

struct Game;

typedef void (*UpdateFn)(void *data);
typedef	void (*KeyboardFn)(void *data, int ch);
typedef	void (*EntryFn)(void *data);
typedef	void (*ExitFn)(void *data);

typedef struct Scene{
	void *data;

	UpdateFn update;
	KeyboardFn keyboard;
	EntryFn entry;
	ExitFn exit;
} Scene;

Scene *init_scene(UpdateFn upd, KeyboardFn kb, EntryFn entry, ExitFn exit);

/*
void scene_init(struct Game *game);

void scene_change(struct Game *game, int oldSc, int newSc);

void scene_exit(struct Game *game);
*/

#endif
