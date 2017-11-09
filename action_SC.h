#ifndef ACTION_SC_H
#define ACTION_SC_H
#include "game.h"

typedef struct {
	int pos;
} ActionData;

void action_sc_init(Scene *scene, Game *game);

void update(void *args);
void draw(void *args);
void keyboard(void *args, int ch);

void sc_entry(void *args);
void sc_exit(void *args);

#endif
