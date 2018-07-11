#ifndef MAP_H
#define MAP_H

extern "C" {
#include <theatre/layer.h>
}
#include <cstdint>
#include <map>
#include "tile.h"

typedef std::map<unsigned, Unit *> Faction;

class Map {
	public:
		unsigned yLength, xLength;
		Map(const char *filename);
		~Map(void);

		void draw(Layer *layer);

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

		bool place_unit(Unit *unit, std::pair<unsigned, unsigned> pos);
		bool remove_unit(std::pair<unsigned, unsigned> pos);

		short **init_move_grid(void);
		void free_move_grid(short **grid);
		void unit_range(Unit *unit, short **range);
		std::pair<unsigned, unsigned>
				closest_without_unit(std::pair<unsigned, unsigned> pos);
};

#endif
