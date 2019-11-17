#ifndef UNIT_H
#define UNIT_H

#include <map> // for maps
#include <string> // for the subscription names
#include "tile.h" // so that the unit can be placed on a tile
#include "stats.h"

class Tile; // so the compiler will stop bugging me about circular including

class Unit {
	public:
		char name[20];

		char icon[3];
		short hp;
		unsigned factionID, unitID; // faction and ID for the current map.
		Stats base, modified;
		Tile *tile;
		// subscriptions, e.g. is main character or other classifications
		std::map<std::string, bool> sub;
		std::map<int, int> *mvCosts;
		std::map<int, int> modmvCosts;

		Unit(const char *name);
		~Unit(void);
};

#endif
