#ifndef TILE_STRUCT_H
#define TILE_STRUCT_H

#include <stdint.h>
#include "unit_struct.h"

typedef struct Tile{
	char *icon;
	uint8_t r, g, b;
	signed short mvCost;
	unsigned short yPos, xPos;

	Unit *unit;
} Tile;

#endif
