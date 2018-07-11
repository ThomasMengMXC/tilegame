#ifndef MAP_L_H
#define MAP_L_H

#include <luajit-2.0/lua.hpp>
#include "map.h"
#include "backstage.h"

int luaopen_Map(lua_State *L);
Map *get_map(lua_State *L);
void set_backstage(lua_State *L, Backstage *bs);

#endif
