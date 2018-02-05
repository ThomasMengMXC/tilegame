#ifndef SCREEN_H
#define SCREEN_H
#include "layer.h"

typedef struct {
	short depth;
	short xLength, yLength;
	Layer *layer;
} Screen;

#endif
