#include <theatre/theatre.h>
#include "overmap.h"
#include "backstage.h"

int main(int argc, char **argv) {
	Stage *stage;
	Backstage *bs = init_backstage();

	// pre show set up
	init_theatre();

	// preparing the stage just before the show
	stage = init_stage(bs);
	add_scene_to_stage(stage, update, keyboard, arrival, departure);

	enact_play(stage);

	// clean up the stage
	free_backstage(bs);
	free_stage(stage);
	finale();
	return 0;
}
