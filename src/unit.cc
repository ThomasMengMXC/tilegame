#include <cstring> // for copying the function param to the fields
#include "unit.h"

Unit::Unit(const char *name) {
	strncpy(this->name, name, sizeof(this->name) - 1); // copy name to field
	this->name[sizeof(this->name) -1] = '\0';
	strncpy(this->icon, "??", sizeof(this->icon)); //default icon of ??
	this->base.maxHp = 1;
	this->base.move = 1;
	this->base.str = 0;
	this->base.spd = 0;
	this->base.def = 0;
	this->modified = this->base;
	this->hp = this->modified.maxHp;
	this->tile = NULL;
	this->factionID = 0; this->unitID = 0;
	this->mvCosts = NULL; // this will be allocated by the map when it is time.
}

Unit::~Unit(void) {
}
