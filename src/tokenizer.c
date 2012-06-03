#include "emu.h"
#include "tokenizer.h"

int is_delimeter(char character) {

}

int is_letter(char character) {

}

int is_number(char character) {

}

void identify_word(struct Tokenizer_state *tokenizer, struct Token *token) {

}

void identify_number(struct Tokenizer_state *tokenizer, struct Token *token) {

}

void init_tokenizer(struct Tokenizer_state *tokenizer) {
	emu_memset(tokenizer->string, 0, kMaxLineLength);
	emu_memset(tokenizer->currentToken, 0, kMaxTokenLength);
	tokenizer->stringIndex = 0;
	tokenizer->currentTokenIndex = 0;
	tokenizer->errorState = kErrorTokenizerNone;
	tokenizer->tokenizerState = kTokenizerStateInitial;
}

void find_next_token(struct Tokenizer_state *tokenizer) {

	if(tokenizer->tokenizerState == kTokenizerStateInitial) {
		tokenizer->tokenizerState = kTokenizerStateDelimeterSpace;
	}

	for(int i = tokenizer->stringIndex; i < kMaxLineLength; i++) {
		char currentChar = tokenizer->string[i];

		if(! is_delimeter(currentChar)) {

			if(tokenizer->currentTokenIndex > kMaxTokenLength) {
				tokenizer->errorState = kErrorTokenizerTokenLengthExceeded;
				return;
			}

			if(tokenizer->tokenizerState == kTokenizerStateDelimeterSpace) {
				tokenizer->tokenizerState = kTokenizerStateTokenStartFound;
				tokenizer->currentToken[tokenizer->currentTokenIndex] = currentChar;
				tokenizer->currentTokenIndex++;

			} else { //if (tokenizer->tokenizerState == kTokenizerStateTokenStartFound) {
				tokenizer->currentToken[tokenizer->currentTokenIndex] = 0;
				tokenizer->tokenizerState = kTokenizerStateTokenEndFound;

			}
		}
	}

	tokenizer->tokenizerState = kTokenizerStateStringEndReached;
}

void process_next_token(struct Tokenizer_state *tokenizer, struct Token *token) {

	find_next_token(tokenizer);

	if(tokenizer->errorState != kErrorTokenizerNone) {
		return;
	}

	if(tokenizer->tokenizerState == kTokenizerStateStringEndReached) {
		return;
	}

	char firstCharacter = tokenizer->currentToken[0];

	if(is_letter(firstCharacter)) {
		identify_word(tokenizer, token);
	} else if(is_number) {
		identify_number(tokenizer, token);
	}

	tokenizer->tokenizerState = kTokenizerStateDelimeterSpace;
}
