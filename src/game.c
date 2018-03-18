#include <ncurses.h>
#include <stdlib.h>
#include <signal.h>
#include <theatre/stage.h>
#include <theatre/scene.h>
#include <theatre/colour.h>

#include "game.h"
#include "overmap_SC.h"

static void segfault(int signo) {
	endwin();
	printf("segfaulted: %d\n", signo);
	exit(1);
}

void testing(void) {
}

// left as an exercise for the reader :^)
int main(int argc, char **argv) {
	signal(SIGSEGV, segfault);
	fprintf(stderr, "%d\n", rgb_to_term(128, 128, 128));

	Stage *stage;
	Scene *scene;

	init_ncurses();

	stage = init_stage();

	add_scene_to_stage(stage, overmap_update, overmap_keyboard,
			overmap_entry, overmap_exit);	
	stage->currentScene->entry(stage->currentScene->props);
	int ch = 0;
	while((ch = getch()) != 'q') {
		scene = stage->currentScene; // set the current scene
		if (scene) {
			scene->update(scene->props);
			scene->keyboard(scene->props, ch);
			if (scene->props->changeScene != -1) {
				scene_change(stage, scene->props->changeScene);
				scene->props->changeScene = -1;
			}
		}
	}
	stage->currentScene->exit(stage->currentScene->props);
	free_stage(stage);
	endwin();
	return 0;
}

// set up some ncurses specific stuff
int init_ncurses(void) {
	initscr();

	start_color();
	for (int i = 0; i < 256; i++) {
		init_pair(i, COLOR_BLACK, i);
	}
	cbreak();
	noecho();
	curs_set(0);
	timeout(16);
	keypad(stdscr, TRUE);
	return 0;
}
