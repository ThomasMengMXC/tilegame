#ifndef OVERMAP_SC_H
#define OVERMAP_SC_H
#include "game.h"	// for the game variable in the data and arguments
#include "map.h"	// for the map in the data
#include "cursor.h"	// for the cursor in the data
#include "screen.h" // for the screen in the data

typedef enum {
	PRE_PLAYER_PHASE,
	PLAYER_MOVE,
	ENEMY_PHASE,
} OverMapState;

typedef struct {
	Screen *screen;
	Game *game; // Pointer to the original game struct
	MapData *map; // The map
	Team *players, *enemies; // The enemy and player teams
	CursorData *cursor; // The cursor
	OverMapState state; // The state of the map
} OverMapData;

void init_overmap_sc(Scene *scene, Game *game);

void overmap_update(void *args);
void overmap_keyboard(void *args, int ch);

void overmap_entry(void *args);
void overmap_exit(void *args);

#endif
