#include <theatre/play.h>

#include "game.h"
#include "overmap_SC.h"

// left as an exercise for the reader :^)
int main(int argc, char **argv) {
	Stage *stage;
	init_theatre();
	stage = init_stage();
	add_scene_to_stage(stage, overmap_update, overmap_keyboard,
			overmap_entry, overmap_exit);	

	enact_play(stage);

	free_stage(stage);
	endwin();
	return 0;
}
