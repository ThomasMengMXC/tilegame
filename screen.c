#include <ncurses.h>
#include <stdlib.h>
#include "screen.h"
#include "layer.h"

extern FILE *fp;

int draw_screen(Screen *scr) {
	short colourLayer, iconLayer;
	if (!should_draw(scr->layer, scr->depth)) {
		return 0;
	}
	for (short y = 0; y < scr->yLength; y++) {
		for (short x = 0; x < scr->xLength/2; x++) {
			colourLayer = iconLayer = scr->depth;
			while (activate_colour(y, x, scr->layer, colourLayer)) {
				--colourLayer;
			}
			if (colourLayer < 1) {
				mvprintw(y, 2 * x, "  ");
			}
			while (draw_icon(y, x, scr->layer, iconLayer)) {
				--iconLayer;
			}
			if (iconLayer < 1) {
				mvprintw(y, 2 * x, "  ");
			}
			deactivate_colour(y, x, scr->layer, colourLayer);
		}
	}
	refresh();
	return 0;
}
// Returns 1 if nothing is activated, 0 otherwise
int activate_button(short y, short x, Screen *scr, void *args) {
	short buttonLayer = scr->depth;
	while (1) {
		if (buttonLayer < 1) {
			break;
		}
		Layer *lyr = scr->layer[buttonLayer - 1];
		short yRelative = y - lyr->yOffset;
		short xRelative = x - lyr->xOffset;
		if (lyr->visibility == false ||
				yRelative < 0 || yRelative >= lyr->yLength ||
				xRelative < 0 || xRelative >= lyr->xLength) {
			continue;
		}
		Sprite *sprite = lyr->sprite[yRelative] + xRelative;
		if (sprite->buttonDepth == 0) {
			continue;
		}
		sprite->button[sprite->colourDepth - 1](args);
		break;
	}
	return 0;
}


// Are any layers worth drawing?
bool should_draw(Layer **layer, short depth) {
	while (depth > 0 && layer[depth - 1]->draw == false) {
		depth--;
	}
	if (depth < 1) {
		return false;
	}
	return true;
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
		scr->layer = malloc(sizeof(Layer *));
	} else {
		scr->depth++;
		scr->layer = realloc(scr->layer, sizeof(Layer *) * scr->depth);
	}
	layer = scr->layer[scr->depth - 1] = malloc(sizeof(Layer));
	layer->draw = true;
	layer->visibility = true; // setting variables
	layer->yOffset = yOffset; layer->xOffset = xOffset;
	layer->yLength = yLength; layer->xLength = xLength;
	layer->sprite = malloc(sizeof(Sprite *) * yLength); // the sprite
	for (int y = 0; y < yLength; y++) {
		layer->sprite[y] = malloc(sizeof(Sprite) * xLength);
		for (int x = 0; x < xLength; x++) {
			Sprite *sprite = &(layer->sprite[y][x]);
			sprite->colour = NULL;		sprite->icon = NULL;	sprite->button = NULL;
			sprite->colourDepth = 0;	sprite->iconDepth = 0;	sprite->buttonDepth = 0;
		}
	}
	return layer;
}

void remove_layer_from_scr(Screen *scr) {
	Layer *layer = scr->layer[scr->depth - 1];
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
			if (layer->sprite[y][x].button != NULL) {
				free(layer->sprite[y][x].button);
			}
		}
		free(layer->sprite[y]);
	}
	free(layer->sprite);
	free(layer);
	
	if (scr->depth > 1) {
		scr->depth--;
		scr->layer = realloc(scr->layer, sizeof(Layer *) * scr->depth);
	} else if (scr->depth == 1) {
		scr->depth--;
		free(scr->layer);
	}
	return;
}
