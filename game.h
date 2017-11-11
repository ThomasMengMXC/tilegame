#ifndef GAME_H
#define GAME_H
#include "scene.h"
#include "unit.h"

typedef struct Game{
	struct Scene *scene;
	unsigned int sceneCnt;
	UnitData *players;
	unsigned int gold;

	FILE *read;
	FILE *write;
	int row, col;
} Game;

int ncurses_setup(void);

Game *game_setup(void);

void free_game(Game *game);

#endif
