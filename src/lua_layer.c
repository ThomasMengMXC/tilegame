// wrapping the screen part of libtheatre in Lua functions
#include <theatre/layer.h>
#include "lua_screen.h"

static Layer *checkLayer(lua_State *L, int index) {
	luaL_checktype(L, index, LUA_TUSERDATA);
	Layer **layer = (Layer **)luaL_checkudata(L, index, "Layer");
	if (layer == NULL) luaL_error(L, "Not a layer!");
	return *layer;
}

static int LUA_clear_layer(lua_State *L) {
	Layer *layer = checkLayer(L, 1);
	clear_layer(layer);
	return 0;
}

static int LUA_resize_layer(lua_State *L) {
	Layer *layer = checkLayer(L, 1);
	int yLength = luaL_checkinteger(L, 2);
	int xLength = luaL_checkinteger(L, 3);
	resize_layer(layer, yLength, xLength);
	return 0;
}

static int LUA_mv_layer_relative(lua_State *L) {
	Layer *layer = checkLayer(L, 1);
	int y = luaL_checkinteger(L, 2);
	int x = luaL_checkinteger(L, 3);
	mv_layer_relative(layer, y, x);
	return 0;
}

static int LUA_mv_layer_absolute(lua_State *L) {
	Layer *layer = checkLayer(L, 1);
	int y = luaL_checkinteger(L, 2);
	int x = luaL_checkinteger(L, 3);
	mv_layer_absolute(layer, y, x);
	return 0;
}

static int LUA_refresh_layer(lua_State *L) {
	Layer *layer = checkLayer(L, 1);
	refresh_layer(layer);
	return 0;
}

static int LUA_add_colour(lua_State *L) {
	Layer *layer = checkLayer(L, 1);

	int y = luaL_checkinteger(L, 2);
	int x = luaL_checkinteger(L, 3);

	lua_pushstring(L, "r"); lua_gettable(L, 4);
	int r = luaL_checkinteger(L, -1);

	lua_pushstring(L, "g"); lua_gettable(L, 4);
	int g = luaL_checkinteger(L, -1);

	lua_pushstring(L, "b"); lua_gettable(L, 4);
	int b = luaL_checkinteger(L, -1);

	lua_pushstring(L, "a"); lua_gettable(L, 4);
	int a = luaL_checkinteger(L, -1);

	add_colour_to_layer(layer, y, x,
			(Colour) {.r = r, .g = g, .b = b, .a = a});
	return 0;
}

static int LUA_remove_colour(lua_State *L) {
	Layer *layer = checkLayer(L, 1);

	int y = luaL_checkinteger(L, 2);
	int x = luaL_checkinteger(L, 3);
	remove_colour_from_layer(layer, y, x);
	return 0;
}

static int LUA_add_icon(lua_State *L) {
	Layer *layer = checkLayer(L, 1);

	int y = luaL_checkinteger(L, 2);
	int x = luaL_checkinteger(L, 3);
	char *icon = (char *) lua_tostring(L, 4);
	size_t n = luaL_checkinteger(L, 5);
	add_icon_to_layer(layer, y, x, icon, n);
	return 0;
}

static int LUA_remove_icon(lua_State *L) {
	Layer *layer = checkLayer(L, 1);

	int y = luaL_checkinteger(L, 2);
	int x = luaL_checkinteger(L, 3);
	size_t n = luaL_checkinteger(L, 4);
	remove_icon_from_layer(layer, y, x, n);
	return 0;
}

static int LUA_add_button(lua_State *L) {
	Layer *layer = checkLayer(L, 1);

	int y = luaL_checkinteger(L, 2);
	int x = luaL_checkinteger(L, 3);
	lua_pushvalue(L, 4);
	long buttonRef = luaL_ref(L, LUA_REGISTRYINDEX);
	add_button_to_layer(layer, y, x, (Button) buttonRef);
	return 0;
}

static int LUA_remove_button(lua_State *L) {
	Layer *layer = checkLayer(L, 1);

	int y = luaL_checkinteger(L, 2);
	int x = luaL_checkinteger(L, 3);
	remove_button_from_layer(layer, y, x);
	return 0;
}

static int LUA_add_hover(lua_State *L) {
	Layer *layer = checkLayer(L, 1);

	int y = luaL_checkinteger(L, 2);
	int x = luaL_checkinteger(L, 3);
	lua_pushvalue(L, 4);
	long hoverRef = luaL_ref(L, LUA_REGISTRYINDEX);
	add_hover_to_layer(layer, y, x, (Hover) hoverRef);
	return 0;
}

static int LUA_remove_hover(lua_State *L) {
	Layer *layer = checkLayer(L, 1);

	int y = luaL_checkinteger(L, 2);
	int x = luaL_checkinteger(L, 3);
	remove_hover_from_layer(layer, y, x);
	return 0;
}

static int LUA_layer_tostring(lua_State *L) {
	lua_pushfstring(L, "Layer: %p", lua_touserdata(L, 1));
	return 1;
}

static const luaL_Reg Layer_methods[] = {
	{"clear",			LUA_clear_layer},
	{"resize",			LUA_resize_layer},
	{"mv_relative",		LUA_mv_layer_relative},
	{"mv_absolute",		LUA_mv_layer_absolute},
	{"refresh",			LUA_refresh_layer},

	{"add_colour",		LUA_add_colour},
	{"remove_colour",	LUA_remove_colour},
	{"add_icon",		LUA_add_icon},
	{"remove_icon",		LUA_remove_icon},
	{"add_button",		LUA_add_button},
	{"remove_button",	LUA_remove_button},
	{"add_hover",		LUA_add_hover},

	{"remove_hover",	LUA_remove_hover},
	{"__tostring",		LUA_layer_tostring},
	{0,0}
};


int Layer_register (lua_State *L) {
	luaL_newmetatable(L, "Layer");
	luaL_setfuncs(L, Layer_methods, 0);
	lua_pushvalue(L, -1);
	lua_setfield(L, -1, "__index");
	lua_setglobal(L, "Layer");
	return 1;
}
