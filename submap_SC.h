#ifndef SUBMAP_SC_H
#define SUBMAP_SC_H
#include "game.h"
#include "map.h"
#include "unit.h"

typedef struct {
	WINDOW *win;
	unsigned int playerCnt, enemyCnt;
	Game *game;
	MapData *map;
	Unit **players, **enemies;
} SubMapData;

void init_submap_sc(Scene *scene, Game *game);

void submap_update(void *args);
void submap_keyboard(void *args, int ch);

void submap_entry(void *args);
void submap_exit(void *args);

#endif
