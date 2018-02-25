#ifndef MAP_H
#define MAP_H

#include <ncurses.h>
#include "cursor.h"
#include "unit.h"
#include "tile.h"
#include "layer.h"

typedef struct {
	Tile **grid; // First dim is y, second dim is x
	unsigned int yLength, xLength;
	Layer *mapLayer; // the map layer
	Layer *rangeLayer; // the range layer
} Map;

Map *init_map(void);
void free_map(Map *map);
void map_draw(Map *map);

void add_units_to_map(Map *map, Team *team);

void init_move_grids(Team *team, Map *map);
void free_move_grid(Team *team, Map *map);

void find_range(Team *team, Map *map);
void flood_fill(int y, int x, int move, Unit *unit, Map *map);

void draw_range(Unit *unit, Map *map);
void undraw_range(Unit *unit, Map *map);

void update_cursor(Map *map, Cursor *cursor);

#endif
