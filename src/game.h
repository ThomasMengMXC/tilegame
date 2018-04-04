#ifndef GAME_H
#define GAME_H

void *load_scene(Stage *stage, char *name);
void unload_all_scenes(void **handles);
void **load_all_scenes(Stage *stage, char *directory);

#endif
