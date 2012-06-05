#ifndef _PROJECT_AS_PARSER_H_INCLUDED_
#define _PROJECT_AS_PARSER_H_INCLUDED_


#include "tokenizer.h"

#define PARSER_FLAG_REGISTERSIZE_16	0x1
#define PARSER_FLAG_INDEXSIZE_16	0x2

struct Parser {
	int origin;
	int flags;
};

enum ParserNodeType {
	ParserNodeTypeNone,			
	ParserNodeTypeStructural,	//.origin $500		-	controls parsing context and behavior
								//label:
	ParserNodeTypeOpcode,		//mmu $25			-	normal opcode to binary translation
	ParserNodeTypeString,		//"Hello World"		- 	string to binary translation
	ParserNodeTypeNumber		//$320				-	number to binary translation
};

struct ParserNode {
	enum ParserNodeType type;
	int byteSize;
};

void parser_init(struct Parser *self);
void parser_parse_token(struct Parser *self, struct Token *token);

#endif //_PROJECT_AS_PARSER_H_INCLUDED_