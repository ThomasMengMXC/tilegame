#ifndef TEAM_H
#define TEAM_H

#include "unit.h"

typedef struct {
	unsigned char depth;
	Unit **unit;
} Team;

Team *init_team(void);
void free_team(Team *team);

Unit *add_unit_to_team(Team *team, unsigned int *unitIDPool, char *name);
void remove_unit_from_team(Team *team);

#endif
