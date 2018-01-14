#ifndef LINKED_LIST_H
#define LINKED_LIST_H

struct Node;

typedef struct {
	unsigned char playerCnt;
	struct Node *firstNode;
} LinkedList;

/*
// Frees the list linked list
void free_list(LinkedList *list) {
	Node *current = list->firstNode;
	while (current != NULL) {
		if (current->next != NULL) {
			current = current->next;
			free(current->previous->object);
			free(current->previous);
		} else {
			free(current->object);
			free(current);
			current = NULL;	
		}
	}
	free(list);	
	return;	
}
*/

//###########################################################################
#define linked_list_h(Type)													\
typedef struct Node {														\
	struct Node *next;														\
	struct Node *previous;													\
	Type *object;															\
} Node;																		\
void add_Type(LinkedList *list, Type object);

#define linked_list_c(Type)													\
/*Adds a player to the end of the list list*/								\
void add_Type(LinkedList *list, Type object) {								\
	Node *current = NULL;													\
	Node *previous = NULL;													\
	if (list->firstNode == NULL) {											\
		current = list->firstNode = malloc(sizeof(Node));					\
		current->next = NULL;												\
		current->previous = NULL;											\
		current->object = malloc(sizeof(Type));								\
		memcpy(current->object, &object, sizeof(Type));						\
	} else {																\
		current = list->firstNode;											\
		previous = current;													\
		while (current != NULL) {											\
			previous = current;												\
			current = current->next;										\
		}																	\
		/* previous is now the final node in the linked list */				\
		current = previous->next = malloc(sizeof(Node));					\
		current->object = malloc(sizeof(Type));								\
		/* Allocate old final node as the previous node of the new final */	\
		current->previous = previous;										\
		current->next = NULL;												\
		memcpy(current->object, &object, sizeof(Node));						\
																			\
		list->playerCnt++;													\
	}																		\
	return;																	\
}
//###########################################################################
#endif
