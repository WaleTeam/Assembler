#include "emu.h"


char buffer[256];
char message[] = "you entered: ";
char new_line = '\n';

int main(int argc, char *argv[], char **envp) {

	emu_init();

	emu_size_t bytes_read = emu_read(emu_stdin, buffer, sizeof(buffer) - sizeof(char));

	buffer[bytes_read] = 0;

	emu_write(emu_stdout, message, sizeof(message));
	emu_write(emu_stdout, buffer, bytes_read);
	emu_write(emu_stdout, &new_line, sizeof(char));

	return 0;
}