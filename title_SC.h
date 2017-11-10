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

void title_sc_init(Scene *scene, Game *game);
void title_draw(void *args);
void title_update(void *args);
void title_keyboard(void *args, int ch);

void title_entry(void *args);
void title_exit(void *args);
void drawLogo(TitleData *data);
void drawButtons(TitleData *data);
void singleButton(TitleData *data, int i);

#endif
