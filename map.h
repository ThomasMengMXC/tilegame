#ifndef MAP_H
#define MAP_H

#include <ncurses.h>
#include "game.h"

typedef struct {
	char icon;
	char tileType;
	int mvCost;
	Unit *unit;
	unsigned int yPos, xPos;
} Tile;

typedef struct {
	Tile **grid; // First dim is y, second dim is x
	unsigned int width, height;
} MapData;

void map_init(MapData *map);
void map_free(MapData *map);
void map_draw(MapData *map);
void tile_draw(Tile tile, bool colorOn);

void add_units_to_map(MapData *map, Game *game);

void init_move_grids(Team *team, MapData *map);
void free_move_grid(Team *team, MapData *map);

void find_range(Team *team, MapData *map);
void flood_fill(int x, int y, int move, Unit *unit, MapData *map);
void draw_range(Unit *unit, MapData *map);

#endif
