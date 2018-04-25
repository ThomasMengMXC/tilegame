#ifndef GAME_H
#define GAME_H

extern "C" {
#include <theatre/stage.h>
}

void *load_scene(Stage *stage, char *name);
void unload_all_scenes(void **handles);
void **load_all_scenes(Stage *stage, const char *directory);

#endif
