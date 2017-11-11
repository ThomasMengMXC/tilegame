#ifndef SUBMAP_SC_H
#define SUBMAP_SC_H
#include "game.h"
#include "map.h"
#include "unit.h"

typedef struct {
	unsigned int playerCnt, enemyCnt;
	Game *game;
	MapData *map;
	UnitData *players, *enemies;
} SubMapData;

void submap_sc_init(Scene *scene, Game *game);

void submap_update(void *args);
void submap_draw(void *args);
void submap_keyboard(void *args, int ch);

void submap_entry(void *args);
void submap_exit(void *args);

#endif
