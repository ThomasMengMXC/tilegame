#ifndef UNIT_H
#define UNIT_H

#include <cstdint>

class Unit {
	public:
		char name[20];
		uint16_t unitID;

		char icon[3];
		int16_t hp;
		uint16_t move;
		uint16_t str, spd, def;

		Unit(const char *name, uint16_t unitId);
};

#endif
