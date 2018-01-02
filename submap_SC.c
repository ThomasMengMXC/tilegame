#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>

#include "submap_SC.h"
#include "scene.h"
#include "game.h"
#include "map.h"
#include "unit.h"

#define DATASTRUCT SubMapData

void submap_sc_init(Scene *scene, Game *game) {
	DATASTRUCT *data = malloc(sizeof(DATASTRUCT));
	data->game = game;
	scene->data = data;

	scene->update = submap_update;
	scene->draw = submap_draw;
	scene->keyboard = submap_keyboard;

	scene->sc_entry = submap_entry;
	scene->sc_exit = submap_exit;
	return;
}

void submap_update(void *args) {
	return;
}
void submap_draw(void *args) {
	return;
}
void submap_keyboard(void *args, int ch) {
	//DATASTRUCT *data = (DATASTRUCT *) args;
	return;
}

void submap_entry(void *args) {
	DATASTRUCT *data = (DATASTRUCT *) args;
	clear();
	data->playerCnt = 10; // TEMPORARY VALUE
	data->enemyCnt = 10; // TEMPORARY VALUE
	data->map = map_init();
	map_draw(data->map);
	data->players = malloc(sizeof(Unit) * data->playerCnt);
	data->enemies = malloc(sizeof(Unit) * data->enemyCnt);
	return;
}

void submap_exit(void *args) {
	DATASTRUCT *data = (DATASTRUCT *) args;
	map_free(data->map);
	free(data->map);
	free(data->players);
	free(data->enemies);
	return;
}