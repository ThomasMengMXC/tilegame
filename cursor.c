#include <stdlib.h>
#include "cursor.h"

CursorData *init_cursor(void) {
	CursorData *cursor = malloc(sizeof(CursorData));
	cursor->xPos = 0;				cursor->yPos = 0;
	cursor->xOld = cursor->xPos;	cursor->yOld = cursor->yPos;
	cursor->unit = NULL;
	cursor->state = DEFAULT;
	return cursor;
}

void select_unit(Tile *tile, CursorData *cursor) {
	if (cursor->state == DEFAULT) {
		cursor->unit = tile->unit;
		cursor->state = UNIT_SELECTED;
	}
	return;
}
