// wrapping the screen part of libtheatre in Lua functions
#include <theatre/screen.h>
#include "lua_screen.h"

static Screen *checkScreen(lua_State *L, int index) {
	luaL_checktype(L, index, LUA_TUSERDATA);
	Screen **scr = (Screen **)luaL_checkudata(L, index, "Screen");
	if (scr == NULL) luaL_error(L, "Not a screen!");
	return *scr;
}

static int LUA_init_screen(lua_State *L) {
	int yLength = luaL_checkinteger(L, 1);
	int xLength = luaL_checkinteger(L, 2);
	Screen *tmp = init_screen(yLength, xLength);
	// push our newly created instance into lua to get returned
	Screen **scr = (Screen **)lua_newuserdata(L, sizeof(Screen *));
	*scr = tmp;
	luaL_getmetatable(L, "Screen");
	lua_setmetatable(L, -2);
	return 1;
}

static int LUA_free_screen(lua_State *L) {
	Screen *scr = checkScreen(L, 1);
	
	free_screen(scr);
	return 0;
}

static int LUA_draw_screen(lua_State *L) {
	Screen *scr = checkScreen(L, 1);
	int tmp = draw_screen(scr);
	lua_pushinteger(L, tmp);
	return 1;
}

static int LUA_clear_screen(lua_State *L) {
	Screen *scr = checkScreen(L, 1);
	clear_screen(scr);
	return 0;
}

static int LUA_resize_screen(lua_State *L) {
	Screen *scr = checkScreen(L, 1);
	resize_screen(scr);
	return 0;
}

static int LUA_add_layer(lua_State *L) {
	Screen *scr = checkScreen(L, 1);
	int yOffset = luaL_checkinteger(L, 2);
	int xOffset = luaL_checkinteger(L, 3);

	int yLength = luaL_checkinteger(L, 4);
	int xLength = luaL_checkinteger(L, 5);

	Layer *tmp = add_layer_to_scr(scr, yOffset, xOffset, yLength, xLength);
	// push our newly created instance into lua to get returned
	Layer **layer = (Layer **) lua_newuserdata(L, sizeof(Layer *));
	*layer = tmp;
	luaL_getmetatable(L, "Layer");
	lua_setmetatable(L, -2);
	return 1;
}

static int LUA_remove_layer(lua_State *L) {
	Screen *scr = checkScreen(L, 1);
	remove_layer_from_scr(scr);
	return 0;
}

static int LUA_get_button(lua_State *L) {
	Screen *scr = checkScreen(L, 1);

	int y = luaL_checkinteger(L, 2);
	int x = luaL_checkinteger(L, 3);
	long buttonRef = (long) get_button(scr, y, x);
	lua_rawgeti(L, LUA_REGISTRYINDEX, buttonRef);
	return 1;
}

static int LUA_get_hover(lua_State *L) {
	Screen *scr = checkScreen(L, 1);

	int y = luaL_checkinteger(L, 2);
	int x = luaL_checkinteger(L, 3);
	long hoverRef = (long) get_hover(scr, y, x);
	lua_rawgeti(L, LUA_REGISTRYINDEX, hoverRef);
	return 1;
}

static int LUA_screen_tostring(lua_State *L) {
	lua_pushfstring(L, "Screen: %p", checkScreen(L, 1));
	return 1;
}

static const luaL_Reg Screen_methods[] = {
	{"new",			LUA_init_screen},
	{"draw",		LUA_draw_screen},
	{"clear",		LUA_clear_screen},
	{"resize",		LUA_resize_screen},
	{"add_layer",	LUA_add_layer},
	{"remove_layer",LUA_remove_layer},
	{"get_button",	LUA_get_button},
	{"get_hover",	LUA_get_hover},
	{"__gc",		LUA_free_screen},
	{"__tostring",	LUA_screen_tostring},
	{0,0}
};


int Screen_register (lua_State *L) {
	luaL_newmetatable(L, "Screen");
	luaL_setfuncs(L, Screen_methods, 0);
	lua_pushvalue(L, -1);
	lua_setfield(L, -1, "__index");
	lua_setglobal(L, "Screen");
	return 1;
}

