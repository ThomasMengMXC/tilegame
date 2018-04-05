#include <stdlib.h>
#include "team.h"

Team *init_team(void) {
	Team *team = malloc(sizeof(Team));
	team->depth = 0;
	team->unit = 0;
	return team;
}

// Frees the team list
void free_team(Team *team) {
	while (team->depth) {
		remove_unit_from_team(team);
	}
	free(team);
	return;
}

// creates a new unit on the team and returns a pointer to said new unit
Unit *add_unit_to_team(Team *team, Unit unit) {
	unit.unitID = team->depth;
	team->depth++;
	team->unit = realloc(team->unit, sizeof(Unit *) * team->depth);
	team->unit[team->depth - 1] = init_unit(unit);
	return team->unit[team->depth - 1];
}

void remove_unit_from_team(Team *team) {
	free_unit(team->unit[team->depth - 1]);
	
	if (team->depth > 1) {
		team->depth--;
		team->unit = realloc(team->unit, sizeof(Team *) * team->depth);
	} else if (team->depth == 1) {
		team->depth--;
		free(team->unit);
		team->unit = NULL;
	}
	return;
}
