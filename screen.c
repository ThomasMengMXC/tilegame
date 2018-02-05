#include "screen.h"
#include "layer.h"

int activate_colour(short y, short x, Layer *layer, short colourLayer);

int draw_screen(Screen *scr) {
	short colourLayer, iconLayer;
	colourLayer = iconLayer = scr->depth;
	for (short y = 0; y < scr->yLength; y++) {
		for (short x = 0; x < scr->xLength; x++) {
			while (activate_colour(y, x, scr->layer, colourLayer)) {
				--colourLayer;
			}
			while (draw_sprite(y, x, scr->layer, iconLayer)) {
				--iconLayer;
			}
			deactivate_colour(y, x, scr->layer, colourLayer);
		}
	}
	return 0;
}

int activate_colour(short y, short x, Layer *layer, short colourLayer) {
	short relativeY = y - layer->yOffset;
	short relativeX = x - layer->xOffset;
	if (colourLayer < 0) {
		return 0;
	}
	if (layer->visibility == false ||
			relativeY < 0 || relativeY >= layer->yLength ||
			relativeX < 0 || relativeX >= layer->xLength) {
		return 1;
	}
	//if (sprite_colour(layer->sprites[y + layer->yOffset][x + layer->xOffset]))
	return 0;
}
