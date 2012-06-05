#include "list.h"


void list_init(struct List *self, void *ptr) {

	self->next = self;
	self->previous = self;
	self->ptr = ptr;
}

void list_free(struct List *self) {
	if(self->ptr != 0) {
		emu_free(self->ptr);
	}
	emu_free(self);
}

void list_insert_after(struct List *self, struct List *other) {
	other->previous = self;
	other->next = self->next;

	self->next = other;
}

void list_insert_before(struct List *self, struct List *other) {
	other->next = self;
	other->previous = self->previous;

	self->previous = other;
}

void list_unlink(struct List *self) {
	self->previous->next = self->next;
	self->next->previous = self->previous;

	self->next = self;
	self->previous = self;
}