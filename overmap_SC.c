#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include "overmap_SC.h"
#include "scene.h"
#include "game.h"
#include "map.h"
#include "unit.h"

#define DATASTRUCT OverMapData

void overmap_sc_init(Scene *scene, Game *game) {
	DATASTRUCT *data = malloc(sizeof(DATASTRUCT));
	data->game = game;
	scene->data = data;

	scene->update = overmap_update;
	scene->draw = overmap_draw;
	scene->keyboard = overmap_keyboard;

	scene->sc_entry = overmap_entry;
	scene->sc_exit = overmap_exit;
	return;
}

void overmap_update(void *args) {
	return;
}
void overmap_draw(void *args) {
	return;
}
void overmap_keyboard(void *args, int ch) {
	//DATASTRUCT *data = (DATASTRUCT *) args;
	return;
}

void overmap_entry(void *args) {
	DATASTRUCT *data = (DATASTRUCT *) args;
	clear();
	data->playerCnt = 10; // TEMPORARY VALUE
	data->enemyCnt = 10; // TEMPORARY VALUE
	data->map = malloc(sizeof(MapData));
	map_init(data->map);
	map_draw(data->map);
	data->players = malloc(sizeof(UnitData) * data->playerCnt);
	data->enemies = malloc(sizeof(UnitData) * data->enemyCnt);
	return;
}

void overmap_exit(void *args) {
	DATASTRUCT *data = (DATASTRUCT *) args;
	map_free(data->map);
	free(data->map);
	free(data->players);
	free(data->enemies);
	return;
}
