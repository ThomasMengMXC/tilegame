#ifndef CURSOR_H
#define CURSOR_H

#include "unit.h"

typedef enum {
	DEFAULT, UNIT_SELECTED, UNIT_PLACED,
} CursorState;

typedef struct {
	unsigned char xPos, yPos;
	unsigned char xOld, yOld;
	Unit *unit;
	CursorState state;
} CursorData;

CursorData *cursor_init(void);

#endif
