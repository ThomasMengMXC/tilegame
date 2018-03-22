#ifndef TILE_H
#define TILE_H

#include <ncurses.h>
//#include <theatre/theatre.h>
#include <stdint.h>
#include "unit.h"

typedef struct Tile{
	char *icon;
	uint8_t r, g, b;
	signed short mvCost;
	unsigned short yPos, xPos;

	Unit *unit;
} Tile;

#endif
