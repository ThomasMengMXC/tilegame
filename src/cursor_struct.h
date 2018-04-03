#ifndef CURSOR_STRUCT_H
#define CURSOR_STRUCT_H

#include "unit_struct.h"
#include "tile_struct.h"

typedef enum {
	DEFAULT, UNIT_SELECTED, UNIT_PLACED,
} CursorState;

typedef struct {
	char canClick;
	unsigned char yPos, xPos;
	unsigned char yOld, xOld;
	Unit *unit;
	char *icon;
	CursorState state;
} Cursor;

#endif
