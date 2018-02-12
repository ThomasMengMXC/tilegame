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
MapData *init_map(void) {
	MapData *map = malloc(sizeof(MapData));
	Tile *tile = NULL;
	map->yLength = 25; // TEMPORARY VALUE
	map->xLength = 40; // TEMPORARY VALUE
	map->grid = malloc(sizeof(Tile *) * map->yLength);
	for (int i = 0; i < map->yLength; i++) {
		map->grid[i] = malloc(sizeof(Tile) * map->xLength);
		for (int j = 0; j < map->xLength; j++) {
			tile = &(map->grid[i][j]);
			tile->icon = ". ";
			tile->colour = 1;
			tile->mvCost = 1;
			tile->unit = NULL;
			tile->yPos = i;
			tile->xPos = j;
		}
	}
	return map;
}

void free_map(MapData *map) {
	for (int i = 0; i < map->yLength; i++) {
		free(map->grid[i]);
	}
	free(map->grid);
	free(map);
	return;
}

void map_draw(MapData *map) {
	for (int i = 0; i < map->yLength; i++) {
		for (int j = 0; j < map->xLength; j++) {
			tile_draw(&(map->grid[i][j]), true, true);
		}
	}
	refresh();
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

void free_move_grid(Team *team, MapData *map) {
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

void draw_range(Unit *unit, MapData *map) {
	Tile *tile = NULL;
	for(int y = 0; y < map->yLength; y++) {
		for (int x = 0; x < map->xLength; x++) {
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
		for(int y = 0; y < map->yLength; y++) {
			for (int x = 0; x < map->xLength; x++) {
				if (unit->moveGrid[y][x] != INT_MIN){
					tile = &(map->grid[y][x]);
					tile->colour = 1;
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
		mvprintw(tile->yPos, tile->xPos * 2, tile->unit->icon);
		attroff(COLOR_PAIR(2));
	} else {
		attron(COLOR_PAIR(2));
		mvprintw(tile->yPos, tile->xPos * 2, tile->icon);
		attroff(COLOR_PAIR(2));
	}
	refresh();
	return;
}
