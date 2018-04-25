#ifndef OVERMAP_SC_H
#define OVERMAP_SC_H
extern "C" {
#include <theatre/props.h> // for scene function pointers
}

#include "map.h"	// for the map in the data

typedef enum {
	PRE_PLAYER_PHASE,
	PLAYER_MOVE,
	ENEMY_PHASE,
} OverMapState;

class OverMap{
	public:
		Map *map;
		std::vector<Unit *> players, enemies; // The enemy and player teams
		OverMapState state; // The state of the map
		Layer *mapLayer, *rangeLayer, *cursorLayer;

		OverMap(void);
		~OverMap(void);
};

extern "C" {
void update(Props *props);
int keyboard(Props *props, int ch);

void arrival(Props *props);
void departure(Props *props);
}

#endif
