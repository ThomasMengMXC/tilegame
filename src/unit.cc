#include <cstring>

#include "unit.h"

Unit::Unit(const char *name, uint16_t unitID) {
	strncpy(this->name, name, 20);
	this->unitID = unitID;
	strncpy(this->icon, ":)", 3);
	this->hp = 10;
	this->move = 5;
	this->str = 5; this->spd = 5; this->def = 5;
	this->tile = NULL;
}
