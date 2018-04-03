#ifndef OVERMAP_STRUCT_H
#define OVERMAP_STRUCT_H

#include <theatre/props_struct.h> // for props
#include "map_struct.h"	// for the map in the data
#include "cursor_struct.h"	// for the cursor in the data
#include "team_struct.h"

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

#endif
