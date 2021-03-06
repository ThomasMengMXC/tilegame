#ifndef MAP_H
#define MAP_H

extern "C" {
#include <theatre/layer.h> // so that the map can be drawn onto a layer
}
#include <vector> // for the maps from ID to whatever
#include "tile.h" // the map has a grid of tiles

/*
 * Map is a class which holds all the information about things which appear
 * on the map.
 * todo, put some of the parts into overmap. Factions probably fit better in
 * overmap rather than the map. change y/xLength to private and create getter
 * functions. 
 */
class Map {
	public:
		unsigned yLength, xLength;
		Map(const char *filename);
		~Map(void);

		void draw(Layer *layer);
		void draw(void);

		int new_faction(void);

		int add_unit(unsigned factionID, Unit *unit, unsigned y, unsigned x);
		void change_faction(unsigned unitID, unsigned factionID);
		unsigned faction_size(unsigned factionID);

		bool move_unit(unsigned yOld, unsigned xOld,
				unsigned yNew, unsigned xNew);

		void event_tile(unsigned y, unsigned x,
				const char *name, Colour col, int eventID);
	private:
		Tile **grid; // the grid
		std::vector<Unit *> units; // unitID -> unit
		std::vector<std::vector<Unit *>> factions; // factionID -> faction
		Layer *layer;

		bool place_unit(Unit *unit, Coordinate pos);
};

#endif
