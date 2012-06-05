#ifndef _PROJECT_AS_LIST_H_INCLUDED_
#define _PROJECT_AS_LIST_H_INCLUDED_

#include "emu.h"

struct List {
	void *ptr;

	struct List *next;
	struct List *previous;
};

void list_init(struct List *self, void *ptr);
void list_free(struct List *self);

void list_insert_before(struct List *self, struct List *other);
void list_insert_after(struct List *self, struct List *other);
void list_remove(struct List *self);

#endif //_PROJECT_AS_LIST_H_INCLUDED_