#include "button.h"
#include "overmap_SC.h"
#include "cursor.h"

int unit_button(void *args) {
	OverMap *data = (OverMap *) args;
	Cursor *cursor = data->cursor;
	cursor->canClick = false;
	short yPos = cursor->yPos;
	short xPos = cursor->xPos;
	Tile *tile = &(data->map->grid[yPos][xPos]);
	if (tile->unit && cursor->state == DEFAULT) {
		cursor->unit = tile->unit;
		cursor->icon = tile->unit->icon;
		cursor->state = UNIT_SELECTED;
	}
	return 0;
}
