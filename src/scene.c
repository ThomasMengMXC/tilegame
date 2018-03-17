#include <stdlib.h>
#include "scene.h"

Scene *init_scene(UpdateFn upd, KeyboardFn kb, EntryFn entry, ExitFn exit) {
	Scene *scene = malloc(sizeof(Scene));
	scene->update = upd; scene->keyboard = kb; //update & kb hooks during run
	scene->entry = entry; scene->exit = exit; //entry/exit hooks
	return scene;
}
