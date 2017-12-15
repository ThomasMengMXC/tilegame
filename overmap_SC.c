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
	DATASTRUCT *data = (DATASTRUCT *) args;
	switch(data->state) {
		case PRE_PLAYER_PHASE:
			find_range(data->players, data->map);
			draw_range(*data->players->units, data->map);
			data->state = PLAYER_MOVE;
			break;
		case PLAYER_MOVE:
			break;
		case ENEMY_PHASE:
			break;
	}
	return;
}
void overmap_draw(void *args) {
	//DATASTRUCT *data = (DATASTRUCT *) args;
	return;
}

void overmap_keyboard(void *args, int ch) {
	//DATASTRUCT *data = (DATASTRUCT *) args;
	return;
}

void overmap_entry(void *args) {
	DATASTRUCT *data = (DATASTRUCT *) args;
	clear();

	data->map = malloc(sizeof(MapData));
	map_init(data->map);

	data->state = PRE_PLAYER_PHASE;

	data->players = malloc(sizeof(Team));
	data->players->units = &(data->game->units);
	data->players->playerCnt = &(data->game->playerCnt);
	data->players->playerMax = &(data->game->playerMax);
	init_move_grids(data->players, data->map);
	add_units_to_map(data->map, data->game);

	map_draw(data->map);
	return;
}

void overmap_exit(void *args) {
	DATASTRUCT *data = (DATASTRUCT *) args;
	free_move_grid(data->players, data->map);
	free(data->players);
	map_free(data->map);
	//free(data->enemies);
	return;
}
