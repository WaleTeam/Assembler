#include "emu.h"
#include "tokenizer.h"
#include "parser.h"
#include "list.h"

char message[] = "refilled buffer with:\n";
char message1[] = "found token:\n";

	struct Tokenizer tokenizer;
	struct Parser parser;

emu_size_t token_fill_buffer(void *buffer, emu_size_t buffer_size) {
	emu_size_t bytes_read = emu_read(emu_stdin, buffer, buffer_size);

	if(bytes_read > 0) {
		emu_write(emu_stdout, message, sizeof(message));
		emu_write(emu_stdout, buffer, bytes_read);
		emu_write(emu_stdout, "\n", sizeof(char));
	}

	return bytes_read;
}

void print_token(struct Token *token) {
	char tokenType = token->type + '0';

	emu_write(emu_stdout, message1, sizeof(message));
	emu_write(emu_stdout, token->string, token->stringIndex);
	emu_write(emu_stdout, "\n", sizeof(char));
	emu_write(emu_stdout, &tokenType, sizeof(char));
	emu_write(emu_stdout, "\n", sizeof(char));
}

void token_handler(struct Tokenizer *tokenizer, struct Token *token) {
	parser_parse_token(&parser, token);
	// print_token(token);
}

int main(int argc, char *argv[]) {

	emu_init();

	// init_token_list();

	tokenizer_init(&tokenizer, token_fill_buffer, token_handler);
	parser_init(&parser);

	tokenizer_find_tokens(&tokenizer);
	parser_finish(&parser);

	// print_tokens();
	// free_tokens();

	return 0;
}
