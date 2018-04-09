#include <stdlib.h>

#include "button.h"
#include "overmap_SC.h"

int unit_button(void *args, short y, short x) {
	Props *props = (Props *) args;
	OverMap *data = (OverMap *) props->data;
	static int bin = 1;
	if (bin) {
		char *icon = malloc(sizeof(char) * 3);
		snprintf(icon, 3, "%2d", data->map->grid[y][x].unit->unitID);
		add_icon_to_layer(data->rangeLayer, y, x, icon);
		free(icon);
		bin--;
	} else {
		remove_icon_from_layer(data->rangeLayer, y, x);
		bin++;
	}
	return 0;
}
