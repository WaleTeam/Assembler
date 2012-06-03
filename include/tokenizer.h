#ifndef _PROJECT_AS_TOKENIZER_H_INCLUDED_
#define _PROJECT_AS_TOKENIZER_H_INCLUDED_

#define kMaxLineLength 256
#define kMaxTokenLength 256

#define kErrorTokenizerNone						0
#define kErrorTokenizerTokenLengthExceeded 		1

#define kTokenizerStateInitial					0
#define kTokenizerStateStringEndReached			1
#define kTokenizerStateDelimeterSpace			2
#define kTokenizerStateTokenStartFound			3
#define kTokenizerStateTokenEndFound			4

struct Tokenizer_state {
	int errorState;
	int tokenizerState;

	char string[kMaxLineLength];
	int stringIndex;
	char currentToken[kMaxTokenLength];
	int currentTokenIndex;
};

struct Token {
	int tokenType;
	char tokenString;
};

void process_next_token(struct Tokenizer_state *tokenizer, struct Token *token);

#endif //_PROJECT_AS_TOKENIZER_H_INCLUDED_