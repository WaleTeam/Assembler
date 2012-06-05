#ifndef _PROJECT_AS_PARSER_H_INCLUDED_
#define _PROJECT_AS_PARSER_H_INCLUDED_


#include "tokenizer.h"

#define kParserFlagRegSize16		0x1
#define kParserFlagIndexSize16		0x2
#define kMaxStringLength			kMaxTokenLength

enum ParserNodeType {
	ParserNodeTypeNone,			
	ParserNodeSTypetructuralOrigin,
	ParserNodeTypeStructuralRegMode,
	ParserNodeTypeStructuralLabel,
	ParserNodeTypeOpcode,				//mmu $25			-	normal opcode to binary translation
	ParserNodeTypeString,				//"Hello World"		- 	string to binary translation
	ParserNodeTypeNumber				//$320				-	number to binary translation
};

struct ParserNode {
	enum ParserNodeType type;
	int byteSize;
};

struct ParserNodeStructural {
	struct ParserNode parserNode;
};

struct ParserNodeStructuralOrigin {
	struct ParserNode parserNode;
	int origin;
};

struct ParserNodeStructuralRegMode {
	struct ParserNode parserNode;
	int flags;
};

struct ParserNodeStructuralLabel {
	struct ParserNode parserNode;
	char labelName[kMaxStringLength];
	int address;
};

struct ParserNodeOpcode {
	struct ParserNode parserNode;
	int opcode;
	int param1;
	int param2;
};

struct ParserNodeString {
	struct ParserNode parserNode;
	char string[kMaxStringLength];
	int stringSize;
};

struct ParserNodeNumber {
	struct ParserNode parserNode;
	int number;
};

struct Parser {
	int origin;
	int flags;
	int currentAddress;
};

void parser_init(struct Parser *self);
void parser_parse_token(struct Parser *self, struct Token *token);

struct ParserNode *parserNode_create(enum ParserNodeType type);

#endif //_PROJECT_AS_PARSER_H_INCLUDED_