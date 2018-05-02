#ifndef TILE_STRUCT_H
#define TILE_STRUCT_H

extern "C" {
#include <theatre/colour.h>
#include <theatre/vector2D.h>
}

#include <cstdint>
#include "unit.h"

class Unit;

class Tile {
	public:
		char icon[3];
		Colour colour;
		int8_t mvCost;
		Vector2D pos;

		Unit *unit;
};

#endif
