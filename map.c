#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <limits.h>
#include "game.h"
#include "map.h"
#include "cursor.h"
#include "unit.h"
#include "tile.h"

// initialises the map object and
MapData *map_init(void) {
	MapData *map = malloc(sizeof(MapData));
	Tile *tile = NULL;
	map->height = 25; // TEMPORARY VALUE
	map->width = 40; // TEMPORARY VALUE
	map->grid = malloc(sizeof(Tile *) * map->height);
	for (int i = 0; i < map->height; i++) {
		map->grid[i] = malloc(sizeof(Tile) * map->width);
		for (int j = 0; j < map->width; j++) {
			tile = &(map->grid[i][j]);
			tile->icon = '.';
			tile->defaultColour = 1;
			tile->colour = tile->defaultColour;
			tile->mvCost = 1;
			tile->unit = NULL;
			tile->yPos = i;
			tile->xPos = j;
		}
	}
	return map;
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
			tile_draw(&(map->grid[i][j]), true, true);
		}
	}
	return;
}

void add_units_to_map(MapData *map, Team *team) {
	Node *current = team->firstNode;
	while (current != NULL) {
		map->grid[current->unit->yPos][current->unit->xPos].unit =
			current->unit;
		current = current->next;
	}
	return;
}

// Initialise the movement grids to default for all the units on a team
void init_move_grids(Team *team, MapData *map) {
	Node *current = team->firstNode;
	while (current != NULL) {
		current->unit->moveGrid = malloc(map->height * sizeof(Tile *));
		for (int j = 0; j < map->height; j++) {
			current->unit->moveGrid[j]
				= malloc(map->width * sizeof(Tile));
			for (int k = 0; k < map->width; k++) {
				current->unit->moveGrid[j][k] = INT_MIN;
			}
		}
		current = current->next;
	}
	return;
}

void free_move_grid(Team *team, MapData *map) {
	Node *current = team->firstNode;
	while (current != NULL) {
		for (int j = 0; j < map->height; j++) {
			free(current->unit->moveGrid[j]);
		}
		free(current->unit->moveGrid);
		current = current->next;
	}
	return;
}


void find_range(Team *team, MapData *map) {
	Node *current = team->firstNode;
	while (current != NULL) {
		flood_fill(current->unit->yPos, current->unit->xPos, 
				current->unit->move, current->unit, map);
		current = current->next;
	}
	return;
}

void flood_fill(int y, int x, int move, Unit *unit, MapData *map) {
	if (unit->moveGrid[y][x] < move) {
		unit->moveGrid[y][x] = move;
		if (move > 0) {
			if (y < map->height - 1) {
				flood_fill(y + 1, x,move - map->grid[y + 1][x].mvCost,
						unit, map);
			}
			if (x < map->width - 1) {
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

void draw_range(Unit *unit, MapData *map) {
	Tile *tile = NULL;
	for(int y = 0; y < map->height; y++) {
		for (int x = 0; x < map->width; x++) {
			if (unit->moveGrid[y][x] != INT_MIN){
				tile = &(map->grid[y][x]);
				tile->colour = 3;
				tile_draw(tile, true, false);
			}
		}
	}
	refresh();
	return;
}

void undraw_range(Unit *unit, MapData *map) {
	if (unit) {
		Tile *tile = NULL;
		for(int y = 0; y < map->height; y++) {
			for (int x = 0; x < map->width; x++) {
				if (unit->moveGrid[y][x] != INT_MIN){
					tile = &(map->grid[y][x]);
					tile->colour = tile->defaultColour;
					tile_draw(tile, true, false);
				}
			}
		}
		refresh();
	}
	return;
}

void update_cursor(MapData *map, CursorData *cursor) {
	Tile *tile = &(map->grid[cursor->yPos][cursor->xPos]);
	Tile *tileOld = &(map->grid[cursor->yOld][cursor->xOld]);
	if (cursor->yOld != cursor->yPos || cursor->xOld != cursor->xPos) {
		tile_draw(tileOld, true, false);
		undraw_range(tileOld->unit, map);
		cursor->yOld = cursor->yPos;
		cursor->xOld = cursor->xPos;
	}
	if (tile->unit) {
		draw_range(tile->unit, map);
		attron(COLOR_PAIR(2));
		mvprintw(tile->yPos, 2 * tile->xPos, tile->unit->icon);
		attroff(COLOR_PAIR(2));
	} else {
		attron(COLOR_PAIR(2));
		mvaddch(tile->yPos, 2 * tile->xPos, tile->icon);
		addch(' ');
		attroff(COLOR_PAIR(2));
	}

	return;
}
