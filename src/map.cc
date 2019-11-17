extern "C" {
#include <curses.h> // import curses for the attr_t flags
}
#include <climits>
#include <luajit-2.0/lua.hpp> // for Lua
#include <cstring> // for strncpy
#include <queue> // for the queues for the pathfinding
#include "map.h" // it's own .h file

// STATIC --------------------------------------------------------------------
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
		row[i].pos.y = rowNum;
		row[i].pos.x = i;
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
// END STATIC ----------------------------------------------------------------

/*
 * This function creates a map from a lua file.
 * If any part of the map is wrong, the function will return NULL.
 */
Map::Map(const char *filename) {
	unsigned yLength = 0, xLength = 0;
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
		 	// sets the xLength as the length of the y string
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
	for (auto unit: this->units) {
		delete unit;
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

			// if the tile has a unit on it, then do something
			if (tile->unit) {
				// add the unit's icon in addition to the tile's icon
				add_icon_to_layer(layer, y, x, tile->unit->icon, 2);
			}
		}
	}
}

/*
 * Just returns a new factionID
 */
int Map::new_faction(void) {
	this->factions.push_back(std::vector<Unit *>());
	return this->factions.size() - 1;
}

/*
 * Adds a unit to a faction
 */
int Map::add_unit(unsigned factionID, Unit *unit, unsigned y, unsigned x) {
	Coordinate pos; // the position to place the unit
	// check if the position of the unit is out of bounds or not
	if (	y < this->yLength && y >= 0 &&
			x < this->xLength && x >= 0) {
		// set the unit's factionID and unitID
		unit->factionID = factionID;
		unit->unitID = this->units.size();
		pos.y = y;
		pos.x = x;
		// place the unit at the position, returns if it failed.
		if (!this->place_unit(unit, pos)) {
			unit->unitID = 0;
			unit->factionID = 0;
			return -1;
		}
		// add the unit to the list of all units.
		this->units.push_back(unit);
		// add the unit to the faction's list of units
		this->factions[factionID].resize(unit->unitID + 1);
		this->factions[factionID][unit->unitID] = unit;

		return unit->unitID;
	} else {
		// says something failed
		fprintf(stderr, "wow\n");
		return -1;
	}
}

/*
 * Changes the faction of the unit
 */
void Map::change_faction(unsigned unitID, unsigned factionID) {
	// if the unit or faction does not exist, return
	if (!(this->units.size() > unitID)
			|| this->units[unitID] == NULL)
		return;
	if (!(this->factions.size() > factionID))
		return;
	// get the unit
	Unit *unit = this->units[unitID];
	// remove unit from old faction
	this->factions[unit->factionID][unitID] = NULL;
	// change the unit's factionID
	this->units[unitID]->factionID = factionID;
	// add the unit to the faction's list of units
	this->factions[unit->factionID][unitID] = unit;
}

/*
 * Getter for the size of a faction
 */
unsigned Map::faction_size(unsigned factionID) {
	if (this->factions.size() > factionID)
		return this->factions[factionID].size();
	else
		return 0;
}

/*
 * Moves the unit from one tile to another.
 * If the tile is occupied, then returns false
 */
bool Map::move_unit(unsigned yOld, unsigned xOld,
		unsigned yNew, unsigned xNew) {
	// the original and destination coordinates
	Coordinate origTmp, destTmp;
	origTmp.y= yOld; origTmp.x = xOld;
	destTmp.y = yNew; destTmp.x = xNew;
	// check if the grid already has a unit on it
	if (this->grid[destTmp.y][destTmp.x].unit)
		return false;
	Unit *unit = this->grid[origTmp.y][origTmp.x].unit;
	// remove unit from the old tile
	this->grid[origTmp.y][origTmp.x].unit = NULL;

	// set the tile to hold unit and vice versa
	this->place_unit(unit, destTmp);

	// ??? I dunno why I had these two lines
	//this->grid[destTmp.y][destTmp.x].unit = unit;
	//unit->tile = &(this->grid[destTmp.y][destTmp.x]);
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

// PRIVATE -------------------------------------------------------------------
/*
 * Places a unit onto the map
 * Eventually I should change this to check whether the unit can stand on the
 * tile but for now it only checks if another unit is on it.
 */
bool Map::place_unit(Unit *unit, Coordinate pos) {
	unsigned radius = 0;
	unsigned circumference = radius * 4;
	unsigned radPos = 0;
	if (	pos.y < this->yLength && pos.y >= 0 &&
			pos.x < this->xLength && pos.x >= 0) {
		if (this->grid[pos.y][pos.x].unit == NULL) {
			this->grid[pos.y][pos.x].unit = unit;
			unit->tile = &(this->grid[pos.y][pos.x]);
			return true;
		}
	} else {
		// don't place it off the map please
		return false;
	}
	for (unsigned i = 0; i < this->yLength * this->xLength * 2; i++) {
		if (radPos < circumference) {
			if (	pos.y < this->yLength && pos.y >= 0 &&
					pos.x < this->xLength && pos.x >= 0) {
				if (this->grid[pos.y][pos.x].unit == NULL) {
					this->grid[pos.y][pos.x].unit = unit;
					unit->tile = &(this->grid[pos.y][pos.x]);
					return true;
				}
			}
			if (radPos < circumference >> 1)
				pos.y++;
			else
				pos.y--;

			if ((radPos + radius) % circumference < circumference >> 1)
				pos.x++;
			else
				pos.x--;
			radPos++;
		} else {
			radPos = 0;
			radius++;
			circumference = radius * 4;
			pos.y--;
		}
	}
	return false;
}
