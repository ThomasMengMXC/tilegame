#ifndef TILE_STRUCT_H
#define TILE_STRUCT_H

extern "C" {
#include <theatre/colour.h>
}

#include <cstdint>
#include "unit.h"

class Tile {
	public:
		char icon[3];
		Colour colour;
		int8_t mvCost;
		uint16_t yPos, xPos;

		Unit *unit;
};

#endif
