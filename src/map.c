#include <stdlib.h>
#include <limits.h>
#include <theatre/layer.h>

#include "map.h"	// it's own .h file
#include "button.h"
#include "hover.h"

// initialises the map object and
Map *init_map(void) {
	Map *map = malloc(sizeof(Map));
	Tile *tile = NULL;
	map->yLength = 25; // TEMPORARY VALUE
	map->xLength = 40; // TEMPORARY VALUE
	map->grid = malloc(sizeof(Tile *) * map->yLength);
	for (int y = 0; y < map->yLength; y++) {
		map->grid[y] = malloc(sizeof(Tile) * map->xLength);
		for (int x = 0; x < map->xLength; x++) {
			tile = &(map->grid[y][x]);

			tile->icon = ". ";
			tile->r = 0; tile->g = 135; tile->b = 0;
			tile->mvCost = 1;
			tile->unit = NULL;
			tile->yPos = y; tile->xPos = x;
		}
	}
	return map;
}

void free_map(Map *map) {
	for (int y = 0; y < map->yLength; y++) {
		free(map->grid[y]);
	}
	free(map->grid);
	free(map);
	return;
}

void map_draw(Map *map, Layer *layer) {
	for (int y = 0; y < map->yLength; y++) {
		for (int x = 0; x < map->xLength; x++) {
			Tile *tile = &(map->grid[y][x]);
			add_icon_to_layer(layer, y, x, tile->icon);
			add_colour_to_layer(layer, y, x, 256, 
					tile->r, tile->g, tile->b);
			add_hover_to_layer(layer, y, x, map_hover);
			if (map->grid[y][x].unit) {
				add_icon_to_layer(layer, y, x, tile->unit->icon);
				add_button_to_layer(layer, y, x, unit_button);
			}
		}
	}
	return;
}

void add_team_to_map(Map *map, Team *team) {
	for (int i = 0; i < team->depth ; i++ ) {
		unsigned int yPos = 0;
		unsigned int xPos = i;
		map->grid[yPos][xPos].unit = team->unit[i];
	}
	return;
}
