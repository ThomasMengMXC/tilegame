#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include "overmap_SC.h"
#include "scene.h"
#include "game.h"
#include "map.h"
#include "cursor.h"
#include "unit.h"
#include "screen.h"

#define DATASTRUCT OverMapData

//DEBUG
FILE *fp = NULL;

void init_overmap_sc(Scene *scene, Game *game) {
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
			update_cursor(data->map, data->cursor);
			data->state = PLAYER_MOVE;
			break;
		case PLAYER_MOVE:
			draw_screen(data->screen);
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
			getmaxyx(stdscr, data->screen->xLength, data->screen->yLength);
			map_draw(data->map);
			break;	
		case KEY_UP:
			data->map->layer->yOffset--;
			if (data->cursor->yPos > 0) {
				data->cursor->yPos--;
				update_cursor(data->map, data->cursor);
			}
			break;
		case KEY_DOWN:
			data->map->layer->yOffset++;
			if (data->cursor->yPos < data->map->yLength - 1) {
				data->cursor->yPos++;
				update_cursor(data->map, data->cursor);
			}
			break;
		case KEY_LEFT:
			data->map->layer->xOffset--;
			if (data->cursor->xPos > 0) {
				data->cursor->xPos--;
				update_cursor(data->map, data->cursor);
			}
			break;
		case KEY_RIGHT:
			data->map->layer->xOffset++;
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
	data->map = init_map();
	data->cursor = init_cursor();
	data->screen = init_screen();
	getmaxyx(stdscr, data->screen->xLength, data->screen->yLength);
	data->screen->yLength = 10;
	data->screen->xLength = 10;
	data->map->layer = add_layer_to_scr(data->screen, 0, 0, 10, 10);

	data->state = PRE_PLAYER_PHASE;

	data->players = data->game->players;
	init_move_grids(data->players, data->map);
	add_units_to_map(data->map, data->players);

	map_draw(data->map);
	draw_screen(data->screen);

	return;
}

void overmap_exit(void *args) {
	DATASTRUCT *data = (DATASTRUCT *) args;
	free_move_grid(data->players, data->map);
	remove_layer_from_scr(data->screen);
	free_screen(data->screen);
	free_map(data->map);
	free(data->cursor);

	//DEBUG
	fclose(fp);
	return;
}
