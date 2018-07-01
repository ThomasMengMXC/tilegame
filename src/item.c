#include <stdlib.h>
#include <string.h>
#include "item.h"

#define CAST(obj, type) (((type *) & obj)[0])

static inline Item prim_item(char *name, char *desc);
static inline StatusItem prim_statusItem(char *name, char *desc, char *event);
static inline Weapon prim_weapon(char *name, char *desc, int damage);
static inline Heavy prim_heavy(char *name, char *desc, int damage);

StatusItem *init_statusitem(void) {
	StatusItem *status = malloc(sizeof(StatusItem));
	*status = prim_statusItem("Cake", "A delicious piece of cake.", "EAT");
	return status;
}

Weapon *init_weapon(void) {
	Weapon *weapon = malloc(sizeof(Weapon));
	*weapon = prim_weapon("Knife", "A sharp knife.", 5);
	return weapon;
}

Heavy *init_heavy(void) {
	Heavy *heavy = malloc(sizeof(Heavy));
	*heavy = prim_heavy("Club", "A heavy club.", 8);
	return heavy;
}

static inline Item prim_item(char *name, char *desc) {
	Item item;
	strncpy(item.name, name, sizeof(name) - 1);
	item.name[sizeof(name) - 1] = '\0';
	strncpy(item.desc, desc, sizeof(desc) - 1);
	item.desc[sizeof(desc) - 1] = '\0';
	item.type = ITEM; // please override this
	return item;
}

static inline StatusItem prim_statusItem(char *name, char *desc, char *event) {
	StatusItem status;
	CAST(status, Item) = prim_item(name, desc);
	status.type = STATUS;
	strncpy(status.event, event, sizeof(event) - 1);
	status.event[sizeof(event) - 1] = '\0';
	return status;
}

static inline Weapon prim_weapon(char *name, char *desc, int damage) {
	Weapon weapon;
	CAST(weapon, Item) = prim_item(name, desc);
	weapon.damage = damage;
	weapon.type = WEAPON;
	return weapon;
}

static inline Heavy prim_heavy(char *name, char *desc, int damage) {
	Heavy heavy;
	CAST(heavy, Weapon) = prim_weapon(name, desc, damage);
	heavy.heavyDamage = damage;
	heavy.type = HEAVY;
	return heavy;
}
