#ifndef TILE_H
#define TILE_H
#include <theatre/colour.h>
#include <theatre/vector2D.h>

typedef struct Tile {
	char icon[3];
	Colour col;
	int mvCost;
	Vector2D pos;
	void *unit; // replace this with the actual unit eventually
} Tile;

#endif
