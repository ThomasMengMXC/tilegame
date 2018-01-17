#include "tile.h"
#include <ncurses.h>

void tile_draw(Tile *tile, bool colorOn, bool defaultValue) {
	if (tile->unit) {
		attron(COLOR_PAIR(tile->colour));
		mvaddstr(tile->yPos, tile->xPos * 2, tile->unit->icon);
		attroff(COLOR_PAIR(tile->colour));
	} else {
		attron(COLOR_PAIR(tile->colour));
		mvaddstr(tile->yPos, tile->xPos * 2, tile->icon);
		attroff(COLOR_PAIR(tile->colour));
	}
	return;
}
