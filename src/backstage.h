#ifndef BACKSTAGE_H
#define BACKSTAGE_H

#include <list>
#include "unit.h"

class Backstage {
	public:
		std::list<Unit *> units;
		int money;

		Backstage(void);
		~Backstage(void);
};

#endif
