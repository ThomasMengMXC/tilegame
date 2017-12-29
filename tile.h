#ifndef TILE_H
#define TILE_H

#include <ncurses.h>
#include "unit.h"

typedef struct Tile{
	char icon;
	char defaultColour;
	char colour;
	int mvCost;
	Unit *unit;
	unsigned int yPos, xPos;
} Tile;

void tile_draw(Tile *tile, bool colorOn, bool defaultValue);

#endif
