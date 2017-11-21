#ifndef GAME_H
#define GAME_H
#include <stdio.h>
#include "scene.h"
#include "unit.h"

typedef struct Game{
	struct Scene *scene;
	unsigned int sceneCnt;

	unsigned int playerCnt;
	unsigned int playerMax;
	Unit *units;

	unsigned int gold;
	FILE *level;

	FILE *read;
	FILE *write;
	int row, col;
} Game;

int ncurses_setup(void);

Game *game_setup(void);

void free_game(Game *game);

void add_player(struct Game *game, Unit unit);

#endif
