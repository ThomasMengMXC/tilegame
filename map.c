#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include "map.h"

void map_init(MapData *map) {
	map->height = 25; // TEMPORARY VALUE
	map->width = 40; // TEMPORARY VALUE
	map->grid = malloc(sizeof(Tile *) * map->height);
	for (int i = 0; i < map->height; i++) {
		map->grid[i] = malloc(sizeof(Tile) * map->width);
		for (int j = 0; j < map->width; j++) {
			map->grid[i][j].tileType = '.';
			map->grid[i][j].icon = map->grid[i][j].tileType;
			map->grid[i][j].objects = NULL;
			map->grid[i][j].yPos = i;
			map->grid[i][j].xPos = j;
		}
	}
	return;
}

void map_free(MapData *map) {
	for (int i = 0; i < map->height; i++) {
		free(map->grid[i]);
	}
	free(map->grid);
	free(map);
	return;
}

void map_draw(MapData *map) {
	for (int i = 0; i < map->height; i++) {
		for (int j = 0; j < map->width; j++) {
			tile_draw(map->grid[i][j], true);
		}
	}
	return;
}

void tile_draw(Tile tile, bool colorOn) {
	if (colorOn) {
		attron(COLOR_PAIR(1));
	}
	mvaddch(tile.yPos, tile.xPos * 2, tile.icon);
	addch(' ');
	if (colorOn) {
		attroff(COLOR_PAIR(1));
	}
	return;
}
