#ifndef VECTOR_H
#define VECTOR_H

#define NEW_VECTOR(name)							\
	name *name##VAR = malloc(sizeof(name));			\
	unsigned int name##INDEX = 0;					\
	unsigned int name##MAX = 1;

#define EXPAND_VECTOR(name)							\
	if (name##INDEX > name##MAX - 1) {				\
		name##MAX *= 2;								\
	}												\
	name##VAR = realloc(name##VAR, name##MAX);		\

#define SHRINK_VECTOR(name)							\
	while (name##INDEX * 2  < name##MAX - 1) {		\
		name##MAX /= 2;								\
	}												\
	name##VAR = realloc(name##VAR, name##MAX);		\


#endif
