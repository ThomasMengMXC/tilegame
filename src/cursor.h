#ifndef CURSOR_H
#define CURSOR_H

#include "unit.h"

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

Cursor *init_cursor(void);

#endif
