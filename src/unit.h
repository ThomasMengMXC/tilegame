#ifndef UNIT_H
#define UNIT_H

#include "tile.h"

struct Tile;

typedef struct Unit {
	char name[20];
	unsigned unitID;

	char icon[3];
	short hp;
	short move;
	unsigned str, spd, def;
	struct Tile *tile;
} Unit;

Unit *init_unit(char *name, unsigned unitID);
void free_unit(Unit *unit);

#endif
