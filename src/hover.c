#include <theatre/colour.h>
#include "hover.h"
#include "overmap_SC.h"

int map_hover(Props *props, Layer *layer, char start, short y, short x) {
	OverMap *data = (OverMap *) props->data;
	if (start) {
		add_colour_to_layer(data->cursorLayer, y, x,
				(Colour){.r = 255, .g = 255, .b = 255, .a = 128});
	} else {
		remove_colour_from_layer(data->cursorLayer, y, x);
	}
	return 0;
}
