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

void *emu_memcpy(void *dest, const void *src, emu_size_t n) {
	return memcpy(dest, src, n);
}

void *emu_memset(void *ptr, int value, emu_size_t num) {
	return memset(ptr, value, num);
}