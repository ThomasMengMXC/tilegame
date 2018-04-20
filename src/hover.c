#include <theatre/colour.h>
#include "hover.h"
#include "overmap_SC.h"

int map_hover(Props *props, Layer *layer, char start, short y, short x) {
	OverMap *data = (OverMap *) props->data;
	if (start) {
		add_colour_to_layer(data->cursorLayer, y, x,
				rgb_to_term256(255, 255, 255));
	} else {
		remove_colour_from_layer(data->cursorLayer, y, x);
	}
	return 0;
}
