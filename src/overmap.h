#ifndef OVERMAP_H
#define OVERMAP_H

extern "C" {
#include <theatre/props.h>
}
#include <luajit-2.0/lua.hpp>
#include "map.h"

typedef enum Phase {
	PLAYER,
	ENEMY,
} Phase;

class OverMap {
	public:
		lua_State *L;
		Map *map;
		Layer *mapLayer;
		int winRef;

		OverMap(const char *directory);
		~OverMap(void);
		bool in_directory(void);
	private:
		Phase phase;
};

extern "C" {
void update(Props *props);
int keyboard(Props *props, int ch);
void arrival(Props *props, int sceneNum);
void departure(Props *props);
}

#endif
