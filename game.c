#include <ncurses.h>
#include <stdlib.h>
#include <string.h>
#include "game.h"
#include "scene.h"
#include "unit.h"

// left as an exercise for the reader
int main(int argc, char **argv) {
	ncurses_setup();

	Game *game = game_setup();
	scene_setup(game);
	int ch = 0;
	while((ch = getch()) != 'q') {
		if (ch == KEY_RESIZE) {
			getmaxyx(stdscr, game->row, game->col);
		}
		game->scene->draw(game->scene->data);
		game->scene->update(game->scene->data);
		game->scene->keyboard(game->scene->data, ch);
	}
	scene_exit(game);
	free_game(game);
	endwin();
	return 0;
}

// set up some ncurses specific stuff
int ncurses_setup(void) {
	initscr();

	start_color();
	init_pair(1, COLOR_BLACK, COLOR_GREEN);
	init_pair(2, COLOR_BLACK, COLOR_WHITE);
	init_pair(3, COLOR_BLACK, COLOR_BLUE);

	cbreak();
	noecho();
	curs_set(0);
	timeout(16);
	keypad(stdscr, TRUE);
	return 0;
}

// Creates the game object and everything inside then returns it
Game *game_setup(void) {
	Game *game = malloc(sizeof(Game));
	game->sceneCnt = 3; // TEMPORARY VALUE
	game->scene = NULL;

	game->players = malloc(sizeof(Team));
	game->players->playerCnt = 0;
	game->players->firstNode = NULL;

	Unit player = {
		.icon = ":)",
		.hp = 10,
		.xPos = 10, .yPos = 10,
		.move = 1,
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
