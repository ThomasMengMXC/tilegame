#include <stdlib.h>
#include <stdio.h>
#include "game.h"
#include "title_SC.h"
#include "action_SC.h"

Scene *origScene = NULL;

void scene_setup(struct Game *game) {
	game->scene = malloc(sizeof(Scene) * game->scCnt);
	origScene = game->scene;

	title_sc_init(game->scene, game);
	action_sc_init(game->scene + 2, game);

	game->scene->sc_entry(game->scene->data);
	return;
}

void scene_change(struct Game *game, int oldSc, int newSc) {
	game->scene->sc_exit(game->scene->data);
	game->scene += newSc - oldSc;
	game->scene->sc_entry(game->scene->data);
	return;
}

void scene_exit(struct Game *game) {
	game->scene->sc_exit(game->scene->data);
	free(origScene);
	return;
}
