#include <luajit-2.0/lua.hpp>
#include <cstdlib>
#include <cstring>
#include <queue>
#include "map.h"

static void free_grid(Tile **grid, unsigned yLength, unsigned xLength);
static Tile *init_row(lua_State *L, int index, unsigned rowNum);

/*
 * This function creates a map from a lua file.
 * If any part of the map is wrong, the funciton will return NULL.
 */
Map::Map(const char *filename) {
	unsigned yLength = 0, xLength = 0;
	this->unitIDs = 0;
	this->factionIDs = 0;
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

Map::~Map(void) {
	free_grid(this->grid, this->yLength, this->xLength);
	for (auto &facPair: this->factions) {
		for (auto &unitPair: facPair.second) {
			delete unitPair.second;
		}
	}
	for (auto &grids: this->moveGrids) {
		this->free_move_grid(grids.second);
	}
}

void Map::draw(Layer *layer) {
	clear_layer(layer);
	for (unsigned y = 0; y < this->yLength; y++) {
		for (unsigned x = 0; x < this->xLength; x++) {
			Tile *tile = &(this->grid[y][x]);
			add_icon_to_layer(layer, y, x, tile->icon, 2);
			add_colour_to_layer(layer, y, x, tile->colour);
			//add_hover_to_layer(layer, y, x, this_hover);
			if (this->grid[y][x].unit) {
				add_icon_to_layer(layer, y, x, tile->unit->icon, 2);
				//add_button_to_layer(layer, y, x, unit_button);
			}
		}
	}
}

int Map::new_faction(void) {
	this->factionIDs++;
	return this->factionIDs - 1;
}

int Map::add_unit(unsigned factionID, Unit *unit, unsigned y, unsigned x) {
	std::pair<unsigned, unsigned> pos;

	if (	y < this->yLength && y >= 0 &&
			x < this->xLength && x >= 0) {
		unit->factionID = factionID;
		unit->unitID = this->unitIDs;
		pos.first = y;
		pos.second = x;
		this->place_unit(unit, pos);
		this->units[unit->unitID] = unit;
		this->factions[factionID][unit->unitID] = unit;

		this->unitIDs++;
		return unit->unitID - 1;
	} else {
		return -1;
	}
}

void Map::change_faction(unsigned unitID, unsigned factionID) {
	if (this->units.count(unitID) == 0
			|| this->factions.count(factionID) == 0)
		return;
	Unit *unit = this->units[unitID];
	// remove unit from old faction
	this->factions[unit->factionID].erase(unit->unitID);
	this->units[unitID]->factionID = factionID;
	this->factions[unit->factionID][unit->unitID] = unit;
}

unsigned Map::faction_size(unsigned factionID) {
	return this->factions[factionID].size();
}

bool Map::move_unit(unsigned yOld, unsigned xOld,
		unsigned yNew, unsigned xNew) {
	std::pair<unsigned, unsigned> origTmp, destTmp;
	origTmp.first = yOld; origTmp.second = xOld;
	destTmp.first = yNew; destTmp.second = xNew;
	std::pair<unsigned, unsigned> tmp = this->closest_without_unit(destTmp);
	if (tmp.first != destTmp.first || tmp.second != destTmp.first)
		return false;
	Unit *unit = this->grid[origTmp.first][origTmp.second].unit;
	return this->place_unit(unit, destTmp);
}

void Map::event_tile(unsigned y, unsigned x,
		const char *name, Colour col, int eventID) {
	if (	y < this->yLength && y >= 0 &&
			x < this->xLength && x >= 0) {
		Tile *tile = &(this->grid[y][x]);
		tile->eventID = eventID;
		tile->eventColour = col;
		strncpy(tile->eventName, name, sizeof(tile->eventName));
	}
}
// private ---------------------

bool Map::place_unit(Unit *unit, std::pair<unsigned, unsigned> pos) {
	pos = this->closest_without_unit(pos);
	Tile *tile = &(this->grid[pos.first][pos.second]);
	if (!tile->unit) {
		if (this->moveGrids.count(unit->unitID) == 0) {
			this->moveGrids[unit->unitID] = init_move_grid();
		}
		this->grid[pos.first][pos.second].unit = unit;
		unit->tile = &(this->grid[pos.first][pos.second]);
		return true;
	} else {
		return false;
	}
}

bool Map::remove_unit(std::pair<unsigned, unsigned> pos) {
	Tile *tile = &(this->grid[pos.first][pos.second]);
	if (tile->unit) {
		tile->unit->tile = NULL;
		tile->unit = NULL;
		return true;
	} else {
		return false;
	}
}

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

void Map::free_move_grid(short **grid) {
	for (unsigned y = 0; y < this->yLength; y++) {
		delete[] grid[y];
	}
	delete[] grid;
}

void Map::unit_range(Unit *unit, short **range) {
	for (unsigned y = 0; y < this->yLength; y++) {
		for (unsigned x = 0; x < this->xLength; x++) {
			range[y][x] = INT16_MIN;
		}
	}
	std::pair<unsigned, unsigned> pos, tmp;
	short moveRange;
	std::queue<std::pair<unsigned, unsigned>> posQueue;
	std::queue<short> moveQueue;
	posQueue.push(unit->tile->pos);
	moveQueue.push(unit->move);
	while (!posQueue.empty()) {
		pos = posQueue.front();
		moveRange = moveQueue.front();
		posQueue.pop();
		moveQueue.pop();
		if (range[pos.first][pos.second] < moveRange) {
			range[pos.first][pos.second] = moveRange;
		}
		if (moveRange < 1) continue;
		for (unsigned i = 0; i < 4; i++) {
			tmp.first = pos.first + ((i + 0) % 2) * (i >> 1 ? +1 : -1);
			tmp.second = pos.second + ((i + 1) % 2) * (i >> 1 ? +1 : -1);
			if (tmp.first >= 0 && tmp.first < this->yLength &&
					tmp.second >= 0 && tmp.second < this->xLength) {
				if (range[tmp.first][tmp.second] < moveRange - 1) {
					// TEMPORARY, MAKE THIS DO SOMETHING
					posQueue.push(tmp);
					moveQueue.push(moveRange - 1); // THIS TOO
				}
			}
		}
	}
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
	char subbuff[2]; // for the substring key
	Tile *row = new Tile[length]();
	for (unsigned i = 0; i < length; i++) {
		memcpy(subbuff, &string[i], 1); // getting the substring
		subbuff[1] = '\0';
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
