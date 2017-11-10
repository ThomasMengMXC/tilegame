#include <ncurses.h>
#include <stdlib.h>
#include <string.h>
#include "scene.h"
#include "title_SC.h"
#include "game.h"

#define DATASTRUCT TitleData

static char logo[][50] = {
	{" _____ _ _        _____                      "},
	{"|_   _(_) |      |  __ \\                     "},
	{"  | |  _| | ___  | |  \\/ __ _ _ __ ___   ___ "},
	{"  | | | | |/ _ \\ | | __ / _` | '_ ` _ \\ / _ \\"},
	{"  | | | | |  __/ | |_\\ \\ (_| | | | | | |  __/"},
	{"  \\_/ |_|_|\\___|  \\____/\\__,_|_| |_| |_|\\___|"},
};

void title_sc_init(Scene *scene, Game *game) {
	DATASTRUCT *data = malloc(sizeof(DATASTRUCT));
	data->game = game;
	scene->data = data;

	scene->update = title_update;
	scene->draw = title_draw;
	scene->keyboard = title_keyboard;

	scene->sc_entry = title_entry;
	scene->sc_exit = title_exit;
	return;
}

void title_update(void *args) {
	return;
}
void title_draw(void *args) {
	return;
}
void title_keyboard(void *args, int ch) {
	DATASTRUCT *data = (DATASTRUCT *) args;
	int oldPos = data->pos;
	switch(ch){
		case KEY_UP:
			if (data->pos > 0){
				data->pos--;
				singleButton(data, oldPos);
				singleButton(data, data->pos);
			}
			break;
		case KEY_DOWN:
			if (data->pos < data->btnCnt - 1){
				data->pos++;
				singleButton(data, oldPos);
				singleButton(data, data->pos);
			}
			break;
		case 'z':
			if (data->pos == 0) {
				scene_change(data->game, 0, 2);
			}
			break;
	}
	return;
}

void title_entry(void *args) {
	DATASTRUCT *data = (DATASTRUCT *) args;
	data->btnCnt = 3;
	data->pos = 0;
	data->menu = malloc(sizeof(char *) * data->btnCnt);
	data->menu[data->pos++] = "New Game";
	data->menu[data->pos++] = "Continue";
	data->menu[data->pos++] = "Exit";
	data->pos = 0;

	drawLogo(data);
	drawButtons(data);
	return;
}

void title_exit(void *args) {
	DATASTRUCT *data = (DATASTRUCT *) args;
	free(data->menu);
	free(data);
	return;
}

void drawLogo(DATASTRUCT *data) {
	for (int i = 0; i<6; i++){
		mvprintw(data->game->row / 2 + i - 6,
				(data->game->col-strlen(logo[0])) / 2, logo[i]);
	}
}

void drawButtons(DATASTRUCT *data) {
	for (int i = 0; i < data->btnCnt; i++){
		singleButton(data, i);
	}
	data->pos = 0;
}

void singleButton(DATASTRUCT *data, int i) {
	move(data->game->row / 2 + 2 * (i + 1),
			(data->game->col - strlen(data->menu[0])) / 2);
	if (i == data->pos) {
		attron(COLOR_PAIR(1));
		printw(data->menu[i]);
		attroff(COLOR_PAIR(1));
	} else {
		printw(data->menu[i]);
	}
	return;
}
