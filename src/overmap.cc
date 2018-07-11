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
	props->data = new OverMap();
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

OverMap::OverMap(void) {
	this->L = luaL_newstate();
	luaopen_Map(L);
	luaL_dostring(L, "Map:load(\"../data/level1/forest.lua\")");
	luaL_dostring(L, "Map:new_faction(\"Bandit\")");
	luaL_dostring(L, "Map:add_unit(\"Bandit\", {name = \"John\", icon = \"X<\", hp = 10, move = 5, str = 5, spd = 5, def = 5}, 5, 5)");
	this->map = get_map(L);
}

OverMap::~OverMap(void) {
	luaL_dostring(L, "Map:clear()");
	lua_close(this->L);
}
