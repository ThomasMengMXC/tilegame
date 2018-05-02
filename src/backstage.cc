#include "backstage.h"

Backstage::Backstage(void) {
	this->team = new std::vector<Unit *>;
	this->money = 0;
	this->unitIDPool = 0;
}

Backstage::~Backstage(void) {
	for (size_t i = 0; i < this->team->size(); i++) {
		delete (*this->team)[i];
	}
	delete this->team;
}
