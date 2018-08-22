extern "C" {
#include <curses.h> // import curses for the attr_t flags
}
#include <climits>
#include <luajit-2.0/lua.hpp> // for Lua
#include <cstring> // for strncpy
#include <queue> // for the queues for the pathfinding
#include "map.h" // it's own .h file

// these are static and therefore local to the this file
static void free_grid(Tile **grid, unsigned yLength, unsigned xLength);
static Tile *init_row(lua_State *L, int index, unsigned rowNum);

/*
 * This function creates a map from a lua file.
 * If any part of the map is wrong, the function will return NULL.
 */
Map::Map(const char *filename) {
	unsigned yLength = 0, xLength = 0;
	this->unitIDs = 0;
	this->factionIDs = 0;
	this->layer = NULL;
	lua_State *L = luaL_newstate();
	if (luaL_dofile(L, filename)) // load the file into the lua state
		goto error;
	lua_getfield(L, 1, "map");
	if (lua_type(L, -1) != LUA_TTABLE)
		goto error; // fails if the map is not a table
	lua_getfield(L, 1, "tiles");
	if (lua_type(L, -1) != LUA_TTABLE)
		goto error;
	if ((yLength = lua_objlen(L, 2)) <= 0)
		goto error; // fails if the length of the map is not > 0

	this->grid = new Tile*[yLength]();
	for (unsigned i = 0; i < yLength; i++) { // iterate over the rows
		lua_pushinteger(L, i + 1);
		lua_gettable(L, 2);
		if (lua_type(L, -1) != LUA_TSTRING)
			goto maperror; // fails if the type is not a string
		if (i == 0)
		 	// sets the xLength as the length of the first string
			xLength = lua_objlen(L, -1);
		else if (xLength != lua_objlen(L, -1)) // is this consistent?
			goto maperror;
		// load the rows via the tile descriptions, fail if it failed
		if ((this->grid[i] = init_row(L, 3, i)) == NULL)
			goto maperror;
		lua_pop(L, 1); // pop the string off the stack.
	}
	lua_pop(L, 1); // pop the map table off the stack
	this->yLength = yLength;
	this->xLength = xLength;
	goto out;
maperror:
	fprintf(stderr, "Failed in init_map(%s): maperror\n", filename);
	for (unsigned i = 0; i < yLength; i++) {
		if (this->grid[i])
			delete[] this->grid[i];
	}
	delete[] this->grid;
error: // clean up map and set it to be null to be returned
	fprintf(stderr, "Failed in init_map(%s): error\n", filename);
out:
	lua_close(L);
}

/*
 * Destructs the map.
 */
Map::~Map(void) {
	free_grid(this->grid, this->yLength, this->xLength);
	// iterate through the list of units and delete them all
	for (auto unitPair: this->units) {
		delete unitPair.second;
	}
	// iterate through the list of movegrids and delete them all
	for (auto grids: this->moveGrids) {
		this->free_move_grid(grids.second);
	}
}

/*
 * Draws the map to a layer
 */
void Map::draw(void) {
	this->draw(layer);
}

/*
 * Non-void version
 */
