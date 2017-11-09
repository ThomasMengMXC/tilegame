#include <ncurses.h>
#include <stdlib.h>
#include "scene.h"
#include "action_SC.h"
#include "game.h"

#define DATASTRUCT ActionData

void action_sc_init(Scene *scene, Game *game) {
	DATASTRUCT *data = malloc(sizeof(DATASTRUCT));
	data->game = game;
	scene->data = data;

	scene->update = update;
	scene->draw = draw;
	scene->keyboard = keyboard;

	scene->sc_entry = sc_entry;
	scene->sc_exit = sc_exit;
	return;
}

void update(void *args) {
	return;
}
void draw(void *args) {
	return;
}
void keyboard(void *args, int ch) {
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

void sc_entry(void *args) {
	DATASTRUCT *data = (DATASTRUCT *) args;
	data->btnCnt = 3;
	data->menu = malloc(sizeof(char *) * data->btnCnt);
	data->menu[data->pos++] = "New Game";
	data->menu[data->pos++] = "Continue";
	data->menu[data->pos++] = "Exit";
	data->pos = 0;

	drawLogo(data);
	drawButtons(data);
	return;
}

void sc_exit(void *args) {
	DATASTRUCT *data = (DATASTRUCT *) args;
	free(data->menu);
	free(data);
	return;
}
