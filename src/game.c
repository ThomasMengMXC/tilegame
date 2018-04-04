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

	load_scene(stage, "~/.config/tilegame/scenes/overmap.so");
	enact_play(stage);

	free_stage(stage);
	finale();
	return 0;
}

void load_scene(Stage *stage, char *name) {
	void *handle;
    wordexp_t p;
    // evaluate the name
	wordexp(name, &p, 0);

    // dlopen the evaluated path name
	handle = dlopen(p.we_wordv[0], RTLD_LAZY);
	wordfree(&p);
    
    // check if the handle screwed up
	if (!handle) {
		fprintf (stderr, "%s\n", dlerror());
		return 1;
	}
	dlerror();

	UpdateFn update; KeyboardFn keyboard;
	EntryFn entry; ExitFn exit;

    // load all of the library functions
	update = (UpdateFn) dlsym(handle, "overmap_update");
	keyboard = (KeyboardFn) dlsym(handle, "overmap_keyboard");
	entry = (EntryFn) dlsym(handle, "overmap_entry");
	exit = (ExitFn) dlsym(handle, "overmap_exit");
    // add the functions to the stage
	add_scene_to_stage(stage, update, keyboard, entry, exit);
    return;
}
