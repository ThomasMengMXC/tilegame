#include <stdlib.h>
#include <string.h>
#include "sprite.h"

short *add_colour_to_sprite(Sprite *sprite, short colour) {
	if (sprite->colourDepth == 0) {
		sprite->colourDepth++;
		sprite->colour = malloc(sizeof(short));
	} else {
		sprite->colourDepth++;
		sprite->colour = realloc(sprite->colour, sizeof(short) * sprite->colourDepth);
	}
	sprite->colour[sprite->colourDepth] = colour;
	return sprite->colour + sprite->colourDepth;
}

char **add_icon_to_sprite(Sprite *sprite, char *icon) {
	if (sprite->iconDepth == 0) {
		sprite->iconDepth++;
		sprite->icon = malloc(sizeof(char *));
	} else {
		sprite->iconDepth++;
		sprite->icon = realloc(sprite->icon, sizeof(char *) * sprite->iconDepth);
	}
	sprite->icon[sprite->iconDepth] = malloc(sizeof(char) * 2);
	strncpy(sprite->icon[sprite->iconDepth], icon, 2);
	return sprite->icon + sprite->iconDepth;
}
