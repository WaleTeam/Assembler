#include "emu.h"
#include "tokenizer.h"
#include "parser.h"
#include "list.h"

char message[] = "refilled buffer with:\n";
char message1[] = "found token:\n";

struct List *tokenList;
struct List *currentToken;

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
	// print_token(token);

	// struct Token *copyToken = emu_malloc(sizeof(struct Token));

	// emu_memcpy(copyToken, token, sizeof(struct Token));

	// currentToken->ptr = copyToken;

	// struct List *nextListItem = emu_malloc(sizeof(struct List));
	// list_init(nextListItem, 0);
	// list_insert_after(currentToken, nextListItem);
	// currentToken = nextListItem;
}

// void print_tokens() {
// 	struct List *list = tokenList;

// 	while(list->ptr != 0) {
// 		struct Token *token = (struct Token *)list->ptr;
// 		print_token(token);

// 		list = list->next;
// 	}
// }

// void free_tokens() {
// 	struct List *list = tokenList;
// 	struct List *lastToken = list->previous;

// 	do {
// 		struct List *next = list = list->next;

// 		emu_free(list->ptr);
// 		emu_free(list);

// 		list = next;
// 	} while(list != lastToken);

// 	tokenList = lastToken;
// 	currentToken = lastToken;
// }

// void init_token_list() {
// 	tokenList = emu_malloc(sizeof(struct List));

// 	list_init(tokenList, 0);
// 	currentToken = tokenList;
// }

int main(int argc, char *argv[]) {

	struct Tokenizer tokenizer;
	struct Parser parser;

	emu_init();

	// init_token_list();

	tokenizer_init(&tokenizer, token_fill_buffer, token_handler);
	parser_init(&parser);

	tokenizer_find_tokens(&tokenizer);

	// print_tokens();
	// free_tokens();

	return 0;
}
