#ifndef _PROJECT_AS_PARSER_H_INCLUDED_
#define _PROJECT_AS_PARSER_H_INCLUDED_


#include "tokenizer.h"
#include "list.h"

#define kParserFlagRegSize16		0x1
#define kParserFlagIndexSize16		0x2
#define kMaxStringLength			kMaxTokenLength
#define kAddressMax					-1

enum ParserNodeType {
	ParserNodeTypeNone,
	ParserNodeTypeStructural,
	ParserNodeTypeStructuralOrigin,
	ParserNodeTypeStructuralRegMode,
	ParserNodeTypeStructuralLabel,
	ParserNodeTypeWord,
	ParserNodeTypeOpcode,				//mmu $25			-	normal opcode to binary translation
	ParserNodeTypeString,				//"Hello World"		- 	string to binary translation
	ParserNodeTypeNumber				//$320				-	number to binary translation
};

enum ParserNodeState {
	ParserNodeStateInitialized,
	ParserNodeStateParsing,
	ParserNodeStateError,
	ParserNodeStateEarlySuccess,
	ParserNodeStateSuccessful
};

struct Parser;
struct ParserNode;

typedef int (*NodeTokenTester)(struct ParserNode *self, struct Parser *parser, struct Token *token);
typedef void (*NodeParser)(struct ParserNode *self, struct Parser *parser, struct Token *token);
typedef void (*NodeEmitter)(struct ParserNode *self, struct Parser *parser);

struct  ParserNode {
	enum ParserNodeType type;
	enum ParserNodeState state;
	int byteSize;

	NodeTokenTester tokenTester;
	NodeParser parse;
	NodeEmitter emit;
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

struct ParserNodeWord {
	struct ParserNode parserNode;
	struct ParserNode *subNode;
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

enum SymbolType {
	SymbolTypeNone,
	SymbolTypeLabel
};

struct Symbol {
	char name[kMaxStringLength];

	int address;
	enum SymbolType type;
};

enum ParserState {
	ParserStateInitial,
	ParserStateParsing,
	ParserStateError,
	ParserStateSuccessful
};


struct Parser {
	enum ParserState state;

	int origin;
	int flags;
	int currentAddress;

	struct ParserNode *currentNode;
	struct List *symbolTable;
};

void parser_init(struct Parser *self);
void parser_parse_token(struct Parser *self, struct Token *token);
void parser_finish(struct Parser *self);
struct Symbol *parser_find_symbol(struct Parser *self, char *name);
void parser_add_symbol(struct Parser *self, struct Symbol *symbol);

struct ParserNode *parserNode_create(enum ParserNodeType type);
void parserNode_free(struct ParserNode *node);

#endif //_PROJECT_AS_PARSER_H_INCLUDED_