#ifndef _PROJECT_AS_TOKENIZER_H_INCLUDED_
#define _PROJECT_AS_TOKENIZER_H_INCLUDED_

#include "emu.h"

#define kTokenStateBufferSize 256
#define kMaxTokenLength 256

enum TokenizerErrorSate {
	TokenizerErrorSateNone,
	TokenizerErrorSateTokenLengthExceeded
};

enum TokenizerState {
	TokenizerStateInitial,
	TokenizerStateDone
};

enum TokenType {
	TokenTypeNone,
	TokenTypeWord,
	TokenTypeNumber,
	TokenTypeString,
	TokenTypeStructural
};

struct Tokenizer;
struct Token;

typedef emu_size_t (*Token_fill_buffer)(void *buffer, emu_size_t buffer_size);
typedef void (*Token_handler)(struct Tokenizer *tokenizer, struct Token *token);

struct Tokenizer {
	enum TokenizerErrorSate errorState;
	enum TokenizerState state;

	char buffer[kTokenStateBufferSize];
	int bufferIndex;
	int bufferFillIndex;

	Token_handler token_handler;
	Token_fill_buffer fill_buffer;
};

struct Token {
	enum TokenType type;
	char string[kMaxTokenLength];
	int stringIndex;
};

void tokenizer_init(struct Tokenizer *self, Token_fill_buffer fill_buffer, Token_handler token_handler);
void tokenizer_find_tokens(struct Tokenizer *self);

#endif //_PROJECT_AS_TOKENIZER_H_INCLUDED_