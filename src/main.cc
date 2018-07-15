extern "C" {
#include <theatre/theatre.h> // for the theatre setup functions
}
#include "main.h" // does nothing
#include "overmap.h" // for getting the overmap functions
#include "backstage.h" // for creating the backstage and giving to the stage

int main(int argc, char **argv) {
	Stage *stage;
	Backstage *bs = new Backstage();

	// pre show set up
	init_theatre();

	// preparing the stage just before the show
	// stage requires bs so that all the scenes have access to bs
	stage = init_stage(bs);
	// adding the overmap to the stage
	// the functions could be named better but since there is only one scene,
	// it doesn't matter too much
	add_scene_to_stage(stage, update, keyboard, arrival, departure);

	// starts the show
	enact_play(stage);

	// clean up the stage
	delete bs;
	free_stage(stage);
	// the oppositve of init_theatre
	finale();
	return 0;
}
