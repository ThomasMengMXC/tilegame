#include <ncurses.h>
#include "layer.h"
#include "sprite.h"

// Returns 1 if nothing is drawn, 0 otherwise
int activate_colour(short y, short x, Layer *layer, short colourLayer) {
	short relativeY = y - layer->yOffset;
	short relativeX = x - layer->xOffset;
	// equivalent to &layer->sprite[y + layer->yOffset][x + layer->xOffset];
	Sprite *sprite = layer->sprite[y + layer->yOffset] + x + layer->xOffset;
	if (colourLayer < 0) {
		return 0;
	}
	if (layer->visibility == false ||
			relativeY < 0 || relativeY >= layer->yLength ||
			relativeX < 0 || relativeX >= layer->xLength ||
			!sprite->colourDepth) {
		return 1;
	}
	attron(COLOR_PAIR(sprite->colour[sprite->colourDepth]));
	return 0;
}

int deactivate_colour(short y, short x, Layer *layer, short colourLayer) {
	short relativeY = y - layer->yOffset;
	short relativeX = x - layer->xOffset;
	// equivalent to &layer->sprite[y + layer->yOffset][x + layer->xOffset];
	Sprite *sprite = layer->sprite[y + layer->yOffset] + x + layer->xOffset;
	if (colourLayer < 0) {
		return 0;
	}
	if (layer->visibility == false ||
			relativeY < 0 || relativeY >= layer->yLength ||
			relativeX < 0 || relativeX >= layer->xLength ||
			!sprite->colourDepth) {
		return 1;
	}
	attroff(COLOR_PAIR(sprite->colour[sprite->colourDepth]));
	return 0;
}

// Returns 1 if nothing is drawn, 0 otherwise
int draw_icon(short y, short x, Layer *layer, short iconLayer) {
	short relativeY = y - layer->yOffset;
	short relativeX = x - layer->xOffset;
	// equivalent to &layer->sprite[y + layer->yOffset][x + layer->xOffset];
	Sprite *sprite = layer->sprite[y + layer->yOffset] + x + layer->xOffset;
	if (iconLayer < 0) {
		return 0;
	}
	if (layer->visibility == false ||
			relativeY < 0 || relativeY >= layer->yLength ||
			relativeX < 0 || relativeX >= layer->xLength ||
			!sprite->iconDepth) {
		return 1;
	}
	mvprintw(y, x, sprite->icon[sprite->iconDepth]);
	return 0;
}
