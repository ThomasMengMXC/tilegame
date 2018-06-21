#include <luajit-2.0/luajit.h>
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
	Backstage *bs = *props->backstage;
	fprintf(stderr, "%d\n", bs->halp);
	Map *map = init_map(40, 25);
	free_map(map);
}

void departure(Props *props) {
}
