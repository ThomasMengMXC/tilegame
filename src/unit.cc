#include <stdlib.h>
#include <string.h>
#include "unit.h"

Unit::Unit(const char *name) {
	strncpy(this->name, name, sizeof(this->name) - 1);
	this->name[sizeof(this->name) -1] = '\0';
	strncpy(this->icon, "??", sizeof(this->icon));
	this->hp = this->maxHp = 1;
	this->move = 1;
	this->str = 0;
	this->spd = 0;
	this->def = 0;
	this->tile = NULL;
	this->factionID = 0; this->unitID = 0;
}

Unit::~Unit(void) {
}
