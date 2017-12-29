#include "tile.h"
#include <ncurses.h>

void tile_draw(Tile *tile, bool colorOn, bool defaultValue) {
	if (colorOn) {
		attron(COLOR_PAIR(defaultValue ?
					tile->defaultColour : tile->colour));
	}
	if (tile->unit) {
		mvprintw(tile->yPos, tile->xPos * 2, tile->unit->icon);
	} else {
		mvaddch(tile->yPos, tile->xPos * 2, tile->icon);
		addch(' ');
	}
	if (colorOn) {
		attroff(COLOR_PAIR(defaultValue ?
					tile->defaultColour : tile->colour));
	}
	return;
}
