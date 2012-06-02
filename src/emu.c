#include "emu.h"
#include <stdio.h>

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