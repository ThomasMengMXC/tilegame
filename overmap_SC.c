#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include "overmap_SC.h"
#include "scene.h"
#include "game.h"
#include "map.h"
#include "cursor.h"
#include "unit.h"

#define DATASTRUCT OverMapData

//DEBUG
FILE *fp = NULL;

void overmap_sc_init(Scene *scene, Game *game) {
	fp = fopen("debug0", "a");
	DATASTRUCT *data = malloc(sizeof(DATASTRUCT));
	data->game = game;
	scene->data = data;

	scene->update = overmap_update;
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
			//draw_range(data->players->firstNode->unit, data->map);
			data->state = PLAYER_MOVE;
			break;
		case PLAYER_MOVE:
			break;
		case ENEMY_PHASE:
			break;
	}
	return;
}

void overmap_keyboard(void *args, int ch) {
	DATASTRUCT *data = (DATASTRUCT *) args;
	switch(ch){
		case KEY_RESIZE:
			getmaxyx(stdscr, data->game->row, data->game->col);
			map_draw(data->map);
			break;	
		case KEY_UP:
			if (data->cursor->yPos > 0) {
				data->cursor->yPos--;
				update_cursor(data->map, data->cursor);
			}
			break;
		case KEY_DOWN:
			if (data->cursor->yPos < data->map->yLength - 1) {
				data->cursor->yPos++;
				update_cursor(data->map, data->cursor);
			}
			break;
		case KEY_LEFT:
			if (data->cursor->xPos > 0) {
				data->cursor->xPos--;
				update_cursor(data->map, data->cursor);
			}
			break;
		case KEY_RIGHT:
			if (data->cursor->xPos < data->map->xLength - 1) {
				data->cursor->xPos++;
				update_cursor(data->map, data->cursor);
			}
			break;
		case 'z':
			select_unit(&(data->map->grid[data->cursor->yPos][data->cursor->xPos]), data->cursor);
			break;
		case 'x':
			break;
	}
	return;
}

void overmap_entry(void *args) {
	DATASTRUCT *data = (DATASTRUCT *) args;
	data->map = map_init();
	data->cursor = cursor_init();

	data->state = PRE_PLAYER_PHASE;

	data->players = data->game->players;
	init_move_grids(data->players, data->map);
	add_units_to_map(data->map, data->players);

	map_draw(data->map);
	update_cursor(data->map, data->cursor);
	return;
}

void overmap_exit(void *args) {
	DATASTRUCT *data = (DATASTRUCT *) args;
	free_move_grid(data->players, data->map);
	map_free(data->map);
	free(data->cursor);

	//DEBUG
	fclose(fp);
	return;
}