void Map::draw(Layer *layer) {
	if (layer != NULL)
		this->layer = layer;
	else
		return;

	// clears the layer so that it will be empty when we draw on it
	clear_layer(layer);
	// iterate through every tile
	for (unsigned y = 0; y < this->yLength; y++) {
		for (unsigned x = 0; x < this->xLength; x++) {
			Tile *tile = &(this->grid[y][x]);
			// add the tile's icon to the layer at y and x
			add_icon_to_layer(layer, y, x, tile->icon, 2);
			// add the tile's colour to the layer at y and x
			add_colour_to_layer(layer, y, x, tile->colour);
			// this is commented out since I haven't gotten around to
			// writing the hover function yet
			//add_hover_to_layer(layer, y, x, this_hover);
			
			// if the tile has a unit on it, then do something
			if (tile->unit) {
				// add the unit's icon in addition to the tile's icon
				add_icon_to_layer(layer, y, x, tile->unit->icon, 2);
				// this is commented out for the same reason
				//add_button_to_layer(layer, y, x, unit_button);
			}
			// if the eventID is not 0
			if (tile->eventID) {
				// add the event's colour on top of the tile
				add_colour_to_layer(layer, y, x, tile->eventColour);
				// add the blinking attribute to the tile
				add_attr_to_layer(layer, y, x, A_BLINK);
			}
		}
	}
	Colour col;
	col.r = 0;
	col.g = 0;
	col.b = 255;
	col.a = 100;
	for (auto pair: this->units) {
		short **grid = this->moveGrids[pair.second->unitID];
		for (unsigned y = 0; y < this->yLength; y++) {
			for (unsigned x = 0; x < this->xLength; x++) {
				if (grid[y][x] > INT16_MIN) {
					add_colour_to_layer(layer, y, x, col);
				}
			}
		}
	}
}

/*
 * Just returns a new factionID
 */
int Map::new_faction(void) {
	this->factionIDs++;
	return this->factionIDs - 1;
}

/*
 * Adds a unit to a faction
 */
int Map::add_unit(unsigned factionID, Unit *unit, unsigned y, unsigned x) {
	std::pair<unsigned, unsigned> pos; // the position to place the unit
	// check if the position of the unit is out of bounds or not
	if (	y < this->yLength && y >= 0 &&
			x < this->xLength && x >= 0) {
		// set the unit's factionID and unitID
		unit->factionID = factionID;
		unit->unitID = this->unitIDs;
		pos.first = y;
		pos.second = x;
		// add the unit to the list of all units.
		this->units[unit->unitID] = unit;
		// add the unit to the faction's list of units
		this->factions[factionID][unit->unitID] = unit;
		// place the unit at the position
		this->place_unit(unit, pos);

		// increment the unitID so that the next unit will have a diff ID
		this->unitIDs++;
		return unit->unitID - 1;
	} else {
		// says something failed
		return -1;
	}
}

/*
 * Changes the faction of the unit
 */
void Map::change_faction(unsigned unitID, unsigned factionID) {
	// if the unit or faction does not exist, return
	if (this->units.count(unitID) == 0
			|| this->factions.count(factionID) == 0)
		return;
	// get the unit
	Unit *unit = this->units[unitID];
	// remove unit from old faction
	this->factions[unit->factionID].erase(unitID);
	// change the unit's factionID
	this->units[unitID]->factionID = factionID;
	// add the unit to the faction's list of units
	this->factions[unit->factionID][unitID] = unit;
}

/*
 * Getter for the size of a faction
 */
unsigned Map::faction_size(unsigned factionID) {
	return this->factions[factionID].size();
}

/*
 * Moves the unit from one tile to another.
 * If the tile is occupied, then returns false
 */
bool Map::move_unit(unsigned yOld, unsigned xOld,
		unsigned yNew, unsigned xNew) {
	// the original and destination coordinates
	std::pair<unsigned, unsigned> origTmp, destTmp;
	origTmp.first = yOld; origTmp.second = xOld;
	destTmp.first = yNew; destTmp.second = xNew;
	// check if the grid already has a unit on it
	if (this->grid[destTmp.first][destTmp.second].unit)
		return false;
	Unit *unit = this->grid[origTmp.first][origTmp.second].unit;
	// remove unit from the old tile
	this->grid[origTmp.first][origTmp.second].unit = NULL;

	// set the tile to hold unit and vice versa
	this->grid[destTmp.first][destTmp.second].unit = unit;
	unit->tile = &(this->grid[destTmp.first][destTmp.second]);
	return true;
}

/*
 * Adds an event tile to the map
 */
