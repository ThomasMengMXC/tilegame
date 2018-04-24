#include <stdlib.h>

#include "button.h"
#include "overmap_SC.h"

int unit_button(Props *props, Layer *layer, short y, short x) {
	OverMap *data = (OverMap *) props->data;
	static int bin = 1;
	y -= layer->yOffset;
	x -= layer->xOffset;
	if (y < 0 || y >= layer->yLength ||
			x < 0 || x >= layer->xLength) {
		return 0;
	}
	if (bin) {
		char *icon = malloc(sizeof(char[2]) * 3 + 1);
		snprintf(icon, 6, "WOW:%1d", data->map->grid[y][x].unit->unitID);
		add_icon_to_layer(data->rangeLayer, y, x, icon, 5);
		free(icon);
		bin--;
	} else {
		remove_icon_from_layer(data->rangeLayer, y, x, 5);
		bin++;
	}
	return 0;
}
