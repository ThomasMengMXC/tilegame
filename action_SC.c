#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include "scene.h"
#include "action_SC.h"
#include "game.h"

#define DATASTRUCT ActionData

void action_sc_init(Scene *scene, Game *game) {
	DATASTRUCT *data = malloc(sizeof(DATASTRUCT));
	data->game = game;
	scene->data = data;

	scene->update = action_update;
	scene->draw = action_draw;
	scene->keyboard = action_keyboard;

	scene->sc_entry = action_entry;
	scene->sc_exit = action_exit;
	return;
}

void action_update(void *args) {
	return;
}
void action_draw(void *args) {
	return;
}
void action_keyboard(void *args, int ch) {
	DATASTRUCT *data = (DATASTRUCT *) args;
	return;
}

void action_entry(void *args) {
	DATASTRUCT *data = (DATASTRUCT *) args;
	clear();
	return;
}

void action_exit(void *args) {
	DATASTRUCT *data = (DATASTRUCT *) args;
	free(data);
	return;
}
