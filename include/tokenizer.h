#ifndef _PROJECT_AS_TOKENIZER_H_INCLUDED_
#define _PROJECT_AS_TOKENIZER_H_INCLUDED_

#include "emu.h"

#define kTokenStateBufferSize 256
#define kMaxTokenLength 256

#define kErrorTokenizerNone						0
#define kErrorTokenizerTokenLengthExceeded 		1


#define kTokenizerSateInitial					0


struct Tokenizer_state;
struct Token;

typedef void (*Token_fill_buffer)(void *buffer, emu_size_t buffer_size);
typedef void (*Token_handler)(struct Tokenizer_state *tokenizer, struct Token *token);

struct Tokenizer_state {
	int errorState;
	int tokenizerState;

	char buffer[kTokenStateBufferSize];
	int bufferIndex;

	Token_handler token_handler;
	Token_fill_buffer fill_buffer;
};

struct Token {
	int tokenType;
	char tokenString[kMaxTokenLength];
};

void init_tokenizer(struct Tokenizer_state *self, Token_fill_buffer fill_buffer, Token_handler token_handler);
void process_tokens(struct Tokenizer_state *self, struct Token *token);

#endif //_PROJECT_AS_TOKENIZER_H_INCLUDED_