#include <dlfcn.h>
#include <wordexp.h>
#include <theatre/play.h>
#include <theatre/stage.h>
#include <theatre/props.h>

#include "game.h"
//#include "overmap_SC.h"

int main(int argc, char **argv) {
	Stage *stage;
	init_theatre();
	stage = init_stage();
	// TESTING STARTS HERE

	wordexp_t p;
	wordexp("~/.config/tilegame/scenes/overmap.so", &p, 0);

	void *handle;
	char *error;
	UpdateFn update;
	KeyboardFn keyboard;
	EntryFn entry;
	ExitFn exit;

	handle = dlopen(p.we_wordv[0], RTLD_LAZY);
	wordfree(&p);
	if (!handle) {
		fprintf (stderr, "%s\n", dlerror());
		return 1;
	}
	dlerror();

	update = (UpdateFn) dlsym(handle, "overmap_update");
	keyboard = (KeyboardFn) dlsym(handle, "overmap_keyboard");
	entry = (EntryFn) dlsym(handle, "overmap_entry");
	exit = (ExitFn) dlsym(handle, "overmap_exit");
	add_scene_to_stage(stage, update, keyboard, entry, exit);

	// TESTING ENDS HERE
	//add_scene_to_stage(stage, overmap_update, overmap_keyboard,
			//overmap_entry, overmap_exit);	

	enact_play(stage);

	free_stage(stage);
	finale();
	return 0;
}
