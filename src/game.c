#include <dlfcn.h>
#include <wordexp.h>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <theatre/play.h>
#include <theatre/stage.h>
#include <theatre/props.h>

#include "game.h"
//#include "overmap_SC.h"

int main(int argc, char **argv) {
	Stage *stage;

    // pre show setup
	init_theatre();

    // preparing the stage just before the show
	stage = init_stage();

    // getting the scripts ready from ~/.config/tilegame/scenes
    char *config = "~/.config/tilegame/scenes";
    void **handles = load_all_scenes(stage, config);

    // start the show
	enact_play(stage);

    // free the handles
    free(handles);

    // clean up the stage
	free_stage(stage);

    // post show cleanup
	finale();
	return 0;
}

void **load_all_scenes(Stage *stage, char *directory) {
    wordexp_t p;
	wordexp(directory, &p, 0);

    DIR *dir;
    struct dirent *ent;
    int sceneCount = 0;
    void **handles = NULL;
    handles = realloc(handles, sizeof(void *) * (sceneCount + 1));
    if ((dir = opendir(p.we_wordv[0])) != NULL) {
        while ((ent = readdir (dir)) != NULL) {
            char *name = malloc(sizeof(char) * (strlen(directory)
                        + strlen(ent->d_name) + 2));
            if (ent->d_name[0] != '.') {
                sprintf(name, "%s/%s", directory, ent->d_name);
                sceneCount++;
                handles = realloc(handles, sizeof(void *) * (sceneCount + 1));
                handles[sceneCount - 1] = load_scene(stage, name);
            }
            free(name);
        }
        closedir (dir);
    };
    handles[sceneCount] = NULL;
	wordfree(&p);
    return handles;
}

void unload_all_scenes(void **handles) {
    for (int i = 0; handles[i] != NULL; i++) {
        dlclose(handles[i]);
    }
    return;
}

void *load_scene(Stage *stage, char *name) {
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
		exit(1);
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
    return handle;
}
