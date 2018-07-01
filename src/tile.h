#ifndef TILE_H
#define TILE_H
#include <theatre/colour.h>
#include <theatre/vector2D.h>
#include "unit.h"

struct Unit;

typedef struct Tile {
	char icon[3];
	Colour colour;
	int mvCost;
	Vector2D pos;
	struct Unit *unit;
} Tile;

#endif
