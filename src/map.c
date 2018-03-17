#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <limits.h>
#include <theatre/layer.h>

#include "map.h"	// it's own .h file
#include "cursor.h"	// for update cursor's arguments
#include "unit.h"	// for flood fill's arguments's arguments
#include "tile.h"	// for creating tiles
#include "button.h" // for the unit buttons

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
			tile->colour = GRASS;
			tile->mvCost = 1;
			tile->unit = NULL;
			tile->yPos = y;
			tile->xPos = x;
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
			add_colour_to_layer(map->mapLayer, y, x, tile->colour);
			if (map->grid[y][x].unit) {
				add_icon_to_layer(map->mapLayer, y, x, tile->unit->icon);
				add_button_to_layer(map->mapLayer, y, x, unit_button);
			}
		}
	}
	return;
}

void add_units_to_map(Map *map, Team *team) {
	Node *current = team->firstNode;
	while (current != NULL) {
		unsigned int yPos = current->unit->yPos;
		unsigned int xPos = current->unit->xPos;
		map->grid[yPos][xPos].unit = current->unit;	
		current = current->next;
	}
	return;
}

// Initialise the movement grids to default for all the units on a team
void init_move_grids(Team *team, Map *map) {
	Node *current = team->firstNode;
	while (current != NULL) {
		current->unit->moveGrid = malloc(map->yLength * sizeof(Tile *));
		for (int j = 0; j < map->yLength; j++) {
			current->unit->moveGrid[j]
				= malloc(map->xLength * sizeof(Tile));
			for (int k = 0; k < map->xLength; k++) {
				current->unit->moveGrid[j][k] = INT_MIN;
			}
		}
		current = current->next;
	}
	return;
}

void free_move_grid(Team *team, Map *map) {
	Node *current = team->firstNode;
	while (current != NULL) {
		for (int j = 0; j < map->yLength; j++) {
			free(current->unit->moveGrid[j]);
		}
		free(current->unit->moveGrid);
		current = current->next;
	}
	return;
}


void find_range(Team *team, Map *map) {
	Node *current = team->firstNode;
	while (current != NULL) {
		flood_fill(current->unit->yPos, current->unit->xPos, 
				current->unit->move, current->unit, map);
		current = current->next;
	}
	return;
}

void flood_fill(short y, short x, int move, Unit *unit, Map *map) {
	if (unit->moveGrid[y][x] < move) {
		unit->moveGrid[y][x] = move;
		if (move > 0) {
			if (y < map->yLength - 1) {
				flood_fill(y + 1, x,move - map->grid[y + 1][x].mvCost,
						unit, map);
			}
			if (x < map->xLength - 1) {
				flood_fill(y, x+1, move - map->grid[y][x+1].mvCost,
						unit, map);
			}
			if (y > 0) {
				flood_fill(y-1, x, move - map->grid[y-1][x].mvCost,
						unit, map);
			}
			if (x > 0) {
				flood_fill(y, x-1, move - map->grid[y][x-1].mvCost,
						unit, map);
			}
		}
	}
}

void draw_range(Unit *unit, Map *map) {
	for(int y = 0; y < map->yLength; y++) {
		for (int x = 0; x < map->xLength; x++) {
			if (unit->moveGrid[y][x] != INT_MIN){
				add_colour_to_layer(map->rangeLayer, y, x, CURSOR);
			}
		}
	}
	return;
}

void undraw_range(Unit *unit, Map *map) {
	if (unit) {
		for(int y = 0; y < map->yLength; y++) {
			for (int x = 0; x < map->xLength; x++) {
				if (unit->moveGrid[y][x] != INT_MIN){
					remove_colour_from_layer(map->rangeLayer, y, x);
				}
			}
		}
	}
	return;
}

void update_cursor(Map *map, Cursor *cursor) {
	Tile *tile = &(map->grid[cursor->yPos][cursor->xPos]);
	Tile *tileOld = &(map->grid[cursor->yOld][cursor->xOld]);
	if (cursor->icon) {
		add_icon_to_layer(map->rangeLayer, tile->yPos, tile->xPos, cursor->icon);
	}
	if (cursor->yOld != cursor->yPos || cursor->xOld != cursor->xPos) {
		remove_colour_from_layer(map->rangeLayer, cursor->yOld, cursor->xOld);
		if (cursor->icon) {
			remove_icon_from_layer(map->rangeLayer, cursor->yOld, cursor->xOld);
		}
		undraw_range(tileOld->unit, map);
		cursor->yOld = cursor->yPos;
		cursor->xOld = cursor->xPos;
	}
	if (tile->unit) {
		draw_range(tile->unit, map);
	}
	add_colour_to_layer(map->rangeLayer, tile->yPos, tile->xPos, MOVE_RANGE);
	return;
}
