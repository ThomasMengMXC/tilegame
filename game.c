#include <ncurses.h>
#include <stdlib.h>
#include "game.h"
#include "scene.h"

int main(int argc, char **argv) {
	ncurses_setup();

	Game *game = game_setup();
	scene_setup(game);
	int ch = 0;
	while((ch = getch()) != 'q') {
		game->scene->draw(game->scene->data);
		game->scene->update(game->scene->data);
		game->scene->keyboard(game->scene->data, ch);
	}
	scene_exit(game);
	free_game(game);
	endwin();
	return 0;
}

int ncurses_setup(void) {
	initscr();

	start_color();
	init_pair(1, COLOR_BLACK, COLOR_GREEN);
	init_pair(2, COLOR_BLACK, COLOR_WHITE);

	cbreak();
	noecho();
	curs_set(0);
	timeout(16);
	keypad(stdscr, TRUE);
	return 0;
}

Game *game_setup(void) {
	Game *game = malloc(sizeof(Game));
	game->scCnt = 3;
	game->scene = NULL;
	getmaxyx(stdscr, game->row, game->col);
	return game;
}

void free_game(Game *game) {
	free(game);
	return;
}
