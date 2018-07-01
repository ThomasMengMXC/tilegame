#ifndef MAP_H
#define MAP_H

#include <stdint.h>
#include <theatre/layer.h>
#include "tile.h"

typedef struct Map {
	unsigned yLength, xLength;
	Tile **grid;
} Map;

Map *init_map(char *filename);
void free_map(Map *map);
void draw_map(Map *map, Layer *layer);

#endif
