#ifndef UNIT_H
#define UNIT_H

#include <map>
#include <string>
#include "tile.h"

class Tile;

class Unit {
	public:
		char name[20];

		char icon[3];
		unsigned maxHp;
		short hp;
		short move;
		unsigned str, spd, def;
		unsigned factionID, unitID; // faction and ID for the current map.
		Tile *tile;
		// subscriptions, e.g. is main character or other classifications
		std::map<std::string, bool> sub;

		Unit(const char *name);
		~Unit(void);
};

#endif
