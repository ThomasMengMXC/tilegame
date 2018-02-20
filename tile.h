#ifndef TILE_H
#define TILE_H

#include <ncurses.h>
#include "unit.h"
#include "sprite.h"

typedef struct Tile{
	char *icon;
	Colour colour;
	int mvCost;
	unsigned int yPos, xPos;

	Unit *unit;
} Tile;

#endif
