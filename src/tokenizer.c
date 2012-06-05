#include "emu.h"
#include "tokenizer.h"

#define tokenizer_get_current_char(self) self->buffer[self->bufferIndex]

void tokenizer_root_handler(struct Tokenizer *self);

void tokenizer_init(struct Tokenizer *self, Token_fill_buffer fill_buffer, Token_handler token_handler) {
	emu_memset(self->buffer, 0, sizeof(char [kTokenStateBufferSize]));

	self->bufferIndex = 0;
	self->errorState = TokenizerErrorSateNone;
	self->state = TokenizerStateInitial;

	self->token_handler = token_handler;
	self->fill_buffer = fill_buffer;
}

void tokenizer_refill_buffer(struct Tokenizer *self) {
	
	self->bufferIndex = 0;
	emu_size_t bytes_read = self->fill_buffer(self->buffer, sizeof(char [kTokenStateBufferSize]));
	self->bufferFillIndex = bytes_read;
	
	if(bytes_read == 0) {
		self->state = TokenizerStateDone;
	}
}

void tokenizer_find_tokens(struct Tokenizer *self) {
	tokenizer_refill_buffer(self);
	tokenizer_root_handler(self);
}

char tokenizer_advance_chracter_pointer(struct Tokenizer *self) {

	self->bufferIndex++;

	if(self->bufferIndex == self->bufferFillIndex) {
		tokenizer_refill_buffer(self);
	}

	return self->buffer[self->bufferIndex];
}

void token_init(struct Token *self, enum TokenType type) {
	self->type = type;
	self->stringIndex = 0;
	emu_memset(self->string, 0, sizeof(self->string));
}

void token_store_character(struct Token *self, char character) {

	//FIXME: check for overflow
	self->string[self->stringIndex] = character;
	self->stringIndex++;
	self->string[self->stringIndex] = 0;
}

int tokenizer_is_delimeter(struct Tokenizer *self, char character) {

	if(self->state == TokenizerStateDone) {
		return 1;
	}

	switch(character) {
		case ' ':
		case ',':
			return 1;
		default:
			return 0;
	}
}

int tokenizer_is_comment_delimeter(char character) {
	if(character == ';') {
		return 1;
	}

	return 0;
}

int tokenizer_is_string_delimeter(struct Tokenizer *self, char character) {

	if(self->state == TokenizerStateDone) {
		return 3;
	}

	switch(character) {
		case '\'':
			return 1;
		case '"':
			return 2;
		default:
			return 0;
	}
}

int tokenizer_is_number(char character) {
	if(character >= '0' && character <= '9') {
		return 1;
	}

	if(character == '$') {
		return 1;
	}

	return 0;
}

int tokenizer_is_letter(char character) {
	if(character >= 'a' && character <= 'z') {
		return 1;
	}

	if(character >= 'A' && character <= 'Z') {
		return 1;
	}

	return 0;
}

int tokenizer_is_structure_delimeter(char character) {
	switch(character) {
		case '.':
		case ':':
		case '(':
		case ')':
		case '=':
		case '#':
			return 1;
		default:
			return 0;
	}
}

int tokenizer_is_escape(char character) {
	return character == '\\';
}

//########################################################################################
// ### word tokenizer ###
//########################################################################################
void tokenizer_word_handler(struct Tokenizer *self) {
	struct Token token;

	token_init(&token, TokenTypeWord);


	char currentChar = tokenizer_get_current_char(self);

	while(tokenizer_is_letter(currentChar) || tokenizer_is_number(currentChar)) {

		token_store_character(&token, currentChar);
		currentChar = tokenizer_advance_chracter_pointer(self);
	}

	self->token_handler(self, &token);
}

//########################################################################################
// ### number tokenizer ###
//########################################################################################
void tokenizer_number_handler(struct Tokenizer *self) {
	struct Token token;
	
	token_init(&token, TokenTypeNumber);

	char currentChar = tokenizer_get_current_char(self);

	while(tokenizer_is_number(currentChar)) {

		token_store_character(&token, currentChar);
		currentChar = tokenizer_advance_chracter_pointer(self);
	}

	self->token_handler(self, &token);
}

//########################################################################################
// ### string tokenizer ###
//########################################################################################
void tokenizer_string_default_escape_handler(struct Tokenizer *self, struct Token *token) {

}

void tokenizer_string_number_esacpe_handler(struct Tokenizer *self, struct Token *token, char currentChar) {
	//TODO: redirect to number parsing in this place
}

void tokenizer_string_esacpe_handler(struct Tokenizer *self, struct Token *token) {
	char currentChar = tokenizer_advance_chracter_pointer(self);
	int end_of_escape = 0;

	token_store_character(token, currentChar);

	// if(currentChar == 'x' || is_number(currentChar)) {
	// 	string_number_esacpe_handler(self, token, currentChar);
	// } else if(currentChar == '"') {
	// 	token_store_character(token, currentChar);
	// } else {
	// 	string_default_escape_handler(self, token);
	// }
}

void tokenizer_string_handler(struct Tokenizer *self) {
	struct Token token;

	token_init(&token, TokenTypeString);

	char currentChar = tokenizer_advance_chracter_pointer(self);

	while(! tokenizer_is_string_delimeter(self, currentChar)) {

		if(tokenizer_is_escape(currentChar)) {
			tokenizer_string_esacpe_handler(self, &token);
		} else {
			token_store_character(&token, currentChar);
		}

		currentChar = tokenizer_advance_chracter_pointer(self);
	}

	tokenizer_advance_chracter_pointer(self);

	self->token_handler(self, &token);
}
//########################################################################################
// ### structure tokenizer ###
//########################################################################################
void tokenizer_structure_handler(struct Tokenizer *self) {
	struct Token token;

	token_init(&token, TokenTypeStructural);

	char currentChar = tokenizer_get_current_char(self);
	token_store_character(&token, currentChar);

	tokenizer_advance_chracter_pointer(self);

	self->token_handler(self, &token);
}

//########################################################################################
// ### comment tokenizer ###
//########################################################################################
void tokenizer_comment_handler(struct Tokenizer *self) {
	char currentChar = tokenizer_advance_chracter_pointer(self);

	while(currentChar != '\n') {
		currentChar = tokenizer_advance_chracter_pointer(self);
	}
}

//########################################################################################
// ### root tokenizer ###
//########################################################################################
void tokenizer_root_handler(struct Tokenizer *self) {

	while(! self->state == TokenizerStateDone) {

		char currentChar = tokenizer_get_current_char(self);

		if(tokenizer_is_delimeter(self, currentChar)) {

			currentChar = tokenizer_advance_chracter_pointer(self);
		} else {

			if(tokenizer_is_letter(currentChar)) {

				tokenizer_word_handler(self);
			} else if(tokenizer_is_string_delimeter(self, currentChar)) {

				tokenizer_string_handler(self);
			} else if(tokenizer_is_number(currentChar)) {

				tokenizer_number_handler(self);
			} else if(tokenizer_is_structure_delimeter(currentChar)) {

				tokenizer_structure_handler(self);
			} else if(tokenizer_is_comment_delimeter(currentChar)) {

				tokenizer_comment_handler(self);
			} else {

				currentChar = tokenizer_advance_chracter_pointer(self);
			}
		}
	}
}
