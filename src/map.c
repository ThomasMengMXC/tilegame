#include <lua.h>
#include <lauxlib.h>
#include <stdlib.h>
#include <string.h>
#include "map.h"

static void free_grid(Tile **grid, unsigned yLength, unsigned xLength);
static Tile *init_row(lua_State *L, unsigned rowNum);

/*
 * This function creates a map from a lua file.
 * If any part of the map is wrong, the funciton will return NULL.
 */
Map *init_map(char *filename) {
	unsigned yLength, xLength;
	Map *map = malloc(sizeof(Map));
	lua_State *L = luaL_newstate();
	luaL_dofile(L, filename); // load the file into the lua state
	if (lua_getfield(L, 1, "map") != LUA_TTABLE)
		goto error; // fails if the map is not a table
	if (lua_getfield(L, 1, "tiles") != LUA_TTABLE)
		goto error;
	if ((yLength = luaL_len(L, 2)) <= 0)
		goto error; // fails if the length of the map is not > 0

	map->grid = calloc(sizeof(Tile *), yLength);
	for (int i = 0; i < yLength; i++) { // iterate over the rows
		if (lua_geti(L, 2, (lua_Integer) i + 1) != LUA_TSTRING)
			goto maperror; // fails if the type is not a string
		if (i == 0)
		 	// sets the xLength as the length of the first string
			xLength = luaL_len(L, -1);
		else if (xLength != luaL_len(L, -1)) // is this consistent?
			goto maperror;
		// load the rows via the tile descriptions, fail if it failed
		if ((map->grid[i] = init_row(L, i)) == NULL)
			goto maperror;
		lua_pop(L, 1); // pop the string off the stack.
	}
	lua_pop(L, 1); // pop the map table off the stack
	map->yLength = yLength;
	map->xLength = xLength;
	goto out;
maperror:
	for (int i = 0; i < yLength; i++) {
		if (map->grid[i])
			free(map->grid[i]);
	}
	free(map->grid);
error: // clean up map and set it to be null to be returned
	fprintf(stderr, "Failed in init_map(%s)\n", filename);
	free(map);
	map = NULL;
out:
	lua_close(L);
	return map;
}

void free_map(Map *map) {
	free_grid(map->grid, map->yLength, map->xLength);
	free(map);
}

static void free_grid(Tile **grid, unsigned yLength, unsigned xLength) {
	for (int y = 0; y < yLength; y++) {
		free(grid[y]);
	}
	free(grid);
}

static Tile *init_row(lua_State *L, unsigned rowNum) {
	unsigned length = luaL_len(L, -1);
	const char *string = lua_tostring(L, -1);
	char subbuff[2]; // for the substring key
	Tile *row = calloc(sizeof(Tile), length);
	for (int i = 0; i < length; i++) {
		memcpy(subbuff, &string[i], 1); // getting the substring
		subbuff[1] = '\0';
		lua_pushstring(L, subbuff);
		if (lua_gettable(L, 3) != LUA_TTABLE)
			goto error;

		if (lua_getfield(L, -1, "icon") != LUA_TSTRING)
			goto error;
		memcpy(row[i].icon, lua_tostring(L, -1), 2);
		row[i].icon[2] = '\0';
		lua_pop(L, 1);

		if (lua_getfield(L, -1, "r") != LUA_TNUMBER)
			goto error;
		row[i].col.r = lua_tointeger(L, -1);
		lua_pop(L, 1);

		if (lua_getfield(L, -1, "g") != LUA_TNUMBER)
			goto error;
		row[i].col.g = lua_tointeger(L, -1);
		lua_pop(L, 1);

		if (lua_getfield(L, -1, "b") != LUA_TNUMBER)
			goto error;
		row[i].col.b = lua_tointeger(L, -1);
		lua_pop(L, 1);

		if (lua_getfield(L, -1, "a") != LUA_TNUMBER)
			goto error;
		row[i].col.a = lua_tointeger(L, -1);
		lua_pop(L, 1);

		if (lua_getfield(L, -1, "mvCost") != LUA_TNUMBER)
			goto error;
		row[i].mvCost = lua_tointeger(L, -1);
		lua_pop(L, 1);

		lua_pop(L, 1);
		row[i].pos.y = rowNum;
		row[i].pos.x = i;
		row[i].unit = NULL;
	}
	goto out;
error:
	fprintf(stderr, "Failed in init_row(%s)\n", string);
	lua_pop(L, 1); // get rid of the nil value
	free(row);
	row = NULL;
out:
	return row;
}
