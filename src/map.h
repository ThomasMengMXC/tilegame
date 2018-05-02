#ifndef MAP_H
#define MAP_H
extern "C" {
#include <theatre/layer.h>
}

#include <vector>
#include <map>

#include "unit.h" // Adding team to map
#include "tile.h" // the tile grid

class Map {
	public:
		Map(void);
		~Map(void);
		void draw(Layer *layer);

		// setters
		void place_team(std::vector<Unit *> *team);
		bool place_unit(Unit *unit, Vector2D pos);
		bool remove_unit(Unit *unit, Vector2D pos);
		bool move_unit(Vector2D original, Vector2D destination);

		void calculate_ranges(void);

		// getter
		Unit *unit_at(Vector2D pos);

		void draw_move_grid(Layer *layer, Unit *unit);
		void remove_move_grid(Layer *layer, Unit *unit);

	private:
		uint16_t yLength, xLength;
		Tile **grid;

		// key: unitID, object: movegrid
		std::map<Unit *, int16_t **> moveGrids;

		int16_t **init_move_grid(void);
		void free_move_grid(int16_t **grid);
		void unit_range(Unit *unit, int16_t **range);
		Vector2D closest_without_unit(Vector2D pos);
};

#endif
