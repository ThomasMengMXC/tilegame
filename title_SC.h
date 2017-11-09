#ifndef TITLE_SC_H
#define TITLE_SC_H
#include "game.h"

typedef struct {
	Game *game;
	char **logo;
	char **menu;
	char btnCnt;
	int pos;
	char shouldDraw;
} TitleData;

void tile_sc_init(Scene *scene, Game *game);
void update(void *args);
void draw(void *args);
void keyboard(void *args, int ch);

void sc_entry(void *args);
void sc_exit(void *args);
void drawLogo(TitleData *data);
void drawButtons(TitleData *data);
void singleButton(TitleData *data, int i);

#endif
