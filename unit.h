#ifndef UNIT_H
#define UNIT_H

typedef struct {
	char *icon;
	int hp;
	unsigned int xPos, yPos;
	unsigned int move;
	unsigned int str, mag, spd, def;
	int **moveGrid;
} Unit;

typedef struct {
	unsigned int *playerCnt;
	unsigned int *playerMax;
	Unit **units;
} Team;

#endif
