#include "emu.h"
#include "tokenizer.h"

char message[] = "refilled buffer with:\n";
char message1[] = "found token:\n";

emu_size_t token_fill_buffer(void *buffer, emu_size_t buffer_size) {
	emu_size_t bytes_read = emu_read(emu_stdin, buffer, buffer_size);

	if(bytes_read > 0) {
		emu_write(emu_stdout, message, sizeof(message));
		emu_write(emu_stdout, buffer, bytes_read);
		emu_write(emu_stdout, "\n", sizeof(char));
	}

	return bytes_read;
}

void token_handler(struct Tokenizer *tokenizer, struct Token *token) {

	char tokenType = token->type + '0';

	emu_write(emu_stdout, message1, sizeof(message));
	emu_write(emu_stdout, token->string, token->stringIndex);
	emu_write(emu_stdout, "\n", sizeof(char));
	emu_write(emu_stdout, &tokenType, sizeof(char));
	emu_write(emu_stdout, "\n", sizeof(char));
}

int main(int argc, char *argv[]) {

	struct Tokenizer tokenizer;

	emu_init();

	init_tokenizer(&tokenizer, token_fill_buffer, token_handler);
	process_tokens(&tokenizer);

	return 0;
}