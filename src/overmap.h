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
		Map *map;
		Layer *mapLayer;
		lua_State *L;
		Phase phase;

		OverMap(void);
		~OverMap(void);
};

extern "C" {
void update(Props *props);
int keyboard(Props *props, int ch);
void arrival(Props *props, int sceneNum);
void departure(Props *props);
}

#endif
