#ifndef TILE_H
#define TILE_H

extern "C" {
#include <theatre/colour.h>
}
#include "unit.h"

class Unit;

class Tile {
	public:
		char icon[3];
		Colour colour;
		int type; // the type will be defined by the Lua scripts
		std::pair<unsigned, unsigned> pos;
		Unit *unit;

		// event stuff, each tile can have 1 event
		int eventID; // basically a lua ref to a function
		Colour eventColour;
		char eventName[16];
};

#endif
