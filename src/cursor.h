#ifndef CURSOR_H
#define CURSOR_H

#include "unit.h"
#include "tile.h"

typedef enum {
	DEFAULT, UNIT_SELECTED, UNIT_PLACED,
} CursorState;

typedef struct {
	bool canClick;
	unsigned char yPos, xPos;
	unsigned char yOld, xOld;
	Unit *unit;
	char *icon;
	CursorState state;
} Cursor;

Cursor *init_cursor(void);

#endif
