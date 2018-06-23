#ifndef STAGE_H
#define STAGE_H

#include <stdint.h>
#include "tile.h"

typedef struct Map {
	unsigned yLength, xLength;
	Tile **grid;
} Map;

Map *init_map(char *filename);
void free_map(Map *map);

#endif
