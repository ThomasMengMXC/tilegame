#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
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

void draw_all_units(MapData *map, Game *game) {
	for (int i = 0; i < game->playerCnt; i++) {
		unit_draw_to_map(map, game->units + i);
	}
	return;
}

void unit_draw_to_map(MapData *map, Unit *unit) {
	mvprintw(unit->yPos, unit->xPos * 2, ":)");
	return;
}

void init_move_grids(Team *team, MapData *map) {
	for (int i = 0; i < *team->playerCnt; i++) {
		(*team->units)[i].moveGrid = calloc(map->height, sizeof(Tile *));
		for (int j = 0; j < map->height; j++) {
			(*team->units)[i].moveGrid[j]
				= calloc(map->width, sizeof(Tile));
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


void find_range(Team *team) {
	for (int i = 0; i < *team->playerCnt; i++) {
		flood_fill((*team->units)[i].xPos, (*team->units)[i].yPos, 
				(*team->units)[i].move, *team->units + i);
	}
	return;
}

void flood_fill(int x, int y, int move, Unit *unit) {
	if (x >= 0 && x < 10 && y >= 0 && y < 10
			&& unit->moveGrid[x][y] < move) {
		unit->moveGrid[x][y] = move;
		if (move > 0) {
			flood_fill(x + 1, y, move - 1, unit);
			flood_fill(x, y + 1, move - 1, unit);
			flood_fill(x - 1, y, move - 1, unit);
			flood_fill(x, y - 1, move - 1, unit);
		}
	}
}
