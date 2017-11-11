#include <stdlib.h>
#include <stdio.h>
#include "game.h"
#include "title_SC.h"
#include "overmap_SC.h"
#include "submap_SC.h"

Scene *origScene = NULL;

void scene_setup(struct Game *game) {
	game->scene = malloc(sizeof(Scene) * game->sceneCnt);
	origScene = game->scene;

	title_sc_init(game->scene, game);
	overmap_sc_init(game->scene + 1, game);
	submap_sc_init(game->scene + 2, game);

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
	// FIND A BETTER SOLUTION FOR THIS LATER
	// Probably just iterate through the scenes when all scenes are filled
	for (int i = 0; i < game->sceneCnt; i++) {
		free((origScene + i)->data);
	}
	free(origScene);
	return;
}
