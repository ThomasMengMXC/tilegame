#include <ncurses.h>
#include <stdlib.h>
#include <string.h>
#include "layer.h"
#include "sprite.h"

// Returns 1 if nothing is drawn, 0 otherwise
int activate_colour(short y, short x, Layer *layer, short colourLayer) {
	short yRelative = y - layer->yOffset;
	short xRelative = x - layer->xOffset;
	layer += colourLayer;
	if (colourLayer < 0) {
		return 0;
	}
	if (layer->visibility == false ||
			yRelative < 0 || yRelative >= layer->yLength ||
			xRelative < 0 || xRelative >= layer->xLength) {
		return 1;
	}

	Sprite *sprite = layer->sprite[yRelative] + xRelative;
	if (sprite->colourDepth == 0) {
		return 1;
	}
	attron(COLOR_PAIR(sprite->colour[sprite->colourDepth - 1]));
	return 0;
}

int deactivate_colour(short y, short x, Layer *layer, short colourLayer) {
	short yRelative = y - layer->yOffset;
	short xRelative = x - layer->xOffset;
	layer += colourLayer;
	if (colourLayer < 0) {
		return 0;
	}
	if (layer->visibility == false ||
			yRelative < 0 || yRelative >= layer->yLength ||
			xRelative < 0 || xRelative >= layer->xLength) {
		return 1;
	}
	Sprite *sprite = layer->sprite[yRelative] + xRelative;
	if (sprite->colourDepth == 0) {
		return 1;
	}
	attroff(COLOR_PAIR(sprite->colour[sprite->colourDepth - 1]));
	return 0;
}

// Returns 1 if nothing is drawn, 0 otherwise
int draw_icon(short y, short x, Layer *layer, short iconLayer) {
	short yRelative = y - layer->yOffset;
	short xRelative = x - layer->xOffset;
	layer += iconLayer;
	if (iconLayer < 0) {
		return 0;
	}
	if (layer->visibility == false ||
			yRelative < 0 || yRelative >= layer->yLength ||
			xRelative < 0 || xRelative >= layer->xLength) {
		return 1;
	}
	Sprite *sprite = layer->sprite[yRelative] + xRelative;
	if (sprite->iconDepth == 0) {
		return 1;
	}
	mvprintw(y, 2 * (x),
			sprite->icon[sprite->iconDepth - 1]);
	return 0;
}

void add_colour_to_layer(Layer *layer, short y, short x, short colour) {
	if (	y < 0 || y >= layer->yLength ||
			x < 0 || x >= layer->xLength) {
		return;
	}
	Sprite *sprite = &(layer->sprite[y][x]);
	if (sprite->colourDepth == 0) {
		sprite->colourDepth++;
		sprite->colour = malloc(sizeof(short));
	} else {
		sprite->colourDepth++;
		sprite->colour = realloc(sprite->colour, sizeof(short) * sprite->colourDepth);
	}
	sprite->colour[sprite->colourDepth - 1] = colour;
	return;
}

void remove_colour_from_layer(Layer *layer, short y, short x) {
	if (	y < 0 || y >= layer->yLength ||
			x < 0 || x >= layer->xLength) {
		return;
	}
	Sprite *sprite = &(layer->sprite[y][x]);
	if (sprite->colourDepth > 1) {
		sprite->colourDepth--;
		sprite->colour = realloc(sprite->colour, sizeof(short) * sprite->colourDepth);
	} else if (sprite->colourDepth == 1) {
		sprite->colourDepth--;
		free(sprite->colour);
		sprite->colour = NULL;
	}
	return;
}

void add_icon_to_layer(Layer *layer, short y, short x, char *icon) {
	if (	y < 0 || y >= layer->yLength ||
			x < 0 || x >= layer->xLength) {
		return;
	}
	Sprite *sprite = &(layer->sprite[y][x]);
	if (sprite->iconDepth == 0) {
		sprite->iconDepth++;
		sprite->icon = malloc(sizeof(char *));
	} else {
		sprite->iconDepth++;
		sprite->icon = realloc(sprite->icon, sizeof(char *) * sprite->iconDepth);
	}
	sprite->icon[sprite->iconDepth - 1] = malloc(sizeof(char) * 3);
	strncpy(sprite->icon[sprite->iconDepth - 1], icon, 3);
	return;
}

void remove_icon_from_layer(Layer *layer, short y, short x) {
	if (	y < 0 || y >= layer->yLength ||
			x < 0 || x >= layer->xLength) {
		return;
	}
	Sprite *sprite = &(layer->sprite[y][x]);
	free(sprite->icon[sprite->iconDepth - 1]);
	if (sprite->iconDepth > 1) {
		sprite->iconDepth--;
		sprite->icon = realloc(sprite->icon, sizeof(char *) * sprite->iconDepth);
	} else if (sprite->iconDepth == 1) {
		sprite->iconDepth--;
		free(sprite->icon);
		sprite->icon = NULL;
	}
	return;
}
