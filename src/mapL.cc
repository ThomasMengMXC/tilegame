#include <cstring>
#include "mapL.h"

static Unit *lua_tounit(lua_State *L, int index);
static Colour lua_tocolour(lua_State *L, int index);

static int load(lua_State *L) {
	const char *filename;
	if (lua_type(L, 1) != LUA_TTABLE || lua_type(L, 2) != LUA_TSTRING)
		goto error;
	filename = lua_tostring(L, 2);
	lua_pushlightuserdata(L, (void *) new Map(filename));
	lua_setfield(L, 1, "cdata");
	lua_newtable(L);
	lua_setfield(L, 1, "factions");
	goto out;
error:
	fprintf(stderr, "Failed in load, arguments incorrect\n");
out:
	return 0;
}

// this might be a very bad idea but I'll keep it here just in case.
static int clear(lua_State *L) {
	if (lua_type(L, 1) != LUA_TTABLE)
		goto error;

	lua_getfield(L, 1, "cdata");
	delete ((Map *) lua_touserdata(L, -1));
	lua_pop(L, 1);
	lua_pushnil(L);
	lua_setfield(L, 1, "cdata");

	lua_pushnil(L);
	lua_setfield(L, 1, "factions");
	goto out;
error:
	fprintf(stderr, "Failed in clear, arguments incorrect\n");
out:
	return 0;
}

static int new_faction(lua_State *L) {
	Map *map;
	int factionID;
	if (lua_type(L, 1) != LUA_TTABLE || lua_type(L, 2) != LUA_TSTRING)
		goto error;
	lua_getfield(L, 1, "cdata");
	map = (Map *) lua_touserdata(L, -1);
	factionID = map->new_faction();
	lua_getfield(L, 1, "factions");
	lua_pushvalue(L, 2);
	lua_pushinteger(L, factionID);
	lua_settable(L, -3);

	goto out;
error:
	fprintf(stderr, "Failed in new_faction, arguments incorrect\n");
out:
	return 0;
}

static int add_unit(lua_State *L) {
	Map *map;
	unsigned factionID;
	Unit *unit;
	unsigned y, x;
	int unitID;
	if (lua_type(L, 1) != LUA_TTABLE
			|| lua_type(L, 2) != LUA_TSTRING 
			|| lua_type(L, 3) != LUA_TTABLE
			|| lua_type(L, 4) != LUA_TNUMBER
			|| lua_type(L, 5) != LUA_TNUMBER)
		goto error;
	// get map
	lua_getfield(L, 1, "cdata");
	map = (Map *) lua_touserdata(L, -1);
	lua_pop(L, 1);
	// get factions
	lua_getfield(L, 1, "factions");
	lua_pushvalue(L, 2);
	lua_gettable(L, -2);
	factionID = lua_tointeger(L, -1);
	lua_pop(L, 2);
	// get unit
	unit = lua_tounit(L, 3);

	// get y and x
	y = lua_tointeger(L, 4);
	x = lua_tointeger(L, 5);
	unitID = map->add_unit(factionID, unit, y, x);
	if (unitID == -1)
		delete unit;
	lua_pushinteger(L, unitID);
	goto out;
error:
	fprintf(stderr, "Failed in add_unit, arguments incorrect\n");
out:
	return 1;
}

static int event_tile(lua_State *L) {
	Map *map;
	unsigned y, x;
	const char *name;
	Colour col;
	int eventID;
	if (lua_type(L, 1) != LUA_TTABLE
			|| lua_type(L, 2) != LUA_TNUMBER
			|| lua_type(L, 3) != LUA_TNUMBER
			|| lua_type(L, 4) != LUA_TSTRING
			|| lua_type(L, 5) != LUA_TTABLE
			|| lua_type(L, 6) != LUA_TFUNCTION)
		goto error;
	// get map
	lua_getfield(L, 1, "cdata");
	map = (Map *) lua_touserdata(L, -1);
	lua_pop(L, 1);
	y = lua_tointeger(L, 2);
	x = lua_tointeger(L, 3);
	name = lua_tostring(L, 4);
	col = lua_tocolour(L, 5);
	eventID = luaL_ref(L, LUA_REGISTRYINDEX);
	map->event_tile(y, x, name, col, eventID);
	goto out;
error:
	fprintf(stderr, "Failed in add_unit, arguments incorrect\n");
out:
	return 1;
}

static const struct luaL_Reg map[] = {
	{"load", load},
	{"clear", clear},
	{"new_faction", new_faction},
	{"add_unit", add_unit},
	{"event_tile", event_tile},
	{NULL, NULL},
};

int luaopen_Map(lua_State *L) {
	luaL_register(L, "Map", map);
	return 1;
}

Map *get_map(lua_State *L) {
	lua_getglobal(L, "Map");
	lua_getfield(L, -1, "cdata");
	Map *map = (Map *) lua_touserdata(L, -1);
	lua_pop(L, 2);
	return map;
}

void set_backstage(lua_State *L, Backstage *bs) {
	lua_getglobal(L, "Map");
	lua_pushlightuserdata(L, (void *) bs);
	lua_setfield(L, 1, "bs");
}

// Helper functions ----------------
static Unit *lua_tounit(lua_State *L, int index) {
	const char *name, *icon;
	int stat;
	lua_pushvalue(L, index);
	lua_getfield(L, -1, "name");
	name = lua_tostring(L, -1);
	lua_pop(L, 1);
	Unit *unit = new Unit(name);

	lua_getfield(L, -1, "icon");
	icon = lua_tostring(L, -1);
	lua_pop(L, 1);
	strncpy(unit->icon, icon, sizeof(unit->icon) - 1);

	lua_getfield(L, -1, "hp");
	stat = lua_tointeger(L, -1);
	lua_pop(L, 1);
	unit->base.maxHp = stat;

	lua_getfield(L, -1, "move");
	stat = lua_tointeger(L, -1);
	lua_pop(L, 1);
	unit->base.move = stat;

	lua_getfield(L, -1, "str");
	stat = lua_tointeger(L, -1);
	lua_pop(L, 1);
	unit->base.str = stat;

	lua_getfield(L, -1, "spd");
	stat = lua_tointeger(L, -1);
	lua_pop(L, 1);
	unit->base.spd = stat;

	lua_getfield(L, -1, "def");
	stat = lua_tointeger(L, -1);
	lua_pop(L, 2); // pop both def and the table
	unit->base.def = stat;
	unit->modified = unit->base;
	return unit;
}

static Colour lua_tocolour(lua_State *L, int index) {
	Colour col;
	lua_pushvalue(L, index);

	lua_getfield(L, -1, "r");
	col.r = lua_tointeger(L, -1);
	lua_pop(L, 1);

	lua_getfield(L, -1, "g");
	col.g = lua_tointeger(L, -1);
	lua_pop(L, 1);

	lua_getfield(L, -1, "b");
	col.b = lua_tointeger(L, -1);
	lua_pop(L, 1);

	lua_getfield(L, -1, "a");
	col.a = lua_tointeger(L, -1);
	lua_pop(L, 1);
	return col;
}
