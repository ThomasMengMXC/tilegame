#include <stdlib.h>
#include "backstage.h"

Backstage *init_backstage(void) {
	Backstage *bs = malloc(sizeof(Backstage));
	bs->halp = 0;
	return bs;
}

void free_backstage(Backstage *bs) {
	free(bs);
}
