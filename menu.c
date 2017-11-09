#include <ncurses.h>
#include <string.h>
#include <time.h>
#include "menu.h"

static int row, col;
static char logo[][50] = {
	{" _____ _ _        _____                      "},
	{"|_   _(_) |      |  __ \\                     "},
	{"  | |  _| | ___  | |  \\/ __ _ _ __ ___   ___ "},
	{"  | | | | |/ _ \\ | | __ / _` | '_ ` _ \\ / _ \\"},
	{"  | | | | |  __/ | |_\\ \\ (_| | | | | | |  __/"},
	{"  \\_/ |_|_|\\___|  \\____/\\__,_|_| |_| |_|\\___|"},
};
static char buttons = 3;
static char menu[4][10];
static char pos = 0;
static char shouldDraw = 1;


void menuInit(void) {
	drawLogo();
	strcpy(menu[pos++], "New game");
	strcpy(menu[pos++], "Options");
	strcpy(menu[pos++], "Exit");
	pos = 0;
	
}

void menuLoop(void) {
	if (shouldDraw) {
		clear();
		drawLogo();
		drawButtons();
		refresh();
		shouldDraw = 0;
	}
}

void menuInput(int ch) {
	switch(ch){
	case KEY_UP:
		if (pos > 0){
			pos--;
			selectionMove();
		}
		break;
	case KEY_DOWN:
		if (pos < buttons-1){
			pos++;
			selectionMove();
		}
		break;
	case 'z':
		if (pos == 0)
			mainState = lobby;
		break;
	}
}


static void drawLogo(void) {
	getmaxyx(stdscr, row, col);
	for (int i = 0; i<6; i++){
		mvprintw(row/2+i-6, (col-strlen(logo[0]))/2, logo[i]);
	}
}

static void drawButtons(void) {
	for (pos = 0; pos < buttons; pos++){
		printMiddle();
	}
	pos = 0;
	selectionMove();
}

static void printMiddle(void) {
	selectionMove();
	printw(menu[pos]);
}

static void selectionMove(void) {
	move(row/2+2*(pos+1),(col-strlen(menu[0]))/2);
}
