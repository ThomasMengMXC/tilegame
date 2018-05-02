#ifndef BACKSTAGE_H
#define BACKSTAGE_H

#include <vector>

#include "unit.h"

class Backstage {
	public:
		std::vector<Unit *> *team;
		int money;
		unsigned int unitIDPool;

		Backstage(void);
		~Backstage(void);
};

#endif
