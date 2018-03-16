#include <ncurses.h>
#include <stdlib.h>
#include <signal.h>
#include "game.h"
#include "stage.h"
#include "scene.h"
#include "unit.h"
#include "sprite.h"
#include "overmap_SC.h"

static void catch_function(int signo) {
	endwin();
	printf("segfaulted LOL: %d\n", signo);
	exit(1);
}

// left as an exercise for the reader :^)
int main(int argc, char **argv) {
	signal(SIGSEGV, catch_function);

	Game *game;
	Scene *scene; // the scene currently being worked on

	ncurses_init();

	game = game_init();
	game->stage = stage_init();
	add_scene_to_stage(game->stage, overmap_update, overmap_keyboard,
			overmap_entry, overmap_exit);

	int ch = 0;
	while((ch = getch()) != 'q') {
		scene = game->stage->currentScene; // set the current scene
		if (scene) {
			if (ch == KEY_RESIZE) {
				getmaxyx(stdscr, game->row, game->col);
			}
			scene->update(scene->data);
			scene->keyboard(scene->data, ch);
		}
	}
	stage_exit(game->stage);
	free_game(game);
	endwin();
	return 0;
}

// set up some ncurses specific stuff
int ncurses_init(void) {
	initscr();

	start_color();
	init_pair(GRASS, COLOR_BLACK, COLOR_GREEN);
	init_pair(MOVE_RANGE, COLOR_BLACK, COLOR_WHITE);
	init_pair(CURSOR, COLOR_BLACK, COLOR_BLUE);

	cbreak();
	noecho();
	curs_set(0);
	timeout(16);
	keypad(stdscr, TRUE);
	return 0;
}

// Creates the game object and everything inside then returns it
Game *game_init(void) {
	Game *game = malloc(sizeof(Game));
	game->stage = NULL;

	game->players = malloc(sizeof(Team));
	game->players->playerCnt = 0;
	game->players->firstNode = NULL;

	Unit player = {
		.icon = ":)",
		.hp = 10,
		.xPos = 0, .yPos = 0,
		.move = 5,
		.str = 5, .mag = 5, .spd = 5, .def = 5
	};
	add_player(game->players, player);

	getmaxyx(stdscr, game->row, game->col);
	return game;
}

// Frees the game object and everything inside that was also malloc'd
void free_game(Game *game) {
	free_team(game->players);
	free(game);
	return;
}
