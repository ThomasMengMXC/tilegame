#ifndef CURSOR_H
#define CURSOR_H

#include "unit.h"
#include "tile.h"

typedef enum {
	DEFAULT, UNIT_SELECTED, UNIT_PLACED,
} CursorState;

typedef struct {
	unsigned char xPos, yPos;
	unsigned char xOld, yOld;
	Unit *unit;
	Tile *tile;
	CursorState state;
} CursorData;

CursorData *cursor_init(void);
void select_unit(Tile *tile, CursorData *cursor);

#endif
