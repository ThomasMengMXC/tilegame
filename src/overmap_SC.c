#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <theatre/scene.h>

#include "overmap_SC.h" // own .h file
#include "map.h"		// for creating the map in the data
#include "cursor.h"		// for creating the cursor in the data

DATASTRUCT *init_overmap(void) {
	DATASTRUCT *data = malloc(sizeof(DATASTRUCT));

	data->state = PRE_PLAYER_PHASE;

	// initialising primary data
	data->map = init_map();
	data->cursor = init_cursor();
	return data;
}

void overmap_update(Props *props) {
	DATASTRUCT *data = (DATASTRUCT *) props->data;
	switch(data->state) {
		case PRE_PLAYER_PHASE:
			//find_range(data->players, data->map);
			update_cursor(data->map, data->cursor);
			data->state = PLAYER_MOVE;
			break;
		case PLAYER_MOVE:
			draw_screen(props->screen);
			break;
		case ENEMY_PHASE:
			break;
	}
	return;
}

void overmap_keyboard(Props *props, int ch) {
	DATASTRUCT *data = (DATASTRUCT *) props->data;
	Cursor *cursor = data->cursor;
	switch(ch){
		case KEY_RESIZE:
			getmaxyx(stdscr, props->screen->xLength, props->screen->yLength);
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
				activate_button(cursor->yPos, cursor->xPos, props);
			}
			break;
		case 'x':
			request_sc_change(props, 1);
			break;
	}
	return;
}

void overmap_entry(Props *props) {
	short yLength, xLength;
	getmaxyx(stdscr, yLength, xLength);
	props->screen = init_screen(yLength, xLength);

	props->data = init_overmap();
	DATASTRUCT *data = (DATASTRUCT *) props->data;

	// initialising secondary data
	data->map->mapLayer = add_layer_to_scr(props->screen, 0, 0, 0, 0);
	data->map->rangeLayer = add_layer_to_scr(props->screen, 0, 0, 0, 0);

	// intialising a player
	//data->players = data->game->players;
	//init_move_grids(data->players, data->map);
	//add_units_to_map(data->map, data->players);

	// draw the map
	map_draw(data->map);
	draw_screen(props->screen);
	return;
}

void overmap_exit(Props *props) {
	DATASTRUCT *data = (DATASTRUCT *) props->data;
	// remove the two layers from the screen and the screen itself
	remove_layer_from_scr(props->screen);
	remove_layer_from_scr(props->screen);
	free_screen(props->screen);

	// free the map's move grid and map
	//free_move_grid(data->players, data->map);
	free_map(data->map);

	// free the cursor
	free(data->cursor);
	free(data);
	return;
}
