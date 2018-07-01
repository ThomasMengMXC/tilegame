#ifndef OVERMAP_H
#define OVERMAP_H

#include <theatre/props.h>
#include "map.h"

typedef enum Phase {
	PLAYER,
	ENEMY,
} Phase;

typedef struct OverMap {
	Map *map;
	Layer *mapLayer;
	Phase phase;
} OverMap;

void update(Props *props);
int keyboard(Props *props, int ch);
void arrival(Props *props, int sceneNum);
void departure(Props *props);

#endif
