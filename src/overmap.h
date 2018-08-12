#ifndef OVERMAP_H
#define OVERMAP_H

extern "C" {
#include <theatre/props.h>
}
#include <luajit-2.0/lua.hpp>
#include "map.h"

typedef enum Phase {
	SETUP,
	PLAYER,
	NONPLAYER,
} Phase;

class OverMap {
	public:
		lua_State *L;
		Map *map;
		Layer *mapLayer;
		int winRef;
		Phase phase;

		OverMap(const char *directory);
		~OverMap(void);

	private:
		bool in_directory(void);
};

extern "C" {
void update(Props *props);
int keyboard(Props *props, int ch);
void arrival(Props *props, int sceneNum);
void departure(Props *props);
}

#endif
