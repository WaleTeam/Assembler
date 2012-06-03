#include "emu.h"
#include "tokenizer.h"

#define get_current_char(self) self->buffer[self->bufferIndex]

void root_handler(struct Tokenizer_state *self);

void init_token(struct Token *self) {

}

void init_tokenizer(struct Tokenizer_state *self, Token_fill_buffer fill_buffer, Token_handler token_handler) {
	emu_memset(self->buffer, 0, sizeof(char [kTokenStateBufferSize]));

	self->bufferIndex = 0;
	self->errorState = TokenizerErrorSateNone;
	self->tokenizerState = TokenizerStateInitial;

	self->token_handler = token_handler;
	self->fill_buffer = fill_buffer;
}

void process_tokens(struct Tokenizer_state *self) {
	root_handler(self);
}

char advance_chracter_pointer(struct Tokenizer_state *self) {

	self->bufferIndex++;

	if(self->bufferIndex == kTokenStateBufferSize) {
		self->bufferIndex = 0;
		self->fill_buffer(self->buffer, sizeof(char [kTokenStateBufferSize]));
	}

	return self->buffer[self->bufferIndex];
}

void store_character_in_token(struct Token *self, char character) {
	self->string[self->stringIndex] = character;
	self->stringIndex++;
}

int is_delimeter(char character) {
	switch(character) {
		case ' ':
		case ',':
			return 1;
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

int is_string_delimeter(char character) {
	switch(character) {
		case '\'':
			return 1;
		case '"':
			return 2;
		default:
			return 0;
	}
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
void word_handler(struct Tokenizer_state *self) {
	struct Token token;
	int string_pointer = 0;

	token.type = TokenTypeWord;
	emu_memset(token.string, 0, sizeof(token.string));

	char currentChar = token.string[string_pointer] = get_current_char(self);

	while(is_letter(currentChar) || is_number(currentChar)) {
		token.string[string_pointer] = currentChar;
		string_pointer++;

		currentChar = advance_chracter_pointer(self);
	}

	self->token_handler(self, &token);
}

//########################################################################################
// ### number tokenizer ###
//########################################################################################
void number_handler(struct Tokenizer_state *self) {
	struct Token token;
	int string_pointer = 0;

	token.type = TokenTypeNumber;
	emu_memset(token.string, 0, sizeof(token.string));

	char currentChar = token.string[string_pointer] = get_current_char(self);

	while(is_number(currentChar)) {
		token.string[string_pointer] = currentChar;
		string_pointer++;

		currentChar = advance_chracter_pointer(self);
	}

	self->token_handler(self, &token);
}

//########################################################################################
// ### string tokenizer ###
//########################################################################################
void string_default_escape_handler(struct Tokenizer_state *self, struct Token *token) {

}

void string_number_esacpe_handler(struct Tokenizer_state *self, struct Token *token, char currentChar) {
	//TODO: redirect to number parsing in this place
}

void string_esacpe_handler(struct Tokenizer_state *self, struct Token *token) {
	char currentChar = advance_chracter_pointer(self);
	int end_of_escape = 0;

	if(currentChar == 'x' || is_number(currentChar)) {
		string_number_esacpe_handler(self, token, currentChar);
	} else if(currentChar == '"') {
		store_character_in_token(token, currentChar);
	} else {
		string_default_escape_handler(self, token);
	}
}

void string_handler(struct Tokenizer_state *self) {
	struct Token token;

	token.type = TokenTypeString;
	token.stringIndex = 0;
	emu_memset(token.string, 0, sizeof(token.string));

	char currentChar = token.string[token.stringIndex] = advance_chracter_pointer(self);

	while(! is_string_delimeter(currentChar)) {

		if(is_escape(currentChar)) {
			string_esacpe_handler(self, &token);
		} else {
			store_character_in_token(&token, currentChar);
		}

		currentChar = advance_chracter_pointer(self);
	}

	self->token_handler(self, &token);
}

void structure_handler(struct Tokenizer_state *self) {

}

void root_handler(struct Tokenizer_state *self) {

	char currentChar = get_current_char(self);

	if(! is_delimeter(currentChar)) {
		if(is_letter(currentChar)) {

			word_handler(self);
		} else if(is_string_delimeter(currentChar)) {

			string_handler(self);
		} else if(is_number(currentChar)) {

			number_handler(self);
		} else if(is_structure_delimeter(currentChar)) {

			structure_handler(self);
		}
	}

	currentChar = advance_chracter_pointer(self);
}
