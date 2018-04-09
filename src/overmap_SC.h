#ifndef OVERMAP_SC_H
#define OVERMAP_SC_H

#include <theatre/props.h> // for scene function pointers
#include "map.h"	// for the map in the data

#define DATASTRUCT OverMap

typedef enum {
	PRE_PLAYER_PHASE,
	PLAYER_MOVE,
	ENEMY_PHASE,
} OverMapState;

typedef struct {
	Map *map;
	Team *players, *enemies; // The enemy and player teams
	OverMapState state; // The state of the map
	Layer *mapLayer, *rangeLayer, *cursorLayer;
} OverMap;

DATASTRUCT *init_overmap(void);

void update(Props *props);
int keyboard(Props *props, int ch);

void arrival(Props *props);
void departure(Props *props);

#endif
