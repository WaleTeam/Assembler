#include "emu.h"
#include "tokenizer.h"

void root_handler(struct Tokenizer_state *self);

void init_tokenizer(struct Tokenizer_state *self, Token_fill_buffer fill_buffer, Token_handler token_handler) {
	emu_memset(self->buffer, 0, sizeof(char [kTokenStateBufferSize]));

	self->bufferIndex = 0;
	self->errorState = kErrorTokenizerNone;
	self->tokenizerState = kTokenizerSateInitial;

	self->token_handler = token_handler;
	self->fill_buffer = fill_buffer;
}

void process_tokens(struct Tokenizer_state *self, struct Token *token) {
	root_handler(self);
}

void advance_chracter_pointer(struct Tokenizer_state *self) {

	self->bufferIndex++;

	if(self->bufferIndex == kTokenStateBufferSize) {
		self->bufferIndex = 0;
		self->fill_buffer(self->buffer, sizeof(char [kTokenStateBufferSize]));
	}
}

int is_delimeter(char character) {
	return 1;
}

int is_number(char character) {
	
}

int is_letter(char character) {

}

void keyword_handler(struct Tokenizer_state *self) {

}

void number_handler(struct Tokenizer_state *self) {

}

void root_handler(struct Tokenizer_state *self) {

	char currentChar = self->buffer[self->bufferIndex];

	if(! is_delimeter(currentChar)) {
		if(is_letter(currentChar)) {

			keyword_handler(self);
		} else if(is_number) {

			number_handler(self);
		}
	}

	advance_chracter_pointer(self);
}
