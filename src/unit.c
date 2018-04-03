#include <string.h>
#include <stdlib.h>

#include "unit.h"

Unit *init_unit(Unit unit) {
	Unit *newUnit = malloc(sizeof(Unit));
	memcpy(newUnit, &unit, sizeof(Unit));
	return newUnit;
}

void free_unit(Unit *unit) {
	free(unit);
	return;
}
