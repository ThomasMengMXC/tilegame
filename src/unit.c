#include <stdlib.h>
#include <string.h>
#include "unit.h"

Unit *init_unit(char *name, unsigned unitID) {
	Unit *unit = malloc(sizeof(Unit));
	strncpy(unit->name, name, sizeof(unit->name) - 1);
	unit->name[sizeof(unit->name) -1] = '\0';
	unit->unitID = unitID;
	strncpy(unit->icon, ":)", sizeof(unit->icon));
	unit->hp = 10;
	unit->move = 5;
	unit->str = 5;
	unit->spd = 5;
	unit->def = 5;
	unit->tile = NULL;
	return unit;
}

void free_unit(Unit *unit) {
	free(unit);
}
