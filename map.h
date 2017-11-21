#ifndef MAP_H
#define MAP_H

#include <ncurses.h>
#include "game.h"

typedef struct {
	char icon;
	char tileType;
	int mvCost;
	void *objects;
	unsigned int yPos, xPos;
} Tile;

typedef struct {
	Tile **grid;
	unsigned int width, height;
} MapData;

void map_init(MapData *map);
void map_free(MapData *map);
void map_draw(MapData *map);
void tile_draw(Tile tile, bool colorOn);

void draw_all_units(MapData *map, Game *game);
void unit_draw_to_map(MapData *map, Unit *unit);

void init_move_grids(Team *team, MapData *map);
void free_move_grid(Team *team, MapData *map);

void find_range(Team *team);
void flood_fill(int x, int y, int move, Unit *unit);

#endif
