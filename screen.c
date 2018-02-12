#include <ncurses.h>
#include <stdlib.h>
#include "screen.h"
#include "layer.h"

int draw_screen(Screen *scr) {
	short colourLayer, iconLayer;
	for (short y = 0; y < scr->yLength; y++) {
		for (short x = 0; x < scr->xLength; x++) {
			colourLayer = iconLayer = scr->depth;
			while (activate_colour(y, x, scr->layer, colourLayer)) {
				--colourLayer;
			}
			while (draw_icon(y, x, scr->layer, iconLayer)) {
				--iconLayer;
			}
			deactivate_colour(y, x, scr->layer, colourLayer);
		}
	}
	return 0;
}

Screen *init_screen(void) {
	Screen *scr = malloc(sizeof(Screen));
	scr->depth = 0;
	scr->xLength = scr->yLength = 0;
	scr->layer = malloc(sizeof(Layer) * scr->depth);
	return scr;
}

void free_screen(Screen *scr) {
	free(scr->layer);
	free(scr);
}

// creates a new layer on the screen and returns a pointer to said new layer
Layer *add_layer_to_scr(Screen *scr, short xOffset, short yOffset,
		short xLength, short yLength) {
	Layer *layer;
	if (scr->depth == 0) {
		scr->depth++;
		scr->layer = malloc(sizeof(Layer));
	} else {
		scr->depth++;
		scr->layer = realloc(scr->layer, sizeof(Layer) * scr->depth);
	}
	layer = scr->layer + scr->depth;
	layer->visibility = true;
	layer->yOffset = yOffset;
	layer->xOffset = xOffset;
	layer->yLength = yLength;
	layer->xLength = xLength;
	layer->sprite = malloc(sizeof(Sprite *) * yLength);
	for (int i = 0; i < yLength; i++) {
		layer->sprite[i] = malloc(sizeof(Sprite) * xLength);
	}
	return scr->layer + scr->depth;
}

void remove_layer_from_scr(Screen *scr) {
	Layer *layer;
	layer = scr->layer + scr->depth;
	for (int i = 0; i < scr->yLength; i++) {
		free(layer->sprite[i]);
	}
	free(layer->sprite);
	
	if (scr->depth > 0) {
		scr->depth--;
		scr->layer = realloc(scr->layer, sizeof(Layer) * scr->depth);
	} else {
		free(scr->layer);
		scr->layer = NULL;
	}
	return;
}
