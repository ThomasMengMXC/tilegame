#ifndef TEAM_H
#define TEAM_H

#include "team_struct.h"

Team *init_team(void);
void free_team(Team *team);

Unit *add_unit_to_team(Team *team, Unit unit);
void remove_unit_from_team(Team *team);

#endif
