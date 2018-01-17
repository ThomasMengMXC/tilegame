#ifndef TILE_H
#define TILE_H

#include <ncurses.h>
#include "unit.h"

typedef struct Tile{
	char *icon;
	char colour;
	int mvCost;
	unsigned int yPos, xPos;

	Unit *unit;
} Tile;

void tile_draw(WINDOW *win, Tile *tile, bool colorOn, bool defaultValue);

#endif
