extern "C" {
#include <theatre/colour.h>
}

#include "hover.h"
#include "overmap_SC.h"

int map_hover(Props *props, Layer *layer, char start, short y, short x) {
	OverMap *data = (OverMap *) props->data;
	Colour colour;
	colour.r = 255; colour.g = 255; colour.b = 255; colour.a = 128;
	Vector2D pos;
	pos.y = y;
	pos.x = x;
	if (start) {
		add_colour_to_layer(data->cursorLayer, y, x, colour);
		if (data->map->unit_at(pos) != NULL) {
			data->map->draw_move_grid(data->rangeLayer, data->map->unit_at(pos));
		}
	} else {
		remove_colour_from_layer(data->cursorLayer, y, x);
		if (data->map->unit_at(pos) != NULL) {
			data->map->remove_move_grid(data->rangeLayer, data->map->unit_at(pos));
		}
	}
	return 0;
}
