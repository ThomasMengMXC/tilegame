#ifndef SPRITE_H
#define SPRITE_H

typedef struct {
	short colourDepth, iconDepth;
	short *colour;
	char **icon;
} Sprite;

short *add_colour_to_sprite(Sprite *sprite, short colour);
char **add_icon_to_sprite(Sprite *sprite, char *icon);

#endif
