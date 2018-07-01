#ifndef ITEM_H
#define ITEM_H

typedef enum ItemType {
	ITEM,
	STATUS,
	WEAPON,
	HEAVY,
} ItemType;

typedef struct Item {
	ItemType type;
	char name[16];
	char desc[32];
} Item;

typedef struct StatusItem {
	Item;
	char event[8]; // the key to a lua event
} StatusItem;

typedef struct Weapon {
	Item;
	int damage;
} Weapon;

typedef struct Heavy {
	Weapon;
	int heavyDamage;
} Heavy;

Weapon *init_weapon(void);
Heavy *init_heavy(void);

#endif
