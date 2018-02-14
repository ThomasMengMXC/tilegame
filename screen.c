#include <ncurses.h>
#include <stdlib.h>
#include "screen.h"
#include "layer.h"

int draw_screen(Screen *scr) {
	short colourLayer, iconLayer;
	for (short y = 0; y < scr->yLength; y++) {
		for (short x = 0; x < scr->xLength; x++) {
			colourLayer = iconLayer = scr->depth - 1;
			while (activate_colour(y, x, scr->layer, colourLayer)) {
				--colourLayer;
			}
			if (colourLayer < 0) {
				mvprintw(y, 2 * x, "  ");
			}
			while (draw_icon(y, x, scr->layer, iconLayer)) {
				--iconLayer;
			}
			if (iconLayer < 0) {
				mvprintw(y, 2 * x, "  ");
			}
			deactivate_colour(y, x, scr->layer, colourLayer);
		}
	}
	refresh();
	return 0;
}

Screen *init_screen(void) {
	Screen *scr = malloc(sizeof(Screen));
	scr->depth = 0;
	scr->xLength = scr->yLength = 0;
	return scr;
}

void free_screen(Screen *scr) {
	free(scr);
}

// creates a new layer on the screen and returns a pointer to said new layer
Layer *add_layer_to_scr(Screen *scr, short yOffset, short xOffset,
		short yLength, short xLength) {
	Layer *layer;
	if (scr->depth == 0) {
		scr->depth++;
		scr->layer = malloc(sizeof(Layer));
	} else {
		scr->depth++;
		scr->layer = realloc(scr->layer, sizeof(Layer) * scr->depth);
	}
	layer = scr->layer + scr->depth - 1;
	layer->visibility = true;
	layer->yOffset = yOffset;
	layer->xOffset = xOffset;
	layer->yLength = yLength;
	layer->xLength = xLength;
	layer->sprite = malloc(sizeof(Sprite *) * yLength);
	for (int y = 0; y < yLength; y++) {
		layer->sprite[y] = malloc(sizeof(Sprite) * xLength);
		for (int x = 0; x < xLength; x++) {
			layer->sprite[y][x].colour = NULL;
			layer->sprite[y][x].icon = NULL;
			layer->sprite[y][x].colourDepth = 0;
			layer->sprite[y][x].iconDepth = 0;
		}
	}
	return layer;
}

void remove_layer_from_scr(Screen *scr) {
	Layer *layer = scr->layer + scr->depth - 1;
	for (int y = 0; y < layer->yLength; y++) {
		for (int x = 0; x < layer->xLength; x++) {
			for (int i = 0; i < layer->sprite[y][x].iconDepth; i++) {
				free(layer->sprite[y][x].icon[i]);
			}
			if (layer->sprite[y][x].icon != NULL) {
				free(layer->sprite[y][x].icon);
			}
			if (layer->sprite[y][x].colour != NULL) {
				free(layer->sprite[y][x].colour);
			}
		}
		free(layer->sprite[y]);
	}
	free(layer->sprite);
	
	if (scr->depth > 1) {
		scr->depth--;
		scr->layer = realloc(scr->layer, sizeof(Layer) * scr->depth);
	} else if (scr->depth == 1) {
		scr->depth--;
		free(scr->layer);
	}
	return;
}
