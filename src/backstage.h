#ifndef BACKSTAGE_H
#define BACKSTAGE_H

#include "team.h"

typedef struct {
	Team *team;
	int money;
} Backstage;

Backstage *init_backstage(void);
void free_backstage(Backstage *bs);

#endif