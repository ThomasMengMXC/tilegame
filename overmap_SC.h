#ifndef OVERMAP_SC_H
#define OVERMAP_SC_H
#include "game.h"	// for the game variable in the data and arguments
#include "map.h"	// for the map in the data
#include "cursor.h"	// for the cursor in the data

#define DATASTRUCT OverMap

typedef enum {
	PRE_PLAYER_PHASE,
	PLAYER_MOVE,
	ENEMY_PHASE,
} OverMapState;

typedef struct {
	Map *map; // The map
	Team *players, *enemies; // The enemy and player teams
	Cursor *cursor; // The cursor
	OverMapState state; // The state of the map
} OverMap;

void overmap_update(Props *props);
void overmap_keyboard(Props *props, int ch);

DATASTRUCT *init_overmap(void);

void overmap_entry(Props *props);
void overmap_exit(Props *props);

#endif
