#ifndef STAGE_H
#define STAGE_H

#include <stdint.h>
#include "tile.h"

typedef struct Map {
	unsigned yLength, xLength;
	Tile **grid;
} Map;

Map *init_map(unsigned yLength, unsigned xLength);
void free_map(Map *map);

Tile **init_grid(unsigned yLength, unsigned xLength);
void free_grid(Tile **grid, unsigned yLength, unsigned xLength);

#endif
