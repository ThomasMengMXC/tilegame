#include <stdlib.h>
#include "stage.h"
#include "scene.h"

Stage *stage_init(void) {
	Stage *stage = malloc(sizeof(Stage));
	stage->depth = 0;
	stage->scene = NULL;
	stage->currentScene = NULL;
	return stage;
}

void scene_change(Stage *stage, int newSc) {
	if (stage) {
		stage->currentScene->exit(stage->currentScene->data);
		stage->currentScene = stage->scene[newSc];
		stage->currentScene->entry(stage->currentScene->data);
	}
	return;
}

void stage_exit(Stage *stage) {
	if (stage) {
		if (stage->depth) {
			stage->currentScene->exit(stage->currentScene->data);
			for (int i = 0; i < stage->depth; i++) {
				free(stage->scene[i]);
			}
			free(stage->scene);
		}

		free(stage);
	}
	return;
}

// creates a new scene on the stage and returns a pointer to said new layer
void add_scene_to_stage(Stage *stage, UpdateFn upd, KeyboardFn kb,
		EntryFn entry, ExitFn exit) {
	stage->depth++;
	stage->scene = realloc(stage->scene, sizeof(Scene *) * stage->depth);

	stage->scene[stage->depth - 1] = init_scene(upd, kb, entry, exit);
	if (stage->currentScene == NULL) {
		stage->currentScene = stage->scene[stage->depth - 1];
	}
	return;
}

void remove_scene_from_stage(Stage *stage) {
	Scene *scene = stage->scene[stage->depth - 1];
	free(scene);
	scene = NULL;
	if (stage->depth > 1) {
		stage->depth--;
		stage->scene = realloc(stage->scene, sizeof(Scene *) * stage->depth);
	} else if (stage->depth == 1) {
		stage->depth--;
		free(stage->scene);
		stage->scene = NULL;
	}
	return;
}
