#ifndef OVERMAP_SC_H
#define OVERMAP_SC_H
#include "game.h"
#include "map.h"
#include "unit.h"
#include "cursor.h"

typedef enum {
	PRE_PLAYER_PHASE,
	PLAYER_MOVE,
	ENEMY_PHASE,
} OverMapState;

typedef struct {
	Game *game;
	MapData *map;
	Team *players, *enemies;
	CursorData *cursor;
	OverMapState state;
} OverMapData;

void overmap_sc_init(Scene *scene, Game *game);

void overmap_update(void *args);
void overmap_draw(void *args);
void overmap_keyboard(void *args, int ch);

void overmap_entry(void *args);
void overmap_exit(void *args);

#endif
