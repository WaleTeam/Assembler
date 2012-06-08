#ifndef _PROJECT_AS_SMART_PTR_H_INCLUDED_
#define _PROJECT_AS_SMART_PTR_H_INCLUDED_

struct SmartPointer {
	void *pointer;

	int retainCount;
};

struct SmartPointer *smartPointer_create(void *pointer);
void smartPointer_retain(struct SmartPointer *self);
void smartPointer_release(struct SmartPointer *self);


#endif//_PROJECT_AS_SMART_PTR_H_INCLUDED_