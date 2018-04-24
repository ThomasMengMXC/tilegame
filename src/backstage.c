#include <stdlib.h>
#include "backstage.h"

Backstage *init_backstage(void) {
	Backstage *bs = malloc(sizeof(Backstage));
	bs->team = init_team();
	bs->money = 0;
	bs->unitIDPool = 0;
	return bs;
}

void free_backstage(Backstage *bs) {
	free_team(bs->team);
	free(bs);
}
