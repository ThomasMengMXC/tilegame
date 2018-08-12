extern "C" {
#include <unistd.h>
}
#include <cstring>
#include "overmap.h"
#include "backstage.h"
#include "mapL.h"

void update(Props *props) {
	OverMap *om = (OverMap *) props->data;
	switch (om->phase) {
		case SETUP:
			break;
		case PLAYER:
			break;
		case NONPLAYER:
			break;
	}
	draw_screen(props->screen);
}

int keyboard(Props *props, int ch) {
	// these are basically the escape characters
	switch (ch) {
		case 'q':
		case 'Q':
			return -2;
	}

	OverMap *om = (OverMap *) props->data;
	switch (om->phase) {
		case SETUP:
			break;
		case PLAYER:
			break;
		case NONPLAYER:
			break;
	}
	return -1;
}

void arrival(Props *props, int sceneNum) {
	//Backstage *bs = props->backstage;
	props->screen = (Screen *) init_screen(0, 0);
	props->data = new OverMap("../data/level1");
	OverMap *om = (OverMap *) props->data;
	om->mapLayer = add_layer_to_scr(props->screen, 0, 0, 25, 40);
	om->map->draw(om->mapLayer);
}

void departure(Props *props) {
	OverMap *om = (OverMap *) props->data;
	delete om;
	free_screen(props->screen);
	props->screen = NULL;
}

OverMap::OverMap(const char *directory) {
	char cwd[1024];
	if (getcwd(cwd, sizeof(cwd)) == NULL)
		perror("getcwd() error");
	this->L = luaL_newstate();
	luaopen_Map(L);
	lua_pop(L, 1);
	chdir(directory);
	this->in_directory();
	chdir(cwd);
	this->map = get_map(L);

	this->phase = SETUP;
}

OverMap::~OverMap(void) {
	luaL_dostring(this->L, "Map:clear()");
	lua_close(this->L);
}

// private
bool OverMap::in_directory(void) {
	if (luaL_dofile(L, "level.lua")){ // load the file into the lua state
		fprintf(stderr, "%s\n", lua_tostring(L, -1));
		return false;
	}
	lua_getfield(L, -1, "win");
	this->winRef = luaL_ref(L, LUA_REGISTRYINDEX);

	lua_getfield(L, -1, "map");
	lua_getglobal(L, "Map");
	lua_getfield(L, -1, "load");
	lua_pushvalue(L, -2);
	lua_pushvalue(L, -4);
	lua_pcall(L, 2, 0, 0);
	lua_pop(L, 2); // pop Map and map off the stack
	
	lua_getfield(L, -1, "starting");
	lua_pop(L, 1);

	lua_getfield(L, -1, "builder");
	if (lua_pcall(L, 0, 0, 0)) {
		fprintf(stderr, "%s\n", lua_tostring(L, -1));
		return false;
	}
	lua_pop(L, 1);
	return true;
}
