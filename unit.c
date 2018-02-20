#include <string.h>
#include <stdlib.h>
#include "unit.h"

// Adds a player to the end of the team list
void add_player(Team *team, Unit unit) {
	Node *current = NULL;
	Node *previous = NULL;
	if (team->firstNode == NULL) {
		current = team->firstNode = malloc(sizeof(Node));
		current->next = NULL;
		current->previous = NULL;
		current->unit = malloc(sizeof(Unit));
		memcpy(current->unit, &unit, sizeof(Unit));
	} else {
		current = team->firstNode;
		previous = current;
		while (current != NULL) {
			previous = current;
			current = current->next;
		}
		// previous is now the final node in the linked list
		current = previous->next = malloc(sizeof(Node));
		current->unit = malloc(sizeof(Unit));
		// Allocate the old final node as the previous node of the new final
		current->previous = previous;
		current->next = NULL;
		memcpy(current->unit, &unit, sizeof(Node));

		team->playerCnt++;
	}
	return;
}

// Frees the team linked list
void free_team(Team *team) {
	Node *current = team->firstNode;
	while (current != NULL) {
		if (current->next != NULL) {
			current = current->next;
			free(current->previous->unit);
			free(current->previous);
		} else {
			free(current->unit);
			free(current);
			current = NULL;
		}
	}
	free(team);
	return;
}

