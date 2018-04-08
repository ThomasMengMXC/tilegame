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

void update_cursor(Map *map, Cursor *cursor);

#endif