void Map::event_tile(unsigned y, unsigned x,
		const char *name, Colour col, int eventID) {
	// checks if y and x are in bounds
	if (	y < this->yLength && y >= 0 &&
			x < this->xLength && x >= 0) {
		Tile *tile = &(this->grid[y][x]);
		tile->eventID = eventID;
		tile->eventColour = col;
		// copy the name of the event
		strncpy(tile->eventName, name, sizeof(tile->eventName) - 1);
		tile->eventName[sizeof(tile->eventName) - 1] = '\0';
	}
}
// private ---------------------

/* 
 * Places the unit onto position pos or a nearby tile. Returns false
 * if it could not place it on pos.
 */
bool Map::place_unit(Unit *unit, std::pair<unsigned, unsigned> pos) {
	// get the closest tile without a unit
	auto tmp = this->closest_without_unit(pos);
	// if the unit doesn't have a movegrid, then make one
	if (this->moveGrids.count(unit->unitID) == 0) {
		this->moveGrids[unit->unitID] = init_move_grid();
	}
	// place unit on the tile
	this->grid[tmp.first][tmp.second].unit = unit;
	// tell the unit what tile it's on
	unit->tile = &(this->grid[tmp.first][tmp.second]);
	this->unit_range(unit->unitID);
	// checks if the pos is the original
	if (tmp.first != pos.first || tmp.second != pos.second)
		return false;
	else
		return true;
}

/*
 * Removes the unit from the grid
 */
bool Map::remove_unit(std::pair<unsigned, unsigned> pos) {
	Tile *tile = &(this->grid[pos.first][pos.second]);
	// checks if the tile actually has a unit on it
	if (tile->unit) {
		Unit *unit = tile->unit;
		// remove unit from its faction
		this->factions[unit->factionID].erase(unit->unitID);
		// change the unit's factionID
		this->units.erase(unit->unitID);
		unit->tile = NULL;
		tile->unit = NULL;
		return true;
	} else {
		return false;
	}
}

/*
 * Creates a grid of shorts the same size as the map.
 */
short **Map::init_move_grid(void) {
	short **grid = new short *[this->yLength];
	for (unsigned y = 0; y < this->yLength; y++) {
		grid[y] = new short[this->xLength];
		for (unsigned x = 0; x < this->xLength; x++) {
			grid[y][x] = INT16_MIN;
		}
	}
	return grid;
}

/*
 * Deletes aforementioned grid.
 */
void Map::free_move_grid(short **grid) {
	for (unsigned y = 0; y < this->yLength; y++) {
		delete[] grid[y];
	}
	delete[] grid;
}

/*
 * Sets the range grid to be the positions where the unit can move
 */
void Map::unit_range(unsigned unitID) {
	short **range = this->moveGrids[unitID];
	Unit *unit = this->units[unitID];
	// set everything in range to INT16_MIN
	for (unsigned y = 0; y < this->yLength; y++) {
		for (unsigned x = 0; x < this->xLength; x++) {
			range[y][x] = INT16_MIN;
		}
	}
	// co-oridinates
	std::pair<unsigned, unsigned> pos, tmp;
	// the amount of tiles that can be moved
	short moveRange;
	// temporary definition
#define mv_pos std::pair<short, std::pair<unsigned, unsigned>>
	// a pair of moveRanges and coordinates
	std::queue<mv_pos> posQueue;

	posQueue.push(mv_pos(unit->move, unit->tile->pos));
	while (!posQueue.empty()) {
		auto tmp2 = posQueue.front();
		moveRange = tmp2.first;
		pos = tmp2.second;
		posQueue.pop();
		if (range[pos.first][pos.second] < moveRange)
			range[pos.first][pos.second] = moveRange;
		if (moveRange < 1)
			continue;
		for (unsigned i = 0; i < 4; i++) {
			tmp.first = pos.first + ((i + 0) % 2) * (i >> 1 ? +1 : -1);
			tmp.second = pos.second + ((i + 1) % 2) * (i >> 1 ? +1 : -1);
			if (tmp.first >= 0 && tmp.first < this->yLength &&
					tmp.second >= 0 && tmp.second < this->xLength) {
				if (range[tmp.first][tmp.second] < moveRange - 1) {
					// TEMPORARY, MAKE THIS DO SOMETHING
					posQueue.push(mv_pos(moveRange - 1, tmp));
				}
			}
		}
	}
#undef mv_pos
}

