#ifndef LAYER_H
#define LAYER_H
#include <ncurses.h>
#include "sprite.h"

typedef struct {
	bool visibility;
	short xOffset, yOffset;
	short xLength, yLength;
	Sprite **sprites;
} Layer;

#endif
