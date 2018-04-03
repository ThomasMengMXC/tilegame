#ifndef OVERMAP_SC_H
#define OVERMAP_SC_H

#include <theatre/props_struct.h> // for props
#include "overmap_struct.h"

#define DATASTRUCT OverMap

DATASTRUCT *init_overmap(void);

void overmap_update(Props *props);
void overmap_keyboard(Props *props, int ch);

void overmap_entry(Props *props);
void overmap_exit(Props *props);

#endif
