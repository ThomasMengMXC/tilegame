#ifndef MAP_H
#define MAP_H

extern "C" {
#include <theatre/layer.h> // so that the map can be drawn onto a layer
}
#include <map> // for the maps from ID to whatever
#include "tile.h" // the map has a grid of tiles

// Faction is just a map of unitIDs to units.
typedef std::map<unsigned, Unit *> Faction;

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
		unsigned unitIDs; // keeps track of the used unitIDs
		unsigned factionIDs; // keeps track of the used factionIDs
		Tile **grid; // the grid
		std::map<unsigned, short **> moveGrids; // unitID -> moveGrid
		std::map<unsigned, Unit *> units; // unitID -> unit
		std::map<unsigned, Faction> factions; // factionID -> faction

		Layer *layer; // Layer for drawing on

		bool place_unit(Unit *unit, std::pair<unsigned, unsigned> pos);
		bool remove_unit(std::pair<unsigned, unsigned> pos);

		short **init_move_grid(void);
		void free_move_grid(short **grid);
		void unit_range(unsigned unitID);
		std::pair<unsigned, unsigned>
				closest_without_unit(std::pair<unsigned, unsigned> pos);
};

#endif
