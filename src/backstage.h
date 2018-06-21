#ifndef BACKSTAGE_H
#define BACKSTAGE_H

typedef struct Backstage {
	int halp;
} Backstage;

Backstage *init_backstage(void);
void free_backstage(Backstage *bs);

#endif
