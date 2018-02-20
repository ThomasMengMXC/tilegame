#ifndef LAYER_H
#define LAYER_H
#include <ncurses.h>
#include "sprite.h"

typedef struct {
	bool draw; // has this layer been updated
	bool visibility;
	short xOffset, yOffset;
	short xLength, yLength;
	Sprite **sprite;
} Layer;

int activate_colour(short y, short x, Layer **layer, short colourLayer);
int deactivate_colour(short y, short x, Layer **layer, short colourLayer);
int draw_icon(short y, short x, Layer **layer, short iconLayer);

void add_colour_to_layer(Layer *layer, short y, short x, Colour colour);
void remove_colour_from_layer(Layer *layer, short y, short x);

void add_icon_to_layer(Layer *layer, short y, short x, char *icon);
void remove_icon_from_layer(Layer *layer, short y, short x);

void add_button_to_layer(Layer *layer, short y, short x, Button button);
void remove_button_from_layer(Layer *layer, short y, short x);

#endif
