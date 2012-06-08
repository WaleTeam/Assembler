#include "emu.h"
#include "smart_ptr.h"

struct SmartPointer *smartPointer_create(void *pointer) {
	struct SmartPointer *result = emu_malloc(sizeof(struct SmartPointer));

	result->pointer = pointer;
	result->retainCount 1;

	return result;
}

void smartPointer_retain(struct SmartPointer *self) {
	self->retainCount++;
}

void smartPointer_release(struct SmartPointer *self) {
	self->retainCount--;

	if(self->retainCount <= 0) {
		emu_free(self->pointer);
		emu_free(self);
	}
}