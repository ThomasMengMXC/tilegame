#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <theatre/layer.h>
#include <theatre/colour.h>

#include "map.h"	// it's own .h file
#include "button.h"
#include "hover.h"

// initialises the map object and
Map *init_map(void) {
	Map *map = malloc(sizeof(Map));
	map->yLength = 25; // TEMPORARY VALUE
	map->xLength = 40; // TEMPORARY VALUE
	map->grid = init_grid(map->yLength, map->xLength);
	return map;
}

void free_map(Map *map) {
	free_grid(map->grid, map->yLength, map->xLength);
	free(map);
}

Tile **init_grid(unsigned short yLength, unsigned short xLength) {
	Tile *tile = NULL;
	Tile **grid = malloc(sizeof(Tile *) * yLength);
	for (int y = 0; y < yLength; y++) {
		grid[y] = malloc(sizeof(Tile) * xLength);
		for (int x = 0; x < xLength; x++) {
			tile = &(grid[y][x]);
			tile->icon = ". ";
			tile->r = 0; tile->g = 135; tile->b = 0;
			tile->mvCost = 1;
			tile->unit = NULL;
			tile->yPos = y; tile->xPos = x;
		}
	}
	return grid;
}

void free_grid(Tile **grid, unsigned short yLength, unsigned short xLength) {
	for (int y = 0; y < yLength; y++) {
		free(grid[y]);
	}
	free(grid);
}

void map_draw(Map *map, Layer *layer) {
	for (int y = 0; y < map->yLength; y++) {
		for (int x = 0; x < map->xLength; x++) {
			Tile *tile = &(map->grid[y][x]);
			add_icon_to_layer(layer, y, x, tile->icon, strlen(tile->icon));
			add_colour_to_layer(layer, y, x, (Colour) {
					.r = tile->r,
					.g = tile->g,
					.b = tile->b,
					.a = 255});
			add_hover_to_layer(layer, y, x, map_hover);
			if (map->grid[y][x].unit) {
				add_icon_to_layer(layer, y, x,
						tile->unit->icon, strlen(tile->unit->icon));
				add_button_to_layer(layer, y, x, unit_button);
			}
		}
	}
}

void add_team_to_map(Map *map, Team *team) {
	for (int i = 0; i < team->depth ; i++ ) {
		unsigned int yPos = 0;
		unsigned int xPos = i;
		map->grid[yPos][xPos].unit = team->unit[i];
	}
}
