#include "cursor.h"
#include <stdlib.h>

CursorData *cursor_init(void) {
	CursorData *cursor = malloc(sizeof(CursorData));
	cursor->xPos = 0;				cursor->yPos = 0;
	cursor->xOld = cursor->xPos;	cursor->yOld = cursor->yPos;
	cursor->unit = NULL;
	cursor->state = DEFAULT;
	return cursor;
}
