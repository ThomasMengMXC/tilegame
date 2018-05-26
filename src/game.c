#include <theatre/theatre.h>
#include "lua_scene.h"

int main(int argc, char **argv) {
	Stage *stage;

	// pre show set up
	init_theatre();

	// preparing the stage just before the show
	stage = init_stage();
	add_scene_to_stage(stage, update, keyboard, arrival, departure);

	enact_play(stage);

	// clean up the stage
	free_stage(stage);
	finale();
	return 0;
}
