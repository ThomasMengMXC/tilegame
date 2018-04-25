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
		char *icon = new char[7];
		snprintf(icon, 6, "WOW:%1d", data->map->grid[y][x].unit->unitID % 10);
		add_icon_to_layer(data->rangeLayer, y, x, icon, 6);
		delete[] icon;
		bin--;
	} else {
		remove_icon_from_layer(data->rangeLayer, y, x, 6);
		bin++;
	}
	return 0;
}
