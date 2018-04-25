#ifndef MAP_H
#define MAP_H
extern "C" {
#include <theatre/layer.h>
}

#include <vector>

#include "unit.h" // Adding team to map
#include "tile.h" // the tile grid

class Map {
	public:
		uint16_t yLength, xLength;
		Tile **grid;

		Map(void);
		~Map(void);
		void map_draw(Layer *layer);
		void add_team_to_map(std::vector<Unit *> team);

	private:
		Tile **init_grid(void);
		void free_grid(void);
};

#endif
