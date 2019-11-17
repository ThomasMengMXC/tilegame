#ifndef TILE_H
#define TILE_H

extern "C" {
#include <theatre/colour.h> // for the tile's colour
#include <theatre/coordinate.h> // for the tile's colour
}
#include "unit.h" // for storing the unit

class Unit; // so the compiler will stop bugging me about circular requiring

class Tile {
	public:
		char icon[3];
		Colour colour;
		int type; // the type will be defined by the Lua scripts
		Coordinate pos;
		Unit *unit;

		// event stuff, each tile can have 1 event
		int eventID; // basically a lua ref to a function
		Colour eventColour;
		char eventName[16];
};

#endif
