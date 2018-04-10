#include <string.h>
#include <stdlib.h>

#include "unit.h"

Unit *init_unit(char *name, unsigned int unitID) {
	Unit *unit = malloc(sizeof(Unit));
	strncpy(unit->name, name, 20);
	unit->unitID = unitID;
	strncpy(unit->icon, ":)", 3);
	unit->hp = 10;
	unit->move = 5;
	unit->str = 5; unit->spd = 5; unit->def = 5;
	return unit;
}

void free_unit(Unit *unit) {
	free(unit);
	return;
}
