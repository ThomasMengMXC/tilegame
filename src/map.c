#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#include <theatre/layer.h>
#include "map.h"	// it's own .h file
#include "button.h"

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

void map_draw(Map *map) {
	for (int y = 0; y < map->yLength; y++) {
		for (int x = 0; x < map->xLength; x++) {
			Tile *tile = &(map->grid[y][x]);
			add_icon_to_layer(map->mapLayer, y, x, tile->icon);
			add_colour_to_layer(map->mapLayer, y, x, 256, 
					tile->r, tile->g, tile->b);
			if (map->grid[y][x].unit) {
				add_icon_to_layer(map->mapLayer, y, x, tile->unit->icon);
				add_button_to_layer(map->mapLayer, y, x, unit_button);
			}
		}
	}
	return;
}

void add_team_to_map(Map *map, Team *team) {
	for (int i = 0; i < team->depth ; i++ ) {
		unsigned int yPos = team->unit[i]->yPos;
		unsigned int xPos = team->unit[i]->xPos;
		map->grid[yPos][xPos].unit = team->unit[i];
	}
	return;
}
void update_cursor(Map *map, Cursor *cursor) {
	Tile *tile = &(map->grid[cursor->yPos][cursor->xPos]);
	//Tile *tileOld = &(map->grid[cursor->yOld][cursor->xOld]);
	if (cursor->icon) {
		add_icon_to_layer(map->rangeLayer, tile->yPos, tile->xPos,
				cursor->icon);
	}
	if (cursor->yOld != cursor->yPos || cursor->xOld != cursor->xPos) {
		remove_colour_from_layer(map->rangeLayer,
				cursor->yOld, cursor->xOld);
		if (cursor->icon) {
			remove_icon_from_layer(map->rangeLayer, cursor->yOld,
					cursor->xOld);
		}
		//undraw_range(tileOld->unit, map);
		cursor->yOld = cursor->yPos;
		cursor->xOld = cursor->xPos;
	}
	if (tile->unit) {
		//draw_range(tile->unit, map);
	}
	add_colour_to_layer(map->rangeLayer, tile->yPos, tile->xPos, 256,
			255, 255, 255);
	return;
}


