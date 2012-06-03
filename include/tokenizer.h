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
	TokenizerStateInitial
};

enum TokenType {
	TokenTypeNone,
	TokenTypeWord,
	TokenTypeNumber,
	TokenTypeString,
	TokenTypeStructural
};

struct Tokenizer_state;
struct Token;

typedef void (*Token_fill_buffer)(void *buffer, emu_size_t buffer_size);
typedef void (*Token_handler)(struct Tokenizer_state *tokenizer, struct Token *token);

struct Tokenizer_state {
	enum TokenizerErrorSate errorState;
	enum TokenizerState tokenizerState;

	char buffer[kTokenStateBufferSize];
	int bufferIndex;

	Token_handler token_handler;
	Token_fill_buffer fill_buffer;
};

struct Token {
	enum TokenType type;
	char string[kMaxTokenLength];
	int stringIndex;
};

void init_tokenizer(struct Tokenizer_state *self, Token_fill_buffer fill_buffer, Token_handler token_handler);
void process_tokens(struct Tokenizer_state *self);

#endif //_PROJECT_AS_TOKENIZER_H_INCLUDED_