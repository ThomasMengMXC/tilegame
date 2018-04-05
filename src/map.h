#ifndef MAP_H
#define MAP_H

#include "team.h" // Adding team to map
#include "cursor.h" // Updating the cursor
#include "tile.h" // the tile grid

typedef struct {
	Tile **grid; // First dim is y, second dim is x
	unsigned short yLength, xLength;
	Layer *mapLayer; // the map layer
	Layer *rangeLayer; // the range layer
	
} Map;

Map *init_map(void);
void free_map(Map *map);
void map_draw(Map *map);

void add_team_to_map(Map *map, Team *team);

void init_move_grids(Team *team, Map *map);
void free_move_grid(Team *team, Map *map);

void find_range(Team *team, Map *map);
void flood_fill(short y, short x, int move, Unit *unit, Map *map);

void draw_range(Unit *unit, Map *map);
void undraw_range(Unit *unit, Map *map);

void update_cursor(Map *map, Cursor *cursor);

#endif
