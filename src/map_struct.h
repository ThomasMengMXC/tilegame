#ifndef MAP_STRUCT_H
#define MAP_STRUCT_H

#include <theatre/layer_struct.h>
#include "tile_struct.h"

typedef struct {
	Tile **grid; // First dim is y, second dim is x
	unsigned short yLength, xLength;
	Layer *mapLayer; // the map layer
	Layer *rangeLayer; // the range layer
	
} Map;

#endif
