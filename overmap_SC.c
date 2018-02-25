#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include "overmap_SC.h" // own .h file
#include "scene.h"		// for setting the scene variables
#include "game.h"		// for setting the game variable in the data
#include "map.h"		// for creating the map in the data
#include "cursor.h"		// for creating the cursor in the data
#include "screen.h"		// for the screen functions and creating the object

#define DATASTRUCT OverMap

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
	Cursor *cursor = data->cursor;
	switch(ch){
		case KEY_RESIZE:
			getmaxyx(stdscr, data->screen->xLength, data->screen->yLength);
			map_draw(data->map);
			break;	
		case KEY_UP:
			if (cursor->yPos > 0) {
				cursor->yPos--;
				update_cursor(data->map, cursor);
			}
			break;
		case KEY_DOWN:
			if (cursor->yPos < data->map->yLength - 1) {
				cursor->yPos++;
				update_cursor(data->map, cursor);
			}
			break;
		case KEY_LEFT:
			if (cursor->xPos > 0) {
				cursor->xPos--;
				update_cursor(data->map, cursor);
			}
			break;
		case KEY_RIGHT:
			if (cursor->xPos < data->map->xLength - 1) {
				cursor->xPos++;
				update_cursor(data->map, cursor);
			}
			break;
		case 'z':
			if (cursor->canClick) {
				activate_button(cursor->yPos, cursor->xPos, data->screen, args);
			}
			break;
		case 'x':
			break;
	}
	return;
}

void overmap_entry(void *args) {
	DATASTRUCT *data = (DATASTRUCT *) args;

	// initialising primary data
	data->map = init_map();
	data->cursor = init_cursor();
	data->screen = init_screen();
	getmaxyx(stdscr, data->screen->yLength, data->screen->xLength);
	// initialising secondary data
	data->map->mapLayer = add_layer_to_scr(data->screen, 0, 0, 25, 40);
	data->map->rangeLayer = add_layer_to_scr(data->screen, 0, 0, 25, 40);

	data->state = PRE_PLAYER_PHASE;

	// intialising a player
	data->players = data->game->players;
	init_move_grids(data->players, data->map);
	add_units_to_map(data->map, data->players);

	// draw the map
	map_draw(data->map);
	draw_screen(data->screen);
	return;
}

void overmap_exit(void *args) {
	DATASTRUCT *data = (DATASTRUCT *) args;
	// remove the two layers from the screen and the screen itself
	remove_layer_from_scr(data->screen);
	remove_layer_from_scr(data->screen);
	free_screen(data->screen);

	// free the map's move grid and map
	free_move_grid(data->players, data->map);
	free_map(data->map);

	// free the cursor
	free(data->cursor);

	//DEBUG
	fclose(fp);
	return;
}
