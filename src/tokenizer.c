#include "emu.h"
#include "tokenizer.h"

#define get_current_char(self) self->buffer[self->bufferIndex]

void root_handler(struct Tokenizer *self);

void init_tokenizer(struct Tokenizer *self, Token_fill_buffer fill_buffer, Token_handler token_handler) {
	emu_memset(self->buffer, 0, sizeof(char [kTokenStateBufferSize]));

	self->bufferIndex = 0;
	self->errorState = TokenizerErrorSateNone;
	self->state = TokenizerStateInitial;

	self->token_handler = token_handler;
	self->fill_buffer = fill_buffer;
}

void refill_buffer(struct Tokenizer *self) {
	
	self->bufferIndex = 0;
	emu_size_t bytes_read = self->fill_buffer(self->buffer, sizeof(char [kTokenStateBufferSize]));
	self->bufferFillIndex = bytes_read;
	
	if(bytes_read == 0) {
		self->state = TokenizerStateDone;
	}	
}

void process_tokens(struct Tokenizer *self) {
	refill_buffer(self);
	root_handler(self);
}

char advance_chracter_pointer(struct Tokenizer *self) {

	self->bufferIndex++;

	if(self->bufferIndex == self->bufferFillIndex) {
		refill_buffer(self);
	}

	return self->buffer[self->bufferIndex];
}

void init_token(struct Token *self, enum TokenType type) {
	self->type = type;
	self->stringIndex = 0;
	emu_memset(self->string, 0, sizeof(self->string));
}

void store_character_in_token(struct Token *self, char character) {
	self->string[self->stringIndex] = character;
	self->stringIndex++;
}

int is_delimeter(struct Tokenizer *self, char character) {

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

int is_string_delimeter(struct Tokenizer *self, char character) {

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

int is_number(char character) {
	if(character >= '0' && character <= '9') {
		return 1;
	}

	return 0;
}

int is_letter(char character) {
	if(character >= 'a' && character <= 'z') {
		return 1;
	}

	if(character >= 'A' && character <= 'Z') {
		return 1;
	}

	return 0;
}

int is_structure_delimeter(char character) {
	switch(character) {
		case '.':
		case ':':
		case ';':
		case '(':
		case ')':
		case '=':
		case '#':
			return 1;
		default:
			return 0;
	}
}

int is_escape(char character) {
	return character == '\\';
}

//########################################################################################
// ### word tokenizer ###
//########################################################################################
void word_handler(struct Tokenizer *self) {
	struct Token token;

	init_token(&token, TokenTypeWord);

	char currentChar = get_current_char(self);

	while(is_letter(currentChar) || is_number(currentChar)) {

		store_character_in_token(&token, currentChar);
		currentChar = advance_chracter_pointer(self);
	}

	self->token_handler(self, &token);
}

//########################################################################################
// ### number tokenizer ###
//########################################################################################
void number_handler(struct Tokenizer *self) {
	struct Token token;
	
	init_token(&token, TokenTypeNumber);

	char currentChar = get_current_char(self);

	while(is_number(currentChar)) {

		store_character_in_token(&token, currentChar);
		currentChar = advance_chracter_pointer(self);
	}

	self->token_handler(self, &token);
}

//########################################################################################
// ### string tokenizer ###
//########################################################################################
void string_default_escape_handler(struct Tokenizer *self, struct Token *token) {

}

void string_number_esacpe_handler(struct Tokenizer *self, struct Token *token, char currentChar) {
	//TODO: redirect to number parsing in this place
}

void string_esacpe_handler(struct Tokenizer *self, struct Token *token) {
	char currentChar = advance_chracter_pointer(self);
	int end_of_escape = 0;

	store_character_in_token(token, currentChar);

	// if(currentChar == 'x' || is_number(currentChar)) {
	// 	string_number_esacpe_handler(self, token, currentChar);
	// } else if(currentChar == '"') {
	// 	store_character_in_token(token, currentChar);
	// } else {
	// 	string_default_escape_handler(self, token);
	// }
}

void string_handler(struct Tokenizer *self) {
	struct Token token;

	init_token(&token, TokenTypeString);

	char currentChar = advance_chracter_pointer(self);

	while(! is_string_delimeter(self, currentChar)) {

		if(is_escape(currentChar)) {
			string_esacpe_handler(self, &token);
		} else {
			store_character_in_token(&token, currentChar);
		}

		currentChar = advance_chracter_pointer(self);
	}

	self->token_handler(self, &token);
}

void structure_handler(struct Tokenizer *self) {
	struct Token token;

	init_token(&token, TokenTypeStructural);

	char currentChar = get_current_char(self);
	store_character_in_token(&token, currentChar);

	advance_chracter_pointer(self);

	self->token_handler(self, &token);
}

void root_handler(struct Tokenizer *self) {

	while(! self->state == TokenizerStateDone) {

		char currentChar = get_current_char(self);

		if(is_delimeter(self, currentChar)) {

			currentChar = advance_chracter_pointer(self);
		} else {
			if(is_letter(currentChar)) {

				word_handler(self);
			} else if(is_string_delimeter(self, currentChar)) {

				string_handler(self);
			} else if(is_number(currentChar)) {

				number_handler(self);
			} else if(is_structure_delimeter(currentChar)) {

				structure_handler(self);
			} else {

				currentChar = advance_chracter_pointer(self);
			}
		}
	}
}
