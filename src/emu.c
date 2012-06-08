#include "emu.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int emu_stdin;
int emu_stdout;

void emu_init() {
	emu_stdin = stdin->_file;
	emu_stdout = stdout->_file;
}

emu_ssize_t emu_read(int fh, void *buffer, emu_size_t size) {

	return read(fh, buffer, size);
}

emu_ssize_t emu_write(int fh, const void *buffer, emu_size_t size) {
	return write (fh, buffer, size);
}

void *emu_memcpy(void *destination, const void *source, emu_size_t size) {
	return memcpy(destination, source, size);
}

void *emu_memset(void *ptr, int value, emu_size_t num) {
	return memset(ptr, value, num);
}

char *emu_strncpy(char *destination, const char *source, emu_size_t size) {
	return strncpy(destination, source, size);
}

void *emu_malloc(emu_size_t size) {
	return malloc(size);
}

void *emu_realloc(void *ptr, emu_size_t size) {
	return realloc(ptr, size);
}

void emu_free(void *ptr) {
	return free(ptr);
}

void emu_log(char *log) {
	printf("%s\n", log);
}