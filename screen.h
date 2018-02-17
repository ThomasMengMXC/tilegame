#ifndef SCREEN_H
#define SCREEN_H
#include "layer.h"

typedef struct {
	short depth;
	short xLength, yLength;
	Layer **layer;
} Screen;

int draw_screen(Screen *scr);
Screen *init_screen(void);
void free_screen(Screen *scr);
Layer *add_layer_to_scr(Screen *scr, short yOffset, short xOffset,
		short yLength, short xLength);
void remove_layer_from_scr(Screen *scr);

#endif
