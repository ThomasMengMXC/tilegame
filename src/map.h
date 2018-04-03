#ifndef MAP_H
#define MAP_H

#include "map_struct.h"
#include "team_struct.h"
#include "unit_struct.h"
#include "cursor_struct.h"

Map *init_map(void);
void free_map(Map *map);
void map_draw(Map *map);

void add_team_to_map(Map *map, Team *team);

void init_move_grids(Team *team, Map *map);
void free_move_grid(Team *team, Map *map);

void find_range(Team *team, Map *map);
void flood_fill(short y, short x, int move, Unit *unit, Map *map);

void draw_range(Unit *unit, Map *map);
void undraw_range(Unit *unit, Map *map);

void update_cursor(Map *map, Cursor *cursor);

#endif
