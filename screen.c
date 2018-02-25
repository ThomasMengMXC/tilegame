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
			while (activate_colour(scr->layer, y, x, colourLayer)) {
				--colourLayer;
			}
			if (colourLayer < 1) {
				mvprintw(y, 2 * x, "  ");
			}
			while (draw_icon(scr->layer, y, x, iconLayer)) {
				--iconLayer;
			}
			if (iconLayer < 1) {
				mvprintw(y, 2 * x, "  ");
			}
			deactivate_colour(scr->layer, y, x, colourLayer);
		}
	}
	refresh();
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

// Returns 1 if nothing is activated, 0 otherwise
int activate_button(short y, short x, Screen *scr, void *args) {
	short buttonLayer = scr->depth;
	while (1) {
		if (buttonLayer < 1) {
			break;
		}
		Layer *lyr = scr->layer[buttonLayer - 1];
		buttonLayer--;
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
	layer->sprite = init_sprite(yLength, xLength);
	return layer;
}

void remove_layer_from_scr(Screen *scr) {
	Layer *layer = scr->layer[scr->depth - 1];
	free_sprite(layer->yLength, layer->xLength, layer->sprite);
	free(layer);
	layer = NULL;
	
	if (scr->depth > 1) {
		scr->depth--;
		scr->layer = realloc(scr->layer, sizeof(Layer *) * scr->depth);
	} else if (scr->depth == 1) {
		scr->depth--;
		free(scr->layer);
		scr->layer = NULL;
	}
	return;
}

Sprite **init_sprite(short yLength, short xLength) {
	Sprite **spriteArray = malloc(sizeof(Sprite *) * yLength); // the sprite
	for (int y = 0; y < yLength; y++) {
		spriteArray[y] = malloc(sizeof(Sprite) * xLength);
		for (int x = 0; x < xLength; x++) {
			Sprite *sprite = &(spriteArray[y][x]);
			sprite->colour = NULL;		sprite->icon = NULL;	sprite->button = NULL;
			sprite->colourDepth = 0;	sprite->iconDepth = 0;	sprite->buttonDepth = 0;
		}
	}
	return spriteArray;
}

void free_sprite(short yLength, short xLength, Sprite **sprite) {
	for (int y = 0; y < yLength; y++) {
		for (int x = 0; x < xLength; x++) {
			for (int i = 0; i < sprite[y][x].iconDepth; i++) {
				free(sprite[y][x].icon[i]);
			}
			if (sprite[y][x].icon != NULL) {
				free(sprite[y][x].icon);
			}
			if (sprite[y][x].colour != NULL) {
				free(sprite[y][x].colour);
			}
			if (sprite[y][x].button != NULL) {
				free(sprite[y][x].button);
			}
		}
		free(sprite[y]);
	}
	free(sprite);
	return;
}
