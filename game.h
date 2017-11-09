#ifndef GAME_H
#define GAME_H
#include "scene.h"

typedef struct Game{
	struct Scene *scene;
	unsigned int scCnt;
	int row, col;
} Game;

int ncurses_setup(void);

Game *game_setup(void);

void free_game(Game *game);

#endif
