#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
#include <theatre/props.h>
#include "lua_scene.h"
#include "lua_screen.h"
#include "lua_layer.h"

void update(Props *props) {
	lua_State *l = (lua_State *) props->data;
	lua_getglobal(l, "update");
	lua_getglobal(l, "props");
	if (lua_pcall(l, 1, 0, 0) != 0) {
		fprintf(stderr, "something went wrong in update\n");
	}
}

int keyboard(Props *props, int ch) {
	lua_State *l = (lua_State *) props->data;
	lua_getglobal(l, "keyboard");
	lua_getglobal(l, "props");
	lua_pushinteger(l, ch);
	if (lua_pcall(l, 2, 1, 0) != 0) {
		fprintf(stderr, "something went wrong in keyboard\n");
		// return if the keyboard messed up
		return -2;
	}
	// the only valid values are, -2, -1, 0.
	// 0 resets everything as the scene switches in and out
	// -1 continues the scene running
	// -2 asks the library to exit the loop
	int returnVal = lua_tonumber(l, -1);
	lua_pop(l, -1);

	return returnVal;
}

void arrival(Props *props, int sceneNumber) {
	lua_State *l = luaL_newstate();
	props->data = l;

	// loads the main file
	luaL_dofile(l, "main.lua");
	lua_newtable(l);
	lua_setglobal(l, "props");

	// runs the arrival function
	lua_getglobal(l, "arrival");
	lua_getglobal(l, "props");
	Screen_register(l);
	Layer_register(l);
	luaL_openlibs(l);
	if (lua_pcall(l, 1, 0, 0) != 0) {
		fprintf(stderr, "Something went wrong in arrival\n");
	}
}

void departure(Props *props) {
	lua_State *l = (lua_State *) props->data;

	// runs the departure function
	lua_getglobal(l, "departure");
	lua_getglobal(l, "props");
	if (lua_pcall(l, 1, 0, 0) != 0) {
		fprintf(stderr, "something went wrong in departure\n");
	}

	lua_close(l);
}
