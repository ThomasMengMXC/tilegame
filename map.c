#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <limits.h>
#include "map.h"
#include "game.h"

void map_init(MapData *map) {
	map->height = 25; // TEMPORARY VALUE
	map->width = 40; // TEMPORARY VALUE
	map->grid = malloc(sizeof(Tile *) * map->height);
	for (int i = 0; i < map->height; i++) {
		map->grid[i] = malloc(sizeof(Tile) * map->width);
		for (int j = 0; j < map->width; j++) {
			map->grid[i][j].tileType = '.';
			map->grid[i][j].icon = map->grid[i][j].tileType;
			map->grid[i][j].mvCost = 1;
			map->grid[i][j].unit = NULL;
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
	if (tile.unit) {
		mvprintw(tile.yPos, tile.xPos * 2, tile.unit->icon);
	} else {
		if (colorOn) {
			attron(COLOR_PAIR(1));
		}
		mvaddch(tile.yPos, tile.xPos * 2, tile.icon);
		addch(' ');
		if (colorOn) {
			attroff(COLOR_PAIR(1));
		}
	}
	return;
}

void add_units_to_map(MapData *map, Game *game) {
	for (int i = 0; i < game->playerCnt; i++) {
		map->grid[game->units[i].yPos][game->units[i].xPos].unit =
			game->units + i;
	}
	return;
}

void init_move_grids(Team *team, MapData *map) {
	for (int i = 0; i < *team->playerCnt; i++) {
		(*team->units)[i].moveGrid = malloc(map->height * sizeof(Tile *));
		for (int j = 0; j < map->height; j++) {
			(*team->units)[i].moveGrid[j]
				= malloc(map->width * sizeof(Tile));
			for (int k = 0; k < map->width; k++) {
				(*team->units)[i].moveGrid[j][k] = INT_MIN;
			}
		}
	}
	return;
}

void free_move_grid(Team *team, MapData *map) {
	for (int i = 0; i < *team->playerCnt; i++) {
		for (int j = 0; j < map->height; j++) {
			free((*team->units)[i].moveGrid[j]);
		}
		free((*team->units)[i].moveGrid);
	}
	return;
}


void find_range(Team *team, MapData *map) {
	for (int i = 0; i < *team->playerCnt; i++) {
		flood_fill((*team->units)[i].xPos, (*team->units)[i].yPos, 
				(*team->units)[i].move, *team->units + i, map);
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
	for(int y = 0; y < map->height; y++) {
		for (int x = 0; x < map->width; x++) {
			if (unit->moveGrid[y][x] != INT_MIN){
				if (map->grid[y][x].unit) {
					mvprintw(y, 2 * x, map->grid[y][x].unit->icon);
				} else {
					attron(COLOR_PAIR(2));
					mvaddch(y, 2 * x, map->grid[y][x].icon);
					addch(' ');
					attroff(COLOR_PAIR(2));
				}
			}
		}
	}
	refresh();
	return;
}
