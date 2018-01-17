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
	WINDOW *win; // The windows which stuff will be draw on

	Game *game; // Pointer to the original game struct
	MapData *map; // The map
	Team *players, *enemies; // The enemy and player teams
	CursorData *cursor; // The cursor
	OverMapState state; // The state of the map
} OverMapData;

void overmap_sc_init(Scene *scene, Game *game);

void overmap_update(void *args);
void overmap_keyboard(void *args, int ch);

void overmap_entry(void *args);
void overmap_exit(void *args);

#endif
