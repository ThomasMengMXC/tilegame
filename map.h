#ifndef MAP_H
#define MAP_H

#include <ncurses.h>

typedef struct {
	char icon;
	char tileType;
	int mvCost;
	void *objects;
	unsigned int yPos, xPos;
} Tile;

typedef struct {
	Tile **grid;
	unsigned int width, height;
} MapData;

void map_init(MapData *map);
void map_free(MapData *map);
void map_draw(MapData *map);
void tile_draw(Tile tile, bool colorOn);

#endif
