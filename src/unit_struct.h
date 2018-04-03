#ifndef UNIT_STRUCT_H
#define UNIT_STRUCT_H

typedef struct {
	char name[20];
	unsigned int unitID;

	char icon[3];
	short hp;
	unsigned short yPos, xPos;
	unsigned short move;
	unsigned short str, spd, def;
} Unit;

#endif
