#include "overmap.h"
#include "backstage.h"
#include "map.h"

void update(Props *props) {
}

int keyboard(Props *props, int ch) {
	if (ch == 'q' || ch == 'Q') {
		return -2;
	}
	return -1;
}

void arrival(Props *props, int sceneNum) {
	//Backstage *bs = *props->backstage;
	Map *map = init_map("lev1.lua");
	if (map)
		free_map(map);
}

void departure(Props *props) {
}
