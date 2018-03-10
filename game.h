#ifndef GAME_H
#define GAME_H
#include <stdio.h>
#include "stage.h"
#include "unit.h"

typedef struct Game{
	Stage *stage;

	Team *players;

	unsigned int gold;
	FILE *level;

	FILE *read;
	FILE *write;
	int row, col;
} Game;

int ncurses_init(void);

Game *game_init(void);

void free_game(Game *game);

#endif
