extern "C" {
#include <unistd.h>
}
#include <cstring>
#include "overmap.h"
#include "backstage.h"
#include "mapL.h"

void update(Props *props) {
	draw_screen(props->screen);
}

int keyboard(Props *props, int ch) {
	switch (ch) {
		case 'q':
		case 'Q':
			return -2;
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
	//lua_pop(L, 1);
	//chdir(directory);
	//this->in_directory();
	luaL_dostring(L, "Map:load(\"../data/level1/forest.lua\")");
	luaL_dostring(L, "Map:new_faction(\"Bandit\")");
	luaL_dostring(L, "Map:add_unit(\"Bandit\", {name = \"John\", icon = \"X<\", hp = 10, move = 5, str = 5, spd = 5, def = 5}, 5, 5)");
	//chdir(cwd);
	this->map = get_map(L);
}

OverMap::~OverMap(void) {
	luaL_dostring(this->L, "Map:clear()");
	lua_close(this->L);
}

bool OverMap::in_directory(void) {
	if (luaL_dofile(L, "level.lua")) // load the file into the lua state
		return false;
	lua_getfield(L, -1, "win");
	this->winRef = luaL_ref(L, LUA_REGISTRYINDEX);
	return true;
}
