#include <stdlib.h>

#include "overmap_SC.h" // own .h file
#include "backstage.h"	// for creating the backstage

// Initialising the data struct, not related to scene creation
DATASTRUCT *init_overmap(void) {
	DATASTRUCT *data = malloc(sizeof(DATASTRUCT));
	data->state = PRE_PLAYER_PHASE;
	// initialising primary data
	data->map = init_map();
	data->mapLayer = NULL;
	data->rangeLayer = NULL;
	data->cursorLayer = NULL;
	return data;
}

void update(Props *props) {
	DATASTRUCT *data = (DATASTRUCT *) props->data;
	switch(data->state) {
		case PRE_PLAYER_PHASE:
			//find_range(data->players, data->map);
			mv_cursor_absolute(props, 0, 0);
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

// Returning -2 quits the game, returning -1 is normal, and returning a 
// positive number requests a scene change
int keyboard(Props *props, int ch) {
	DATASTRUCT *data = (DATASTRUCT *) props->data;
	Cursor *cursor = props->screen->cursor;
	switch(ch){
		case KEY_RESIZE:
			map_draw(data->map, data->mapLayer);
			break;	
		case KEY_UP: mv_cursor_relative(props, -1, 0); break;
		case KEY_DOWN: mv_cursor_relative(props, 1, 0); break;
		case KEY_LEFT: mv_cursor_relative(props, 0, -1); break;
		case KEY_RIGHT: mv_cursor_relative(props, 0, 1);  break;
		case 'z':
			if (cursor->canClick) {
				activate_button(props, cursor->yPos, cursor->xPos);
			}
			break;
		case 'x': return 1;
		case 'q':
			free_backstage(*(props->backstage));
			return -2;
	}
	return -1;
}

void arrival(Props *props) {
	props->screen = init_screen(0, 0);

	props->data = init_overmap();
	DATASTRUCT *data = (DATASTRUCT *) props->data;
	if (*(props->backstage) == NULL) {
		*(props->backstage) = init_backstage();
	}
	data->players = ((Backstage *) props->backstage[0])->team;

	// initialising secondary data
	data->mapLayer = add_layer_to_scr(props->screen, 0, 0, 25, 40);
	data->rangeLayer = add_layer_to_scr(props->screen, 0, 0, 25, 40);
	data->cursorLayer = add_layer_to_scr(props->screen, 0, 0, 0, 0);

	// initialising a player
	Unit unit0 = { .name = "John Citizen",
		.icon = ":)",
		.hp = 10,
		.move = 5,
		.str = 5, .str = 5, .def = 5
	};
	
	Unit unit1 = {
		.name = "Joanne Citizen",
		.icon = ":)",
		.hp = 10,
		.move = 5,
		.str = 5, .str = 5, .def = 5
	};
	add_unit_to_team(data->players, unit0);
	add_unit_to_team(data->players, unit1);
	add_team_to_map(data->map, data->players);

	// draw the map
	map_draw(data->map, data->mapLayer);
	return;
}

void departure(Props *props) {
	DATASTRUCT *data = (DATASTRUCT *) props->data;
	// remove the two layers from the screen and the screen itself
	free_screen(props->screen);

	free_map(data->map);

	free(data);
	return;
}
