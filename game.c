#include <ncurses.h>
#include <stdlib.h>
#include <string.h>
#include "game.h"
#include "scene.h"
#include "unit.h"

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
	game->sceneCnt = 3; // TEMPORARY VALUE
	game->scene = NULL;

	game->playerCnt = 0;
	game->playerMax = 1;
	game->units = calloc(game->playerMax, sizeof(Unit));

	Unit player = {
		.hp = 10,
		.xPos = 0, .yPos = 0,
		.move = 5,
		.str = 5, .mag = 5, .spd = 5, .def = 5
	};
	add_player(game, player);

	getmaxyx(stdscr, game->row, game->col);
	return game;
}

void free_game(Game *game) {
	free(game->units);
	free(game);
	return;
}

void add_player(Game *game, Unit unit) {
	if (game->playerCnt >= game->playerMax) {
		game->playerMax *= 2;
		game->units = realloc(game->units,
				sizeof(Unit) * game->playerMax);
	}
	memcpy(game->units + game->playerCnt, &unit, sizeof(Unit));
	game->playerCnt++;
	return;
}
