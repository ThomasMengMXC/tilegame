#ifndef UNIT_H
#define UNIT_H

typedef struct {
	char name[20];
	unsigned int unitID;

	char icon[3];
	short hp;
	unsigned short move;
	unsigned short str, spd, def;
} Unit;

Unit *init_unit(char *name, unsigned int unitID);
void free_unit(Unit *unit);

#endif
