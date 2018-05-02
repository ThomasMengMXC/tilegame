#ifndef UNIT_H
#define UNIT_H

#include <cstdint>
#include "tile.h"

class Tile;

class Unit {
	public:
		char name[20];
		uint16_t unitID;

		char icon[3];
		int16_t hp;
		int16_t move;
		uint16_t str, spd, def;
		Tile *tile;

		Unit(const char *name, uint16_t unitId);
};

#endif
