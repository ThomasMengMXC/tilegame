extern "C" {
#include <theatre/colour.h>
}

#include "overmap_SC.h" // own .h file
#include "backstage.h"	// for creating the backstage

// Initialising the data struct, not related to scene creation
OverMap::OverMap(void) {
	this->state = PRE_PLAYER_PHASE;
	this->map = new Map();
	this->mapLayer = NULL;
	this->rangeLayer = NULL;
	this->cursorLayer = NULL;
	this->enemies = new std::vector<Unit *>;
}

OverMap::~OverMap(void) {
	delete this->enemies;
	delete this->map;
}

extern "C" {
void update(Props *props) {
	OverMap *data = (OverMap *) props->data;
	Cursor *cursor = props->screen->cursor;
	switch(data->state) {
	case PRE_PLAYER_PHASE:
		data->map->calculate_ranges();
		activate_hover(props, 1, cursor->yPos, cursor->xPos);
		data->state = PLAYER_MOVE;
		break;
	case PLAYER_MOVE:
		draw_screen(props->screen);
		break;
	case ENEMY_PHASE:
		break;
	}
}

// Returning -2 quits the game, returning -1 is normal, and returning a 
// positive number requests a scene change
int keyboard(Props *props, int ch) {
	if (!ch) return -1;
	OverMap *data = (OverMap *) props->data;
	Cursor *cursor = props->screen->cursor;
	activate_hover(props, 0, cursor->yPos, cursor->xPos);
	switch(ch){
	case 'w':
		mv_layer_relative(data->mapLayer, -1, 0);
		mv_layer_relative(data->rangeLayer, -1, 0);
		break;
	case 's':
		mv_layer_relative(data->mapLayer, 1, 0);
		mv_layer_relative(data->rangeLayer, 1, 0);
		break;
	case 'a':
		mv_layer_relative(data->mapLayer, 0, -1);
		mv_layer_relative(data->rangeLayer, 0, -1);
		break;
	case 'd':
		mv_layer_relative(data->mapLayer, 0, 1);
		mv_layer_relative(data->rangeLayer, 0, 1);
		break;
	case KEY_RESIZE: resize_screen(props->screen); break;
	case KEY_UP: mv_cursor_relative(props, -1, 0); break;
	case KEY_DOWN: mv_cursor_relative(props, 1, 0); break;
	case KEY_LEFT: mv_cursor_relative(props, 0, -1); break;
	case KEY_RIGHT: mv_cursor_relative(props, 0, 1);  break;
	case 'z': activate_button(props, cursor->yPos, cursor->xPos); break;
	case 'x': {
		Layer *layer = remove_layer_from_scr(props->screen);
		if (layer == data->mapLayer) data->mapLayer = NULL;
		if (layer == data->rangeLayer) data->rangeLayer = NULL;
		if (layer == data->cursorLayer) data->cursorLayer = NULL;
		break;
	}
	case 'q': delete (Backstage *) *props->backstage; return -2;
	case 'c': return 1;
	}
	activate_hover(props, 1, cursor->yPos, cursor->xPos);
	return -1;
}

void arrival(Props *props) {
	props->screen = init_screen(0, 0);

	props->data = (OverMap *) new OverMap();
	OverMap *data = (OverMap *) props->data;
	Backstage *bs = NULL;
	if (*props->backstage == NULL) {
		*props->backstage = new Backstage();
	}
	bs = (Backstage *) *props->backstage;
	data->players = bs->team;

	// initialising secondary data
	data->mapLayer = add_layer_to_scr(props->screen, 0, 0, 25, 40);
	data->rangeLayer = add_layer_to_scr(props->screen, 0, 0, 25, 40);
	data->cursorLayer = add_layer_to_scr(props->screen, 0, 0, 0, 0);


	// initialising a player
	data->players->push_back(new Unit("John Citizen", bs->unitIDPool++));
	data->players->push_back(new Unit("Jane Citizen", bs->unitIDPool++));
	data->map->place_team(data->players);

	// draw the map
	data->map->draw(data->mapLayer);
}

void departure(Props *props) {
	OverMap *data = (OverMap *) props->data;
	for (size_t i = 0; i < data->enemies->size(); i++) {
		delete (*data->enemies)[i];
	}
	// remove the two layers from the screen and the screen itself
	clear_screen(props->screen);
	free_screen(props->screen);
	props->screen = NULL;
	delete data;
}
}
