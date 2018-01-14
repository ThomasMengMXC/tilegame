#include "tile.h"
#include <ncurses.h>

void tile_draw(Tile *tile, bool colorOn, bool defaultValue) {
	if (tile->unit) {
		mvprintw(tile->yPos, tile->xPos * 2, tile->unit->icon);
	} else {
		attron(COLOR_PAIR(tile->colour));
		mvprintw(tile->yPos, tile->xPos * 2, tile->icon);
		attroff(COLOR_PAIR(tile->colour));
	}
	return;
}
