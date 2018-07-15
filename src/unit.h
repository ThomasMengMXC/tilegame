#ifndef UNIT_H
#define UNIT_H

#include <map> // for maps
#include <string> // for the subscription names
#include "tile.h" // so that the unit can be placed on a tile

class Tile; // so the compiler will stop bugging me about circular including

class Unit {
	public:
		char name[20];

		char icon[3];
		unsigned maxHp;
		short hp;
		short move; // the amount amount of map units this unit can move
		unsigned str, spd, def;
		unsigned factionID, unitID; // faction and ID for the current map.
		Tile *tile;
		// subscriptions, e.g. is main character or other classifications
		std::map<std::string, bool> sub;

		Unit(const char *name);
		~Unit(void);
};

#endif
