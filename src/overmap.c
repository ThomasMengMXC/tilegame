#include "overmap.h"
#include "backstage.h"

static OverMap *init_overmap(void); 
static void free_overmap(OverMap *om);

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
	props->screen = init_screen(0, 0);
	OverMap *om = props->data = init_overmap();
	om->mapLayer = add_layer_to_scr(props->screen, 0, 0, 25, 40);
	draw_map(om->map, om->mapLayer);
}

void departure(Props *props) {
	OverMap *om = props->data;
	free_overmap(om);
	free_screen(props->screen);
	props->screen = NULL;
}

static OverMap *init_overmap(void) {
	OverMap *om = malloc(sizeof(OverMap));
	om->map = init_map("../data/level1/plain.lua");
	return om;
}

static void free_overmap(OverMap *om) {
	if (om->map)
		free_map(om->map);
	free(om);
}