std::pair<unsigned, unsigned>
		Map::closest_without_unit(std::pair<unsigned, unsigned> pos) {
	std::pair<unsigned, unsigned> tmp0, tmp1;
	short **grid = init_move_grid(); // this isn't the intended use :^)
	std::queue<std::pair<unsigned, unsigned>> queue;
	queue.push(pos);
	while (!queue.empty()) {
		tmp0 = queue.front();
		queue.pop();
		if (tmp0.first >= this->yLength || tmp0.second >= this->xLength)
			continue;
		if (this->grid[tmp0.first][tmp0.second].unit == NULL) {
			pos = tmp0;
			break;
		}
		for (unsigned i = 0; i < 4; i++) {
			tmp1.first = tmp0.first + ((i + 0) % 2) * (i >> 1 ? +1 : -1);
			tmp1.second = tmp0.second + ((i + 1) % 2) * (i >> 1 ? +1 : -1);
			if (tmp1.first >= 0 && tmp1.first < this->yLength &&
					tmp1.second >= 0 && tmp1.second < this->xLength
					&& grid[tmp1.first][tmp1.second] != 1) {
				grid[tmp1.first][tmp1.second] = 1;
				queue.push(tmp1);
			}
		}
	}
	free_move_grid(grid);
	return pos;
}


// STATIC ------------------------
static void free_grid(Tile **grid, unsigned yLength, unsigned xLength) {
	for (unsigned y = 0; y < yLength; y++) {
		delete[] grid[y];
	}
	delete[] grid;
}

static Tile *init_row(lua_State *L, int index, unsigned rowNum) {
	unsigned length = lua_objlen(L, -1);
	const char *string = lua_tostring(L, -1);
	char subbuff[2] = {0}; // for the substring key
	Tile *row = new Tile[length]();
	for (unsigned i = 0; i < length; i++) {
		subbuff[0] = string[i];
		lua_pushstring(L, subbuff);
		lua_gettable(L, index);
		if (lua_type(L, -1) != LUA_TTABLE)
			goto error;

		lua_getfield(L, -1, "icon");
		if (lua_type(L, -1) != LUA_TSTRING)
			goto error;
		memcpy(row[i].icon, lua_tostring(L, -1), 2);
		row[i].icon[2] = '\0';
		lua_pop(L, 1);

		lua_getfield(L, -1, "r");
		if (lua_type(L, -1) != LUA_TNUMBER)
			goto error;
		row[i].colour.r = lua_tointeger(L, -1);
		lua_pop(L, 1);

		lua_getfield(L, -1, "g");
		if (lua_type(L, -1) != LUA_TNUMBER)
			goto error;
		row[i].colour.g = lua_tointeger(L, -1);
		lua_pop(L, 1);

		lua_getfield(L, -1, "b");
		if (lua_type(L, -1) != LUA_TNUMBER)
			goto error;
		row[i].colour.b = lua_tointeger(L, -1);
		lua_pop(L, 1);

		lua_getfield(L, -1, "a");
		if (lua_type(L, -1) != LUA_TNUMBER)
			goto error;
		row[i].colour.a = lua_tointeger(L, -1);
		lua_pop(L, 1);

		lua_getfield(L, -1, "tileType");
		if (lua_type(L, -1) != LUA_TNUMBER)
			goto error;
		row[i].type = lua_tointeger(L, -1);
		lua_pop(L, 1);

		lua_pop(L, 1);
		row[i].pos.first = rowNum;
		row[i].pos.second = i;
		row[i].unit = NULL;
		row[i].eventColour = {0};
		row[i].eventID = 0;
	}
	goto out;
error:
	fprintf(stderr, "Failed in init_row(%s)\n", string);
	lua_pop(L, 1); // get rid of the nil value
	delete[] row;
	row = NULL;
out:
	return row;
}
