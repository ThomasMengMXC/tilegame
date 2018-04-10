#ifndef MAP_H
#define MAP_H

#include "team.h" // Adding team to map
#include "tile.h" // the tile grid

typedef struct {
	Tile **grid; // First dim is y, second dim is x
	unsigned short yLength, xLength;
} Map;

Map *init_map(void);
void free_map(Map *map);

Tile **init_grid(unsigned short yLength, unsigned short xLength);
void free_grid(Tile **grid, unsigned short yLength, unsigned short xLength);

void map_draw(Map *map, Layer *layer);

void add_team_to_map(Map *map, Team *team);

#endif
