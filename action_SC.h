#ifndef ACTION_SC_H
#define ACTION_SC_H
#include "game.h"

typedef struct {
	char icon;
	char tileType;
	int mvCost;
	void *objects;
	unsigned int y;
	unsigned int x;
} Tile;

typedef struct {
	Tile **grid;
	unsigned int width, height;
} MapData;

typedef struct {
	int hp;
	unsigned int xPos, yPos;
	unsigned int move;
	unsigned int str, mag, spd, def;
} UnitData;

typedef struct {
	Game *game;
	MapData *map;
	UnitData *players, *enemies;
} ActionData;

void action_sc_init(Scene *scene, Game *game);

void action_update(void *args);
void action_draw(void *args);
void action_keyboard(void *args, int ch);

void action_entry(void *args);
void action_exit(void *args);

#endif
