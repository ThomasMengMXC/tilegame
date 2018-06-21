#include <stdlib.h>
#include <string.h>
#include "map.h"

Map *init_map(unsigned yLength, unsigned xLength) {
	Map *map = malloc(sizeof(Map));
	map->yLength = yLength;
	map->xLength = xLength;
	map->grid = init_grid(yLength, xLength);
	return map;
}

void free_map(Map *map) {
	free_grid(map->grid, map->yLength, map->xLength);
	free(map);
}

Tile **init_grid(unsigned yLength, unsigned xLength) {
	Tile *tile;
	Tile **grid = malloc(sizeof(Tile *) * yLength);
	for (int y = 0; y < yLength; ++y) {
		grid[y] = malloc(sizeof(Tile) * xLength);
		for (int x = 0; x < xLength; ++x) {
			tile = &grid[y][x];
			strncpy(tile->icon, ". ", 3);
			tile->col = (Colour) {.r = 0, .g = 0, .b = 0, .a = 0};
			tile->mvCost = 1;
			tile->pos = (Vector2D) {.y = y, .x = x};
			tile->unit = NULL;
		}
	}
	return grid;
}

void free_grid(Tile **grid, unsigned yLength, unsigned xLength) {
	for (int y = 0; y < yLength; y++) {
		free(grid[y]);
	}
	free(grid);
}
